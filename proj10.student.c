/*Chang Ge
Computer Project #10
*The program will simulate the steps that an operating
*system takes to manage user processes.  The operating 
*system uses a five-state process model:  New, Ready,
* Running, Blocked, and Exit. 
* 
*/
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <errno.h>
#include <iomanip>
#include <algorithm>
using namespace std;





enum PCBItem
{
	Id=0,
	Priority,
	CPU_Num,
	BurstTime,
	BlockedTime,
	ArrivalTime,
	DepartureTime,
	TotalReadyTime,
	TotalRunningTime,
	TotalBlockedTime,
	
};



//copy of all of the process
vector<vector<int>> CP_AP;

//all of the process
vector<vector<int>> AllProcess;
//new
vector<vector<int>> NewState;//(inner vector)the first item is id, the second is time still need to be in the new state
//Ready
vector<int> ReadyState;//id of each process
//Timer for item in ready state
vector<int> ReadyTimer;
//Running
vector<int> RunningState; //only one process could exist in the running state
//Blocked
vector<vector<int>> BlockedState;//(inner vector)the first item is id,the second is the time still need to be in the blocked state
//Exit
vector<int> ExitState;//id of each process
//check when to terminate
bool Flag=false;
//check which pcb i need
int pcb_num=0;
//algorithm of the programe
string algorithm="";
//max process time for the inner process
int TimeQuantum;
//total inner process time
int InitialTime;

//check if the input string is positive integer
bool IsPositiveInt(string);
//Get the pcb by its priority
int GetByPri();
//process by FCFS,argument is total ticks
void Do_FCFS(int);
//process by Priority,argument is total ticks
void Do_Priority(int);
//process by RR,argument is total ticks
void Do_RR(int);




//check if is integer
bool IsPositiveInt(string ss)
{
		for (unsigned int j =0;j<ss.length();j++)
		{
			if (isdigit(ss[j])==false)
			{
				return false;
			}
		}
	
	return true;
}

//Get the pcb by its priority
int GetByPri()
{
	int id =ReadyState[0];
	for (auto ready_id:ReadyState)
	{
		if (AllProcess[ready_id-1][Priority]<AllProcess[id-1][Priority])
		{
			id=ready_id;
		}
		
	}
	return id;
}

//process by FCFS,argument is total ticks
void Do_FCFS(int TotalTimes)
{
	for (int tick=0;tick<TotalTimes;tick++)
		{
			cout<<"Tick num: "<<tick<<endl;
			
			
			//erase vector for block
			vector<vector<int>> TotalErase2;
			int count=0;
			for (auto item2:BlockedState)
			{
				//id of item2
				int id_block=item2[0];
				AllProcess[id_block-1][TotalBlockedTime]+=1;
				
				
				BlockedState[count][1]-=1;
				item2[1]-=1;
				count+=1;
				//check if the time is 0, if 0,remove it later
				if (item2[1]==0)
				{
					vector<int> erase2;
					ReadyState.push_back(id_block);
					erase2.push_back(id_block);
					erase2.push_back(0);
					TotalErase2.push_back(erase2);
					cout<<"ID: "<<id_block<<"     Block ==> Ready"<<endl;
					erase2.clear();
				}
				
			}
			
			for (auto erase_b:TotalErase2)
			{
				BlockedState.erase(find(BlockedState.begin(),BlockedState.end(),erase_b));
			}
					
			TotalErase2.clear();
			
			
			
			
			//total erase vector for newstate	
			vector<vector<int>> TotalErase1;
			int count2=0;
			for (auto item1:NewState)
			{
				//item1 is each PCB in the NewState
				int my_id=item1[0];//id
				if(item1[1]==0)
				{
					
					//the vector we need to delete
					vector<int> erase1;
					erase1.push_back(my_id);
					erase1.push_back(0);
					//switch from newstate to the ready state
					TotalErase1.push_back(erase1);
					cout<<"ID: "<<my_id<<"     New ==> Ready"<<endl;
					erase1.clear();
					ReadyState.push_back(my_id);
					
					
					
				}
				
				else
				{
					
					NewState[count2][1]-=1;
				}
				count2+=1;
				
			}
			for (auto erase_a:TotalErase1)
			{
				NewState.erase(find(NewState.begin(),NewState.end(),erase_a));
			}
			TotalErase1.clear();
			
			
			
			
			if (RunningState.size()!=0)
			{
				//id for running item
				int id_running=RunningState[0];
				AllProcess[id_running-1][TotalRunningTime]+=1;
				RunningState[1]-=1;
				//check if there still have running time in running state
				if(RunningState[1]==0)
				{
					RunningState.clear();
					AllProcess[id_running-1][CPU_Num]-=1;
					//check if the process finishes
					if (AllProcess[id_running-1][CPU_Num]==0)
					{
						ExitState.push_back(id_running);
						cout<<"ID: "<<id_running<<"     Running ==> Exit"<<endl;
						AllProcess[id_running-1][DepartureTime]=tick;
						Flag=true;
						pcb_num=id_running;
						
						
					}
					else
					{
						//block vector we need to add into blockstate
						vector<int> block2;
						block2.push_back(id_running);
						block2.push_back(AllProcess[id_running-1][BlockedTime]);
						BlockedState.push_back(block2);
							
						
						cout<<"ID: "<<id_running<<"     Running ==> Block"<<endl;
					}
				}
				
				
				
			}
			
			
			if(RunningState.size()==0)
			{
				
				if(ReadyState.size()!=0)
				{
					//id for ready item
				
					int id_ready=ReadyState[0];
					
					int burst_time=AllProcess[id_ready-1][BurstTime];

					
					//first element of the running state is the id
					RunningState.push_back(id_ready);
					//second element of the runing state is the time it still need to stay in the state
					RunningState.push_back(burst_time);
					cout<<"ID: "<<id_ready<<"     Ready ==> Running"<<endl;
					ReadyState.erase(ReadyState.begin());
				}
					
				
			}
			//Set the total ready time
			for (auto id: ReadyState)
			{
				AllProcess[id-1][TotalReadyTime]+=1;
			}
			
			
			
			
			//display the pcb which are in block state
			cout<<"ReadyState: ";
			for (auto item4:ReadyState)
			{
				cout<<item4<<", ";
			}
			cout<<endl;
			
			
			//display running state
			cout<<"RunningState: ";
			if (RunningState.size()!=0)
			{
				cout<<RunningState[0];
			}
			cout<<endl;
			
			//display the pcb which are in block state
			cout<<"BlockState: ";
			for (auto item3:BlockedState)
			{
				cout<<item3[0]<<", ";
			}
			cout<<endl;
			
			
			
			if(Flag==true)
			{
				cout<<"-----------------"<<endl;
				cout<<"process terminate"<<endl;
				cout<<"-----------------"<<endl;
				cout<<"ID: "<<pcb_num<<endl;
				cout<<"Priority: "<<CP_AP[pcb_num-1][Priority]<<endl;
				cout<<"Number of Burst: "<<CP_AP[pcb_num-1][CPU_Num]<<endl;
				cout<<"Burst Time: "<<CP_AP[pcb_num-1][BurstTime]<<endl;
				cout<<"Blocked Time: "<<CP_AP[pcb_num-1][BlockedTime]<<endl;
				cout<<"Arrival Time: "<<CP_AP[pcb_num-1][ArrivalTime]<<endl;
				
				
				cout<<"Departure time:  "<<AllProcess[pcb_num-1][DepartureTime]<<endl;
				cout<<"Cumulative ready time:  "<<AllProcess[pcb_num-1][TotalReadyTime]<<endl;
				cout<<"Cumulative running time:  "<<AllProcess[pcb_num-1][TotalRunningTime]<<endl;
				cout<<"Cumulative blocked time:  "<<AllProcess[pcb_num-1][TotalBlockedTime]<<endl;
				cout<<"Turn around time:  "<<AllProcess[pcb_num-1][TotalReadyTime]+AllProcess[pcb_num-1][TotalRunningTime]+AllProcess[pcb_num-1][TotalBlockedTime]<<endl;
				cout<<"Normalized turn around time:  "<< fixed << setprecision(2) <<float(AllProcess[pcb_num-1][TotalReadyTime]+AllProcess[pcb_num-1][TotalRunningTime]+AllProcess[pcb_num-1][TotalBlockedTime])/float(AllProcess[pcb_num-1][TotalRunningTime])<<endl;
				
				Flag=false;
			}
			
			cout<<endl;
				
		}
	
}


//process by Priority,argument is total ticks
void Do_Priority(int TotalTimes)
{
	for (int tick=0;tick<TotalTimes;tick++)
		{
			cout<<"Tick num: "<<tick<<endl;
			
			
			//erase vector for block
			vector<vector<int>> TotalErase2;
			int count=0;
			for (auto item2:BlockedState)
			{
				//id of item2
				int id_block=item2[0];
				AllProcess[id_block-1][TotalBlockedTime]+=1;
				
				
				BlockedState[count][1]-=1;
				item2[1]-=1;
				count+=1;
				//check if the time is 0, if 0,remove it later
				if (item2[1]==0)
				{
					vector<int> erase2;
					ReadyState.push_back(id_block);
					erase2.push_back(id_block);
					erase2.push_back(0);
					TotalErase2.push_back(erase2);
					cout<<"ID: "<<id_block<<"     Block ==> Ready"<<endl;
					erase2.clear();
				}
				
			}
			
			for (auto erase_b:TotalErase2)
			{
				BlockedState.erase(find(BlockedState.begin(),BlockedState.end(),erase_b));
			}
					
			TotalErase2.clear();
			
			
			
			
			//total erase vector for newstate	
			vector<vector<int>> TotalErase1;
			int count2=0;
			for (auto item1:NewState)
			{
				//item1 is each PCB in the NewState
				int my_id=item1[0];//id
				if(item1[1]==0)
				{
					
					//the vector we need to delete
					vector<int> erase1;
					erase1.push_back(my_id);
					erase1.push_back(0);
					//switch from newstate to the ready state
					TotalErase1.push_back(erase1);
					cout<<"ID: "<<my_id<<"     New ==> Ready"<<endl;
					erase1.clear();
					ReadyState.push_back(my_id);
					
					
					
				}
				
				else
				{
					
					NewState[count2][1]-=1;
				}
				count2+=1;
				
			}
			for (auto erase_a:TotalErase1)
			{
				NewState.erase(find(NewState.begin(),NewState.end(),erase_a));
			}
			TotalErase1.clear();
			
			
			
			
			if (RunningState.size()!=0)
			{
				//id for running item
				int id_running=RunningState[0];
				AllProcess[id_running-1][TotalRunningTime]+=1;
				RunningState[1]-=1;
				//check if there still have running time in running state
				if(RunningState[1]==0)
				{
					RunningState.clear();
					AllProcess[id_running-1][CPU_Num]-=1;
					//check if the process finishes
					if (AllProcess[id_running-1][CPU_Num]==0)
					{
						ExitState.push_back(id_running);
						cout<<"ID: "<<id_running<<"     Running ==> Exit"<<endl;
						AllProcess[id_running-1][DepartureTime]=tick;
						Flag=true;
						pcb_num=id_running;
						
						
					}
					else
					{
						//block vector we need to add into blockstate
						vector<int> block2;
						block2.push_back(id_running);
						block2.push_back(AllProcess[id_running-1][BlockedTime]);
						BlockedState.push_back(block2);
							
						
						cout<<"ID: "<<id_running<<"     Running ==> Block"<<endl;
					}
				}
				
				
				
			}
			
			
			if(RunningState.size()==0)
			{
				
				if(ReadyState.size()!=0)
				{
					//id for ready item
				
					int id_ready=GetByPri();
					
					int burst_time=AllProcess[id_ready-1][BurstTime];

					
					//first element of the running state is the id
					RunningState.push_back(id_ready);
					//second element of the runing state is the time it still need to stay in the state
					RunningState.push_back(burst_time);
					cout<<"ID: "<<id_ready<<"     Ready ==> Running"<<endl;
					ReadyState.erase(find(ReadyState.begin(),ReadyState.end(),id_ready));
				}
					
				
			}
			//Set the total ready time
			for (auto id: ReadyState)
			{
				AllProcess[id-1][TotalReadyTime]+=1;
			}
			
			
			
			
			//display the pcb which are in block state
			cout<<"ReadyState: ";
			for (auto item4:ReadyState)
			{
				cout<<item4<<", ";
			}
			cout<<endl;
			
			
			//display running state
			cout<<"RunningState: ";
			if (RunningState.size()!=0)
			{
				cout<<RunningState[0];
			}
			cout<<endl;
			
			//display the pcb which are in block state
			cout<<"BlockState: ";
			for (auto item3:BlockedState)
			{
				cout<<item3[0]<<", ";
			}
			cout<<endl;
			
			
			
			if(Flag==true)
			{
				cout<<"-----------------"<<endl;
				cout<<"process terminate"<<endl;
				cout<<"-----------------"<<endl;
				cout<<"ID: "<<pcb_num<<endl;
				cout<<"Priority: "<<CP_AP[pcb_num-1][Priority]<<endl;
				cout<<"Number of Burst: "<<CP_AP[pcb_num-1][CPU_Num]<<endl;
				cout<<"Burst Time: "<<CP_AP[pcb_num-1][BurstTime]<<endl;
				cout<<"Blocked Time: "<<CP_AP[pcb_num-1][BlockedTime]<<endl;
				cout<<"Arrival Time: "<<CP_AP[pcb_num-1][ArrivalTime]<<endl;
				
				
				cout<<"Departure time:  "<<AllProcess[pcb_num-1][DepartureTime]<<endl;
				cout<<"Cumulative ready time:  "<<AllProcess[pcb_num-1][TotalReadyTime]<<endl;
				cout<<"Cumulative running time:  "<<AllProcess[pcb_num-1][TotalRunningTime]<<endl;
				cout<<"Cumulative blocked time:  "<<AllProcess[pcb_num-1][TotalBlockedTime]<<endl;
				cout<<"Turn around time:  "<<AllProcess[pcb_num-1][TotalReadyTime]+AllProcess[pcb_num-1][TotalRunningTime]+AllProcess[pcb_num-1][TotalBlockedTime]<<endl;
				cout<<"Normalized turn around time:  "<< fixed << setprecision(2) <<float(AllProcess[pcb_num-1][TotalReadyTime]+AllProcess[pcb_num-1][TotalRunningTime]+AllProcess[pcb_num-1][TotalBlockedTime])/float(AllProcess[pcb_num-1][TotalRunningTime])<<endl;
				
				Flag=false;
			}
			
			cout<<endl;
				
		}
	
}

//process by RR,argument is total ticks
void Do_RR(int TotalTimes)

{
	for (int tick=0;tick<TotalTimes;tick++)
		{
			cout<<"Tick num: "<<tick<<endl;
			
			
			//erase vector for block
			vector<vector<int>> TotalErase2;
			int count=0;
			if (RunningState.size()!=0)
			{
				if(TimeQuantum==0)
				{
					if(ReadyState.size()!=0)
					{
						
						//id for running item
						int id_running=RunningState[0];
						//the burst time remaining
						int burst_time_remain=RunningState[1];
						
						ReadyState.push_back(id_running);
						ReadyTimer.push_back(burst_time_remain);
						cout<<"ID: "<<id_running<<"     Running ==> Ready"<<endl;
						//id for ready item
						int id_ready=ReadyState[0];
						
						int burst_time=ReadyTimer[0];

						RunningState.clear();
						//first element of the running state is the id
						RunningState.push_back(id_ready);
						//second element of the runing state is the time it still need to stay in the state
						RunningState.push_back(burst_time);
						cout<<"ID: "<<id_ready<<"     Ready ==> Running"<<endl;
						ReadyState.erase(ReadyState.begin());
						ReadyTimer.erase(ReadyTimer.begin());
					}
					TimeQuantum=InitialTime;//reset the timer

				}
			}
			for (auto item2:BlockedState)
			{
				//id of item2
				int id_block=item2[0];
				AllProcess[id_block-1][TotalBlockedTime]+=1;
				
				
				BlockedState[count][1]-=1;
				item2[1]-=1;
				count+=1;
				//check if the time is 0, if 0,remove it later
				if (item2[1]==0)
				{
					vector<int> erase2;
					ReadyState.push_back(id_block);
					ReadyTimer.push_back(AllProcess[id_block-1][BurstTime]);//push the burst time this pcb need
					erase2.push_back(id_block);
					erase2.push_back(0);
					TotalErase2.push_back(erase2);
					cout<<"ID: "<<id_block<<"     Block ==> Ready"<<endl;
					erase2.clear();
				}
				
			}
			
			for (auto erase_b:TotalErase2)
			{
				BlockedState.erase(find(BlockedState.begin(),BlockedState.end(),erase_b));
			}
					
			TotalErase2.clear();
			
			
			
			
			//total erase vector for newstate	
			vector<vector<int>> TotalErase1;
			int count2=0;
			for (auto item1:NewState)
			{
				//item1 is each PCB in the NewState
				int my_id=item1[0];//id
				if(item1[1]==0)
				{
					
					//the vector we need to delete
					vector<int> erase1;
					erase1.push_back(my_id);
					erase1.push_back(0);
					//switch from newstate to the ready state
					TotalErase1.push_back(erase1);
					cout<<"ID: "<<my_id<<"     New ==> Ready"<<endl;
					erase1.clear();
					ReadyState.push_back(my_id);
					ReadyTimer.push_back(AllProcess[my_id-1][BurstTime]);//push the burst time this pcb need
					
					
					
				}
				
				else
				{
					
					NewState[count2][1]-=1;
				}
				count2+=1;
				
			}
			for (auto erase_a:TotalErase1)
			{
				NewState.erase(find(NewState.begin(),NewState.end(),erase_a));
			}
			TotalErase1.clear();
			
			
			
			
			if (RunningState.size()!=0)
			{
				//id for running item
				int id_running=RunningState[0];
				AllProcess[id_running-1][TotalRunningTime]+=1;
				RunningState[1]-=1;
				//check if there still have running time in running state
				if(RunningState[1]==0)
				{
					RunningState.clear();
					AllProcess[id_running-1][CPU_Num]-=1;
					//check if the process finishes
					if (AllProcess[id_running-1][CPU_Num]==0)
					{
						ExitState.push_back(id_running);
						cout<<"ID: "<<id_running<<"     Running ==> Exit"<<endl;
						AllProcess[id_running-1][DepartureTime]=tick;
						Flag=true;
						pcb_num=id_running;
						
						
					}
					else
					{
						//block vector we need to add into blockstate
						vector<int> block2;
						block2.push_back(id_running);
						block2.push_back(AllProcess[id_running-1][BlockedTime]);
						BlockedState.push_back(block2);
							
						
						cout<<"ID: "<<id_running<<"     Running ==> Block"<<endl;
						
					}
					TimeQuantum=InitialTime;//reset the timmer
				}
				else
				{
					TimeQuantum-=1;
					
				}
				
				
				
			}
			
			
			if(RunningState.size()==0)
			{
				
				if(ReadyState.size()!=0)
				{
					//id for ready item
					int id_ready=ReadyState[0];
					
					int burst_time=ReadyTimer[0];

					
					//first element of the running state is the id
					RunningState.push_back(id_ready);
					//second element of the runing state is the time it still need to stay in the state
					RunningState.push_back(burst_time);
					cout<<"ID: "<<id_ready<<"     Ready ==> Running"<<endl;
					ReadyState.erase(ReadyState.begin());
					ReadyTimer.erase(ReadyTimer.begin());
				}
				TimeQuantum=InitialTime;//reset the timmer
			}
			else
			{
				if(TimeQuantum==0)
				{
					if(ReadyState.size()!=0)
					{
						
						//id for running item
						int id_running=RunningState[0];
						//the burst time remaining
						int burst_time_remain=RunningState[1];
						
						ReadyState.push_back(id_running);
						ReadyTimer.push_back(burst_time_remain);
						cout<<"ID: "<<id_running<<"     Running ==> Ready"<<endl;
						//id for ready item
						int id_ready=ReadyState[0];
						
						int burst_time=ReadyTimer[0];

						RunningState.clear();
						//first element of the running state is the id
						RunningState.push_back(id_ready);
						//second element of the runing state is the time it still need to stay in the state
						RunningState.push_back(burst_time);
						cout<<"ID: "<<id_ready<<"     Ready ==> Running"<<endl;
						ReadyState.erase(ReadyState.begin());
						ReadyTimer.erase(ReadyTimer.begin());
					}
					TimeQuantum=InitialTime;//reset the timer

				}
			}
			
			//Set the total ready time
			for (auto id: ReadyState)
			{
				AllProcess[id-1][TotalReadyTime]+=1;
			}
			
			
			
			
			//display the pcb which are in block state
			cout<<"ReadyState: ";
			for (auto item4:ReadyState)
			{
				cout<<item4<<", ";
			}
			cout<<endl;
			
			
			//display running state
			cout<<"RunningState: ";
			if (RunningState.size()!=0)
			{
				cout<<RunningState[0];
			}
			cout<<endl;
			
			//display the pcb which are in block state
			cout<<"BlockState: ";
			for (auto item3:BlockedState)
			{
				cout<<item3[0]<<", ";
			}
			cout<<endl;
			
			
			
			if(Flag==true)
			{
				cout<<"-----------------"<<endl;
				cout<<"process terminate"<<endl;
				cout<<"-----------------"<<endl;
				cout<<"ID: "<<pcb_num<<endl;
				cout<<"Priority: "<<CP_AP[pcb_num-1][Priority]<<endl;
				cout<<"Number of Burst: "<<CP_AP[pcb_num-1][CPU_Num]<<endl;
				cout<<"Burst Time: "<<CP_AP[pcb_num-1][BurstTime]<<endl;
				cout<<"Blocked Time: "<<CP_AP[pcb_num-1][BlockedTime]<<endl;
				cout<<"Arrival Time: "<<CP_AP[pcb_num-1][ArrivalTime]<<endl;
				
				
				cout<<"Departure time:  "<<AllProcess[pcb_num-1][DepartureTime]<<endl;
				cout<<"Cumulative ready time:  "<<AllProcess[pcb_num-1][TotalReadyTime]<<endl;
				cout<<"Cumulative running time:  "<<AllProcess[pcb_num-1][TotalRunningTime]<<endl;
				cout<<"Cumulative blocked time:  "<<AllProcess[pcb_num-1][TotalBlockedTime]<<endl;
				cout<<"Turn around time:  "<<AllProcess[pcb_num-1][TotalReadyTime]+AllProcess[pcb_num-1][TotalRunningTime]+AllProcess[pcb_num-1][TotalBlockedTime]<<endl;
				cout<<"Normalized turn around time:  "<< fixed << setprecision(2) <<float(AllProcess[pcb_num-1][TotalReadyTime]+AllProcess[pcb_num-1][TotalRunningTime]+AllProcess[pcb_num-1][TotalBlockedTime])/float(AllProcess[pcb_num-1][TotalRunningTime])<<endl;
				
				Flag=false;
			}
			
			cout<<endl;
				
		}
}




int main(int argc, char* argv[]) 
{

	//check the argument
	if (argc<2)
	{
		cout << "You entered too less arguments!" << endl;
		return 0;
	}
	else if (argc>2)
	{
		cout << "You entered too many arguments!" << endl;
		return 0;
	}
	else
	{
		
	//process file
	ifstream ifs(argv[1]);;
	//Each line
	string line;
	//when count ==0, read the first line of the file, if 1, read the second line
	int count=0;
	//total times the cpu need to process
	int TotalTimes;
	//open the file
	if(ifs.is_open())
	{
		string process_line;
		while (!ifs.eof())
		{
			
			getline(ifs,line);
			//pass the empty line
			if (line=="")
			{
				continue;
			}
			if (count==0)
			{
				if(IsPositiveInt(line)==false)
				{
					cout<<"The first line of the file is not a positive integer"<<endl;
					exit(EXIT_FAILURE);
				}
				else
				{
				TotalTimes=stoi(line);
				}
				count+=1;
			}
			else if(count==1)
			{
				//check if the algorithm exist
				if (line !="FCFS" && line!="Priority" &&line.substr(0,2)!="RR")
				{
					cout<<"The second line of the file is not an algorithm"<<endl;
					exit(EXIT_FAILURE);
				}
				if(line.substr(0,2)=="RR")
				{
					string temp_str;
					for (unsigned int y=3;y<line.length();y++)
					{
						temp_str+=line[y];
					}
					
					InitialTime=stoi(temp_str);
					TimeQuantum=InitialTime;
					line="RR";
				}
				algorithm=line;
				count+=1;
			}
			else
			{
				//The whole line with many numbers and blanks
				process_line=line;
				//Each number in the line
				string my_number;
				//vector for storing all of the data
				vector<int> each_line_num;
				for (unsigned int i=0;i<process_line.length();i++)
				{
					if (process_line[i] !=' ')
					{
						my_number+=process_line[i];
					}
					else
					{
						if(my_number!="")
						{
							each_line_num.push_back(stoi(my_number));
							
						}
						my_number="";
					}
					
				}
				if(my_number!="")
				{
					
					each_line_num.push_back(stoi(my_number));//arrival time
					
				}
				each_line_num.push_back(0);//departure time
				each_line_num.push_back(0);//cumulative time in the ready state
				each_line_num.push_back(0);//cumulative time in the running state
				each_line_num.push_back(0);//cumulative time in the blocked state
				
				
				//add each data into the vector
				AllProcess.push_back(each_line_num);
				CP_AP.push_back(each_line_num);
				each_line_num.clear();
				
			}
			
		}
		for (auto Each_PCB:AllProcess)
		{
			vector<int> my_new;
			my_new.push_back(Each_PCB[Id]);
			my_new.push_back(Each_PCB[ArrivalTime]);
			if(Each_PCB[CPU_Num]!=0)
			{
				NewState.push_back(my_new);
				//minus 1 on each PCB
				Each_PCB[CPU_Num]-=1;
			}
			else
			{
				//get into the exit vecotr
				ExitState.push_back(Each_PCB[Id]);
				
			}
			my_new.clear();
			
		}
		if(algorithm=="FCFS")
		{
			Do_FCFS(TotalTimes);
		}
		else if (algorithm=="Priority")
		{
			Do_Priority(TotalTimes);
		}
		else if (algorithm=="RR")
		{
			Do_RR(TotalTimes);
		}
	}
	else
	{
		cout << "Could not open file!" << endl;
		return false;
	}
}
	return 0;

}
