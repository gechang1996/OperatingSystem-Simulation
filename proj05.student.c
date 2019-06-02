/*Computer Project #5
Author:Chang Ge
This project is aiming at creating producer and consumer
* Each producer could produce the number from 1 to 100 and thread ID
* Each number and ID will be stroed in the buffer if the buffer has space to store
* Each consumer will free the space of a buffer and read the record
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
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
typedef long int thread_id;
#define BUFFER_SIZE 20 //Max buffer size
#define MAX_SIZE 128	//Max string size
#define INSIDE 125	//Inside time(for cirtical section)
#define OUTSIDE 250	//Outside time
void *producer(void*);//Create number and put it into buffer
void *consumer(void*);//get records from buffer and remove this record
void consume();//get records from buffer and remove this record
void produce();//Create number and put it into buffer
char* buf[MAX_SIZE];//create buffer
sem_t full_sem,empty_sem,cs_sem;//create semaphores

int first=0;//place for putting new record
int last=0;//place for removing record
int count;//number of total produce prgress

void produce(int j)
{
		//create random number 
		int randomNum1=rand();
		//check if it's larger than 250(OUTSIDE).If so, the progress should run later
		if (randomNum1 %1000>OUTSIDE)
		{
			//move the thread to the end of the queue
			sched_yield();
		}
		//wait for the signal for adding things into the buufer
		sem_wait(&full_sem);
		//wait for the signal for entering critical section
		sem_wait(&cs_sem);
		//check if it's larger than 125(INSIDE).If so, the progress should run later
		if (randomNum1 %1000>INSIDE)
		{
			//move the thread to the end of the queue
			sched_yield();
		}
		//create a string for storing ID and Value
		char* str = (char*) calloc(128, sizeof(char));
		thread_id pthreadId=pthread_self();
		//saving this record into the string
		sprintf(str,"Thread id: %ld. Value: %d",pthreadId,j);
		
		//add the string to the buffer
		buf[first]=str;

		//next time, record will be added to the next place
		first+=1;
		first=first%20;
		//post the signal that the critical section is end
		sem_post(&cs_sem);
		//post the signal that there is a record can be removed
		sem_post(&empty_sem);
		
		

}

void consume()
{
	
		//create random number 
		int randomNum2=rand();
		//check if it's larger than 250(OUTSIDE).If so, the progress should run later
		if (randomNum2 %1000>OUTSIDE)
		{
			//move the thread to the end of the queue
			sched_yield();
		}
		//wait for the signal to remove record and read record.
		sem_wait(&empty_sem);
		//wait for the signal for entering critical section
		sem_wait(&cs_sem);
		//check if the producer is done, if so, I need to set a signal to run the rest consumer
		if(count == 0)
		{
			//post the signal that the critical section is end
			sem_post(&cs_sem);	
			//post the signal that allows other consumer in
			sem_post(&empty_sem);
			//end the thread
			pthread_exit(NULL);
		}
		//check if it's larger than 125(INSIDE).If so, the progress should run later
		if (randomNum2 %1000>INSIDE)
		{
			//move the thread to the end of the queue
			sched_yield();
		}
		
		//print out the record
		printf("Record: %s\n",buf[last]);
		//delete it.
		free(buf[last]);
		//next time, the consumer will remove and read next record
		last=last+1;
		last=last%20;
		//count how many times the consume runs
		count-=1;
		//post a signal that the critcal section has end
		sem_post(&cs_sem);
		
		if(count == 0)
		{
			//let other consumers in
			sem_post(&empty_sem);	
			pthread_exit(NULL);
		}
		else
		{
			//let producer in
			sem_post(&full_sem);
		}



}

void *producer(void*)
{
	//create 100 records
	for (int j=0;j<100;j++)
	{
		//produce record
		produce(j);

	}
	
	pthread_exit(NULL);
}

void *consumer(void*)
{
	//run until all of the producer finished
	while (count!=0)
	{
		//consume record
		consume();
		
	}
	pthread_exit(NULL);
	
}


int main(int argc,char* argv[])
{
	//Initialize the random number 
	srand(time(NULL));
	//Initialize semaphores
	sem_init(&cs_sem,0,1);
	sem_init(&empty_sem,0,0);
	sem_init(&full_sem,0,BUFFER_SIZE);
	
	long int pNumber=1;//number to run the producer
	long int cNumber=1;//number to run the consumer
	long int testNumber;//check the argument
	if(argc>5 || argc==4 || argc==2)
	{
		printf("You enter wrong arguments!\n");
		return 0;
	}
	if (argc==1)
	{
		pNumber=1;
		cNumber=1;
	}
	else if (argc==3)
	{
		testNumber=strtol(argv[2],(char**)NULL,10);
		//I need to check if the third argument is a number
		if (testNumber<1 || testNumber>5)
		{
			printf("Input wrong number!\n");
			return 0;
		}
		//I need to check if the second one is a option
		if (strcmp(argv[1],"-p")==0)
		{
			pNumber=strtol(argv[2],(char**)NULL,10);
		}
		else if (strcmp(argv[1],"-c")==0)
		{
			cNumber=strtol(argv[2],(char**)NULL,10);
		}
		else
		{
			printf("You enter wrong arguments!\n");
			return 0;
		}
	}
	else if (argc==5)
	{
		//check if the third argument is a number
		testNumber=strtol(argv[2],(char**)NULL,10);
		if (testNumber<1 || testNumber>5)
		{
			printf("Input wrong number!\n");
			return 0;
		}
		//check if the fifth argument is a number
		testNumber=strtol(argv[4],(char**)NULL,10);
		if (testNumber<1 || testNumber>5)
		{
			printf("Input wrong number!\n");
			return 0;
		}
		//I need to check if the second one or forth one is a option
		if(strcmp(argv[1],"-c")==0 && strcmp(argv[3],"-p")==0)
		{
			cNumber=strtol(argv[2],(char**)NULL,10);
			pNumber=strtol(argv[4],(char**)NULL,10);
		}
		else if (strcmp(argv[1],"-p")==0 && strcmp(argv[3],"-c")==0)
		{
			pNumber=strtol(argv[2],(char**)NULL,10);
			cNumber=strtol(argv[4],(char**)NULL,10);
		}
		else
		{
			printf("You enter wrong arguments!\n");
			return 0;
		}
	}
	//set the total numebr for running the consume function
	count=100*pNumber;
	//create two thread. One for consumer, one for producer
	pthread_t pThread[pNumber],cThread[cNumber];
	
	
	for (int p=0;p<pNumber;p++)
	{
		int check_t=pthread_create(&pThread[p],NULL,producer,NULL);
		if (check_t)
		{
			printf("Fail to ceate the thread!\n");
			exit(EXIT_FAILURE); 
		}
	}
	for (int c=0;c<cNumber;c++)
	{
		int check_t=pthread_create(&cThread[c],NULL,consumer,NULL);
		if (check_t)
		{
			printf("Fail to ceate the thread!\n");
			exit(EXIT_FAILURE); 
		}
	}
	
	for (int p1=0;p1<pNumber;p1++)
	{
		int check_j=pthread_join(pThread[p1],NULL);
		if (check_j)
		{
			printf("Fail to join the thread!\n");
			exit(EXIT_FAILURE); 
		}
	}
	for (int c1=0;c1<cNumber;c1++)
	{
		int check_j=pthread_join(cThread[c1],NULL);
		if (check_j)
		{
			printf("Fail to join the thread!\n");
			exit(EXIT_FAILURE); 
		}
	}
	
	return 0;
}
