//
//	Keaton Coffman
//	Project 10
//	CSE 410
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
	double turnaround_time = 0;
	double normalized_turnaround = 0;
	int time_to_wait = 0;
	int departure_time = 0;
	
};

struct current_system{
	int CPUs; 
	int PCBs;
	int length_of_sim;
	string algorithm;
	int time_quantum;
};

void print_states(vector<process> new_state,vector<process> ready,vector<process> running,vector<process> blocked){
			
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


bool compare_processes(const process &a, const process &b){
	if(a.priority != b.priority){
		return a.priority < b.priority;
	}
}

void print_N(int tick,int N, vector<process> new_state,vector<process> ready,vector<process> running,vector<process> blocked){
	if(tick%N == 0 && tick !=0){
		print_states(new_state,ready,running,blocked);
	}
	
}

int main(int argc, char* argv[]){
	
	deque<process> ready_state;
	vector<process> all_processes;
	
	vector<process> new_state;
	vector<process> ready;
	vector<process> running;
	vector<process> blocked;

	process current_process;
	current_system sys;
	int print=0;
	int N = stoi(argv[1]);
	int ended_processes = 0;
	int time_for_quantum = 0;
	string line;
	
	ifstream infile;
	infile.open(argv[2],ios::out);
	
	if(!infile){
		cout << "Error opening file" <<endl;
	}
	
	// Capture Simulation parameters
	getline(infile,line);
	stringstream cpu;
	cpu << line;
	cpu >> sys.CPUs;
	
	getline(infile,line);
	stringstream pcb;
	pcb << line;
	pcb >> sys.PCBs;

	
	getline(infile,line);
	stringstream length;
	length << line;
	length >> sys.length_of_sim;

	
	getline(infile,line);
	stringstream alg;
	alg << line;
	alg >> sys.algorithm;
	if(sys.algorithm == "RR"){
		alg >> sys.time_quantum;
	}

	
	
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

					
			if(sys.algorithm == "FCFS"){
					
					// RUNNING TO BLOCKED
					// Recognize a request from process in Running state
					for (int i=0;i<running.size();i++ ){
						running[i].time_to_wait--;
						cout << running[i].time_to_wait << endl;
						if( running[i].time_to_wait == 0){
							//running[i].state = "blocked";
							running[i].time_cpu_used++;
							cout << " PROCESS: "<< running[i].PID << " times cpu used : "<< running[i].time_cpu_used << " compared to: " << running[i].cpu_bursts << endl;

							if(running[i].time_cpu_used == running[i].cpu_bursts ){
								cout << " PROCESS : "<< running[i].PID << "  EXITED " << endl;
								
								ended_processes++;
								
								running[i].turnaround_time = j - running[i].arrival_time;
								running[i].departure_time = j;
								running[i].normalized_turnaround = running[i].turnaround_time/(float)running[i].time_in_running;
								end_print(running[i],j);
								running.erase(running.begin()+i);
								i--;
							}
							else{
								running[i].time_to_wait = running[i].blocked_time;
								blocked.push_back(running[i]);
								running.erase(running.begin()+i);
								i--;
							}
						}
						else{
							running[i].time_in_running++;
							//all_processes[i].time_to_wait--;
						}
						
					
					}
					
						
					// BLOCKED TO READY
					// Recognize a process becoming unblocked
					for (int i=0;i<blocked.size();i++ ){
						if( blocked[i].time_to_wait > 0){
								blocked[i].time_to_wait-=1;
								blocked[i].time_in_blocked++;
								
						}
						else {
								ready.push_back(blocked[i]);
								//ready_state.push_back(blocked[i]);
								blocked.erase(blocked.begin()+i);
								i--;
						}
					}
					
					

					// EMPTY TO NEW STATE TO READY
					// Recognize arrival of new process
					for (int i=0;i<all_processes.size();i++) {
						if(all_processes[i].arrival_time == j){
							new_state.push_back(all_processes[i]);							
							//all_processes[i].time_in_new++;
						}
					}
					
					// NEW TO READY
					// Allocate a PCB in the New state
					for (int i=0;i<new_state.size();i++) {
							if (ready.size()<sys.PCBs) {								
								ready.push_back(new_state[i]);
								new_state.erase(new_state.begin()+i);
								i--;
							}
							else{
								//all_processes[i].time_in_new++;
							}
					}
					
					// READY TO RUNNING
					for (int i=0;i<ready.size();i++) {
							if (running.size()<sys.CPUs) {	
								ready[i].time_to_wait = ready[i].burst_time;							
								running.push_back(ready[i]);
								ready.erase(ready.begin()+i);
								i--;
							}
							else{
								//all_processes[i].time_in_new++;
							}
					}
					
				
				
				
				//} // end of for loop
				
				cout << endl;
				cout << "tick: " << j <<endl;
				print_N(j,N,new_state,ready,running,blocked);
				
				if(ended_processes == all_processes.size()){
							//print states function
							print_states(new_state,ready,running,blocked);
							cout << "Program is ending because all processes have exited" <<endl;
							break;
				}
				
			
			} // end of FCFS
			
			
			else if(sys.algorithm == "Priority"){
					
					// RUNNING TO BLOCKED
					// Recognize a request from process in Running state
					for (int i=0;i<running.size();i++ ){
						running[i].time_to_wait--;
						if( running[i].time_to_wait == 0){
							running[i].time_cpu_used++;
							cout << " PROCESS: "<< running[i].PID << " times cpu used : "<< running[i].time_cpu_used << " compared to: " << running[i].cpu_bursts << endl;

							if(running[i].time_cpu_used == running[i].cpu_bursts ){								
								running[i].state = "exit";
								ended_processes++;
								
								running[i].turnaround_time = j - running[i].arrival_time;
								running[i].departure_time = j;
								running[i].normalized_turnaround = running[i].turnaround_time/(float)running[i].time_in_running;
								end_print(running[i],j);
								running.erase(running.begin()+i);
								i--;
							}
							else{
								running[i].time_to_wait = running[i].blocked_time;
								blocked.push_back(running[i]);
								running.erase(running.begin()+i);
								i--;
							}
						}
						else{
							running[i].time_in_running++;

						}
						
					
					}
					
						
					// BLOCKED TO READY
					// Recognize a process becoming unblocked
					for (int i=0;i<blocked.size();i++ ){
						if( blocked[i].time_to_wait > 0){
								blocked[i].time_to_wait-=1;
								blocked[i].time_in_blocked++;
								
						}
						else {
								ready.push_back(blocked[i]);
								std::sort(ready.begin(),ready.end(),compare_processes);
								blocked.erase(blocked.begin()+i);
								i--;
						}
					}
					
					

					// EMPTY TO NEW STATE TO READY
					// Recognize arrival of new process
					for (int i=0;i<all_processes.size();i++) {
						if(all_processes[i].arrival_time == j){
							new_state.push_back(all_processes[i]);							
							//all_processes[i].time_in_new++;
							
						}
					}
					
					// NEW TO READY
					// Allocate a PCB in the New state
					for (int i=0;i<new_state.size();i++) {
							if (ready.size()<sys.PCBs) {								
								ready.push_back(new_state[i]);
								std::sort(ready.begin(),ready.end(),compare_processes);
								new_state.erase(new_state.begin()+i);
								i--;
							}
					}
					
					// READY TO RUNNING
					for (int i=0;i<ready.size();i++) {
						std::sort(ready.begin(),ready.end(),compare_processes);
							if (running.size()<sys.CPUs) {	
								ready[i].time_to_wait = ready[i].burst_time;							
								running.push_back(ready[i]);
								ready.erase(ready.begin()+i);
								i--;
							}
							
					}
					
				
				
				
				//} // end of for loop
				
				cout << endl;
				cout << "tick: " << j <<endl;
				print_N(j,N,new_state,ready,running,blocked);
				
				if(ended_processes == all_processes.size()){
							//print states function
							print_states(new_state,ready,running,blocked);
							cout << "Program is ending because all processes have exited" <<endl;
							break;
				}
				
			
			}
		
		
			else if(sys.algorithm == "RR"){
				
				
					// RUNNING TO BLOCKED
					// Recognize a request from process in Running state
					for (int i=0;i<running.size();i++){
						running[i].time_to_wait--;
												
						if(running[i].burst_time > sys.time_quantum ){
							time_for_quantum++;
							if (time_for_quantum == sys.time_quantum){
								ready.push_back(running[i]);
								running.erase(running.begin()+i);
								i--;
								running[i].time_to_wait = running[i].burst_time - sys.time_quantum;
								time_for_quantum = 0;
							}
							else if(running[i].time_to_wait == 0){
								running[i].time_cpu_used++;
								//cout << " PROCESS: "<< running[i].PID << " times cpu used : "<< running[i].time_cpu_used << " compared to: " << running[i].cpu_bursts << endl;

								if(running[i].time_cpu_used == running[i].cpu_bursts ){
									cout << " PROCESS : "<< running[i].PID << "  EXITED " << endl;
									ended_processes++;
									running[i].turnaround_time = j - running[i].arrival_time;
									running[i].departure_time = j;
									running[i].normalized_turnaround = running[i].turnaround_time/(float)running[i].time_in_running;
									end_print(running[i],j);
									running.erase(running.begin()+i);
									i--;
								}
								else{
									running[i].time_to_wait = running[i].blocked_time;
									blocked.push_back(running[i]);
									running.erase(running.begin()+i);
									i--;
								}
							}
							else{
								//time_for_quantum++;
								running[i].time_in_running++;
							} 
						}
						
						
						
						
						
						
						
						else if(running[i].time_to_wait == 0){
							running[i].time_cpu_used++;
							//cout << " PROCESS: "<< running[i].PID << " times cpu used : "<< running[i].time_cpu_used << " compared to: " << running[i].cpu_bursts << endl;

							if(running[i].time_cpu_used == running[i].cpu_bursts ){
								cout << " PROCESS : "<< running[i].PID << "  EXITED " << endl;
								
								ended_processes++;
								
								running[i].turnaround_time = j - running[i].arrival_time;
								running[i].departure_time = j;
								running[i].normalized_turnaround = running[i].turnaround_time/(float)running[i].time_in_running;
								end_print(running[i],j);
								running.erase(running.begin()+i);
								i--;
							}
							else{
								running[i].time_to_wait = running[i].blocked_time;
								blocked.push_back(running[i]);
								running.erase(running.begin()+i);
								i--;
							}
						}
						else{
							running[i].time_in_running++;
						}
					}
					
					
					
						
					// BLOCKED TO READY
					// Recognize a process becoming unblocked
					for (int i=0;i<blocked.size();i++ ){
						if( blocked[i].time_to_wait > 0){
								blocked[i].time_to_wait-=1;
								blocked[i].time_in_blocked++;		
						}
						else {
								ready.push_back(blocked[i]);
								blocked.erase(blocked.begin()+i);
								i--;
						}
					}
					
					

					// EMPTY TO NEW STATE 
					// Recognize arrival of new process
					for (int i=0;i<all_processes.size();i++) {
						if(all_processes[i].arrival_time == j){
							new_state.push_back(all_processes[i]);							
						}
					}
					
					// NEW TO READY
					// Allocate a PCB in the New state
					for (int i=0;i<new_state.size();i++) {
							if (ready.size()<sys.PCBs) {								
								ready.push_back(new_state[i]);
								new_state.erase(new_state.begin()+i);
								i--;
							}
					}
					
					// READY TO RUNNING
					for (int i=0;i<ready.size();i++) {
						
							if(ready[i].time_to_wait > 0 && ready[i].burst_time > sys.time_quantum){
								if (running.size()<sys.CPUs){
									running.push_back(ready[i]);
									ready.erase(ready.begin()+i);
									i--;
								}
								
							}
							else{
								if (running.size()<sys.CPUs) {	
									ready[i].time_to_wait = ready[i].burst_time;							
									running.push_back(ready[i]);
									ready.erase(ready.begin()+i);
									i--;
									
								}
							}
					}
					

				cout << endl;
				cout << "tick: " << j <<endl;
				print_N(j,N,new_state,ready,running,blocked);
				
				if(ended_processes == all_processes.size()){
							//print states function
							print_states(new_state,ready,running,blocked);
							cout << "Program is ending because all processes have exited" <<endl;
							break;
				}
			}
		
	}// main for loop ends here


	
}// main ends here
	
