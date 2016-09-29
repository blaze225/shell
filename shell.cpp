#include "headers.h"

vector< string > command_table; 
string cwd="/home/saad/Desktop/OS Programs/Assignment3";

int main()
{	string argString;
	vector <int> pipePos;
	
	string prompt;
	while(1){

		prompt = "The_Awesome_Shell:"+cwd+"$";
		cout<<prompt;
		getline(cin,argString);			// Getting input with spaces
		
		if(argString=="exit")			// Checking for exit
		{	cout<<"Leaving? ... GET LOST\n";
			break;
		}
		pipePos = getPipePosition(argString);
		createCommandTable(pipePos,argString);

		if(command_table.size()==1){			// Single command
			
			// Removing leading spaces
			command_table[0] = command_table[0].substr(command_table[0].find_first_not_of(" \t"));
			
			if(command_table[0].find_first_of("cd")== 0)
			{	
				changeDirectory(command_table[0]);
			}	
		}


		cout<<"---TOKENS---\n";
		for(size_t i=0;i<command_table.size();i++)
			cout<<command_table[i]<<endl;

		command_table.clear();				// Clearing command table
	}
	return 0;
}