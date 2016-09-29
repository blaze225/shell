#include "headers.h"

void changeDirectory(string cdCommand){
	string newdir,path;
	int whitespace_pos;
	newdir = cdCommand.substr(cdCommand.find("cd")+2);
	//cout<<"New Dir:"<<newdir<<endl;
	
	whitespace_pos = newdir.find_first_not_of(" \t");
	if(whitespace_pos==0){							// Invalid cd
		cout<<newdir<<": command not found\n";
		return;
	}
	else
		newdir = newdir.substr(whitespace_pos);	// Erasing whitespaces between cd and arguments

	if(newdir.empty() || newdir=="~")
		newdir="/";

	if(chdir(newdir.c_str())==-1)
		cout<<"Invalid directory given!\n";
	else{
		path=getCurrentDirectory();

		if(!path.empty()){		// If not NULL
			cwd = path;
		}
		else
			cout<<"getcwd failed!\n";	
	}
//	cout<<"NEW cwd:"<<cwd<<endl;

}

string getCurrentDirectory(){
	string path;
	char cwdir[1000];
	getcwd(cwdir,sizeof(cwdir));
	path.append(cwdir);
	return path;
}