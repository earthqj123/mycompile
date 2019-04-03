#include "lex.h"
#include "lock.h"
char buf[BUFF_SIZE];
static const char* const tokens[]={
	"and","break","do","else","elseif",
	"end","false","for","function","goto",
	"if","in","local","nil","not","or","repeat",
	"return","then",true,"until","while","//",
	"..","...","==",">=","<=","~=","<<",">>","::",
	"<eof>",
	"<number>","<integer>","<name>","<string>"
};

bool isdigit(const char c)
{
	if(c>='0'&&c<='9')
		return true;
	else
		return false;
}
bool isletter(const char c)
{
	if((c>='a'&&c<='z')||(c>="A"&&c<="Z"))
		return true;
	else
		return false;
}
	/* skip space in the buf */
char *skipspace(char *buf)
{
	while(*buf==' '||*buf=='\t'||*buf=='\r')
	{
		buf++;
	}
	return buf;
}
	/* skip the note or remark in the file */
char *skipnote(char *buf)
{
	char *s=malloc(3);
	*(s+2)='\0';
	if(*buf=='/')
	{
		if((s=strncpy(s,buf,2))==NULL)
		{
			perror("strncpy str error:");
			free(s);
			return buf;
		}
		if(strcmp(s,"//")==0)
		{
			buf+=2;
			free(s);
			if((buf=strchr(buf,'\n'))==NULL)
				return buf;
			else
				return buf+1;
		}
		else if(strcmp(s,"/*")==0)
		{
			buf+=2;
			free(s);
			while(*buf!='\0'&&*(buf+1)!='\0')
			{
				if(*buf=='*'&&*(buf+1)=='/')
					break;
				else
					buf++;
			}
			if(*buf=='\0'||*(buf+1)=='\0')
				return buf;
			else
				return (buf+2);
		}
	}
	return buf;
}
/*open file xxx.c(or other language) and creat a file xxx.c(or other suffix).yy*/
Mcfd* mc_open(const char *pathname,int oflag,int mode)
{
	Mcfd *mcfd;
	int len;
	char *name;
	mcfd=malloc(sizeof(struct Mcfd));
	mcfd->fd=-1;
	mcfd->prefd=-1;
		/* open original file:O_RDWR*/
	if((mcfd->fd=open(pathname,O_RDWR,mode))<0)
	{
		perror("open file error:");
		return -1;
	}
	len=strlen(pathname);
	if((name=malloc(len+4))==NULL)
	{
		perror("malloc space error；");
		return -1;
	}
	strcpy(name,pathname);
	strcat(name,".yy");
	if((mcfd->prefd=open(name,oflag,mode))<0)
	{
		perror("open file error:");
		return -1;
	}
		/*first time creat file */
/*	if((oflag&(O_CREAT|O_TRUNC))==(O_CREAT|O_TRUNC))
	{
			 
		if(writew_lock(mcfd->prefd,0,SEEK_SET,0)<0)
		{
			perror("write lock error:");
			return -1;
		}
	}*/
		/*file has been creat*/
	free(name);
	return mcfd;
}
	/* close file */
Mcfd* mc_close(Mcfd* mcfd)
{
	if(close(mcfd->fd)<0)
	{
		perror("close file error:");
		return -1;
	}
	if(close(mcfd->prefd)<0)
	{
		perror("close file error:");
		return -1;
	}
}
	/* clear space and note */
int mc_predeal(Mcfd* mcfd,void *buff,size_t nbytes)
{
	int bytes=0,m;
	char *ptr,*ptrb,*buffer;
	memset(buff,0,nbytes);
	while((m=read(mcfd->fd,buff,nbytes))>0)
	{
		ptr=buff;
		int i=0,n;
		if((buffer=malloc(m))==NULL)
		{
			perror("malloc space error:");
			return -1;
		}
		ptrb=buffer;
		while(*ptr!='\0')
		{
			if(*ptr==' '||*ptr=='\r'||*ptr=='\t')
			{
				if((ptr=skipspace(ptr))==NULL)
				{
					perror("skip space error:");
					return -1;
				}
			}
			if(*ptr=='/')
			{
				ptr=skipnote(ptr);
			}
			*ptrb=*ptr;
			if((ptrb-buffer)<m-1)
			{
				ptrb++;
			}
			ptr++;
			i++;
		}
		if((n=write(mcfd->prefd,buffer,i))!=i)
		{
			perror("write file error:");
			return -1;
		}
		free(buffer);
		bytes+=n;
		memset(buff,0,nbytes);
	}
	return bytes;
}
