//
//	Keaton Coffman
//	Project 6
//	CSE 410
//
//

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <ostream>



using namespace std;


// Struct holding the page table information
struct Table{
	int index = 0;
	int V_bit = 0;
	int R_bit = 0;
	int M_bit = 0;
	int frame_num = 0;
};

// Print out the header
void header(){
	cout << "#  "<<"V  "<<"R  "<<"M  "<<"FRAME"<<endl;
}




int main(int argc, char* argv[]){
	
	int mem_count = 0;
	int read_count = 0;
	int write_count = 0;

	int number_arg = stoi(argv[1]);
	
	// Initialize the table with empty Table objects
	vector<Table> PageTable;
	for(int i=0;i<=15;i++){
		PageTable.push_back(Table());
	}

	// Print PageTable at beginning of program
	if(number_arg > 0){
		header();
		for(int i=0;i<=15;i++){
			cout << PageTable[i].index << "  ";
			cout << PageTable[i].V_bit << "  ";
			cout << PageTable[i].R_bit << "  ";
			cout << PageTable[i].M_bit << "  ";
			cout << PageTable[i].frame_num << endl;
		}
		cout << endl;
	}
	
	
	string line;
	ifstream infile;
	infile.open(argv[2],ios::out);
	
	// If the file did not open properly print an error messag
	// Else read each line of the file and increment counts initialized
	// 		Print page table if N or number arg is greater than 0
	if(!infile){
		cout << "Error opening file" <<endl;
	}
	else{
		while(std::getline(infile,line)){
			
			cout << line <<endl;
			
			if(line.size() > 0){
				mem_count += 1;
			}
		
			if(line[0] == 'R'){
				read_count++;
			}
			
			if(line[0] == 'W'){
				write_count++;
			}
			
			int number_arg = stoi(argv[1]);
			
			if((number_arg != 0) && (mem_count % number_arg == 0)){
					header();
					for(int i=0;i<=15;i++){
						cout << PageTable[i].index << "  ";
						cout << PageTable[i].V_bit << "  ";
						cout << PageTable[i].R_bit << "  ";
						cout << PageTable[i].M_bit << "  ";
						cout << PageTable[i].frame_num << endl;
					}
					cout << endl;
			}
			
		
		}
		infile.close();
	}
	
	
	// Only print page table if number_arg is greater than one
	if(number_arg > 0){
		header();
		for(int i=0;i<=15;i++){
			cout << PageTable[i].index << "  ";
			cout << PageTable[i].V_bit << "  ";
			cout << PageTable[i].R_bit << "  ";
			cout << PageTable[i].M_bit << "  ";
			cout << PageTable[i].frame_num << endl;
		}
		cout << endl;
	}
	
	// Final print of information 
	cout << "Memory references: "<< mem_count << endl;
	cout << "Read count: "<< read_count << endl;
	cout << "Write count: "<< write_count << endl;
}
