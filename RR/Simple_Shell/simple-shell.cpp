/**
 * 
 * Project: Simple shell interface program.
 * 
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <string.h>

#define MAX_LINE		80 /* 80 chars per line, per command */


/*	parameters: commands is a list of all the commands the user inserted
	This function will receive commands list and returns a list of 10
	most recent commands.
	*/
std::vector<std::string> history(std::vector<std::string> commands);

/*	takes the command line as arguments and implement the command
	if the command has & at the end, it create a new process and
	let it proceed in the background
*/
void implementCommand(char *args[], int &length);

/*	parameters: C++ string and C string
	this function will convert the C++ string to C string.
*/
void Cstring(std::string str, char *args[MAX_LINE/2 + 1], int &lastIndex);

int main(void)
{
	
	bool should_run = true;
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	int argsLength;
	std::string in;

	//list of all commands the user inserted
	std::vector<std::string> commands;
	
	while (should_run){   

		std::cout << "osh>";
		fflush(stdout);
		getline(std::cin, in);

		//parse each word in the string in, then convert it to c string
		Cstring(in, args, argsLength);

		if(args[0][0] != '!'){
			commands.push_back(in); //add this command to the list
		}
		
		std::vector<std::string> top10;
		top10 = history(commands);

		//test the possible command that the user inserted
		if(strcmp(args[0], "exit") == 0){
			should_run = false;
			
		} else if(strcmp(args[0], "history") == 0){

			
			for(int i=0; i < top10.size(); i++){
				std::cout << (i+1) << " - " << top10[i] << std::endl;

			}
		} else if (strcmp(args[0], "!!") == 0){

			std::string command = top10[0];
			commands.push_back(command);

			Cstring(command, args, argsLength);
			
		} else if(args[0][0] == '!'){
			int commandNum = args[0][1] - '0';

			//implement commandNum
			Cstring(top10[commandNum-1], args, argsLength);
		}

		if(should_run)
			implementCommand(args, argsLength);	
	}	

	return 0;
}


std::vector<std::string> history(std::vector<std::string> commands){
	int vecLength = commands.size();
	std::vector<std::string> historyList;
	int k=10;

	if(vecLength < 10){
		for(int i=vecLength-1; i >= 0; i--)
			historyList.push_back(commands[i]);
	} else{
		for(int i= vecLength-1; i >= vecLength-10; i--){
			historyList.push_back(commands[i]);
		}
	}
	return historyList;
}

void implementCommand(char *args[], int &argsLength){
	//check if the last letter is & or NOT
	if(strcmp(args[argsLength-1], "&") == 0){ //if it has & at the end
		if(fork() == 0){
			args[argsLength-1] = NULL;
			execvp(args[0], args);
		} else{
			sleep(1);
		}
	} else{
	//create a process and make the parent wait
	if(fork() == 0){ //if it doesn't have & at the end
			execvp(args[0], args);
		} else{
			wait(NULL);
		}
	}

}

void Cstring(std::string str, char *args[MAX_LINE/2 + 1], int &lastIndex){

	int j=0;

	std::string tmp = "";
	for(int i=0; i < str.length(); i++){

		bool lastChar = (i == str.length()-1);

		if(str[i] == ' ' || lastChar){
			if(lastChar){
				tmp += str[i];
			}

			args[j] = new char[tmp.size()+1];
			std::copy(tmp.begin(), tmp.end(), args[j]);
			args[j][tmp.size()] = '\0';

			tmp = "";
			j++;
		}
		else{
			tmp += str[i];
		}
	}

	for(int i=j; args[i] != NULL; i++){
			args[i] = NULL;
		}

	lastIndex = j;
}