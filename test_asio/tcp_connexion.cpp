#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "tcp_connexion.hpp"

using namespace boost::asio;
using namespace std;

typedef boost::shared_ptr<tcp_connexion> pointer;

pointer tcp_connexion::create(io_service& ios) {
  return pointer(new tcp_connexion(ios));
}

tcp_connexion::tcp_connexion(io_service& ios) : m_sock(ios), m_timer(ios, boost::posix_time::seconds(5)) {}

ip::tcp::socket& tcp_connexion::get_socket() { 
  return m_sock; 
}

void tcp_connexion::do_read() {
  async_read(m_sock, buffer(m_buffer),
	     boost::bind(&tcp_connexion::handle_read, shared_from_this(),
			 placeholders::error));
  m_timer.expires_from_now(boost::posix_time::seconds(5));
  m_timer.async_wait(boost::bind(&tcp_connexion::close, shared_from_this()));
}

void tcp_connexion::start() {
  m_msg = "Bienvenue sur le serveur !";

  async_write(m_sock, 
	      buffer(m_msg), 
	      boost::bind(&tcp_connexion::handle_write,
			  shared_from_this(),
			  placeholders::error)
	      );
}

void tcp_connexion::handle_write(const boost::system::error_code& err) {
  if(!err) {
    do_read();
  } else {
    cout << err.message() << endl;
  }
}

void tcp_connexion::handle_read(const boost::system::error_code& err) {
  if(!err) {
    do_read();
  } else {
    cout << err.message() << endl;
    close();
  }
}

void tcp_connexion::close() {
  m_sock.close();
}


