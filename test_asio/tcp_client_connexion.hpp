#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>

using namespace std;
using namespace boost::asio;

class tcp_client_connexion : public boost::enable_shared_from_this<tcp_client_connexion> {
public:
  typedef boost::shared_ptr<tcp_client_connexion> pointer;

  static pointer create(io_service& ios);
  ip::tcp::socket& get_socket();
  void read();

private:
  tcp_client_connexion(io_service& ios);
  void handle_read(const boost::system::error_code& err, size_t number_bytes_read);

  ip::tcp::socket m_sock;
  boost::array<char, 128> m_buffer;
};
