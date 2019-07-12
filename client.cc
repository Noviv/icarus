#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <vector>
#include <cstring>

constexpr auto PORT = 8080;

int main() {
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    std::cerr << "Socket creation failed!" << std::endl;
    return 1;
  }

  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1");

  std::cout << "Sending greeting..." << std::endl;
  const std::string hello = "hello";
  sendto(sockfd, hello.c_str(), hello.size(), MSG_CONFIRM,
         (const struct sockaddr *)&servaddr, sizeof(servaddr));
  std::cout << "Greeted server!" << std::endl;

  std::cout << "Waiting for response.." << std::endl;
  std::vector<char> buffer(1024);
  int n = recvfrom(sockfd, &buffer[0], buffer.size(), MSG_WAITALL,
                   nullptr, nullptr);
  buffer[n] = 0;

  std::string response(buffer.begin(), buffer.begin() + n);
  std::cout << "Server responded: " << std::move(response) << std::endl;

  close(sockfd);
  return 0;
}
