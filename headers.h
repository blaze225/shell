#include <iostream>
#include <vector>
#include <string>
#include <unistd.h> 			// For chdir(), getcwd() 

using namespace std;

vector <int> getPipePosition(string argString);
void createCommandTable(vector <int> pipePos, string argString);
void changeDirectory(string cdCommand);
string getCurrentDirectory();

extern vector< string > command_table; 
extern string cwd;
