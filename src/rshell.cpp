#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

using namespace std;

void execute(char* source[])
{
	int pid1;
	pid1=fork();
	if(pid1<0)
	{
		perror("fork");
		exit(1);
	}
	if(pid1==0)
	{
		if(execvp(source[0], source)==-1)
			perror("execvp");
		exit(1);
	}
	if(pid1>0)
	{
		if(wait(NULL)==-1)
			perror("wait");
	}		
}
void execute2(char* source[])
{
	int fd[2];
	if(pipe(fd)==-1)
		perror("pipes");
	int pid1;
	pid1=fork();
	if(pid1<0)
		perror("fork");
	if(pid1==0)
	{
		if(-1==dup2(fd[1],1))
			perror("dup2 problem");
		//if(-1==close(fd[0]))
			//perror("cant close stdin");
		if(execvp(source[0], source)==-1)
			perror("execvp");
		exit(1);
	}
	if(pid1>0)
	{
		if(-1==dup2(fd[0],0))
			perror("cannot dup stdin");
		if(-1==close(fd[1]))
			perror("cant close stdout");
		if(wait(NULL)==-1)
			perror("wait");
	}	
}
int  main()
{
	cout << "Welcome to Jmaw001's rshell program! Please type in any bash commands!" << endl;
	string exite="exit";
	int original=0;
	vector <char*> lines(0);
	char cstring[1024];			//getline cstring
	char * pch;				//cstring for word parsing
	char *scapegoat;			//cstring for comment parsing
	int i=0;				//initializing variables 
	int lettercount=0;
	char * cmdlist[500];			//declare argv for execvp		
	char name[8];
	bool has=false;
	int pid;
	while(1)				//infinite while loop for command shell till
	{					//user inputs exit
		for(int k=0;k<500;k++)
		{
			cmdlist[k]=NULL;		//flushes argv with NULL so we can take in more cmds
		}
		if(getlogin()==NULL)		//just error messages for login and host
			perror("The login is unretreivable therefore NULL");
		if(gethostname(name, 8)==-1)
			perror("Error in hostname retreival!");
		cout << getlogin()  << "@" << name << "$ ";	//extra credit!
		cin.getline(cstring, 1024);			//takes input
		scapegoat=strtok(cstring, "#");			//removes comments
		pch = strtok(scapegoat, " &;");			//starts parsing into single words
		if(strcmp(pch,exite.c_str())==0)		//my exit function
		{
			cout << "Good Bye!" << endl;
			exit(1);
		}
          	while(pch[lettercount]!='\0'&&pch[lettercount]!=';'&&pch[lettercount]!='&')
            	{
               		lettercount++;				//gets word length to get
             		original++;
		}
		cmdlist[i]=pch;						//rid of the straggling connectors
            	i++;
		lettercount=0;
		while(pch!=NULL)        //inner while loop parse rest of commands in the line.
               	{
                     	pch = strtok(NULL, " ");
			if(pch==NULL)
			{
				execute(cmdlist);
				i=0;
			}
			else
			{
				if(strcmp(pch, "|")!=0)
				{
					while(pch[lettercount]!='\0'&&pch[lettercount]!=';'
					&&pch[lettercount]!='&')
                        		{
                               		 	lettercount++;
                        		}
                        		cmdlist[i]=pch;
					i++;
                        		lettercount=0;
				}
				else
				{
					has=true;
					break;
				}
			}
		}
		if(has)
		{
			int j=0;
			pch= strtok(NULL, "|");
			while(pch!='\0')
			{
				lines.push_back(pch);
				pch=strtok(NULL, "|");
			}
			int fd[2];
			if(pipe(fd)==-1)
				perror("Error in pipe");
			pid=fork();
			if(pid<0)
				perror("fork error");
			else if(pid==0)
			{
				if(dup2(fd[1],1)==-1)
					perror("dup2");
				if(-1==close(fd[0]))
					perror("close");
				if(-1==execvp(cmdlist[0], cmdlist))
					perror("execvp");
				exit(1);
			}
			else
			{
				int stdinput=dup(0);
				if(stdinput==-1)
					perror("dup");
				if(dup2(fd[0],0)==-1)
					perror("dup2");
				if(close(fd[1]))
					perror("close");
				if(wait(NULL)==-1)
					perror("wait");
				while(lines.size()!=0)
				{
					for(int i=0; i<500;i++)
						cmdlist[i]=NULL;
					j=0;
					pch=strtok(lines.at(0), " ");
					cmdlist[j]=pch;
					while(pch!=NULL)
					{
						pch=strtok(NULL, " ");
						j++;
						cmdlist[j]=pch;
					}
					lines.erase(lines.begin());
					if(lines.size()==0)
					{
						execute(cmdlist);
					}
					else
					{
						execute2(cmdlist);
					}
				}
				if(dup2(stdinput,0)==-1)
					perror("stdin restore failed");
			}
		}
	}
	return 0;
}
