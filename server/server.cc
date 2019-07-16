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
#include <unordered_set>
#include <arpa/inet.h>
#include <chrono>
#include <ctime>


#define PORT 8080
#define MAXLINE 1024


std::unordered_set<std::string> user_cache;
const char* coordinates = "-1,-1,-1,1,1,1,1,-1";
bool calc_time = false;
auto start_time = std::chrono::system_clock::now();
auto end_time = std::chrono::system_clock::now();
std::chrono::duration<double> duration;

void udp_handler(int fdtn)
{

	char  buffer[MAXLINE];
	char str[] = "hello";
	char* ipstring;
	struct sockaddr_in cliaddr;
	int len;
	int hello_check;

	memset(&cliaddr, 0, sizeof(cliaddr));
	while(true)
	{

		int bytes_recv = recvfrom(fdtn, (char*)buffer, MAXLINE, MSG_WAITALL,
						(struct sockaddr*)&cliaddr, (socklen_t*)&len);
		hello_check = memcmp(buffer, str, sizeof(str));
		if(hello_check == 0)
		{
			sendto(fdtn,(const char*)coordinates,strlen(coordinates),MSG_CONFIRM,
					(const struct sockaddr*)&cliaddr,len);
			std::cout << "sending coordinates" << std::endl;
		}
		
		end_time = std::chrono::system_clock::now();
		duration = end_time - start_time;
		std::cout << "duration count: " <<  duration.count() << std::endl;
		std::cout << "end count: " <<  std::chrono::seconds(20).count() << std::endl;
		if(duration  > std::chrono::seconds(20))
		{
			std::cout << "time to reset" << std::endl;
			start_time = std::chrono::system_clock::now();
			calc_time = false;
			user_cache.clear();
			continue;
		}
		
		if(buffer[0] == '1')
		{
			ipstring = inet_ntoa(cliaddr.sin_addr);
			printf("ip address is %s\n", ipstring);
			std::string cache_str(ipstring);
			if(user_cache.find(cache_str) == user_cache.end())
			{
				std::cout << "added user !" << std::endl;
				user_cache.insert(cache_str);
			}
			if(calc_time = false)
			{
				calc_time = true;
				start_time = std::chrono::system_clock::now();
				std::cout << "time to start counting" << std::endl;
			}

		}
		
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

	std::cout << "making a thread" << std::endl;
	std::thread t1(udp_handler, sockfd);
	t1.join();

	return 0;
}
