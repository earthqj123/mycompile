#ifndef LEX_H
#define LEX_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#define BUFF_SIZE 8192
#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
typedef struct Mcfd{
	int fd;
	int prefd;
} Mcfd;
typedef enum{
	false,true
}bool;
bool isdigit(const char c);
bool isletter(const char c);
char *skipspace(char *buf);
char *skipnote(char *buf);
Mcfd* mc_open(const char *pathname,int oflag,int mode);
Mcfd* mc_close(Mcfd* mcfd);
int mc_predeal(Mcfd* mcfd,void *buff,size_t nbytes);
#endif
