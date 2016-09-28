#include <iostream>
#include <vector>
#include <string>

using namespace std;
vector <int> getPipePosition(string argString);
void createCommandTable(vector <int> pipePos, string argString);
extern vector< string > command_table; 