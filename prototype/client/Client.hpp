#pragma once

#include <iostream>
#include <boost/asio.hpp>

#include "../common/TcpConnexion.hpp"
#include "../common/Packet.hpp"

using namespace boost::asio;

class Client {
public:
  Client(io_service& ios, std::string& name);
  ~Client();
  
  bool connect(ip::tcp::endpoint& endpoint);

private:
  void writeLoop();

  void handle_connect(const boost::system::error_code& err);
  void handle_write  (const boost::system::error_code& err);
  void handle_read   (Packet& packet, const boost::system::error_code& err);
  
  io_service m_ios;
  TcpConnexion::pointer m_connexion;

  std::string m_name;
  Packet m_packet;
};
