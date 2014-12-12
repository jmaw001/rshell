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

static void handler(int signum)
{
        if(signal(signum, SIG_IGN)==SIG_ERR)
		perror("signals");
}
void execute(char* source[], char* used[])
{
	int j=0;
	char* holy[256]={0};
	while(used[j]!=NULL)
	{
		holy[j]= new char[256];
		memset(holy[j],0,256);
		strcpy(holy[j],used[j]);
		strcat(holy[j],"/");
		strcat(holy[j],source[0]);
		j++;
	}	
        int pid1;
        pid1=fork();
        if(pid1<0)
        {
                perror("fork");
                exit(1);
        }
        if(pid1==0)
        {
                if(signal(SIGINT, SIG_DFL)==SIG_ERR)
			perror("signals");
                int l=0;
		int errur=0;
		while(l<=j)
		{
			if((errur=execv(holy[l], source))!=-1)
				break;
			l++;
		}
		if(errur==-1)
		{
			l++;
			if(execv(holy[l], source)==-1)
				perror("execv");
		}
                exit(1);
        }
        if(pid1>0)
        {
                if(signal(SIGINT, handler)==SIG_ERR)
			perror("signals");
                if(wait(NULL)==-1)
                        perror("wait");
        }
}
void execute2(char* source[], char* used[])
{
	int j=0;
	char* holy[256]={0};
	while(used[j]!=NULL)
	{
		holy[j]= new char[256];
		memset(holy[j],0,256);
		strcpy(holy[j],used[j]);
		strcat(holy[j],"/");
		strcat(holy[j],source[0]);
		j++;
	}
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
                int l=0;
		int errur=0;
		while(l<=j)
		{
			if((errur=execv(holy[l], source))!=-1)
				break;
			l++;
		}
		if(errur==-1)
		{
			l++;
			if(execv(holy[l], source)==-1)
				perror("execv");
		}
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
void makearray(char** easy)
{
	char dot[256]={0};
	strcpy(dot, ".");
	char* pPath= getenv("PATH");
	easy[0]=dot;
	char* token;
	token=strtok(pPath, ":");
	easy[1]=token;
	int j=2;
	token=strtok(NULL, ":");
	while(token!=NULL)
	{
		easy[j]=token;
		j++;
		token=strtok(NULL, ":");
	}
	easy[j]=NULL;
}
int  main()
{
        cout << "Welcome to Jmaw001's rshell program! Please type in any bash commands!" << endl;
        string exite="exit";
        int original=0;
        vector <char*> lines(0);
        char cstring[1024];                     //getline cstring
        char * pch;                             //cstring for word parsing
        char *scapegoat;                        //cstring for comment parsing
        int i=0;                                //initializing variables 
        int lettercount=0;
        char * cmdlist[500];                    //declare argv for execv              
        char name[8];
        bool has=false;
        int pid;
        char pw[BUFSIZ];
        if(signal(SIGINT, handler)==SIG_ERR)
		perror("signals");
	char* hou[256]={0};
	makearray(hou);
        while(1)                                //infinite while loop for command shell till
        {                                       //user inputs exit
                for(int k=0;k<500;k++)
                {
                        cmdlist[k]=NULL;                //flushes argv with NULL so we can take in more cmds
                }
                if(getcwd(pw, BUFSIZ)==NULL)
                        perror("getcwd");
                if(getlogin()==NULL)            //just error messages for login and host
                        perror("The login is unretreivable therefore NULL");
                if(gethostname(name, 8)==-1)
                        perror("Error in hostname retreival!");
                cout << getlogin()  << "@" << name << pw <<  "$ ";      //extra credit!
                cin.getline(cstring, 1024);                     //takes input
                scapegoat=strtok(cstring, "#");                 //removes comments
                pch = strtok(scapegoat, " &;");                 //starts parsing into single words
                if(strcmp(pch,exite.c_str())==0)                //my exit function
                {
                        cout << "Good Bye!" << endl;
                        break;
                }
                while(pch[lettercount]!='\0'&&pch[lettercount]!=';'&&pch[lettercount]!='&')
                {
                        lettercount++;                          //gets word length to get
                        original++;
                }
                cmdlist[i]=pch;                                         //rid of the straggling connectors
                i++;
                lettercount=0;
                while(pch!=NULL)        //inner while loop parse rest of commands in the line.
                {
                        pch = strtok(NULL, " ");
                        if(pch==NULL)
                        {
                                if(strcmp(cmdlist[0],"cd")==0)
                                {
                                        char noob[]="./";
                                        strcat(noob, cmdlist[1]);
                                        if(chdir(noob)==-1)
						perror("change dir");
                                }
                                else
                                {	
                                        execute(cmdlist, hou);
                                }
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
				int j=0;
				char* hol[256]={0};
				while(hou[j]!=NULL)
				{
					hol[j]= new char[256];
					memset(hol[j],0,256);
					strcpy(hol[j],hou[j]);
					strcat(hol[j],"/");
					strcat(hol[j],cmdlist[0]);
					j++;
				}
				int l=0;
				int errar=0;
				while(l<=j)
				{
					if((errar=execv(hol[l], cmdlist))!=-1)
						break;
					l++;
				}
				if(errar==-1)
				{
					l++;
					if(execv(hol[l], cmdlist)==-1)
						perror("execv");
				}
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
                                                execute(cmdlist, hou);
                                        }
                                        else
                                        {
                                                execute2(cmdlist, hou);
                                        }
                                }
                                if(dup2(stdinput,0)==-1)
                                        perror("stdin restore failed");
                        }
                }
        }
        return 0;
}

