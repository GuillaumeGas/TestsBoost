#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "tcp_client.hpp"
#include "tcp_client_connexion.hpp"

using namespace std;
using namespace boost::asio;

tcp_client::tcp_client(io_service& ios, ip::tcp::endpoint& endpoint) : m_ios(ios) {
  connect(endpoint);
}

void tcp_client::connect(ip::tcp::endpoint& endpoint) {
  tcp_client_connexion::pointer new_connexion = tcp_client_connexion::create(m_ios);
  ip::tcp::socket& sock = new_connexion->get_socket();
  sock.async_connect(endpoint,
		       boost::bind(&tcp_client::handle_connect, this, new_connexion, placeholders::error)
		       );
}

void tcp_client::handle_connect(tcp_client_connexion::pointer new_connexion, const boost::system::error_code& err) {
  if(!err) {
    new_connexion->read();
  } else {
    cout << err.message() << endl;
  }
}
