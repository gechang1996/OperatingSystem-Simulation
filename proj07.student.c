/*Chang Ge
Computer Project #7
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
using namespace std;
#define TOTAL_PAGE 16


//check when to display page table
int count=0;
//used for check when to display page table
int N;
//Page Map Table
map<int,vector<string>> PMT;
//Initialization of PMT
void SetZero();
//check if is integer
bool isInt(string);
//Display information from the given line
void Display_Info(string,string);
//Convert int to 1 digit hex
string IntToHex(int);
//Display the Page table
void DisplayTable();




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





//Display information from the given line
void Display_Info(string address,string operation)
{
	cout<<address<<" "<<address.substr(0,1)<<" "<<address.substr(1,3)<<" "<<operation<<endl;
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
		for (int j =0;j<ss.length();j++)
		{
			if (isdigit(ss[j])==false)
			{
				return false;
			}
		}
	}
	else if(ss[0] =='-')
	{
		for (int j =1;j<ss.length();j++)
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
			Display_Info(l_address,operation);
			//count the operation number
			if (operation =="R")
			{
				ReadNum+=1;
			}
			else if (operation =="W")
			{
				WriteNum+=1;
			}
			count+=1;
			//display the page table based on input N
			if (count==N)
			{
				count=0;
				DisplayTable();
			}
			
		}
		//Display page table at the end of the simulation
		if (count!=0 && N!=1 && N!=0)
		{
			DisplayTable();
		}
		//display the number of operations
		cout<<"MemoryReference ReadOperation WriteOpertion"<<endl;
		cout<<"-------------------------------------------"<<endl;
		cout<<"       "<<ReadNum+WriteNum<<"             "<<ReadNum<<"             "<<WriteNum<<endl;
		
		
	}
	else
	{
		cout << "Could not open file!" << endl;
		return false;
	}
	
}
