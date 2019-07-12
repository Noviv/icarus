#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

int main() {
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    std::cerr << "Socket creation failed!" << std::endl;
    return 1;
  }

  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8080);
  servaddr.sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1");

  std::cout << "Sending greeting..." << std::endl;
  const std::string hello = "hello";
  sendto(sockfd, hello.c_str(), hello.size(), MSG_CONFIRM,
         (const struct sockaddr *)&servaddr, sizeof(servaddr));
  std::cout << "Greeted server!" << std::endl;

  std::cout << "Waiting for response.." << std::endl;
  int len;
  std::string buffer;
  buffer.reserve(1024);
  int n = recvfrom(sockfd, (char *)buffer.data(), 1024, MSG_WAITALL,
                   (struct sockaddr *)&servaddr, (socklen_t *)&len);
  buffer[n] = 0;
  std::cout << "Server responded: " << buffer << std::endl;

  close(sockfd);
  return 0;
}
