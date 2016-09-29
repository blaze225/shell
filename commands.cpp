#include "headers.h"

// CD
void changeDirectory(string cdCommand){
	string newdir,path;
	int whitespace_pos;
	newdir = cdCommand.substr(cdCommand.find("cd")+2);
	//cout<<"New Dir:"<<newdir<<endl;
	
	if(newdir.empty() || newdir==" ~" || newdir==" ``")
		newdir="/home/";
	else{
		
		whitespace_pos = newdir.find_first_not_of(" \t");
		if(whitespace_pos==0){							// Invalid cd
			cout<<newdir<<": command not found\n";
			return;
		}
		else
			newdir = newdir.substr(whitespace_pos);	// Erasing whitespaces between cd and arguments
	}


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

// PWD
string getCurrentDirectory(){
	string path;
	char cwdir[1000];
	getcwd(cwdir,sizeof(cwdir));
	path.append(cwdir);
	return path;
}
// ECHO
void echo(string to_print){
	to_print = to_print.substr(to_print.find("echo")+4);
	to_print = to_print.substr(to_print.find_first_not_of(" \t"));	// Removing extra whitespaces
	if(to_print.find("\"")!=string::npos){							// Double quotes
		to_print = to_print.substr(to_print.find_first_of("\"")+1);
		to_print = to_print.substr(0,to_print.find_last_of("\""));	
	}	
	if(to_print.find("\'")!=string::npos){							// Single quotes
		to_print = to_print.substr(to_print.find_first_of("\'")+1);	
		to_print = to_print.substr(0,to_print.find_last_of("\'"));	
	}
	cout<<to_print<<endl;
}

// FOR NON SHELL
void executeCommand(string cmd){
	vector<string> args = tokenize(cmd);
	int cstatus; /* Exit status of child. */
	pid_t c, child;

	// Converting vector of string to char**
    char ** arr = new char*[args.size()];
	for(size_t i = 0; i < args.size(); i++){
    	arr[i] = new char[args[i].size() + 1];
    	strcpy(arr[i], args[i].c_str());
	}
	// Making last element NULL for execvp()
	arr[args.size()]=NULL;

	// cout<<"AGRs:";
	// for(size_t j=0;j<args.size();j++)
	// 	cout<<arr[j]<<endl;

	


	if((child=fork())==0){		// CHILD
	//	cout<<"Child: PID of Child ="<<(long) getpid()<<endl;
		execvp(arr[0], arr); /* arg[0] has the command name. */
		/* If the child process reaches this point, then */
		/* execvp must have failed. */
		cout<<"Child process could not do execvp.\n";
		exit(1);
	}
	else { /* Parent process. */
		if (child == (pid_t)(-1)) {
		cout<<"Fork failed.\n"; exit(1);
		}
		else {
		c = wait(&cstatus); /* Wait for child to complete. */
		cout<<"-------------Parent: Child"<<(long)c<<" exited with status ="<<cstatus<<endl;
		}
	}

}