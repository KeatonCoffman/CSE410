//
//	Keaton Coffman
//	Project 3 
//	CSE 410
//
//

#include <unistd.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <ctime>
using namespace std;


int main(int argc, char* argv[],char* env[]){
	char user[100];
	int sequence_num = 1;
	string command;
	int exit_flag = 1;
	vector<string> command_list;
	char current_char;
	
	getlogin_r(user,100);
	
	while(exit_flag){
		cout << "<" <<sequence_num << " " << user << ">";
		sequence_num++;
		string current_command;
		
		// While loop that puts all non whitespace characters into a string
		// 		Each string is then put into a vector called command_list
		while(current_char = getchar()){
			while (current_char == ' ' or current_char == '\t'){
				current_char = getchar();
				if(command.length() != 0){
					command_list.push_back(command);
					command.clear();
				}
			}
			if (current_char == '\n'){
				if(command.length() != 0){
					command_list.push_back(command);
					command.clear();
				}
				break;
			}
			command += current_char;
		};
		
		// If there are no commands do not advance the sequence number
		if(command_list.size() == 0){
			sequence_num--;
		}
		
		// Processes each string in vector command_list and d
		for(int i=0;i<command_list.size();i++){
			
			// Processes command quit
			if(command_list[i] == "quit" ){
				exit_flag = 0;
				return 0;
			}
			
			// Processes command env to print all environment variables
			else if(command_list[i] == "env"){
				int i=0;
				while(env[i]){
					cout << env[i] <<endl;
					i++;
				}
				command_list.clear(); 
			}
			
			// Processes command date to print the date and time
			else if(command_list[i] == "date"){
				time_t current_time = std::time(NULL);
				cout << "The current date and time is : " << asctime(std::localtime(&current_time))<<endl;
				command_list.clear(); 	
			}
			
			// Processes command curr to print out current working directory
			else if(command_list[i] == "curr"){
				cout << "Current directory is: " << get_current_dir_name() << endl;
				command_list.clear(); 
			}
			
			// Processes command cd to change the directory according to the following command
			else if(command_list[i] == "cd"){
				if(command_list.size() == 1){
					chdir(getenv("HOME"));
					cout << "Current directory is now your home directory" << endl;
				}
				if(command_list.size() == 2){
					if(chdir((command_list[i+1]).c_str()) == 0){
						cout<< "Directory was successfully changed" <<endl;
					}
					else{
						cout << "The directory you want to change to is invalid " << endl;
					}
				}
				if(command_list.size() >= 2){
					if(chdir((command_list[i+1]).c_str()) == 0){
						cout<< "Directory was successfully changed but there were extra tokens given that were not processed" <<endl;
					}
					
					if(chdir((command_list[i+1]).c_str()) == -1){
						cout<< "Path given after cd command is invalid" << endl;
					}
				}
				command_list.clear(); 	
			}
			
			// Tells the user if tokens were invalid
			else{
				 cout << "Invalid token found" <<endl;
			}
		}
		command_list.clear();
		
	}
	
}
