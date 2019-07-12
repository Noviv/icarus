#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    printf("Socket creation failure!\n");
    return 1;
  }

  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8080);
  servaddr.sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1");

  printf("Sending greeting...\n");
  const char *hello = "hello";
  sendto(sockfd, hello, strlen(hello), MSG_CONFIRM,
         (const struct sockaddr *)&servaddr, sizeof(servaddr));
  printf("Greeted server!\n");

  printf("Waiting for response...\n");
  int len;
  char buffer[1024];
  int n = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL,
                   (struct sockaddr *)&servaddr, (socklen_t *)&len);
  buffer[n] = 0;
  printf("Server responded: %s\n", buffer);

  close(sockfd);
  return 0;
}
