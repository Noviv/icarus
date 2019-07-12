all: client server

client: client.cc
	@g++ -std=c++17 -O3 client.cc -o client

server: server.cc
	@g++ -std=c++17 -O3 server.cc -o server

format: client.cc server.cc $(which clang-format)
	@clang-format -i client.cc
	@clang-format -i server.cc

clean:
	@rm -f server client
