#include "headers.h"


vector <int> getPipePosition(string argString)
{
	int singleQuotes=0,doubleQuotes=0,i;
	
	vector <int> pipePos;
	singleQuotes=0;
	doubleQuotes=0;
	for( i=0;argString[i]!='\0';i++){
		if(argString[i]=='\'' && argString[i-1]!='\\')
			singleQuotes++;
		if(argString[i]=='\"' && argString[i-1]!='\\')
			doubleQuotes++;
		if(argString[i] == '|'){	
			if(singleQuotes%2==0 && doubleQuotes%2==0)		// pipe check
			{	//cout<<"FOUND A PIPE!\n";
				pipePos.push_back(i);
			}	
		}
	}	
	return pipePos;
}

void createCommandTable(vector <int> pipePos, string argString){
	int pos=0;
	if(pipePos.size()==0){				// No pipes
		command_table.push_back(argString);
	}
	else{
		for(size_t i=0;i<=(pipePos.size());i++){
			if(argString[pos]== ' ')			// check for spaces
					pos++;
			if(i <= pipePos.size()-1)	
				command_table.push_back(argString.substr(pos,pipePos[i] - pos));
			else									
				command_table.push_back(argString.substr(pos));		// last token
			pos = pipePos[i]+1;

		//	cout<<"Added new token:"<<command_table.size();
		}
	}	
}

char ** tokenize(string cmd){
	char *ptok,temp_cmd[100];
	vector <string> tokens;
	strcpy(temp_cmd,cmd.c_str());
	ptok = strtok(temp_cmd, " -");

	while(ptok!=NULL){
		tokens.push_back(ptok);
		ptok=strtok(NULL," ");
	}
	// Removing quotes from arguments
	for(size_t i=0;i<tokens.size();i++)
		if((tokens[i][0] == '\"' && tokens[i][tokens[i].size()-1] == '\"')
			|| (tokens[i][0] == '\'' && tokens[i][tokens[i].size()-1] == '\'')){
			tokens[i].erase(0,1);
			tokens[i].erase(tokens[i].size()-1);
		}	

	// Converting vector of string to char**
    char ** arr = new char*[tokens.size()+1];	
	for(size_t i = 0; i < tokens.size(); i++){
    	arr[i] = new char[tokens[i].size() + 1];
    	strcpy(arr[i], tokens[i].c_str());
	}
	// Making last element NULL for execvp()
	arr[tokens.size()]=NULL;

	// cout<<"Tokens are:";
	// for(size_t i=0;i<tokens.size();i++)
	// 	cout<<tokens[i]<<endl;

	return arr;
}

string removeLeadingSpaces(string cmd){
	return cmd.substr(cmd.find_first_not_of(" \t"));
}
string removeQuotes(string argument){
	if(argument.find("\"")!=string::npos){							// Double quotes
		argument = argument.substr(argument.find_first_of("\"")+1);
		argument = argument.substr(0,argument.find_last_of("\""));	
	}	
	if(argument.find("\'")!=string::npos){							// Single quotes
		argument = argument.substr(argument.find_first_of("\'")+1);	
		argument = argument.substr(0,argument.find_last_of("\'"));	
	}
	return argument;
}

vector <string> tokenizeForBuiltins(string cmd, string delim){
	char *ptok,temp_cmd[100];
	vector <string> tokens;
	strcpy(temp_cmd,cmd.c_str());
	ptok = strtok(temp_cmd, delim.c_str());

	while(ptok!=NULL){
		tokens.push_back(ptok);
		ptok=strtok(NULL,delim.c_str());
	}
	// Removing quotes from arguments
	for(size_t i=0;i<tokens.size();i++)
		if((tokens[i][0] == '\"' && tokens[i][tokens[i].size()-1] == '\"')
			|| (tokens[i][0] == '\'' && tokens[i][tokens[i].size()-1] == '\'')){
			tokens[i].erase(0,1);
			tokens[i].erase(tokens[i].size()-1);
		}	

	return tokens;	
}