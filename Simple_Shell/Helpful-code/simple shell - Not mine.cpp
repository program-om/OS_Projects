/*
	---- This code is not mine ----
*/

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command */

using namespace std;

// Functions Prototypes
void run(char *args[], vector<string> &argv, int argc);
void print_command_argv(vector<string>& argv);
void delete_args(char **args, int argc);
void split_string(string command_line, vector<string>& argv);
void argv_to_args(vector<string>& argv, char **args, int& argc);
void execute(int &argc, vector<string> &argv, vector<string> &old_commands,
      string command_line, int &should_run, char *args[]);

int main(void)
{
  
  int argc;
  char *args[MAX_LINE/2 + 1]; /* command line arguments */
  int should_run = 1;         /* flag to determine when to exit program */
  string command_line;
  vector<string> argv;
  vector<string> old_commands;
  
  while (should_run)
    {
      printf("osh>");
      fflush(stdout);
      getline(cin, command_line);

      
      if (command_line == "!!")
 {
   if(old_commands.size() > 0)
     execute(argc, argv, old_commands, old_commands[old_commands.size() - 1], should_run, args);
   else
     cout << "no old commands" << endl;
 }
      else if ((command_line[0] == '!')&&(isdigit(command_line[1])))
  {
    
    string::size_type sz;   // alias of size_t
    string nume = command_line.substr(1);
    
    int command_number = stoi (nume, &sz);
    if ((command_number > 0)&&(command_number < old_commands.size()))
      {
        if(old_commands.size() > 0)
      execute(argc, argv, old_commands, old_commands[command_number], should_run, args);
        else
      cout << "no old commands" << endl;
      }
    else
      {
        cout << "NULL Number" << endl;

      }
  
 }
       
       else
  {
    execute(argc, argv, old_commands, command_line, should_run, args);
  }
  
    }
  
  return 0;
}


void execute(int& argc, vector<string> &argv, vector<string> &old_commands,
      string command_line, int& should_run, char *args[])
{
  if  (command_line != "")
    {
      
      split_string(command_line, argv);    
      argv_to_args(argv, args, argc);    
      
      if ((command_line != "exit")&&(command_line != "history"))
 {
   run(args, argv, argc);
     if (argv[argc - 1] == "&")
       delete_args(args, argc -1);
     else
       delete_args(args, argc -1);
 }
      
      if (command_line == "exit")
 should_run = 0;
      
      if((argv[0] != "") || (argv[0] != " "))
 old_commands.push_back(command_line);
      
      if (command_line == "history")
 {
   if (old_commands.size() < 10)
     {
       for(int c = 0; c < old_commands.size(); c++)
  cout << c <<". "<< old_commands[c] << endl;
     }
   else
       {
  for(int c = old_commands.size() - 10; c < old_commands.size(); c++)
    cout << c <<". "<< old_commands[c] << endl;
       }
 }
      
      argv.clear();
      
    }    
}


// Functions that splits the command line by spaces
void split_string(string command_line, vector<string>& argv )
{ 
  int j = 0;
  int k = 0;
  bool skip = true;
  
  
  for ( int i = 0; i < command_line.length(); i++)
  {
    if ((command_line[i] == ' ') || (command_line[i] == '\0'))
      {

 if (!skip)
   if ((i+1) < command_line.length())
     if ((command_line[i+1] != ' ') && (command_line[i+1] != '\0'))
       j++;
      }
    else
      {
 skip = false;
 if (k == j)
   {
     argv.push_back("");
     k++;
   }
 argv[j].push_back(command_line[i]);
      }
  }

  
}

// Function that converts the vector of strings in Array of Pointers char(arguments)
void argv_to_args(vector<string>& argv, char **args, int& argc)
{

  argc = argv.size();

  if (argv[argc - 1] == "&")
    {
      for (int i = 0; i < argc-1 ; i++)
 { 
   args[i] = new char[argv[i].size() + 1];
   copy(argv[i].begin(), argv[i].end(), args[i]);
   args[i][argv[i].size()] = '\0'; // don't forget the terminating 0
 }
    }
  else
    {
      for (int i = 0; i < argc; i++)
 {
   args[i] = new char[argv[i].size() + 1];
   copy(argv[i].begin(),argv[i].end(), args[i]);
   args[i][argv[i].size()] = '\0'; // don't forget the terminating 0
 }
    }
  

}




// Function that execute the command line argument and forks the processes
void run(char *args[], vector<string> &argv, int argc)
{
  pid_t pid;

  int status;

  cout << "Run starts: " << endl;
  
  if ((pid = fork()) == -1) 
    {
      perror("fork"); 
      return;
    }
  
  if (pid == 0) // child process
    {
           
      //Check if the command has & or not
      if (argv[argc - 1] == "&")
 {
 args[argc -1] = NULL;    //set the last index to null
 pid_t pid2;
   
   pid2=fork();
   if(pid2==0)
     {
       if (execvp(args[0], args) < 0) // excute the command
  {
    printf("*** ERROR: exec failed\n");
    exit(1);
  } 
     }
   else
     {
       exit(0);
     }

 }
      else
 {
   args[argc] = NULL;
   if (execvp(args[0], args) < 0)  /* execute the command  */
     {    
       printf("*** ERROR: exec failed\n");
       exit(1);
     }
 }
      
      
      
    }
  else // parent process
    {
      wait(NULL);
    }
  
}

// Function the prints the arguments
void print_command_argv(vector<string>& argv)
{
  for(int m = 0; m < argv.size(); m++)
    cout << argv[m] << endl;
  
}




// Destructor 
void delete_args(char **args, int argc)
{
  for(int m = 0; m < argc; m++)
    delete[] args[m];
}