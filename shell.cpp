#include "headers.h"

vector< string > command_table; 
vector< string > command_history; 
string cwd= getCurrentDirectory();				// get getCurrentDirectory for prompt
string argString;
string prompt;

void printPrompt(string prompt){
	cout<<prompt;

}
void sighandler(int signum)
{	fflush(stdin);
	fflush(stdout);
 	cout<<endl;
 	printPrompt(prompt);	
 	fflush(stdin);
	fflush(stdout);
}

int main()
{	
	vector <int> pipePos;
	
	history_path = history_path + "/Documents/history.txt";		// Setting path for history
	loadHistory();										// Loading history from history.txt
	cout << "\033[2J\033[1;1H";			// Clearing screen
	while(1){
		

		prompt = "The_Awesome_Shell:"+cwd+"$";
		printPrompt(prompt);
			
    	signal(SIGINT, sighandler);					// Handling Ctrl+C
		
		// Getting input with spaces
		if(getline(cin,argString))				//Handling ENTER 
		{
			if(argString=="")
				continue;
		}
		
		if(argString=="exit")			// Checking for exit
		{	
			cout<<"Leaving? ...BYE\n";
			break;
		}
		

		log_history(argString);					// Logging into history
		pipePos = getPipePosition(argString);
		createCommandTable(pipePos,argString);

		executeCommandTable();


		// cout<<"---TOKENS---\n";
		// for(size_t i=0;i<command_table.size();i++)
		// 	cout<<command_table[i]<<endl;

		command_table.clear();				// Clearing command table
	}
	return 0;
}