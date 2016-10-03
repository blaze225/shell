#include "headers.h"

string history_path = getenv("HOME") ;	// Setting Path for history file

void executeCommandTable(){
	if(command_table.size()==1){			// Single command
			
			// Removing leading spaces
			command_table[0] = removeLeadingSpaces(command_table[0]);
			
			if(command_table[0].find("cd")== 0)
			{	
				changeDirectory(command_table[0]);
			}
			else if(command_table[0].find("pwd")== 0){
				cout<<getCurrentDirectory()<<endl;
			}
			else if(command_table[0].find("echo")== 0){
				echo(command_table[0]);
			}
			else if(command_table[0].find("history")== 0)
				get_history();	
			else if(command_table[0].find("!!")==0)
				shebang(argString);
			else if(command_table[0].find("export")== 0)
				exportCommand(command_table[0]);
			else{
				executeCommand(command_table[0]);
			}
		}
		else{									// Pipes present
			
			for(size_t i=0;i<command_table.size();i++)
				command_table[i]=removeLeadingSpaces(command_table[i]);
			
			processMultiPipes();
		}
}

// CD
void changeDirectory(string cdCommand){
	string newdir,path;
	int whitespace_pos;
	cdCommand = removeLeadingSpaces(cdCommand);
	newdir = cdCommand.substr(2);
	//cout<<"New Dir:"<<newdir<<endl;
	
	if(newdir.empty() || newdir==" ~" || newdir==" ``")		// only cd, cd ~, cd ``
		newdir=getenv("HOME");								
	else{

		whitespace_pos = newdir.find_first_not_of(" \t");
		if(whitespace_pos==-1)			// only cd with spaces
			newdir=getenv("HOME");
		else if(whitespace_pos==0){							// Invalid cd
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
	string environ;
	vector <string> tokens;
	to_print = to_print.substr(to_print.find("echo")+4);
	
	tokens = tokenizeForBuiltins(to_print," ");
	
	for(size_t i=0;i<tokens.size();i++){
		to_print = tokens[i].substr(tokens[i].find_first_not_of(" \t"));	// Removing extra whitespaces
		to_print = removeQuotes(to_print);				// Removing quotes if they exist
		if(to_print.find("$")!=string::npos){				// Environment variables 
			environ = to_print.substr(to_print.find("$")+1);
			//cout<<"ENV:"<<environ.c_str();
			if(environ[0]=='$'){							// $$ case
				cout<<getpid()<<" ";
				if(environ.length()==1){
					continue;
				}
				else
					environ= environ.substr(1);
			}	
			if(getenv(environ.c_str())==NULL)
			{	cout<<"Environment variable does not exist on the awesome shell!\n";
				return;
			}	
			else{
				to_print = getenv(environ.c_str());
			}
		}
		cout<<to_print<<" ";
	}	
	cout<<endl;
}

// FOR NON SHELL
void executeCommand(string cmd){
	char ** args = tokenize(cmd);
	int cstatus; /* Exit status of child. */
	pid_t c, child;

	// cout<<"AGRs:";
	// for(size_t j=0;j<args.size();j++)
	// 	cout<<arr[j]<<endl;

	if((child=fork())==0){		// CHILD
	//	cout<<"Child: PID of Child ="<<(long) getpid()<<endl;
		execvp(args[0], args); /* arg[0] has the command name. */
		/* If the child process reaches this point, then */
		/* execvp must have failed. */
		cout<<cmd<<": command not found\n";
		cout<<"Child process could not do execvp.\n";
		exit(1);
	}
	else { /* Parent process. */
		if (child == (pid_t)(-1)) {
		cout<<"Fork failed.\n"; exit(1);
		}
		else {
		c = wait(&cstatus); /* Wait for child to complete. */
	//	cout<<"-------------Parent: Child"<<(long)c<<" exited with status ="<<cstatus<<endl;
		}
	}

}

void processMultiPipes(){
	int pfds[2];
	pid_t pid;
	int fd_in=0, status,c;
	size_t i=0;
	char ** args;
	while(i < command_table.size()){
		args = tokenize(command_table[i]);
	//	cout<<"# Forking for "<<args[0]<<" #\n";
		pipe(pfds);
		pid = fork();
		if(pid== -1){
			perror("fork");
			exit(1);
		}
		else if (pid==0){			
			dup2(fd_in,STDIN_FILENO);
			if(i!=command_table.size()-1)		// If not the last command
				dup2(pfds[1],STDOUT_FILENO);
			close(pfds[0]);
			if(command_table[i].find("cd")!=string::npos)	// cd in pipe
				changeDirectory(command_table[i]);
			else if(command_table[i].find("pwd")!=string::npos)		// pwd in pipe
				cout<< getCurrentDirectory();
			else if(command_table[i].find("echo")!=string::npos)	// echo in pipe
				echo(command_table[i]);	
			else									// other commands
				execvp(args[0],args);
			perror("execvp");
			exit(1);
		}
		else{		

			c = waitpid(pid,&status,WUNTRACED);
			//cout<<"-------------Pid: Child"<<c<<" exited with status ="<<status<<endl;
			close(pfds[1]);
			fd_in = pfds[0];
			i++;
		}
	}
}

void log_history(string cmd){

	ofstream hfile(history_path.c_str(), fstream::app);	
	if (hfile.is_open()){
		if(command_history[command_history.size()-1] != cmd)		// If last command is not same
			command_history.push_back(cmd);
		hfile << cmd <<"\n";
		hfile.close();
	}
	else
		cout << "Unable to open history file for logging\n";
		
}
void get_history(){
	size_t history_size;
	int i=1, history_limit;
	
	vector <string> history_args = tokenizeForBuiltins(command_table[0]," ");	// getting arguments if any
	
	history_size = command_history.size();
	history_limit=history_size;
	if(history_args.size() > 1){		// args present | Calculating history_limit
		if(history_args.size() == 2)
			history_limit = atoi(history_args[1].c_str());
		
		else{
			cout<<"Extra arguments given!\n";
			return;
		}
	}

	for(size_t i= history_size - history_limit; i<history_size;i++){	//Display
		cout<<i+1<<"\t"<<command_history[i]<<endl;	
	}
}
void loadHistory(){						// Reading from history.txt and writing to vector command history
	string line;
	int f1= creat(history_path.c_str(),0777);
	ofstream hfile(history_path.c_str(), fstream::app);	
	if (hfile.is_open()){
		
		hfile << "cmd\n";
		hfile.close();
	}
	ifstream file(history_path.c_str());		
	if (file.is_open()){						
		command_history.clear();	
    	while ( getline(file,line) )
			command_history.push_back(line);		

     file.close();
  	}
	else 
		cout << "Unable to open history file for reading\n";

}

void shebang(string argstring){
	string last_cmd = command_history[command_history.size()-2];
	command_table.clear();
	command_table.push_back(last_cmd);
	executeCommandTable();
}

void exportCommand(string cmd){
	vector <string> tokens;
	cmd = cmd.substr(cmd.find("export")+6);
	tokens = tokenizeForBuiltins(cmd,"=");
//	cout<<"TOKENs for export:"<<tokens[0]<<" "<<tokens[1]<<endl;
	setenv(tokens[0].c_str(), tokens[1].c_str(), 1);
	//cout<<"VALUE OF:"<<tokens[0]<<"is "<<getenv(tokens[0].c_str());
}