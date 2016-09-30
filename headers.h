#include <iostream>
#include <vector>
#include <string>
#include <string.h>    			// for strtok()
#include <unistd.h> 			// For chdir(), getcwd() , execvp(), pipe()
#include <stdlib.h>
#include <sys/wait.h>			// for wait()
#include  <sys/types.h>			// for

using namespace std;

vector <int> getPipePosition(string argString);
void createCommandTable(vector <int> pipePos, string argString);
void changeDirectory(string cdCommand);
string getCurrentDirectory();
void echo(string to_print);
void executeCommand(string cmd);
char ** tokenize(string cmd);
string removeLeadingSpaces(string cmd);
void processPipes(string cmd1, string cmd2);

extern vector< string > command_table; 
extern string cwd;
