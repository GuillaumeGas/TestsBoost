#include <iostream>

#include "chat_room.hpp"
#include "chat_session.hpp"

using namespace std;
using namespace boost::asio;

chat_room::chat_room() {
  cout << "Nouvelle salle." << endl;
}

void chat_room::join(chat_session_ptr participant) {
  m_participants.insert(participant);

  chat_message e;
  e.type = chat_message::PERSON_CONNECTED;
  deliver(e);
}

void chat_room::leave(chat_session_ptr participant) {
  chat_message e;
  e.type = chat_message::PERSON_LEFT;
  deliver(e);

  m_participants.erase(participant);
}

void chat_room::deliver(const chat_message& msg) {
  std::for_each(m_participants.begin(), m_participants.end(), boost::bind(&chat_session::deliver, _1, boost::ref(msg)));
}
