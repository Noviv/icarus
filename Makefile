all: client server

client: client.cc
	g++ client.cc -o client

server: server.cc
	g++ server.cc -o server
