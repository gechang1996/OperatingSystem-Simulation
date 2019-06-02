/*
Chang Ge
Computer Project #2
This project is aiming at dealing with writing some data by given options 
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


#define MAX_BUFFER 512

//Those messages are used when the user enters a wrong option or wrong file name
char H_MESSAGE[256]="You have such options\n+t: Display Titles\n-t: Hide Titles\n-h: Display help messages\n\n";
char A_MESSAGE[256]="You did not enter any argument!\n\n";
char F_MESSAGE[256]="Fail to open a file!\n\n";
char O_MESSAGE[256]="You enter a wrong option!\nYou have such options\n+t: Display Titles\n-t: Hide Titles\n-h: Display help messages\n\n";
int main(int argc, char* argv[] )
{

  int fp;
//check if the user enter a parameter
  if (argc ==1)
  {
    write(2,A_MESSAGE,256);
  }

  else
  {
    char buffer[MAX_BUFFER];
 
    int gc=2;
//excute each parameter
    for (int i = 1; i<argc;i++)
    {
//if the parameter is a +t, which means show title and gc=1 will be used later
//for checking if we need title
        if (strcmp(argv[i],"+t")==0)
        {
          gc=1;
        }
        else if (strcmp(argv[i],"-t")==0)
        {
//this means hide titles
          gc=2;
        }
        else if (strcmp(argv[i],"-h")==0)
        {
//this option means diplay the help message
         write(2,H_MESSAGE,256);
       
        }
        else
        {
//check if the input is a valid option
          if (argv[i][0]=='+' || argv[i][0]=='-')
          {
          write(2,O_MESSAGE,256);
          }
          else
          {
//try to open file and if we could not open file, return error message
            fp=open(argv[i],O_RDONLY);
            if (fp == -1)
            {

                write(2,F_MESSAGE,256);
            }
            else
            {
//if the file is successfully opened, check if we need to show titles
                if(gc==1)
                {
//create s string cotaining colons
                  int title_len=strlen(argv[i]);
                  char colons[title_len+2]="";
                  char res[3*title_len+6]="";
                  for (int i=0;i<title_len;i++)
                  {
                    strcat(colons,":");
                  }
//combine colon string with the file name string
                  strcat(colons,"\n");
                  strcat(res,colons);
                  strcat(res,argv[i]);
                  strcat(res,"\n");
                  strcat(res,colons);
                  write(1,res,strlen(res));
                }
//display each line in the given file
                int text1=read(fp,buffer,MAX_BUFFER); 
                while (text1!=0)
                {
                  text1=read(fp,buffer,MAX_BUFFER);
                  write(1,buffer,strlen(buffer));
//reset the buffer to ensure it won't display extra codes when the size of the
//latter file is smaller thank the previous one.
                  memset(buffer,'\0',MAX_BUFFER);

                }
                
                write(1,"\n",1);
            }
            close(fp);

          }
        
        }
    }
  }
  return 0;
}
