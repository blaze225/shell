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
	cout<<"Tokens:\n";
	int pos=0;
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