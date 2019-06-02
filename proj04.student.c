/*Chang Ge
Computer Project #4
This project is aiming at doing some actions like terminal.
* We need to check if an action is an external command or internal command.
* For internal command, we do cd,env,date,cwd and quit
* For external command, we check if we need to run the program in the background.
* 
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
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_SIZE 128

int main(int argc,char* argv[],char* envp[])
{
	
	
//run the program until the user input "quit"
  for(int i=1;i>0;i++)
  { 
    char str[MAX_SIZE]; //used for getting the line we input.
    char str2[MAX_SIZE];//used for getting the path for cd command.
    char str3[MAX_SIZE];//used for getting the date string.
	
    char cwd[MAX_SIZE];//used for getting the cwd string
    int count;//used for displaying all of the env data
    int result=false;//used for checking if we need to end the while loop
    int repeated=false;//used for checking if we need to add one to <i userid>
    char* userid=getlogin();//get userid
    char* token;//used for getting each token
    char* token2;//copy one token



    printf("<%d %s>",i,userid);//print the prompt line
    fgets(str,MAX_SIZE,stdin);//get the input line
    
    token=strtok(str," \t\n");
//if the input is tab or space, sequence number would not change.
    if (token ==NULL)
    {
	  i=i-1;
      continue;
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
// check if we need to set the directory to the abs path
// if there is a ~, then yes.
        else
        {
          token2=strtok(NULL," \n\t");
          if(token2==NULL)
          {            
            if(token[0]=='~')
            {
              memset(str2,'\0',MAX_SIZE); 
			//remove ~ from the token
              memmove(token,token+1,strlen(token));
             //if there is nothing in the token. then set the directory to the home directory
              if (strcmp(token,"")==0)
              {
				  strcat(token,userid);
			  }
              strcat(str2,"/user/");
              strcat(str2,token);
              int my_user=chdir(str2);
              //if successful, then show a message to the user.
              if(my_user==0)
              {
                  printf("Set directory to %s.\n",str2);
              }
              //if fail, also show a message to the user
              else
              {
                  printf("Fail to set the current working directory to be %s.\n",str2);
              }      
            }
            // This is setting the directory to the current path.
            //if successful, then show a message to the user.
            else if (chdir(token) ==0)
            {
            printf("Set directory to %s.\n",token);
            
            }
			//if fail, also show a message to the user
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
		  
		  
		  char *cmd=token;  //This is the command string
		  char *argv[MAX_SIZE];//This is the argv which should put into the function execvp
		  int countargv=0;//This is used for adding things into argv
		  char lastitem[MAX_SIZE];//This is the last token, which is used for check if it is a &
		  int stat;//This is a status member
		  memset(argv,'\0',MAX_SIZE); 
		  memset(lastitem,'\0',MAX_SIZE); 
		  //add command and its arguments to the argv without the &
		  while (token!=NULL)
			  {
				  //get the lastitem
				  strcpy(lastitem,token);

				  if(strcmp(token,"&")!=0)
				  {
					argv[countargv]=token;
					countargv+=1;
				  }
				  token=strtok(NULL," \n\t");
			  }
			//fork the program
		  pid_t cpid=fork();
		  //this is the child program
		  if(cpid ==0)
		  {
			  //run the execvp and check if it success
			  if(execvp(cmd,argv)<0)
			  {
				  printf("%s: Command not found.\n",cmd);
			  }
			  exit(EXIT_SUCCESS);
		  }
		  // this is the parent program
		  else if(cpid >0)
		  {
			  //check if the last item is &, if it is ,then run the child program in the background
			  if(strcmp(lastitem,"&")!=0)
			  {
			  cpid=waitpid(cpid,&stat,0);
				
			  }


			  memset(argv,'\0',MAX_SIZE); 
			  //print failure message if the program fail.
			  if (WIFSIGNALED(stat))
			  {
				  printf("Child terminated abnormally,signal %d\n",WTERMSIG(stat));
			  }

		  }
		  else
		  {
			  //error message
			 perror("waitpid");
			 exit(EXIT_FAILURE);
		  } 

      }
      

    }
//check if we need to break the loop
    if (result==true)
    {
      break;
    }

 
   memset(str,'\0',MAX_SIZE); 

  }	



}
