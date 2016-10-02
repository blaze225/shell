#include "headers.h"

vector< string > command_table; 
vector< string > command_history; 
string cwd= getCurrentDirectory();				// get getCurrentDirectory for prompt


int main()
{	string argString;
	vector <int> pipePos;
	string prompt;
	history_path = history_path + "/Documents/history.txt";		// Setting path for history
	cout << "\033[2J\033[1;1H";			// Clearing screen
	while(1){
		

		prompt = "The_Awesome_Shell:"+cwd+"$";
		cout<<prompt;
			
		// Getting input with spaces
		if(getline(cin,argString))				//Handling ENTER 
		{
			if(argString=="")
				continue;
		}
		
		if(argString=="exit")			// Checking for exit
		{	
			cout<<"Leaving? ... GET LOST\n";
			break;
		}

		log_history(argString);					// Logging into history
		pipePos = getPipePosition(argString);
		createCommandTable(pipePos,argString);

		if(command_table.size()==1){			// Single command
			
			// Removing leading spaces
			command_table[0] = removeLeadingSpaces(command_table[0]);
			
			if(command_table[0].find("cd")== 0)
			{	
				changeDirectory(command_table[0]);
			}
			else if(command_table[0].find("pwd")== 0){
				cout<<getCurrentDirectory()<<endl;
			}
			else if(command_table[0].find("echo")== 0){
				echo(command_table[0]);
			}
			else if(command_table[0].find("history")== 0)
				get_history();	
			else{
				executeCommand(command_table[0]);
			}
		}
		else{									// Pipes present
			
			for(size_t i=0;i<command_table.size();i++)
				command_table[i]=removeLeadingSpaces(command_table[i]);
			
			processMultiPipes();
		}


		// cout<<"---TOKENS---\n";
		// for(size_t i=0;i<command_table.size();i++)
		// 	cout<<command_table[i]<<endl;

		command_table.clear();				// Clearing command table
	}
	return 0;
}