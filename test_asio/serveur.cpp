#include <boost/asio.hpp>
#include <iostream>

#include "tcp_server.hpp"

int main() {

  try {
    boost::asio::io_service ios;

    tcp_server server(ios, 8888);
    ios.run();
  } catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
