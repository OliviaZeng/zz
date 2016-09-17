#include "packet.h"

int pos = 0;
char pkgbuf[BUFSIZE];
int pkglen = 0;

static void reset(void)
{
	pos = 0;
	pkglen = 0;
	memset(pkgbuf, 0, sizeof(pkgbuf));
}

static void write_data(const char *str, int len)
{
	if (len < 0 || pos+len > BUFSIZE)
		return;
	memcpy(pkgbuf+pos, str, len);
	pos += len;
}

static void read_data(char *data, int len)
{
	memcpy(data, pkgbuf+pos, len);
	pos += len;
}

short get_cmd(void)
{
	short cmd = 0;
	memcpy(&cmd, pkgbuf+6, sizeof(short));
	return ntohs(cmd);
}

void reset_pkg(void)
{
	reset();
}

void write_head(short cmd)
{
	short netcmd = htons(cmd);
	char version = 1;
	char subversion = 1;
	unsigned char code = 0;

	reset();
	memcpy(pkgbuf+2, "BY", 2*sizeof(char));
	memcpy(pkgbuf+4, &version, sizeof(char));
	memcpy(pkgbuf+5, &subversion, sizeof(char));
	memcpy(pkgbuf+6, &netcmd, sizeof(short));
	memcpy(pkgbuf+8, &code, sizeof(unsigned char));
	pos += 9;
}

void write_end(void)
{
	short npos = htons(pos-2);
	memcpy(pkgbuf, &npos, sizeof(npos));
}

void write_int(int value)
{
	int ndata = htonl(value);
	write_data((char*)&ndata, sizeof(int));
}

void write_short(short value)
{
	short ndata = htons(value);
	write_data((char*)&ndata, sizeof(short));
}

void write_byte(unsigned char value)
{
	write_data((char *)&value, sizeof(unsigned char));
}

void write_str(const char *str)
{
	int len = strlen(str);
	char ch = '\0';
	write_int(len);
	write_data(str, len);
	write_data(&ch, sizeof(char));
}


void read_head(void)
{
	short tmp = 0;
	memcpy(&tmp, pkgbuf, sizeof(short));
	pkglen = ntohs(tmp);
	pos += 9;
}

int read_int(void)
{
	int value = -1;
	read_data((char*)&value, sizeof(int));
	return ntohl(value);
}

short read_short(void)
{
	short value = -1;
	read_data((char*)&value, sizeof(short));
	return ntohs(value);
}

unsigned char read_byte(void)
{
	unsigned char value = -1;
	read_data((char*)&value, sizeof(unsigned char));
	return value;
}


void read_str(char *str)
{
	int strlen = read_int();
	memcpy(str, pkgbuf+pos, strlen+1);
	pos += strlen+1;
}
