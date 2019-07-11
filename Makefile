all: client server

client: client.cc
	@g++ -O3 client.cc -o client

server: server.cc
	@g++ -O3 server.cc -o server

clean:
	@rm -f server client
