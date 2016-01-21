#include <iostream>
#include <boost/asio.hpp>

#include "tcp_client.hpp"

using namespace std;
using namespace boost::asio;
		     	
int main() {

  try {
    io_service ios;
			
    ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8888);
    tcp_client client(ios, endpoint);
    ios.run();
  } catch(exception& e) {
    cerr << e.what() << endl;
  }
					
  return 0;
}
