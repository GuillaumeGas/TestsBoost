#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>

using namespace boost::asio;

class tcp_connexion : public boost::enable_shared_from_this<tcp_connexion> {
public:
typedef boost::shared_ptr<tcp_connexion> pointer;
  static pointer create(io_service& ios);

  ip::tcp::socket& get_socket();
  void start();
  void do_read();

private:
  tcp_connexion(io_service& ios);
  void handle_write(const boost::system::error_code& err);
  void handle_read(const boost::system::error_code& err);
  void close();

  ip::tcp::socket m_sock;
  std::string m_msg;
  deadline_timer m_timer;
  boost::array<char, 128> m_buffer;
};
