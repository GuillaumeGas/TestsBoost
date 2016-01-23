#include "chat_server.hpp"
#include "chat_session.hpp"

using namespace std;
using namespace boost::asio;

chat_server::chat_server(io_service& ios, const ip::tcp::endpoint& endpoint) : m_ios(ios), m_acceptor(ios, endpoint), m_room() {
  cout << "Creation du serveur." << endl;
  wait_for_connexion();
}

void chat_server::wait_for_connexion() {
  connexion_ptr new_connexion(new tcp_connexion(m_ios));

  m_acceptor.async_accept(new_connexion->get_socket(), 
			  boost::bind(&chat_server::handle_accept, this,
				      placeholders::error,
				      new_connexion)
			  );
}

void chat_server::handle_accept(const boost::system::error_code& error, connexion_ptr conn_ptr) {
  if(!error) {
    cout << "Connexion acceptee." << endl;
    chat_session_ptr session = chat_session::create(conn_ptr, m_room);
    m_room->join(session);
    wait_for_connexion();
  } else {
    cerr << "Connexion refusee." << endl;
    cerr << error.message() << endl;
  }
} 
