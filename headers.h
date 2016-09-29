#include <iostream>
#include <vector>
#include <string>
#include <string.h>    			// for strtok()
#include <unistd.h> 			// For chdir(), getcwd() , execvp()
#include <stdlib.h>
#include <sys/wait.h>			// for wait()
// #include  <sys/types.h>			// for

using namespace std;

vector <int> getPipePosition(string argString);
void createCommandTable(vector <int> pipePos, string argString);
void changeDirectory(string cdCommand);
string getCurrentDirectory();
void echo(string to_print);
void executeCommand(string cmd);
vector <string> tokenize(string cmd);

extern vector< string > command_table; 
extern string cwd;
