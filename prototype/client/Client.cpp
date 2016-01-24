#include "Client.hpp"
#include "../common/TcpConnexion.hpp"

using namespace std;
using namespace boost::asio;

Client::Client(io_service& ios, string& name) : m_ios(ios), m_name(name) {}

bool Client::connect(ip::tcp::endpoint& endpoint) {
  m_connexion = TcpConnexion::create(m_ios);
  ip::tcp::socket& sock = m_connexion->get_socket();
  sock.async_connect(endpoint,
		     boost::bind(&Client::handle_connect,
				 this,
				 m_connexion,
				 placeholders::error)
		     );
}

void Client::writeLoop() {
  while(1) {
    m_message.clear();
    getline(cin, m_message);
    m_connexion->async_write(m_message, 
			     boost::bind(&Client::handle_write,
					 this,
					 placeholders::error)
			     );
  }
}

void Client::handle_connect(const boost::system::error_code& err) {
  if(!err) {
    m_connexion->async_write(m_name, 
			       boost::bind(&Client::handle_write,
					   this,
					   placeholders::error)
			       );
    m_connexion->async_read(boost::bind(&Client::handle_read, this, m_paquet, placeholders::error));
    writeLoop();
  } else {
    cout << "Erreur lors de la connexion." << endl;
    cout << err.message() << endl;
  }
}

void Client::handle_write(const boost::system::error_code& err) {
  if(err) {
    cout << "Erreur lors de l'envoie du message." << endl;
    cout << err.message() << endl;
  }
}

void Client::handle_read(string& packet, const boost::system::error_code& err) {
  if(!err) {
    cout << paquet << endl;
  } else {
    cout << "Erreur reception du paquet." << endl;
    cout << err.message() << endl;
  }
}
