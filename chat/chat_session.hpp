#pragma once

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "tcp_connexion.hpp"
#include "chat_message.hpp"

using namespace boost::asio;

class chat_server;
class chat_room;
class chat_session;
typedef boost::shared_ptr<chat_session> chat_session_ptr;
typedef boost::shared_ptr<chat_room>    chat_room_ptr;
typedef boost::weak_ptr<chat_room>      chat_room_wptr;

class chat_session : public boost::enable_shared_from_this<chat_session> {
public:
  ~chat_session();

  static chat_session_ptr create(connexion_ptr tcp_connexion, chat_room_ptr room);

  void deliver(const chat_message& msg);

private:
  chat_session(connexion_ptr tcp_connexion, chat_room_ptr room);
  void wait_for_data();

  void handle_write(const boost::system::error_code& error);
  void handle_read(const boost::system::error_code& error);

  connexion_ptr m_tcp_connexion;
  chat_room_wptr m_room;
  chat_message m_message;

  bool is_leaving;
};
