//
//	Keaton Coffman
//	Project 9
//	CSE 410
//

// I put a lot of time into this and I was still unable to get my times perfectly right
// My Priority is not working but by my logic was that I was just going to re-sort my ready_state container if there was more than one item in it

// The flaws in my logic for FCFS seem to be minor but I cannot seem to figure them out 
// For Priority I was going to use my FCFS algorithm and just sort my ready state deque like I said above

// I don't know if explaining  my logic will get me more points but I worked an extreme amount of hours trying to get this done

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
#include <deque>

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
	float normalized_turnaround = 0;
	int time_to_wait = 0;
	int departure_time = 0;
	
	process& operator=(process& a){
		cpu_bursts = a.cpu_bursts;
		priority = a.priority;
		burst_time = a.burst_time;
		blocked_time = a.blocked_time;
		arrival_time =a.arrival_time;
		PID =a.PID;
		time_in_new =a.time_in_new;
		time_in_ready =a.time_in_ready;
		time_in_running =a.time_in_running;
		time_in_blocked =a.time_in_blocked;
		time_cpu_used =a.time_cpu_used;
		turnaround_time =a.turnaround_time;
		normalized_turnaround =a.normalized_turnaround;
		time_to_wait =a.time_to_wait;
	}
	
};

struct current_system{
	int CPUs; 
	int PCBs;
	int length_of_sim;
	string algorithm;
};

void print_states(vector<process> processes){
			
			deque<process> new_state;
			deque<process> ready;
			deque<process> running;
			deque<process> blocked;
			
			for(auto item:processes){
				if(item.state =="new"){
					new_state.push_front(item);
				}
				if(item.state =="ready"){
					ready.push_front(item);
				}
				if(item.state =="running"){
					running.push_front(item);
				}
				if(item.state =="blocked"){
					blocked.push_front(item);
				}
				
			}
			
			
			cout << "New State: ";
			for(auto item : new_state){
				cout << " " << item.PID << " ";
			}
			cout << endl;
			
			cout << "Ready State: ";
			for(auto item : ready){
				cout << " " << item.PID << " ";
			}
			cout << endl;
			
			cout << "Running State: ";
			for(auto item : running){
				cout << " " << item.PID << " ";
			}
			cout << endl;
			
			cout << "Blocked State: ";
			for(auto item : blocked){
				cout << " " << item.PID << " ";
			}
			cout << endl;
			
			new_state.clear();
			ready.clear();
			running.clear();
			blocked.clear();
}

void end_print(process current, int departure_time){
	cout << endl;
	cout << "PID: " << current.PID << endl;
	cout << "Priority: " << current.priority<< endl;
	cout << "Cpu Bursts: " << current.cpu_bursts << endl;
	cout << "Burst time: " << current.burst_time << endl;
	cout << "Blocked time: " << current.blocked_time << endl;
	cout << "Arrival time: " << current.arrival_time << endl;
	cout << "Departure time: " << current.departure_time << endl;
	cout << "Time in new: " << current.time_in_new << endl;
	cout << "Time in ready: " << current.time_in_ready << endl;
	cout << "Time in running: " << current.time_in_running << endl;
	cout << "Time in blocked: " << current.time_in_blocked << endl;
	cout << "Turnaround time: " << current.turnaround_time << endl;
	cout << "Normalized turnaround time: " << current.normalized_turnaround<< endl;
}


// Function meant to be used in Priority algorithm to sort ready state deque if the ready state had more than one process in it
bool compare_processes(const process &a, const process &b){
	if(a.priority != b.priority){
		return a.priority < b.priority;
	}
}

void print_N(int tick,int N, vector<process> processes){
	if(tick%N == 0 && tick !=0){
		print_states(processes);
	}
	
}

int main(int argc, char* argv[]){
	
	deque<process> ready_state;
	vector<process> all_processes;
	
	
	
	process current_process;
	current_system sys;
	int print=0;
	int N = stoi(argv[1]);
	int no_processes_flag = 0;
	int running_temp_time = 0;
	int blocked_temp_time = 0;
	int ended_processes = 0;
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
	while(getline(infile,line)){
		stringstream ss;
		process current_process;
		ss << line;
		ss >> current_process.PID;
		ss >> current_process.priority;
		ss >> current_process.cpu_bursts;
		ss >> current_process.burst_time;
		ss >> current_process.blocked_time;
		ss >> current_process.arrival_time;
		all_processes.push_back(current_process);
	}
	
	cout << "CPUs : " << sys.CPUs <<endl;
	cout << "PCBs : " << sys.PCBs <<endl;
	cout << "Length of Simulation : " << sys.length_of_sim <<endl;
	cout << "Algorithm : " << sys.algorithm <<endl;
	
	
	for(int j=0;j<sys.length_of_sim;j++){

			cout << endl;
			cout << "tick: " << j <<endl;
			print_N(j,N,all_processes);
			
			if(sys.algorithm == "FCFS"){
				
				
				for(int i=0;i<all_processes.size();i++){
					
					// RUNNING TO BLOCKED
					// Recognize a request from process in Running state
					//cout << "PID: " << all_processes[i].PID << "  state:  " << all_processes[i].state <<endl;
					if(all_processes[i].state == "running"){
						if( all_processes[i].time_to_wait == 0){
							all_processes[i].state = "blocked";
							//cout << " PROCESS: "<< all_processes[i].PID << " WENT TO BLOCKED FOR THIS AMOUNT OF TICKS: "<< all_processes[i].blocked_time << endl;
							all_processes[i].time_cpu_used++;
							all_processes[i].time_to_wait = all_processes[i].blocked_time;
							sys.CPUs++;
						}
						else{
							all_processes[i].time_in_running++;
							all_processes[i].time_to_wait--;
							if(all_processes[i].time_to_wait == 0){
								all_processes[i].state = "blocked";
								//cout << " PROCESS: "<< all_processes[i].PID << " WENT TO BLOCKED FOR THIS AMOUNT OF TICKS: "<< all_processes[i].blocked_time << endl;
								all_processes[i].time_cpu_used++;
								all_processes[i].time_to_wait = all_processes[i].blocked_time;
								sys.CPUs++;
							}
						}
					
					}
					
						
					// BLOCKED TO READY
					// Recognize a process becoming unblocked
					if(all_processes[i].state == "blocked" && all_processes[i].time_to_wait > 0){
							all_processes[i].time_to_wait-=1;
							//cout << "process : " << all_processes[i].PID << " ticks to wait: " << all_processes[i].time_to_wait <<endl;
							all_processes[i].time_in_blocked++;
							if(all_processes[i].time_to_wait == 0){
								all_processes[i].state = "ready";
								ready_state.push_back(all_processes[i]);
							}
					}
					if(all_processes[i].state == "blocked" && all_processes[i].time_to_wait == 0 ){
							all_processes[i].state = "ready";
							ready_state.push_back(all_processes[i]);
					}
					
					
					

					// EMPTY TO NEW STATE TO READY
					// Recognize arrival of new process
					if(all_processes[i].state == "" && all_processes[i].arrival_time == j+1){
						all_processes[i].state = "new";	
						if(sys.PCBs != 0){
							sys.PCBs--;
							all_processes[i].state = "ready";
							ready_state.push_back(all_processes[i]);
						}
						else{
							all_processes[i].time_in_new++;
						}
					}
					
					// NEW TO READY
					// Allocate a PCB in the New state
					if(all_processes[i].state == "new"){
							if(sys.PCBs != 0){
								sys.PCBs--;
								all_processes[i].state = "ready";
								ready_state.push_back(all_processes[i]);
							}
							else{
								all_processes[i].time_in_new++;
							}
					}
					
					// READY TO RUNNING
					// Dispatch a process in the ready state
					if(all_processes[i].state == "ready"){
						if(sys.CPUs != 0){
							process temp = ready_state.front();
							//cout << "the next item to leave the ready state is : " << temp.PID << endl;
							for(int k=0;k<all_processes.size();k++){
								if(all_processes[k].PID == temp.PID){
									sys.CPUs--;
									cout << endl;
									//cout << "READY SENT TO RUNNING" << endl;
									all_processes[k].state = "running";
									ready_state.pop_front();
									//cout << "PID: " << all_processes[k].PID << "  leaves running in this many ticks tick :  " <<  all_processes[k].burst_time <<endl;
									all_processes[k].time_to_wait = all_processes[k].burst_time ;
								}
							}
						}
						else{
							all_processes[i].time_in_ready++;
						}
					}
					
					if(all_processes[i].time_cpu_used == all_processes[i].cpu_bursts && all_processes[i].state != "exit"){
							all_processes[i].state = "exit";
							ended_processes++;
							sys.CPUs++;
							sys.PCBs++;
							all_processes[i].turnaround_time = j - all_processes[i].arrival_time;
							all_processes[i].departure_time = j;
							all_processes[i].normalized_turnaround = all_processes[i].turnaround_time/all_processes[i].time_in_running;
							end_print(all_processes[i],j);
					}
					
				 	//if(ended_processes == all_processes.size()){
							//print states function
					//		print_states(all_processes);
					//		cout << "program is ending because all processes have exited" <<endl;
					//		break;
					//}
					
				} 
				if(ended_processes == all_processes.size()){
							//print states function
							print_states(all_processes);
							cout << "Program is ending because all processes have exited" <<endl;
							break;
				}
			} // end of FCFS
			
			
			// Placeholder for project 10
			else if(sys.algorithm == "RR"){
				
			}
			
			else if(sys.algorithm == "Priority"){
				for(int i=0;i<all_processes.size();i++){
					
					// RUNNING TO BLOCKED
					// Recognize a request from process in Running state
					//cout << "PID: " << all_processes[i].PID << "  state:  " << all_processes[i].state <<endl;
					if(all_processes[i].state == "running"){
						if( all_processes[i].time_to_wait == 0){
							all_processes[i].state = "blocked";
							//cout << " PROCESS: "<< all_processes[i].PID << " WENT TO BLOCKED FOR THIS AMOUNT OF TICKS: "<< all_processes[i].blocked_time << endl;
							all_processes[i].time_cpu_used++;
							all_processes[i].time_to_wait = all_processes[i].blocked_time;
							sys.CPUs++;
						}
						else{
							all_processes[i].time_in_running++;
							all_processes[i].time_to_wait--;
							if(all_processes[i].time_to_wait == 0){
								all_processes[i].state = "blocked";
								//cout << " PROCESS: "<< all_processes[i].PID << " WENT TO BLOCKED FOR THIS AMOUNT OF TICKS: "<< all_processes[i].blocked_time << endl;
								all_processes[i].time_cpu_used++;
								all_processes[i].time_to_wait = all_processes[i].blocked_time;
								sys.CPUs++;
							}
						}
					
					}
					
						
					// BLOCKED TO READY
					// Recognize a process becoming unblocked
					if(all_processes[i].state == "blocked" && all_processes[i].time_to_wait > 0){
							all_processes[i].time_to_wait-=1;
							//cout << "process : " << all_processes[i].PID << " ticks to wait: " << all_processes[i].time_to_wait <<endl;
							all_processes[i].time_in_blocked++;
							if(all_processes[i].time_to_wait == 0){
								all_processes[i].state = "ready";
								ready_state.push_back(all_processes[i]);
							}
					}
					if(all_processes[i].state == "blocked" && all_processes[i].time_to_wait == 0 ){
							all_processes[i].state = "ready";
							ready_state.push_back(all_processes[i]);
					}
					
					
					

					// EMPTY TO NEW STATE TO READY
					// Recognize arrival of new process
					if(all_processes[i].state == "" && all_processes[i].arrival_time == j+1){
						all_processes[i].state = "new";	
						if(sys.PCBs != 0){
							sys.PCBs--;
							all_processes[i].state = "ready";
							ready_state.push_back(all_processes[i]);
						}
						else{
							all_processes[i].time_in_new++;
						}
					}
					
					// NEW TO READY
					// Allocate a PCB in the New state
					if(all_processes[i].state == "new"){
							if(sys.PCBs != 0){
								sys.PCBs--;
								all_processes[i].state = "ready";
								ready_state.push_back(all_processes[i]);
							}
							else{
								all_processes[i].time_in_new++;
							}
					}
					
					// READY TO RUNNING
					// Dispatch a process in the ready state
					if(all_processes[i].state == "ready"){
						if(ready_state.size() > 1){
							//std::sort(ready_state.begin(),ready_state.end(),compare_processes);
							if(sys.CPUs != 0){
								process temp = ready_state.front();
								//cout << "the next item to leave the ready state is : " << temp.PID << endl;
								for(int k=0;k<all_processes.size();k++){
									if(all_processes[k].PID == temp.PID){
										sys.CPUs--;
										cout << endl;
										//cout << "READY SENT TO RUNNING" << endl;
										all_processes[k].state = "running";
										ready_state.pop_front();
										//cout << "PID: " << all_processes[k].PID << "  leaves running in this many ticks tick :  " <<  all_processes[k].burst_time <<endl;
										all_processes[k].time_to_wait = all_processes[k].burst_time ;
									}
								}
								}
						}
						//std::sort(ready_state.begin(),ready_state.end(),compare_processes);
						if(sys.CPUs != 0){
							process temp = ready_state.front();
							//cout << "the next item to leave the ready state is : " << temp.PID << endl;
							for(int k=0;k<all_processes.size();k++){
								if(all_processes[k].PID == temp.PID){
									sys.CPUs--;
									cout << endl;
									//cout << "READY SENT TO RUNNING" << endl;
									all_processes[k].state = "running";
									ready_state.pop_front();
									//cout << "PID: " << all_processes[k].PID << "  leaves running in this many ticks tick :  " <<  all_processes[k].burst_time <<endl;
									all_processes[k].time_to_wait = all_processes[k].burst_time ;
								}
							}
						}
						else{
							all_processes[i].time_in_ready++;
						}
					}
					
					if(all_processes[i].time_cpu_used == all_processes[i].cpu_bursts && all_processes[i].state != "exit"){
							all_processes[i].state = "exit";
							ended_processes++;
							sys.CPUs++;
							sys.PCBs++;
							all_processes[i].turnaround_time = j - all_processes[i].arrival_time;
							all_processes[i].departure_time = j;
							all_processes[i].normalized_turnaround = all_processes[i].turnaround_time/all_processes[i].time_in_running;
							end_print(all_processes[i],j);
					}
				} 
				
				// End program
				if(ended_processes == all_processes.size()){
							//print states function
							print_states(all_processes);
							cout << endl;
							cout << "Program is ending because all processes have exited" <<endl;
							break;
				}
				
			}
		
		
	}// main for loop ends here


	
}// main ends here
	
