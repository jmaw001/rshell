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

int  main()
{
	string exite="exit";
	char cstring[1024];			//getline cstring
	char * pch;				//cstring for word parsing
	char *scapegoat;			//cstring for comment parsing
	int i=0;				//initializing variables 
	int lettercount=0;
	char * argv[500];			//declare argv for execvp
	int pid;				//forking man!
	char name[8];
	while(1)				//infinite while loop for command shell till
	{					//user inputs exit
		for(int k=0;k<500;k++)
		{
			argv[k]=NULL;		//flushes argv with NULL so we can take in more cmds
		}
		if(getlogin()==NULL)		//just error messages for login and host
			perror("The login is unretreivable therefore NULL");
		if(gethostname(name, 8)==-1)
			perror("Error in hostname retreival!");
		cout << getlogin()  << "@" << name << "$ ";	//extra credit!
		cin.getline(cstring, 1024);			//takes input
		scapegoat=strtok(cstring, "#");			//removes comments
		pch = strtok(scapegoat, " ");			//starts parsing into single words
		if(strcmp(pch,exite.c_str())==0)		//my exit function
		{
			cout << "Good Bye!" << endl;
			exit(1);
		}
          	while(pch[lettercount]!='\0'&&pch[lettercount]!=';'&&pch[lettercount]!='|'
            	&&pch[lettercount]!='&')
            	{
               		lettercount++;				//gets word length to get
             	}						//rid of the straggling connectors
              	argv[i]=new char[lettercount+1];		//uses length to make new argv[i]
             	strncpy(argv[i],pch,lettercount);
            	i++;
		lettercount=0;
		while(pch!=NULL)        //inner while loop parse rest of commands in the line.
               	{
                     	pch = strtok(NULL, " ");
			if(pch==NULL)
			{
				pid=fork();
                              	if(pid<0)
                               	{
                       			perror("Problem in fork. Please fix");
                             	}
                              	else if(pid==0)
                               	{
                    	                if(-1==(execvp(argv[0],argv)))
                                    	{
                                      		perror("Please fix the execvp problem");
                                   	}
                               	}
                              	else if(pid>0)
                               	{
                                	if(wait(NULL)==-1)
                                      	perror("The wait() doesn't work");
					for(int k=0;k<500;k++)
					{
						argv[k]=NULL;
					}
					i=0;
                            	}
			}
			else
			{
				while(pch[lettercount]!='\0'&&pch[lettercount]!=';'
				&&pch[lettercount]!='|'&&pch[lettercount]!='&')
                        	{
                                	lettercount++;
                        	}
                        	argv[i]=new char[lettercount+1];
                        	strncpy(argv[i],pch,lettercount);
                        	i++;
                        	lettercount=0;
			}
		}
	}
	return 0;
}
