//
//	Keaton Coffman
//	Project 4
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
#include <sstream>
#include <map>
using namespace std;


int main(int argc, char* argv[],char* env[]){
	char user[100];
	int sequence_num = 1;
	string command;
	int exit_flag = 1;
	vector<string> command_list;
	vector<string> directory_list;
	vector<string> history_list;
	vector<string> prev_cmds;
	vector<string> local_env_var;
	map<string,string> env_variables;
	char current_char;
	
	getlogin_r(user,100);

	int i=0;
	while(env[i]!=NULL){
		local_env_var.push_back(env[i]);
		i++;
	}
	
	for(auto item:local_env_var){
		env_variables[item.substr(0,item.find('='))] = item.substr(item.find('=')+1);
	}
	

	
	
	while(exit_flag){
		cout << "<" <<sequence_num << " " << user << ">";
		sequence_num++;
		string current_command;
		
		// Checks to see if previous commands vector from "!N" input is empty; if not execute the commands inside of previous commands vector
		if(prev_cmds.size() > 0){
			command_list = prev_cmds;
			prev_cmds.clear();
			goto Noinput;
		}
		
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
		
		// For go to statement when "!N" is entered
		Noinput:
		
		// If there are no commands do not advance the sequence number
		if(command_list.size() == 0){
			sequence_num--;
			command_list.push_back("");
		}
			
		// Processes command quit
		if(command_list[0] == "quit" ){
			exit_flag = 0;
			return 0;
		}
		
		// Processes command env to print all environment variables
		else if(command_list[0] == "env"){
			int i=0;
			if (command_list.size() == 1){
				history_list.push_back(command_list[0]);
				for(auto elem : env_variables){
					cout << elem.first << "=" << elem.second << "\n";
				}
			}
			
			if (command_list.size() == 2){
				string tmp = command_list[0] + " " + command_list[1];
				history_list.push_back(tmp);
				string key = command_list[1];
				cout << "The environment variables value is: " << env_variables[key] <<endl;
			}
			
			if (command_list.size() == 3){
				string tmp = command_list[0] + " " + command_list[1] + " " + command_list[2];
				history_list.push_back(tmp);
				
				
				string var_name = command_list[1];
				string value = command_list[2];
				
				env_variables[var_name] = value;			
			}
			
			command_list.clear(); 
		}
		
		
		// Processes command date to print the date and time
		else if(command_list[0] == "date"){
			history_list.push_back(command_list[0]);
			time_t current_time = std::time(NULL);
			cout << "The current date and time is : " << asctime(std::localtime(&current_time));
			command_list.clear(); 	
		}
		
		// Processes command curr to print out current working directory
		else if(command_list[0] == "curr"){
			history_list.push_back(command_list[0]);
			cout << "Current directory is: " << get_current_dir_name() <<endl;
			command_list.clear(); 
		}
		
		// Processes command cd to change the directory according to the following command
		else if(command_list[0] == "cd"){
			
			if(command_list.size() == 1){
				history_list.push_back(command_list[0]);
				chdir(getenv("HOME"));
				directory_list.push_back(get_current_dir_name());
				cout << "Current directory is now your home directory" << endl;
			}
			
			if(command_list.size() == 2){
				string tmp = command_list[0] + " " + command_list[1];
				history_list.push_back(tmp);
				if(command_list[1][0] == '#'){
					string index = command_list[1].erase(0,1);
					int dlist_index = stoi(index);
					if(chdir(directory_list[dlist_index-1].c_str()) == 0){
						directory_list.push_back(get_current_dir_name());
						cout<< "Directory was successfully changed" <<endl;
					}
					else{
						cout << "The directory you want to change to is invalid " << endl;
					}
				}
				
				else if(command_list[1][0] == '~'){
					string dir = command_list[1];
					dir.erase(0,1);
					dir = "/user/"+dir;
					cout << dir << endl;
					if(chdir(dir.c_str()) == 0){
						directory_list.push_back(get_current_dir_name());
						cout<< "Directory was successfully changed" <<endl;
					}
					else{
						cout<< "The directory you want to change to is invalid" <<endl;
					}
				}
				
				else{
					if(chdir((command_list[1]).c_str()) == 0){
						directory_list.push_back(get_current_dir_name());
						cout<< "Directory was successfully changed" <<endl;
					}
					else{
						cout << "The directory you want to change to is invalid " << endl;
					}
				}
			}
			command_list.clear(); 	
		}
		
		// Processes command dlist to print out the ten most recent directories the user has visited
		else if(command_list[0] == "dlist" ){
			if(directory_list.size() == 0){
				cout << "The directory list is empty as it does not include the current working directory" << endl;
			}
			
			else{
				history_list.push_back(command_list[0]);
				if(directory_list.size() <= 10){
					int j=1;
					for(int i = 0; i<directory_list.size(); i++){
						cout<< j << " " << directory_list[i] <<endl;
						j++;
					}
				}
				if(directory_list.size() > 10){
					int start = directory_list.size()-11;
					int j=1;
					for(int i = start; i<=(9+start);i++){
						cout<< j << " " << directory_list[i] <<endl;
						j++;
					}
				}
			}
		}
		
		
		// Processes command clist to print out the ten most recent commands put in by the user
		else if(command_list[0] == "clist" ){
			history_list.push_back(command_list[0]);
			if(history_list.size() <= 10){
				for(int i = 0; i<history_list.size(); i++){
					cout<< i+1 << " " << history_list[i] <<endl;
				}
			}
			if(history_list.size() > 10){
				int start = history_list.size()-11;
				for(int i = start; i<=(10+start);i++){
					cout<< i+1 << " " << history_list[i] <<endl;
				}
			}
		}
		
		// Processes command !N where N is the number of a command in the commandlist, it then executes that command from the list
		else if(command_list[0][0] == '!'){
			string index = command_list[0].erase(0,1);
			int hist_index = stoi(index);
			if(hist_index > history_list.size()){
				cout <<  "Not enough previous commands" <<endl;
			}
			else{
				string temp_string;
				cout << history_list[hist_index-1] << endl;
				istringstream input(history_list[hist_index-1]);
				while(input >> temp_string){
					prev_cmds.push_back(temp_string);
				}
			}
		}
		
		// Tells the user if tokens were invalid and adds invalid commands to the history list
		else{
			string tmp;
			tmp = command[0] + " ";
			for(int i=0;i<command_list.size();i++){
				tmp += command_list[i] + " " ;
			}
			history_list.push_back(tmp);
			 cout << "Command not found" <<endl; 
		}
		command_list.clear();
		
	}
	
}
