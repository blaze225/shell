#include "headers.h"

vector< string > command_table; 


int main()
{	string argString;
	getline(cin,argString);			// Getting input with spaces
	vector <int> pipePos;
	pipePos = getPipePosition(argString);
	createCommandTable(pipePos,argString);

	cout<<"---TOKENS---\n";
	for(size_t i=0;i<command_table.size();i++)
		cout<<command_table[i]<<endl;
	return 0;
}