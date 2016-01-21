#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "tcp_server.hpp"
#include "tcp_connexion.hpp"

using namespace std;
using namespace boost::asio;

tcp_server::tcp_server(io_service& ios, int port) : m_acceptor(ios, ip::tcp::endpoint(ip::tcp::v4(), port)){
  start_accept();
}

void tcp_server::start_accept() {
  tcp_connexion::pointer new_connexion = tcp_connexion::create(m_acceptor.get_io_service());
  m_acceptor.async_accept(new_connexion->get_socket(), 
			  boost::bind(&tcp_server::handle_accept,
				      this,
				      new_connexion,
				      placeholders::error));
  
}

void tcp_server::handle_accept(tcp_connexion::pointer new_connexion, const boost::system::error_code& err) {
  if(!err) {
    cout << "Nouveau client !" << endl;
    new_connexion->start();
    start_accept();
  }
}
