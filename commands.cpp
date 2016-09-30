#include "headers.h"

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
		cout<<"-------------Parent: Child"<<(long)c<<" exited with status ="<<cstatus<<endl;
		}
	}

}

void processPipes(){
	int pfds[2];
	pid_t pid1,pid2,c1,c2;
	int status1,status2;
	pipe(pfds);
	char ** args1 = tokenize(command_table[0]);
	char ** args2 = tokenize(command_table[1]);

	pid1 = fork();
	if(pid1==-1)
	{	perror("fork");
		exit(1);
	}	
	if(pid1==0){
		dup2(pfds[1], STDOUT_FILENO);
      	close(pfds[0]);
      	execvp(args1[0], args1);

      	cout<<"1st Child process could not do execvp.\n";
		
		exit(1);
	}

	pid2 = fork();
	if(pid2==-1)
	{	perror("fork");	
		exit(1);
	}	
	if(pid2==0){
		dup2(pfds[0], STDIN_FILENO);
      	close(pfds[1]);
      	execvp(args2[0],args2);
      	cout<<"2nd Child process could not do execvp.\n";
      	exit(1);
	}

	close(pfds[0]);
   	close(pfds[1]);
   	c1=waitpid(pid1,&status1,WUNTRACED);
   	cout<<"-------------Pid1: Child"<<c1<<" exited with status ="<<status1<<endl;
   	c2=waitpid(pid2,&status2,WUNTRACED);
   	cout<<"-------------Pid2: Child"<<c2<<" exited with status ="<<status2<<endl;


}

void processMultiPipes(){
	int pfds[2];
	pid_t pid;
	int fd_in=0, status,c;
	size_t i=0;
	char ** args;
	while(i < command_table.size()){
		args = tokenize(command_table[i]);
		//cout<<"# Forking for "<<args[0]<<" #\n";
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
			execvp(args[0],args);
			perror("execvp");
			exit(1);
		}
		else{		

			c = waitpid(pid,&status,WUNTRACED);
			cout<<"-------------Pid: Child"<<c<<" exited with status ="<<status<<endl;
			close(pfds[1]);
			fd_in = pfds[0];
			i++;
		}
	}
}
