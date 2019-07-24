#include <arpa/inet.h>
#include <arpa/inet.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <iostream>
#include <mutex>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <unordered_set>

constexpr int UDP_PORT = 8080;
constexpr int TCP_PORT = 8081;
constexpr int MAXLINE = 1024;

std::unordered_set<std::string> user_cache;
std::mutex cache_mutex;
const char *coordinates = "-1,-1,-1,1,1,1,1,-1";

void udp_handler() {
  char buffer[MAXLINE];
  char str[] = "hello";
  char *ipstring;
  struct sockaddr_in servaddr, cliaddr;
  int len;
  int hello_check;

  int sockfd;
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    std::cout << "can't make socket" << std::endl;
    exit(0);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(UDP_PORT);

  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    std::cout << "bind failed" << std::endl;
    exit(0);
  }

  while (true) {
    int bytes_recv = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
                              (struct sockaddr *)&cliaddr, (socklen_t *)&len);
    hello_check = memcmp(buffer, str, sizeof(str));
    if (hello_check == 0) {
      sendto(sockfd, (const char *)coordinates, strlen(coordinates),
             MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
      std::cout << "sending coordinates" << std::endl;
    }

    if (buffer[0] == '1') {
      ipstring = inet_ntoa(cliaddr.sin_addr);
      printf("udp_server thread: ip address is %s\n", ipstring);
      std::string cache_str(ipstring);
      if (user_cache.find(cache_str) == user_cache.end()) {
        std::lock_guard<std::mutex> guard(cache_mutex);
        std::cout << " udp_server thread: added user !" << std::endl;
        user_cache.insert(cache_str);
      }
    }

    buffer[bytes_recv] = '\0';
    printf("udp_server thread: string recv: %s\n", buffer);
  }
}

void time_handler() {
  auto start_time = std::chrono::system_clock::now();
  auto end_time = std::chrono::system_clock::now();
  std::chrono::duration<double> duration;

  while (true) {
    end_time = std::chrono::system_clock::now();
    duration = end_time - start_time;

    if (duration > std::chrono::seconds(20)) {
      std::lock_guard<std::mutex> guard(cache_mutex);
      std::cout << "time thread : time to reset " << std::endl;
      start_time = std::chrono::system_clock::now();
      user_cache.clear();
    }
  }
}

void tcp_handler() {
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (!sockfd) {
    std::cout << "socket creation failed" << std::endl;
    exit(0);
  }
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(TCP_PORT);

  if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) !=
      0) {
    std::cout << "binding failed" << std::endl;
    exit(0);
  }

  if (listen(sockfd, 5) != 0) {
    std::cout << "listen failed" << std::endl;
    exit(0);
  }
  while (true) {
    connfd = accept(sockfd, (struct sockaddr *)&cli, (socklen_t *)&len);
    if (connfd < 0) {
      std::cout << "accept failed" << std::endl;
    }
  }
}

int main() {
  std::thread t1(udp_handler);
  std::thread t2(time_handler);
  std::thread t3(tcp_handler);
  t1.join();
  t2.join();
  t3.join();

  return 0;
}
