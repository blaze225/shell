#include "headers.h"


vector <int> getPipePosition(string argString)
{
	int singleQuotes=0,doubleQuotes=0,i;
	
	vector <int> pipePos;
	singleQuotes=0;
	doubleQuotes=0;
	for( i=0;argString[i]!='\0';i++){
			//cout<<"Current charcter:"<<argString[i]<<endl;
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
	// cout<<"Pipe positions:";
	// for(size_t j=0;j<pipePos.size();j++)
	// 	cout<<pipePos[j]<<" ";
	// cout<<endl;
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

vector <string> tokenize(string cmd){
	char *ptok,temp_cmd[100];
	vector <string> tokens;
	strcpy(temp_cmd,cmd.c_str());
	ptok = strtok(temp_cmd, " -");

	while(ptok!=NULL){
		tokens.push_back(ptok);
		ptok=strtok(NULL," ");
	}

	// cout<<"Tokens are:";
	// for(size_t i=0;i<tokens.size();i++)
	// 	cout<<tokens[i]<<endl;

	return tokens;
}