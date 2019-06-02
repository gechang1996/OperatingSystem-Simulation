/*Chang Ge
Computer Project #8
This project will simulate the steps to manage primary storage using paging.
* The system to be simulated contains 1048576 bytes of RAM which is divided
* into 256 page frames.
* Logical address are 16 bits in length
* 
*/
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <errno.h>
#include <iterator>
#include <algorithm>
using namespace std;
#define TOTAL_PAGE 16
//check if there is empty frame
int Check_frame=0;
//this is the algorithm read from the file.
string algorithm;
//number of frames allocated
int num_frames_allocated;
//page frame begin at "40"
string begin_page ="40";
//check when to display page table
int count_N=0;
//used for check when to display page table
int N;
//Page Map Table
map<int,vector<string>> PMT;



//Initialization of PMT
void SetZero();
//check if is integer
bool isInt(string);
//Convert int to 1 digit hex
string IntToHex(int);
//Display the Page table
void DisplayTable();
//convert Hex to integer
int HexToInt(string);
//FIFO(return next victim
int FIFO(vector<int>);
//get the page number by the frame number
int SearchByFrame(int);
//convert int to 2 digit hex
string IntToHex2(int);


//get the page number by frame number
int SearchByFrame(int frame_num)
{
	for (int i=0;i<TOTAL_PAGE;i++)
	{
		if (frame_num==HexToInt(PMT[i][3]) && PMT[i][0]=="1")
		{
			return i;
		}
		
	}
	return -1;
}

//convert hex to int
int HexToInt(string hex)
{
	int len=hex.length();
	int result=0;
	int base=1;
	int my_num;
	while (len!=0)
	{
		
		if (hex[len-1]=='a')
		{
			my_num=10;
		}
		else if (hex[len-1]=='b')
		{
			my_num=11;
		}
		else if (hex[len-1]=='c')
		{
			my_num=12;
		}
		else if (hex[len-1]=='d')
		{
			my_num=13;
		}
		else if (hex[len-1]=='e')
		{
			my_num=14;
		}
		else if (hex[len-1]=='f')
		{
			my_num=15;
		}
		else
		{
			
			my_num=int(hex[len-1])-48;
		}

		result=result+my_num*base;
		base*=16;
		len-=1;
	}
	return result;
}

//Display the Page Table
void DisplayTable()
{
	cout<<endl;
	cout<<"  V R M FrameNumber"<<endl;
	cout<<"  -----------------"<<endl;
	for (int i =0;i<TOTAL_PAGE;i++)
	{
		cout<<IntToHex(i)<<" "<<PMT[i][0]<<" "<<PMT[i][2]<<" "<<PMT[i][1]<<" "<<PMT[i][3]<<endl;
	}
	cout<<endl;
}


//convert int to 1 digit hex
string IntToHex(int dec)
{
	string hexchar;
	int remain=dec ;
		if (remain<10)
		{
			hexchar=to_string(remain);
		
		}
		else if (remain>=10)
		{
			if (remain ==10)
			{
				hexchar="a";
			}
			else if (remain ==11)
			{
				hexchar="b";
			}
			else if (remain ==12)
			{
				hexchar="c";
			}
			else if (remain ==13)
			{
				hexchar="d";
			}
			else if (remain ==14)
			{
				hexchar="e";
			}
			else if (remain ==15)
			{
				hexchar="f";
			}
		}
		return hexchar;
}


//convert int to 2 digit hex
string IntToHex2(int dec)
{
	string result="00";//return the hex string
	
	int index = 1;
	//when the dec is 0, return the result
	while (dec !=0)
	{
		int remain=dec %16;
		if (remain<10)
		{
		char hexchar='0'+remain;
		
		result[index]=hexchar;
		}
		else if (remain>=10)
		{
			if (remain ==10)
			{
				result[index]='a';
			}
			else if (remain ==11)
			{
				result[index]='b';
			}
			else if (remain ==12)
			{
				result[index]='c';
			}
			else if (remain ==13)
			{
				result[index]='d';
			}
			else if (remain ==14)
			{
				result[index]='e';
			}
			else if (remain ==15)
			{
				result[index]='f';
			}
		}
		dec=dec/16;
		index -=1;	
	}
	return result;
}


//Initialization of PMT
void SetZero()
{
	for (int i =0;i<TOTAL_PAGE;i++)
	{
		PMT[i].push_back("0");//V
		PMT[i].push_back("0");//M
		PMT[i].push_back("0");//Ref
		PMT[i].push_back("00");//Frame number
		
	}
	
	
	 
}
//check if is integer
bool isInt(string ss)
{
	if(ss[0] !='-')
	{
		for (unsigned int j =0;j<ss.length();j++)
		{
			if (isdigit(ss[j])==false)
			{
				return false;
			}
		}
	}
	else if(ss[0] =='-')
	{
		for (unsigned int j =1;j<ss.length();j++)
		{
			if (isdigit(ss[j])==false)
			{
				return false;
			}
		}
	}
	return true;
}


int main(int argc, char* argv[]) 
{
	//Initialize the page table
	SetZero();
	//check the argument
	if (argc<3)
	{
		cout << "You entered too less arguments!" << endl;
		return 0;
	}
	else if (argc>3)
	{
		cout << "You entered too many arguments!" << endl;
		return 0;
	}
	else
	{
		if(isInt(argv[1]))
		{
			//get the number to check when to display the page table
			N=stoi(argv[1]);
			if (N<=0)
			{
				N=0;
				cout<<"You need to enter a nature number. Negative number is not acceptable. But I will set it to 0 now."<<endl;
			}
		}
		else
		{
			//error checking
			cout << "The first argument is not an integer!" << endl;
			return 0;
		}
	}
	
	//process file
	ifstream ifs(argv[2]);;
	//Each line
	string line;
	//Operation 
	string operation;
	//Logical address
	string l_address;
	//Number of read operation
	int ReadNum=0;
	//Number of write operation
	int WriteNum=0;
	//open the file
	if(ifs.is_open())
	{
		//display the page table in the beginning of the simulation
		if(N!=0)
		{
		DisplayTable();
		}
		
		string pageNumStr;
		//this is the algorithm
		getline(ifs,algorithm);
		getline(ifs,pageNumStr);
		//get the number of frames allocated
		int pageNum=stoi(pageNumStr);
		//get the first frame(integer)
		int page_frame_begin=HexToInt(begin_page);
		num_frames_allocated=pageNum;
		//this is the allocated frames,which will be used later.
		vector<int> allocated_frames;
		for (int i=0;i<pageNum;i++)
		{
			
			allocated_frames.push_back(page_frame_begin+i);
			
		}
		
		string page_fault;
		string write_back;
		string phyical_address;
		int ancient_value;
		int vic_page;
		
		int frame_number;
		if (algorithm=="FIFO")
		{
			while (!ifs.eof())
			{
				
				getline(ifs,line);
				//pass the empty line
				if (line=="")
				{
					continue;
				}
				//logical address
				l_address=line.substr(0,4);
				//operation
				operation=line.substr(5,1);
				
				
				
				vector<string> page_line=PMT[HexToInt(l_address.substr(0,1))];
				//check the v bit
				if (page_line[0]=="1")
				{
					page_fault=" ";
					write_back="  ";
					
					
				}
				else if (page_line[0]=="0")
				{
					
					page_fault="F";
					if (Check_frame<num_frames_allocated)
					{
						//empty situation
						ancient_value=allocated_frames[0];
						allocated_frames.erase(allocated_frames.begin());
						allocated_frames.push_back(ancient_value);
						Check_frame+=1;
						write_back="  ";
					}
					else
					{
						//not empty
						ancient_value=allocated_frames[0];
						allocated_frames.erase(allocated_frames.begin());
						allocated_frames.push_back(ancient_value);
						vic_page=SearchByFrame(ancient_value);
						PMT[vic_page][0]="0";
						//check if we need to write back
						if(PMT[vic_page][1]=="1")
						{
							write_back="WB";
						}
						else
						{
							write_back="  ";
						}

					}
					//copy data into PMT and set its V to 1
					PMT[HexToInt(l_address.substr(0,1))][0]="1";
					PMT[HexToInt(l_address.substr(0,1))][3]=IntToHex2(ancient_value);
					
					
					
				}
				//set its r to 1
				PMT[HexToInt(l_address.substr(0,1))][2]="1";
				
				
				//count the operation number
				
				if (operation =="R")
				{
					ReadNum+=1;
				}
				else if (operation =="W")
				{
					WriteNum+=1;
					//set w to 1
					PMT[HexToInt(l_address.substr(0,1))][1]="1";
				}
				
				phyical_address=PMT[HexToInt(l_address.substr(0,1))][3]+l_address.substr(1,3);
				//display the result of each line from the file
				cout<<l_address<<" "<<l_address.substr(0,1)<<" "<<l_address.substr(1,3)<<" "<<operation<<" "<<page_fault<<" "<<write_back<<" "<<phyical_address<<endl;
				
				count_N+=1;
				//display the page table based on input N
				if (count_N==N)
				{
					count_N=0;
					DisplayTable();
				}
				
			}
			//Display page table at the end of the simulation
			if (count_N!=0 && N!=1 && N!=0)
			{
				DisplayTable();
			}
			//display the number of operations
			cout<<"Algorithm NumberOfAllocatedFrames MemoryReference ReadOperation WriteOpertion"<<endl;
			cout<<"-----------------------------------------------------------------------------"<<endl;
			cout<<"     "<<algorithm<<"                "<<num_frames_allocated<<"             "<<ReadNum+WriteNum<<"             "<<ReadNum<<"             "<<WriteNum<<endl;
		}
		
		else if (algorithm=="LRU")
		{
			while (!ifs.eof())
			{
				
				getline(ifs,line);
				//pass the empty line
				if (line=="")
				{
					continue;
				}
				//logical address
				l_address=line.substr(0,4);
				//operation
				operation=line.substr(5,1);
				
				
				
				
				vector<string> page_line=PMT[HexToInt(l_address.substr(0,1))];
				
				
			
				//check the v bit
				if (page_line[0]=="1")
				{
					page_fault=" ";
					write_back="  ";
					frame_number=HexToInt(page_line[3]);
					allocated_frames.erase(find(allocated_frames.begin(),allocated_frames.end(),frame_number));
					allocated_frames.push_back(frame_number);
					
					
				}
				else if (page_line[0]=="0")
				{
				page_fault="F";
				//empty situation
					if (Check_frame<num_frames_allocated)
					{
						ancient_value=allocated_frames[0];
						allocated_frames.erase(allocated_frames.begin());
						allocated_frames.push_back(ancient_value);
						Check_frame+=1;
						write_back="  ";
					}
					//not empty
					else
					{
						ancient_value=allocated_frames[0];
						allocated_frames.erase(allocated_frames.begin());
						allocated_frames.push_back(ancient_value);
						//get the victim page number
						vic_page=SearchByFrame(ancient_value);
						PMT[vic_page][0]="0";
						if(PMT[vic_page][1]=="1")
						{
							write_back="WB";
						}
						else
						{
							write_back="  ";
						}

					}
					//copy data into PMT and set its V to 1
					PMT[HexToInt(l_address.substr(0,1))][3]=IntToHex2(ancient_value);
					PMT[HexToInt(l_address.substr(0,1))][0]="1";
					
					
					
					
				}
				//set its r to 1
				PMT[HexToInt(l_address.substr(0,1))][2]="1";
				
				
				//count the operation number
				
				if (operation =="R")
				{
					ReadNum+=1;
				}
				else if (operation =="W")
				{
					WriteNum+=1;
					PMT[HexToInt(l_address.substr(0,1))][1]="1";
				}
				
				phyical_address=PMT[HexToInt(l_address.substr(0,1))][3]+l_address.substr(1,3);
				//display the result of each line from the file
				cout<<l_address<<" "<<l_address.substr(0,1)<<" "<<l_address.substr(1,3)<<" "<<operation<<" "<<page_fault<<" "<<write_back<<" "<<phyical_address<<endl;
				
				count_N+=1;
				//display the page table based on input N
				if (count_N==N)
				{
					count_N=0;
					DisplayTable();
				}
				
			}
			//Display page table at the end of the simulation
			if (count_N!=0 && N!=1 && N!=0)
			{
				DisplayTable();
			}
			//display the number of operations
			cout<<"Algorithm NumberOfAllocatedFrames MemoryReference ReadOperation WriteOpertion"<<endl;
			cout<<"-----------------------------------------------------------------------------"<<endl;
			cout<<"     "<<algorithm<<"                "<<num_frames_allocated<<"              "<<ReadNum+WriteNum<<"             "<<ReadNum<<"             "<<WriteNum<<endl;
		}
		
		
	}
	else
	{
		cout << "Could not open file!" << endl;
		exit(EXIT_FAILURE);
	}
	
}
