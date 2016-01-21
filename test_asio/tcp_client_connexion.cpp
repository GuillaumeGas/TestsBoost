#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

#include "tcp_client_connexion.hpp"

typedef boost::shared_ptr<tcp_client_connexion> pointer;

tcp_client_connexion::tcp_client_connexion(io_service& ios) : m_sock(ios) {}

pointer tcp_client_connexion::create(io_service& ios) {
  return pointer(new tcp_client_connexion(ios));
}

ip::tcp::socket& tcp_client_connexion::get_socket() {
  return m_sock;
}

void tcp_client_connexion::read() {
  async_read(m_sock, buffer(m_buffer), transfer_at_least(20),
	     boost::bind(&tcp_client_connexion::handle_read, shared_from_this(),
			 placeholders::error,
			 placeholders::bytes_transferred)
	     );
}

void tcp_client_connexion::handle_read(const boost::system::error_code& err, size_t number_bytes_read) {
  if(!err) {
    cout.write(&m_buffer[0], number_bytes_read);
    read();
  } else {
    cout << err.message() << endl;
  }
}
