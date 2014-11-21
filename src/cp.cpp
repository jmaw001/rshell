#include <iostream>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
        if(argv[3][0]=='1')
	{
		ifstream inFile;
		ofstream outFile(argv[2]);
		inFile.open(argv[1]);
		if(!inFile.is_open())
		{
			perror("Input file was not opened");
		}
		char c='0';
		while(inFile.get(c))
		{
			outFile.put(c);
			c='0';
		}
		inFile.close();
	}
	else if(argv[3][0]=='2')
	{
		int r;
		int w;
		if((r=open(argv[1], 0))<0)
			perror("Error in opening read file");
		if((w=creat(argv[2],00700))<0)
			perror("Error in opening write file");
		char c='0';
		int n=1;
		while(read(r,&c, n)!=0)
		{
			write(w,&c, n);
			c='0';
		}
		if(close(r)<0)
			perror("Error closing read file");
		if(close(w)<0)
			perror("Error closing write file");
	}
	else if(argv[3][0]=='3')
	{
		int r;
		int w;
		if((r=open(argv[1], 0))<0)
			perror("Error in opening read file");
		if((w=creat(argv[2],00700))<0)
			perror("Error in opening write file");
		char c[BUFSIZ];
		for(int i =0; i<BUFSIZ;i++)
		{
			c[i]='\0';
			i++;
		}
		int n=BUFSIZ;
		while(read(r,c, n)!=0)
		{
			write(w,c, n);
			for(int i =0; i<BUFSIZ;i++)
			{
				c[i]='\0';
				i++;
			}
		}
		if(close(r)<0)
			perror("Error closing read file");
		if(close(w)<0)
			perror("Error closing write file");
	}
	return 0;
}

