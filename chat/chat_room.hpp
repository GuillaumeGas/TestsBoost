#pragma once

#include <boost/asio.hpp>

#include "chat_session.hpp"

typedef boost::shared_ptr<chat_session> chat_session_ptr;

class chat_room {
public:
  chat_room();

  void join    (chat_session_ptr participant);
  void leave   (chat_session_ptr participant);
  void deliver (const chat_message& msg);

private:
  std::set<chat_session_ptr> m_participants;
};

typedef boost::shared_ptr<chat_room> chat_room_ptr;
typedef boost::weak_ptr<chat_room>   chat_room_wptr;
