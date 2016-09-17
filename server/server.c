#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "packet.h"

#define MAXPENDING 5
extern char pkgbuf[];                                                                                                                             
extern int pos;

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("argc error\n");
		exit(0);
	}

	in_port_t port = atoi(argv[1]);

	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		perror("socket() failed");
		exit(1);
	}

	struct sockaddr_in svr_addr;
	memset(&svr_addr, 0, sizeof(struct sockaddr_in));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port = htons(port);
	svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sock, (struct sockaddr*)&svr_addr, sizeof(svr_addr)) < 0) {
		perror("bind failed");
		exit(1);
	}

	if (listen(sock, MAXPENDING) < 0) {
		perror("listen failed");
		exit(1);
	}


	while(1) {
		struct sockaddr_in cli_addr;
		socklen_t cli_addr_len = sizeof(cli_addr);
		int clisock = accept(sock, (struct sockaddr*)&cli_addr, &cli_addr_len);
		if (clisock < 0) {
			perror("accept failed");
			exit(1);
		}

		char cli_name[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, cli_name, sizeof(cli_name)) != NULL)
			printf("client %s %d\n", cli_name, ntohs(cli_addr.sin_port));

		reset_pkg();
		ssize_t numbytes = recv(clisock, pkgbuf, BUFSIZE, 0);
		read_head();
		short cmd = get_cmd();
		int int_v = read_int();
		short short_v = read_short();
		unsigned char char_v = read_byte();
		char ch[10];
		read_str(ch);

		printf("cmd:0x%x, intvalue:%d, short_v%d, char_v:%d, str=%s\n", cmd, int_v, short_v, char_v, ch);

		close(clisock);
	}
}
