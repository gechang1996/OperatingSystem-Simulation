/*Chang Ge
Computer Project #6
This project is processing the RAM file and Memory file and print the result out
* The first argument is N which is used for checking when to display whole cache
* The second argument is RAM file
* The third argument is Memory file
* 
*/


#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <errno.h>
using namespace std;


#define RAMSIZE 65536 
#define CACHESIZE 16
//used for check when to display cache
int N;
//initial address
string init_address;
//number of datas in RAM
int count_line=0;
//RAM
map<string, string> RAM;
//CACHE
map<string,vector<string>> cache;
//Initialization
void SetZero();
//convert integer to Hex(4 digit)
string IntToHex(int);
//check if the argument is a integer
bool isInt(string);
//process RAM file
bool process_RAM(string);
//process memory file
bool process_memory(string);
//Convert integer to hex(1 digit)
string IntToHex2(int);
//convert Hex to integer
int HexToInt(string);
//print out cache data
void Display_cache();

void Display_cache()
{
	//cache data format
	cout<<"     V M Tag Block contents"<<endl;
	cout<<"     ------- -----------------------------------------------"<<endl;
	for (int i =0;i<16;i++)
	{
		cout<<"["<<IntToHex2(i)<<"]: "<<cache[IntToHex2(i)][1]<<" "<<cache[IntToHex2(i)][2]<<" "<<cache[IntToHex2(i)][0]<<"  "<<cache[IntToHex2(i)][3]<<endl;
	}
}

bool process_memory(string filename)
{
	//process RAM file
	ifstream ifs(filename);;
	int count_times=0;//check when to display cache
	string line;//data of each line 
	string operation;//this is the operation(Read or Write)
	if(ifs.is_open())
	{
		string Init_memory;
		vector<string> data;
		string address;
		string operation;
		bool flag_M; //Get "M" or "H"
		//get each line
		while (!ifs.eof())
		{

			getline(ifs,line);
			//pass empty lines
			if (line=="")
			{
				continue;
			}
			
			string flag_result;
			address=line.substr(0,4);
			operation=line.substr(5,1);
			
			//do read operation
			if (operation=="R")
			{
				string tag=line.substr(0,2);
				string my_cache=line.substr(2,1);
				int index_num=HexToInt(line.substr(3,1))*3;//used for find the postion of the data
				//check if miss or hit
				if(tag!=cache[my_cache][0] || cache[my_cache][1]=="0")
				{
					
					flag_M=1;
					flag_result="M";
				}
				else
				{
					flag_M=0;
					flag_result="H";
				}
				//if modified, do missing process
				if(flag_M)
				{
					//write back when this cash data miss
					if (cache[my_cache][2]=="1")
					{
						string address_write=cache[my_cache][0]+my_cache+"0";
						RAM[address_write]=cache[my_cache][3];
					}
					string address_read=line.substr(0,3)+"0";
					cache[my_cache][3]=RAM[address_read];
					cache[my_cache][0]=tag;
					cache[my_cache][1]="1";
					cache[my_cache][2]="0";
				}
				//print out the result of this process
				cout<<address<<" "<<my_cache<<" "<<flag_result<<" "<<operation <<" "<<cache[my_cache][3].substr(index_num,11)<<endl;
	
			}
			//write process is alomost the same as read process
			else if (operation =="W")
			{
				string mydata=line .substr(7);
				string tag=line.substr(0,2);
				string my_cache=line.substr(2,1);
				int index_num=HexToInt(line.substr(3,1))*3;
			//check if missed
				if(tag!=cache[my_cache][0] || cache[my_cache][1]=="0")
				{
					flag_M=1;
					flag_result="M";
				}
				else
				{
					flag_M=0;
					flag_result="H";
				}
				//check if modified
				if(flag_M)
				{
					if (cache[my_cache][2]=="1")
					{
						string address_write=cache[my_cache][0]+my_cache+"0";
						RAM[address_write]=cache[my_cache][3];
					}
					string address_read=line.substr(0,3)+"0";
					cache[my_cache][3]=RAM[address_read];
					cache[my_cache][0]=tag;
					cache[my_cache][1]="1";

				}
				//write the getting data into cacha
				cache[my_cache][2]="1";
				string new_str=cache[my_cache][3];
				new_str.replace(index_num,11,mydata);
				cache[my_cache][3]=new_str;
				cout<<address<<" "<<my_cache<<" "<<flag_result<<" "<<operation<<" "<<cache[my_cache][3].substr(index_num,11)<<endl;
				
				
				
			}
			else
			{
				cout<<"Wrong operation"<<endl;
				
			}
			count_times+=1;
			//check when to display cache contents
			if (count_times==N)
			{
				Display_cache();
				count_times=0;
			}
		}
		
		ifs.close();
		return true;
	}
	else
	{
		cout << "Could not open file!" << endl;
		return false;
	}
}
//process RAM file
bool process_RAM(string filename)
{
	//process RAM file
	ifstream ifs(filename);;
	
	string line;
	if(ifs.is_open())
	{

		int count=0;
		string address;
		while (!ifs.eof())
		{
			getline(ifs,line);
			if (line=="")
			{
				continue;
			}
			if (count==0)
			{
				address=line;
				init_address=line;
			}
			else
			{
				//put data into RAM
				RAM[address]=line;
				address=IntToHex(HexToInt(address)+16);
				count_line+=1;
			}
			count+=1;
		}
		ifs.close();
		return true;
	}
	else
	{
		cout << "Could not open file!" << endl;
		return false;
	}
}

//convert int to hex
string IntToHex2(int dec)
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


//convert int to 4 digit hex
string IntToHex(int dec)
{
	string result="0000";//return the hex string
	
	int index = 3;
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

//set all address containing 0
void SetZero()
{
	for (int i =0;i<RAMSIZE;i++)
	{
		RAM[IntToHex(i)]="00";
	}
	
	for (int j=0;j<CACHESIZE;j++)
	{

		string data;
		for (int i=0;i<15;i++)
		{
			data+="00 ";
		}
		data+="00";
		cache[IntToHex2(j)].push_back("00");//Tag
		cache[IntToHex2(j)].push_back("0");//V
		cache[IntToHex2(j)].push_back("0");//M
		cache[IntToHex2(j)].push_back(data);//DATA
		
	}
	
}

//check if is integer
bool isInt(string ss)
{
	for (int j =0;j<ss.length();j++)
	{
		if (isdigit(ss[j])==false)
		{

			return false;
		}
	}
	return true;
}

int main(int argc, char* argv[]) 
{
	//set all address containing 0
	SetZero();
	
	if (argc<4)
	{
		cout << "You entered too less arguments!" << endl;
		return 0;
	}
	else if (argc>4)
	{
		cout << "You entered too many arguments!" << endl;
		return 0;
	}
	else
	{
		if(isInt(argv[1]))
		{
			N=stoi(argv[1]);
		}
		else
		{
			cout << "The first argument is not an integer!" << endl;
			return 0;
		}
	}

	
	if (N!=0)
	{
	Display_cache();
	}
	//error checking
	if(process_RAM(argv[2])==false)
	{
		return 0;
	}

	//error checking
	if (process_memory(argv[3])==false)
	{
		return 0;
	}
	
	
	if (N!=0)
	{
	Display_cache();
	}
	//RAM format
	
	cout<<"\n                        RAM"<<endl;
	cout<<"----------------------------------------------------"<<endl;
	while (count_line !=0)
	{
		
		cout<<init_address<<" "<<RAM[init_address]<<endl;
		init_address=IntToHex(HexToInt(init_address)+16);
		count_line-=1;
	}
	
	


}

