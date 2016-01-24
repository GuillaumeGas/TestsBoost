#include <iostream>
#include <boost/asio.hpp>

#include "Client.hpp"

using namespace std;
using namespace boost::asio;

/* 
   On va tenter une connexion avec le serveur.
   Si elle échoue, on affiche la liste des clients pour tenter une connexion.
   On pourra aussi tenter une connexion avec chaque pour n'afficher que ceux en ligne.
*/
int main(int argc, char** argv) {

  string name;
  cout << "Pseudo : ";
  cin >> name;

  io_service ios;
  Client c(name, ios);

  //tentative avec le serveur
  ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 8888);
  if(c.connect(endpoint)) {
    ios.run();
  } else {
    cout << "Serveur inaccessible." << endl;
    cout << "On affiche la liste des clients, on propose une autre connexion." << endl;
  }

  return 0;
}
