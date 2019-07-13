#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <vector>
#include <cstring>
#include <array>

constexpr auto PORT = 8080;
constexpr auto IP = "34.83.142.125";

// flags
static bool DEBUG = false;

// global network config
static int sockfd;
static struct sockaddr_in servaddr;

// parsing structures
struct Coordinate {
  int x, y;
};

// parse CLI options
// true == error
void parse_opts(int argc, char* argv[]) {
  for (int i = 0; i < argc; i++) {
    if (std::string(argv[i]).compare("--debug") == 0) {
      DEBUG = true;
    }
  }
}

// initialize network configuration // true == error
bool init_network() {
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    std::cerr << "Socket creation failed!" << std::endl;
    return true;
  }

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = inet_addr(IP);
  return false;
}

// send greeting to server
std::array<Coordinate, 4> greet() {
  std::cout << "Sending greeting..." << std::endl;
  const std::string hello = "1";
  sendto(sockfd, hello.c_str(), hello.size(), MSG_CONFIRM,
         (const struct sockaddr *)&servaddr, sizeof(servaddr));
  std::cout << "Greeted server!" << std::endl;

  std::cout << "Waiting for response..." << std::endl;
  std::vector<char> buffer(1024);
  int n = recvfrom(sockfd, &buffer[0], buffer.size(), MSG_WAITALL,
                   nullptr, nullptr);
  buffer[n] = 0;

  std::string response(buffer.begin(), buffer.begin() + n);
  std::cout << "Server responded: " << std::move(response) << std::endl;

  return {{{}, {}, {}, {}}};
}

int main(int argc, char* argv[]) {
  parse_opts(argc, argv);
  if (init_network()) {
    return 1;
  }

  auto coordinates = greet();

  close(sockfd);
  return 0;
}
