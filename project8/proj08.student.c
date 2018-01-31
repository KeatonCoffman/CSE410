//
//	Keaton Coffman
//	Project 7
//	CSE 410
//
//

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream> 
#include <utility>
#include <algorithm>
#include <sstream>
#include <iostream> 

using namespace std;

struct process{
	int new_flag = 1;
	string state = "";
	int cpu_bursts = 0;
	int priority = 0;
	int burst_time = 0;
	int blocked_time = 0;
	int arrival_time = 0;
	int PID = 0;
	int time_in_new = 0;
	int time_in_ready = 0;
	int time_in_running = 0;
	int time_in_blocked = 0;
	int time_cpu_used = 0;
	int turnaround_time = 0;
	int normalized_turnaround = 0;
};

struct current_system{
	int CPUs; 
	int PCBs;
	int length_of_sim;
	string algorithm;
};

void print_states(vector<process> one,vector<process> two,vector<process> three, vector<process> four){
			cout << endl;
			cout << "New State: ";
			for(auto item : one){
				cout << " " << item.PID << " ";
			}
			cout << endl;
			
			cout << "Ready State: ";
			for(auto item : two){
				cout << " " << item.PID << " ";
			}
			cout << endl;
			
			cout << "Running State: ";
			for(auto item : three){
				cout << " " << item.PID << " ";
			}
			cout << endl;
			
			cout << "Blocked State: ";
			for(auto item : four){
				cout << " " << item.PID << " ";
			}
			cout << endl;
}

void end_print(process current, int departure_time){
	cout << endl;
	cout << "PID: " << current.PID << endl;
	cout << "Priority: " << current.priority<< endl;
	cout << "Cpu Bursts: " << current.cpu_bursts << endl;
	cout << "Burst time: " << current.burst_time << endl;
	cout << "Blocked time: " << current.blocked_time << endl;
	cout << "Arrival time: " << current.arrival_time << endl;
	cout << "Departure time: " << departure_time << endl;
	cout << "Time in new: " << current.time_in_new << endl;
	cout << "Time in ready: " << current.time_in_ready << endl;
	cout << "Time in running: " << current.time_in_running << endl;
	cout << "Time in blocked: " << current.time_in_blocked << endl;
	cout << "Turnaround time: " << current.turnaround_time << endl;
	cout << "Normalized turnaround time: " << current.normalized_turnaround<< endl;
}



int main(int argc, char* argv[]){
	
	vector<process> new_state;
	vector<process> ready;
	vector<process> running;
	vector<process> blocked;
	
	
	
	process current_process;
	current_system sys;
	int print=0;
	string line;
	
	ifstream infile;
	infile.open(argv[2],ios::out);
	
	if(!infile){
		cout << "Error opening file" <<endl;
	}
	
	// Capture Simulation parameters
	getline(infile,line);
	sys.CPUs = stoi(line);
	
	getline(infile,line);
	sys.PCBs = stoi(line);
	
	getline(infile,line);
	sys.length_of_sim = stoi(line);
	
	getline(infile,line);
	sys.algorithm = line;
	
	
	// Capture process parameters
	getline(infile,line);
	stringstream ss;
	ss << line;
	ss >> current_process.PID;
	ss >> current_process.priority;
	ss >> current_process.cpu_bursts;
	ss >> current_process.burst_time;
	ss >> current_process.blocked_time;
	ss >> current_process.arrival_time;
	
	
	int i=0;
	while( i < (sys.length_of_sim)){
		//cout << "time: " << i <<endl;
		

		// CREATE THE PROCESS AND ARRIVAL TIME TO TICKS
		if(current_process.state == "" ){
			current_process.state = "new";
			new_state.push_back(current_process);
			i = i + current_process.arrival_time;
			if( i%stoi(argv[1]) == 0 && stoi(argv[1])!=0 && i!=0){
				print_states(new_state,ready,running,blocked);
			}
		}
		
		// NEW TO READY
		else if(current_process.state == "new" && sys.PCBs > 0){
			current_process.state = "ready";
			new_state.clear();
			ready.push_back(current_process);
			//cout << "new current process has been changed to ready state" << endl;
			if( i%stoi(argv[1]) == 0 && stoi(argv[1])!=0 && i!=0){
				print_states(new_state,ready,running,blocked);
			}
		}
				
		// WAITING IN NEW
		else if(current_process.state == "new" && sys.PCBs < 1){
			current_process.time_in_new++;
			if( i%stoi(argv[1]) == 0 && stoi(argv[1])!=0 && i!=0){
				print_states(new_state,ready,running,blocked);
			}
		}
		
		// READY TO RUNNING
		else if(current_process.state == "ready" && sys.CPUs > 0){
			current_process.state = "running";
			ready.clear();
			running.push_back(current_process);
			if( i%stoi(argv[1]) == 0 && stoi(argv[1])!=0 && i!=0){
				print_states(new_state,ready,running,blocked);
			}
		}
		
		// WAITING IN READY
		else if(current_process.state == "ready" && sys.CPUs < 1){
			current_process.time_in_ready++;
			if( i%stoi(argv[1]) == 0 && stoi(argv[1])!=0 && i!=0){
				print_states(new_state,ready,running,blocked);
			}
		}
		
		// WAITING IN BLOCKED
		else if(current_process.state == "blocked" && current_process.blocked_time > current_process.time_in_blocked){
			int temp = current_process.blocked_time;
			while(current_process.blocked_time > 0){
				i++;
				cout << i << " in blocked state " << endl;
				current_process.blocked_time--;
				current_process.time_in_blocked++;
				if( i%stoi(argv[1]) == 0 && stoi(argv[1])!=0 && i!=0){
					print_states(new_state,ready,running,blocked);
				}
			}
			current_process.state = "ready";
			blocked.clear();
			ready.push_back(current_process);
			current_process.blocked_time = temp;
		}
		
		// BLOCKED TO READY
		else if(current_process.state == "blocked" && current_process.blocked_time == current_process.time_in_blocked){
			current_process.state == "ready";
			blocked.clear();
			ready.push_back(current_process);
			current_process.time_in_blocked = 0;
			if( i%stoi(argv[1]) == 0 && stoi(argv[1])!=0 && i!=0){
				print_states(new_state,ready,running,blocked);
			}
		}
		
		// WAITING IN RUNNING
		else if(current_process.state == "running" && current_process.cpu_bursts > current_process.time_cpu_used){
			current_process.time_cpu_used++;
			int temp = current_process.burst_time;
			
			while(current_process.burst_time > 0){
				i++;
				//cout << i << " in running state " << endl;
				current_process.time_in_running++;
				current_process.burst_time--;
				
				if( i%stoi(argv[1]) == 0 && stoi(argv[1])!=0 && i!=0){
					print_states(new_state,ready,running,blocked);
				}
			}
			current_process.burst_time = temp;
			current_process.state = "blocked";
			blocked.push_back(current_process);
			running.clear();
			if(current_process.cpu_bursts == current_process.time_cpu_used){
				blocked.clear();
				current_process.state = "exit";
			}
			
		}
		
		else if(current_process.state == "exit"){
			if( i%stoi(argv[1]) == 0 && stoi(argv[1])!=0 && i!=0){
				print_states(new_state,ready,running,blocked);
			}
			
			goto Done;
		}
		
		// LENGTH OF SIMULATION ENDS
		else if(i+1 == sys.length_of_sim){
			//cout << "LENGTH OF SIMULATION IS OVER" <<endl;
			i++;
		}
				
		else {
			i++;
		}
		
	}// while
	Done:
	current_process.turnaround_time = i - current_process.arrival_time;
	current_process.normalized_turnaround = current_process.turnaround_time/current_process.time_in_running;
	end_print(current_process,i);
;	
};

