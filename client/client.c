#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "packet.h"

extern char pkgbuf[];
extern int pos;

int main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("Should like this: ./client addr port\n");
		exit(1);
	}

	char *ip = argv[1];
	in_port_t port = atoi(argv[2]);
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket failed");
		exit(1);
	}

	struct sockaddr_in svr_addr;
	memset(&svr_addr, 0, sizeof(svr_addr));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port = htons(port);
	int ret = inet_pton(AF_INET, ip, &svr_addr.sin_addr.s_addr);
	if (0 == ret) {
		fputs("Illegal address", stderr);
		fputs("\n", stderr);
		exit(1);
	} else if (-1 == ret) {
		perror("error");
		exit(1);
	}

	if (connect(sock, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) < 0)
		perror("connect failed");

	write_head(0x6620);
	write_int(1234);
	write_short(56);
	write_byte(7);
	write_str("ab");
	write_end();
	ssize_t numbytes = send(sock, pkgbuf, pos, 0);

	close(sock);

	exit(0);
}
