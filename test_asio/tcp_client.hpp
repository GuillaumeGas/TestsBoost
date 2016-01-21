#pragma once

#include <iostream>
#include <boost/asio.hpp>

#include "tcp_client_connexion.hpp"

using namespace boost::asio;

class tcp_client {
public:
  tcp_client(io_service& ios, ip::tcp::endpoint& endpoint);

private:
  void connect(ip::tcp::endpoint& endpoint);
  void handle_connect(tcp_client_connexion::pointer new_connexion, const boost::system::error_code& err);

  io_service& m_ios;
};
