/*Chang Ge
Computer Project #3
This project is aiming at doing some actions like terminal.
*/



#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#define MAX_SIZE 128

int main(int argc,char* argv[],char* envp[])
{
//run the program until the user input "quit"
  for(int i=1;i>0;i++)
  { 
    char str[MAX_SIZE];
    char str2[MAX_SIZE];
    char str3[MAX_SIZE];
    char cwd[MAX_SIZE];
    int count;
    int p=0;
    int result=false;
    int repeated=false;
    char* userid=getlogin();
    char* token;
    char* token2;
    printf("<%d %s>",i,userid);
    fgets(str,MAX_SIZE,stdin);
    
    token=strtok(str," \t\n");
//if the input is tab or space, sequence number would not change.
    if (token ==NULL)
    {
      repeated=true;
    }
//using space or table to get each parameter
    while(token !=NULL)
    {
//if the input is quit, break the whole loop
      if (strcmp(token,"quit")==0)
      {
        result=true;
        break;
      }
//if the input is "env", output some env variables
      else if (strcmp(token,"env")==0)
      {
        token=strtok(NULL," \n\t");
//use the token to find the env variable
        if (token!=NULL)
        {
          while (token!=NULL)
          {        
          printf("%s: %s\n",token,getenv(token));
          token=strtok(NULL," \n\t");
          }
        }
//if the input is simple emv(without token),display all environment variables
        else
        {
          for (count=0;envp[count]!=NULL;count++)
          {
            printf("%s\n",envp[count]);
          }

        }
      
      }
//if the token is "date",display the date and time
      else if (strcmp(token,"date")==0)
      {
        
        token=strtok(NULL," \n\t");
        if(token==NULL)
        {

        time_t timer;
        time(&timer);
        struct tm* tm_info=localtime(&timer);
        strftime(str3,26,"%Y-%m-%d %H:%M:%S\n",tm_info);
        printf("Date: %s",str3);
        break;
        }
//check if the number of argument is 1.
        else
        {
          printf("You entered too many argumenets\n");
          break;
        }
       }
//if the input is "cwd",display abs pathname of current working directory
      else if (strcmp(token,"cwd")==0)
      {
        token=strtok(NULL," \n\t");
        if(token==NULL)
        {
        getcwd(cwd,MAX_SIZE);
        printf("cwd: %s\n",cwd);
        break;
        }
//check if the number of argument is 1.
        else
        {
          printf("You entered too many arguments\n");
          break;
        }
      }
//if the input is "cd", manage the current working directory
      else if (strcmp(token,"cd")==0)
      {
        token=strtok(NULL," \n\t");
//if there is no token, manage the current directory to home directory
        if (token==NULL)
        {
          chdir(getenv("HOME"));
          printf("Set directory to home directory.\n");
        }
// if there is a token,manage the current directory to it.
        else
        {
          token2=strtok(NULL," \n\t");
          if(token2==NULL)
          {
          if (chdir(token) ==0)
          {
            printf("Set directory to %s.\n",token);
          }
          else
          {
            printf("Fail to set the current working directory to be $s.\n",token);
          }
          }
//check how many argument it has.
          else
          {
            printf("You entered too many arguments\n");
          }
        }
        break;
      }
//error message
      else
      {
        printf("%s: Command not found.\n",token);
        break;
      }
      

    }
//check if we need to break the loop
    if (result==true)
    {
      break;
    }
//check if we need to add one to sequence number.
    if (repeated==true)
    {
      i=i-1;
    }

 
   memset(str,'\0',MAX_SIZE); 
    
  }



}
