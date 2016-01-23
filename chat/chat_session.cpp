#include <iostream>

#include "chat_session.hpp"
#include "chat_server.hpp"

using namespace std;
using namespace boost::asio;

chat_session::chat_session(connexion_ptr tcp_connexion, chat_room_ptr room) : m_tcp_connexion(tcp_connexion), m_room(room) {
  is_leaving = false;
  cout << "Nouvelle session." << endl;
}

chat_session::~chat_session() {
  cout << "Session detruite." << endl;
}

chat_session_ptr chat_session::create(connexion_ptr tcp_connexion, chat_room_ptr room) {
  chat_session_ptr session(new chat_session(tcp_connexion, room));
  session->wait_for_data();
  return session;
}

void chat_session::wait_for_data() {
  m_tcp_connexion->async_read(m_message, 
			      boost::bind(&chat_session::handle_read, 
					  shared_from_this(),
					  placeholders::error)
			      );
}

void chat_session::handle_read(const boost::system::error_code& error) {
  chat_room_ptr room = m_room.lock();
  if(room) {
    if(!error) {
      room->deliver(m_message);
      wait_for_data();
    } else {
      if(!is_leaving) {
	is_leaving = true;
	room->leave(shared_from_this());
      }
    }
  }
}

void chat_session::deliver(const chat_message& msg) {
  m_tcp_connexion->async_write(msg,
			       boost::bind(&chat_session::handle_write, 
					   shared_from_this(),
					   placeholders::error)
			       );
}

void chat_session::handle_write(const boost::system::error_code& error) {
  chat_room_ptr room = m_room.lock();
  if(room && error && (!is_leaving)) {
    is_leaving = true;
    room->leave(shared_from_this());
  }
}
