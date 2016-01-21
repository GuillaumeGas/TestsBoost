#pragma once

#include <iostream>
#include <boost/asio.hpp>

#include "tcp_connexion.hpp"

using namespace boost::asio;

class tcp_server {
public:
  tcp_server(io_service& ios, int port);

private:
  void start_accept();
  void handle_accept(tcp_connexion::pointer new_connexion, const boost::system::error_code& err);

  ip::tcp::acceptor m_acceptor;
};
