#include "client.h"
#include "server.h"
#include <functional>
#include <iostream>
#include <thread>
const int PORT = 8080;

enum ProgramType {server =0, client=1};

int main(int argc, char *argv[]) {
  if (argc <= 1){
    std::cerr << "Please specify program type\n"
              << "Usage:\n"
              << "[Execution File] [0(Server), 1(Client)]" << std::endl;
  }

  int programType = atoi(argv[1]);
  switch (programType)
  {
  case client: {
    std::string ip, name;
    std::cout << "Server ip address:";
    std::cin >> ip;
    std::cout << "Enter your name:" << std::flush;
    std::cin >> name;
    


    Client c(name, ip, PORT);
    c.Start();
    break;
  }
  case server:{
    int player_count;
    std::cin >> player_count;

    Server s(player_count, PORT);
    s.Start();
  }
  default:{
    // 提示使用者需要輸入正確的執行參數
    // -------------
    // =   區塊2   =
    // -------------
  }
}
