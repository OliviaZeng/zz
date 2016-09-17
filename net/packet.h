#ifndef __PACKET_H__
#define __PACKET_H__

#include <stdio.h>
#include <netinet/in.h>
#include <string.h>

#define BUFSIZE 8192

short get_cmd(void);
void reset_pkg(void);

void write_head(short cmd);
void write_end(void);
void write_int(int value);
void write_short(short value);
void write_byte(unsigned char  value);
void write_str(const char *str);

void read_head(void);
int read_int(void);
short read_short(void);
unsigned char read_byte(void);
void read_str(char *str);

#endif
