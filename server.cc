#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <thread>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080

#define MAXLINE 1024
#define RECVBYTES 100



void udp_handler(int fdtn)
{

	char  buffer[RECVBYTES];
	struct sockaddr_in cliaddr;
	int len;
	memset(&cliaddr, 0, sizeof(cliaddr));
	while(true)
	{

		int bytes_recv = recvfrom(fdtn, (char*)buffer, RECVBYTES, MSG_WAITALL,
						(struct sockaddr*)&cliaddr, (socklen_t*)&len);
		buffer[bytes_recv] = '\0';
		printf("string recv: %s\n", buffer);

	}
}


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
	std::cout<<"now I'm recv" << std::endl;

	int str_check = 1;
	while(str_check != 0)
	{
		n = recvfrom(sockfd, (char*)buffer, MAXLINE, MSG_WAITALL,
		(struct sockaddr *) &cliaddr, (socklen_t*)&len);
		buffer[n] = '\0';
		printf("client : %s\n", buffer);
		str_check  = memcmp(buffer, str, sizeof(str));
		std::cout << str_check << std::endl;
		if(str_check == 0)
		{

			sendto(sockfd, (const char *)hello, strlen(hello),
					MSG_CONFIRM, (const struct sockaddr *)&cliaddr,
						len);
			std::cout << "i sent" << std::endl;	
		}
	}
	std::cout << "making a thread" << std::endl;
	std::thread t1(udp_handler, sockfd);
	t1.join();

	return 0;
}
