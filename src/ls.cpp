#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <pwd.h>
#include <time.h>

using namespace std;

vector <char*> sorting(vector <char*> source)
{
	vector <char*> destination;
	int n;
	char* least;
	while(source.size()!=0)
	{
		n=0;
		least=source.at(0);
		for(int i=0;i<source.size();i++)
		{
			if(strlen(least)>strlen(source.at(i)))
			{
				least=source.at(i);
				n=i;
			}
		}
		source.erase(source.begin()+n);
		destination.push_back(least);
	}
	return destination;
}
void printl(vector <char*> source)
{
	struct stat thisfile;
	if(stat(".", &thisfile)<0)
		perror("failed to stat");
	cout << "Total " << thisfile.st_blocks << endl;
	for(int i=0;i<source.size();i++)
	{
		struct stat file;
		char str[80]="./";
		strcat(str, source.at(i));
		if(stat(str, &file)<0)
		{
			perror("Failed to get stat");
		}
		//struct passwd *getpwuid(file.st_uid);
		printf(S_ISDIR(file.st_mode)? "d" : "-" );
		printf((file.st_mode & S_IRUSR) ? "r" : "-");
		printf((file.st_mode & S_IWUSR) ? "w" : "-");
		printf((file.st_mode & S_IXUSR) ? "x" : "-");
		printf((file.st_mode & S_IRGRP) ? "r" : "-");
		printf((file.st_mode & S_IWGRP) ? "w" : "-");
		printf((file.st_mode & S_IXGRP) ? "x" : "-");
		printf((file.st_mode & S_IROTH) ? "r" : "-");
		printf((file.st_mode & S_IWOTH) ? "w" : "-");
		printf((file.st_mode & S_IXOTH) ? "x" : "-");
		cout << "  " << file.st_nlink << "  " << file.st_uid << "  " << file.st_gid << "  " << file.st_size;
		/*char mb[100];
		time_t t= file.st_mtime;
		struct tm timez;
		localtime_r(&t, &timez);
		strftime(mb, sizeof(mb), "%m  %d  %H:%M" ,  timez);*/
		cout << "  "  << source.at(i) << endl;
	}
}
int main(int argc, char* argv[])
{
	vector <char*> files;
	vector <char*> flags;
	vector <char*> dirfiles;
	vector <char*> sortedfiles;
	bool flaga=false;
	bool flagl=false;
	bool flagR=false;
	char directory[]=".";
	int i=0;
	while(argv[i]!=NULL)
	{
		i++;
	}
	i--;
	while(argv[i][0]!='l')
	{
		if(argv[i][0]=='/')
		{
			files.push_back(argv[i]);
		}
		else if(argv[i][0]=='-')
		{
			flags.push_back(argv[i]);
		}
		i--;
	}
	for(int k=0;k<flags.size();k++)
	{
		int j=0;
		while(flags.at(k)[j]!='\0')
		{
			if(flags.at(k)[j]=='a')
				flaga=true;
			else if(flags.at(k)[j]=='l')
				flagl=true;
			else if(flags.at(k)[j]=='R')
				flagR=true;
			j++;
		}
	}
	cout << flaga << flagl << flagR << endl;
	if(files.size()==0)
	{
		DIR *dirp;
		dirent *dp;
		if((dirp=opendir("."))=='\0')
		{
			perror("Could not open directory");
		}
		do
		{
			errno=0;
			if((dp=readdir(dirp))!='\0')
			{
				if(!flaga)
				{
					if(dp->d_name[0]!='.')
						dirfiles.push_back(dp->d_name);
				}
				else
					dirfiles.push_back(dp->d_name);
			}
		}while(dp!=NULL);
		closedir(dirp);
		sort(dirfiles.begin(), dirfiles.end());
		if(flagR)
		{
			
		}
		else if(flagl)
		{
			printl(dirfiles);
		}
		else
		{
			for(int k=0;k<dirfiles.size();k++)
			{
				cout << dirfiles.at(k) << "  ";
			}
			cout << endl;
		}
	}
	else if(files.size()==1)
	{
		DIR *dirp;
                dirent *dp;
                if((dirp=opendir(files.at(0)))=='\0')
                {
                        perror("Could not open directory");
                }
                do
                {
                        errno=0;
                        if((dp=readdir(dirp))!='\0')
                        {
                                if(!flaga)
                                {
                                        if(dp->d_name[0]!='.')
                                                dirfiles.push_back(dp->d_name);
                                }
                                else
                                        dirfiles.push_back(dp->d_name);
                        }
                }while(dp!=NULL);
                closedir(dirp);
		if(flagR)
		{
		
		}
		else if(flagl)
		{
			printl(dirfiles);
		}
		else
		{
			for(int k=0; k<dirfiles.size();k++)
			{
				cout << dirfiles.at(k) << "  ";
			}
			cout << endl;
		}
	}
	else if(files.size()>1)
	{
		sortedfiles=sorting(files);
		for(int i=0;i<sortedfiles.size();i++)
		{
			cout << sortedfiles.at(i) << ":" << endl;
			DIR *dirp;
	                dirent *dp;
        	        if((dirp=opendir(sortedfiles.at(i)))=='\0')
        	        {
       		                perror("Could not open directory");
                	}
                	do
                	{
                        	errno=0;
                        	if((dp=readdir(dirp))!='\0')
                        	{
                                	if(!flaga)
                                	{
                                        	if(dp->d_name[0]!='.')
                                                	dirfiles.push_back(dp->d_name);
                                	}
                                	else
                                        	dirfiles.push_back(dp->d_name);
                        	}
                	}while(dp!=NULL);
                	closedir(dirp);
			if(flagR)
			{
			
			}
			else if(flagl)
				printl(dirfiles);
			else
			{
				for(int k=0;k<dirfiles.size(); k++)
				{
					cout << dirfiles.at(k) << "  ";
				}
				cout << endl;
			}
			dirfiles.clear();
		}
	}
	return 0;
}
