#ifndef CLIENT_H
#define CLIENT_H
#include <netinet/in.h>
#include <string>
#include <vector>
class Client {

public:
  Client(std::string name,std::string serverIp, int port);
  void Start();

private:
  void Loop();
  void Print();
  void InitConnection();

  // for displaying
  const std::string name;
  const std::string server_ip;
  const sockaddr_in server_addr;

  // identity in the game
  int id;

  
  int socket_fd;
};
#endif
