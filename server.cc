#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>

#define PORT 8080
#define MAXLINE 1024

int main()
{
	int sockfd;
	char buffer[MAXLINE];
	const char *hello = "85768941";
	struct sockaddr_in servaddr, cliaddr;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		std::cout << "can't make socket" << std::endl;
		exit(0);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	if(bind(sockfd, (const struct sockaddr *)&servaddr,
				sizeof(servaddr)) < 0 )
	{
		std::cout <<"bind failed" << std::endl;
		exit(0);
	}

	int len, n;
	char str[] = "hello";
	while(true)
	{
		std::cout<<"now I'm recv" << std::endl;
		n = recvfrom(sockfd, (char*)buffer, MAXLINE, MSG_WAITALL,
			(struct sockaddr *) &cliaddr, (socklen_t*)&len);
		buffer[n] = '\0';
		printf("client : %s\n", buffer);
		int cv = memcmp(buffer, str, sizeof(str));
		std::cout << cv << std::endl;
		
		if(cv == 0)
		{

			sendto(sockfd, (const char *)hello, strlen(hello),
					MSG_CONFIRM, (const struct sockaddr *)&cliaddr,
					len);
		

			std::cout << "i sent" << std::endl;
		}
	}
	return 0;

}
		
