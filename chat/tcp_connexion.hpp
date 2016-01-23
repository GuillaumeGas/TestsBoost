#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

using namespace boost::asio;

class tcp_connexion {
public:
  tcp_connexion(io_service& ios);
  ip::tcp::socket& get_socket();
  
  //ecriture sur la socket
  template<typename T, typename Handler>
  void async_write(const T& t, Handler handler) {
    //serialisation des données
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << t;
    m_outbound_data = archive_stream.str();

    //ecriture du header
    std::ostringstream header_stream;
    header_stream << std::setw(header_length) << std::hex << m_outbound_data.size();
    if(!header_stream || header_stream.str().size() != header_length) {
      //erreur dans le header, on informe l'appelant
      boost::system::error_code error(error::invalid_argument);
      m_sock.get_io_service().post(boost::bind(handler, error));
    }
    m_outbound_header = header_stream.str();

    //ecriture dans la socket
    std::vector<const_buffer> buffers;
    buffers.push_back(buffer(m_outbound_header));
    buffers.push_back(buffer(m_outbound_data));
    boost::asio::async_write(m_sock, buffers, handler);
  }

  //lecture sur depuis la socket
  template<typename T, typename Handler>
  void async_read(T& t, Handler handler) {
    //récupération du header
    void (tcp_connexion::*f)(const boost::system::error_code&, T&, boost::tuple<Handler>) = &tcp_connexion::handle_read_header<T, Handler>;

    //QUESTION: on peut pas foutre "&tcp_connexion::handle_read_header<T, Handler>;" directement dans asyn_read ?

    boost::asio::async_read(m_sock, buffer(m_inbound_header),
			    boost::bind(f,
					this,
					placeholders::error,
					boost::ref(t),
					boost::make_tuple(handler)
					)
			    );
  }

  //interpretation du header
  template<typename T, typename Handler>
  void handle_read_header(const boost::system::error_code& err, T& t, boost::tuple<Handler> handler) {
    if(err) {
      boost::get<0>(handler)(err);
    } else {
      std::istringstream is(std::string(m_inbound_header, header_length));
      std::size_t m_inbound_datasize = 0;
      if(!(is >> std::hex >> m_inbound_datasize)) {
	//header invalide
	boost::system::error_code error(error::invalid_argument);
	boost::get<0>(handler)(error);
      } else {
	//récupération des données
	m_inbound_data.resize(m_inbound_datasize);
	void (tcp_connexion::*f)(const boost::system::error_code&, T&, boost::tuple<Handler>) = &tcp_connexion::handle_read_data<T, Handler>;

	boost::asio::async_read(m_sock, buffer(m_inbound_data),
				boost::bind(f, 
					    this,
					    placeholders::error,
					    boost::ref(t),
					    handler)
				);
      }
    }
  }

  //les données recues sont désérialisées
  template<typename T, typename Handler>
  void handle_read_data(const boost::system::error_code& err, T& t, boost::tuple<Handler> handler) {
    if(err) {
      boost::get<0>(handler)(err);
    } else {
      try {
	std::string archive_data(&m_inbound_data[0], m_inbound_data.size());
	std::istringstream archive_stream(archive_data);
	boost::archive::text_iarchive archive(archive_stream);
	archive >> t;
	boost::get<0>(handler)(err);
      } catch(std::exception& e) {
	boost::system::error_code error(error::invalid_argument);
	boost::get<0>(handler)(error);
      }
    }
  }

private:
  ip::tcp::socket m_sock;

  enum { header_length = 8 };

  std::string m_outbound_header;
  std::string m_outbound_data;
  char m_inbound_header[header_length];
  std::vector<char> m_inbound_data;
};

typedef boost::shared_ptr<tcp_connexion> connexion_ptr;
