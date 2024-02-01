#include "client.h"
#include "CLI.h"
#include "util.h"
#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

sockaddr_in addr(std::string ipString,int port);

Client::Client(std::string name, std::string serverIp, int port):
  name(name),
  server_addr(addr(serverIp, port)){
    InitConnection();
}

sockaddr_in addr(std::string ipString,int port){
    sockaddr_in addr;
    memset(&addr, '0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ipString.c_str());
    if(inet_pton(AF_INET, ipString.c_str(), &addr.sin_addr) <= 0){
      throw "Invalid address/ Address not supported";
    }
    return addr;
}


void Client::InitConnection(){
  std::cout << "Creating client socket" << std::endl;
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    throw "Socket creation error";
  }

  std::cout << "Connecting to server..." << std::endl;
  if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    throw "Connection failed";
  }

  send(socket_fd, name.c_str(), name.length(), 0);
}

void Client::Loop() {
  // string buffer for I/O with server
  char *buf = new char[1024];
  bool running = true;
  // I&O loop
  while (running) {
    // read message from server
    memset(buf, 0, 1024);
    recv(socket_fd, buf, 1024, 0);

    // string for further manipulations
    std::string str(buf);
    str = str.substr(1);
    std::cout << "str:" << str << std::endl;
    std::vector<std::string> ss = Util::string_split(str, ',');

    // init
    if (buf[0] == 'i') {
      std::cout << "Your id is " << ss[0] << std::endl;
      this->id = ss[0].c_str()[0] - '0';
      CLI::client_id = this->id;
      for (int i = 1; i < ss.size(); ++i) {
        CLI::all_player_name.push_back(ss[i]);
      }
      std::cout << ss.size() << std::endl;
      write(socket_fd, "ready", 5);
      continue;
    }

    // end action
    else if (buf[0] == 'e') {
      CLI::print_end(ss[0].c_str()[0] - '0');
      running = false;
      break;
    }
    // update action
    else if (buf[0] == 'u') {

      CLI::print_update(ss);

      // your turn
      if (this->id == ss[0].c_str()[0] - '0') {

        int index = CLI::play_card(ss[2].length() / 2);

        std::string command;
        if (index == -1) {
          command = "d";
        } else {
          command = ss[2].substr(index * 2, 2);
        }
        // wild card
        if (command[0] == 'w' || command[0] == 'f') {
          char color = CLI::get_wild_color();
          command[1] = color;
        }
        std::cout << "[Test] command=" << command << std::endl;
        // send messages to server
        send(socket_fd, command.c_str(), command.length(), 0);
      }
    }
  }

  close(socket_fd);
}


void Client::Start() {
  try {
    Loop();
  } catch (const char *err) {
    std::cerr << err << std::endl;
  }
}
