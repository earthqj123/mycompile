#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include "lex.h"
int main(int argc,char*argv[])
{
	if(argc<2)
	{
		perror("less argument<enter the file pathname>");
		return -1;
	}
	char buffer[BUFF_SIZE];
	Mcfd* mcfd;
	int nbytes;
	if((mcfd=mc_open(argv[1],O_CREAT|O_TRUNC|O_RDWR,FILE_MODE))==NULL)
	{
		perror("mc_open error:");
		return -1;
	}
	if((nbytes=mc_predeal(mcfd,buffer,BUFF_SIZE))<0)
	{
		perror("predeal file error:");
		return -1;
	}
	return 0;
}
