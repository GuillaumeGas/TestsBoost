#pragma once

#include <boost/asio.hpp>

#include "tcp_connexion.hpp"
#include "chat_room.hpp"

using namespace boost::asio;

class chat_server {
public:
  chat_server(io_service& ios, const ip::tcp::endpoint& endpoint);
  void wait_for_connexion();

private:
  void handle_accept(const boost::system::error_code& error, connexion_ptr conn_ptr);

  io_service& m_ios;
  ip::tcp::acceptor m_acceptor;
  chat_room_ptr m_room;
};
