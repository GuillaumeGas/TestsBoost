#include "tcp_connexion.hpp"

using namespace boost::asio;

tcp_connexion::tcp_connexion(io_service& ios) : m_sock(ios) {}

ip::tcp::socket& tcp_connexion::get_socket() {
  return m_sock;
}
