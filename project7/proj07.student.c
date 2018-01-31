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
#include <deque>
#include <stdio.h>
#include <stdlib.h>
#include <sstream> 
#include <utility>
#include <algorithm>

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
	cout << "#    "<<"V  "<<"R  "<<"M  "<<"FRAME"<<endl;
}



int main(int argc, char* argv[]){
	
	int mem_count = 0;
	int read_count = 0;
	int write_count = 0;
	int fault_count = 0;
	int frames_used = 0;
	int fault_flag = 0;
	int reverse_flag = 0;

	int number_arg = stoi(argv[1]);
	
	string algorithm;
	string frames_allocated;
	string index_string = "";
	string pg_number;
	
	deque<int> index_of_allocated;
	vector<int> frame_list;
	deque<pair<int,int>> clock_allocated;
	
	
	// Initialize the table with empty Table objects
	vector<Table> PageTable;
	for(int i=0;i<=15;i++){
		Table New_Table = Table();
		New_Table.index = i;
		PageTable.push_back(New_Table);
	}

	// Print PageTable at beginning of program
	if(number_arg > 0){
		header();
		for(int i=0;i<=15;i++){
			printf("%02d",PageTable[i].index );
			cout << "   ";
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
		
		// Get first two lines
		getline(infile,algorithm);
		getline(infile,frames_allocated);
		int number_of_frames ;
		number_of_frames = stoi(frames_allocated);
		
		int LRU[number_of_frames];
		
		// make a vector of useable frames
		for(int i=40;i<(number_of_frames + 40); i++){
			frame_list.push_back(i);
		}
		
		
		while(std::getline(infile,line)){
			index_string = line[2];
			int index;
			stringstream convert;
			convert << hex << index_string;
			convert >> index;
			
			pg_number = line.substr(3);
			
			//cout << line << " " << index << " " << pg_number <<endl;
			
			if(line.size() > 0){
				mem_count += 1;
			}
		
			if(line[0] == 'R'){
				read_count++;
				if (PageTable[index].V_bit == 1){
					PageTable[index].R_bit = 1;
					// container for LRU?
					int MRU = index;
					for(int i=0; i<sizeof(LRU)-2; i++){
						LRU[i] = LRU[i+1];
					}
					LRU[sizeof(LRU)-1] = MRU;
				}
				if(algorithm == "FIFO"){
					Table current_entry;
					current_entry = PageTable[index];
					int vbits = current_entry.V_bit;
					if (vbits == 0 && index_of_allocated.size() == 0){
						fault_count++;
						index_of_allocated.push_front(index);
						PageTable[index].V_bit = 1;
						PageTable[index].M_bit = 0;
						PageTable[index].R_bit = 1;
						PageTable[index].frame_num = frame_list[frames_used];
						frames_used++;
						if (frames_used > frame_list.size()-1){
							frames_used = frames_used - frames_used;
						}
						while(index_of_allocated.size() > number_of_frames){
							index_of_allocated.pop_back();
						}
						
					}
					else if (vbits == 1){
						PageTable[index].M_bit = 0;
						PageTable[index].R_bit = 1;
					}
					else if (vbits == 0 && index_of_allocated.size() > 0){
						fault_count++;
						int first_out = index_of_allocated.back();
					
						index_of_allocated.push_front(index);
						
						if (index_of_allocated.size() > number_of_frames){
							PageTable[index].frame_num = PageTable[first_out].frame_num;
							PageTable[first_out].frame_num = 0;
							PageTable[first_out].V_bit = 0;
							PageTable[first_out].R_bit = 0;
							PageTable[first_out].M_bit = 0;
						}
						
						if (frames_used > number_of_frames-1){
							frames_used = frames_used - frames_used;
							index_of_allocated.pop_back();
						}

						
						cout << " frame number being replaced: " << PageTable[index].frame_num << " by: " << frame_list[frames_used] << endl;
						
						PageTable[index].frame_num = frame_list[frames_used];
						frames_used++;
						
						PageTable[index].V_bit = 1;
						PageTable[index].R_bit = 1;
						PageTable[index].M_bit = 0;

						
						while(index_of_allocated.size() > number_of_frames){
							index_of_allocated.pop_back();
						}
						
						cout << " frames in use list: "<<endl;
						for(auto item:index_of_allocated){
							cout << item <<endl;
						}
					}
					
				}
			
				if(algorithm == "LRU"){
					Table current_entry;
					current_entry = PageTable[index];
					int vbits = current_entry.V_bit;
					if (vbits == 0 && index_of_allocated.size() == 0){
						fault_count++;
						index_of_allocated.push_front(index);
						PageTable[index].V_bit = 1;
						PageTable[index].M_bit = 1;
						PageTable[index].R_bit = 1;
						PageTable[index].frame_num = frame_list[frames_used];
						frames_used++;
						if (frames_used > frame_list.size()-1){
							frames_used = frames_used - frames_used;
						}
						while(index_of_allocated.size() > number_of_frames){
							index_of_allocated.pop_back();
						}
						
					}
					else if (vbits == 1){
						
						if(index_of_allocated.size() == number_of_frames){
							for(int i=0; i > index_of_allocated.size(); i++){
								index_of_allocated[i] = index_of_allocated[i+1];
							}
							LRU[-1] = index;
						}
						
						PageTable[index].M_bit = 1;
						PageTable[index].R_bit = 1;
					}
					else if (vbits == 0 && index_of_allocated.size() > 0){
						
						//index_of_allocated.push_front(index);
						
						if(index_of_allocated.size() < number_of_frames){
							cout << "INDEX THATS MESSING UP: " << index <<endl;
							cout << "size of index of allocated before push front  " << index_of_allocated.size() <<endl;
							if(index_of_allocated.size() == number_of_frames-1){
								for(int i=0; i< index_of_allocated.size()-1; i++){
									cout << " spot: " << index_of_allocated[i+1] << "  changed to: " << index_of_allocated[i] <<endl;
									index_of_allocated[i+1] = index_of_allocated[i];
								}
								cout << " size of index of allocated is : " << index_of_allocated.size() <<endl;
								index_of_allocated[0] = index;
								cout << " size of index of allocated is : " << index_of_allocated.size() <<endl;
								//index_of_allocated.insert(index,0);
								//index_of_allocated.insert(index);
							}
							
							if(index_of_allocated.size() == 1){
								index_of_allocated.push_front(index);
							}
							
							
							
							
							// An insertion at either end of the deque invalidates all the iterators to the deque, but has no effect on the validity of references to elements of the deque.
							PageTable[index].V_bit = 1;
							PageTable[index].M_bit = 1;
							PageTable[index].R_bit = 1;
							PageTable[index].frame_num = frame_list[frames_used];
							frames_used++;
						}
						
						if(index_of_allocated.size() == number_of_frames){
							int index_to_deallocate = LRU[0];
							
							index_of_allocated.push_front(index);
							
							// copy everything into LRU
							for(int i=0; i< index_of_allocated.size()-1; i++){
								LRU[i] = index_of_allocated[i];
							}
							
							// Deallocate index_of_allocated[index_to_deallocate] and allocate index_of_allocated[index]
							PageTable[index].frame_num = PageTable[index_to_deallocate].frame_num;
							PageTable[index_to_deallocate].frame_num = 0;
							PageTable[index_to_deallocate].V_bit = 0;
							PageTable[index_to_deallocate].R_bit = 0;
							PageTable[index_to_deallocate].M_bit = 0;
							
							PageTable[index].V_bit = 1;
							PageTable[index].M_bit = 1;
							PageTable[index].R_bit = 1;
							
							
							// making the most recently used the last thing in LRU 
							int index_of_MRU ;
							for (int i = 0; i < sizeof(LRU)-1; i++){
								if(LRU[i]== index){
									index_of_MRU = i;
								} 
							}
							int MRU = LRU[index_of_MRU];
							
							// First thing in LRU is lost so it needs to be deallocated
							int index_out = LRU[0];
							PageTable[index_out].frame_num = 0;
							PageTable[index_out].V_bit = 0;
							PageTable[index_out].R_bit = 0;
							PageTable[index_out].M_bit = 0;
							
							for(int i=0; i<sizeof(LRU)-2; i++){
								LRU[i] = LRU[i+1];
							}
							
							LRU[-1] = MRU;
							
						}
						
						if (frames_used > frame_list.size()-1){
								frames_used = frames_used - frames_used;
							}
						while(index_of_allocated.size() > number_of_frames){
							index_of_allocated.pop_back();
						}	
					}
					cout << " frames in use list: "<<endl;
						for(auto item:index_of_allocated){
							cout << item <<endl;
						}
					
				}
				
				if(algorithm == "Clock"){
					Table current_entry;
					current_entry = PageTable[index];
					int vbits = current_entry.V_bit;
					if (vbits == 0 && clock_allocated.size() == 0){
						fault_count++;
						pair<int,int> new_pair = make_pair(index,1);
						clock_allocated.push_front(new_pair);
						PageTable[index].V_bit = 1;
						PageTable[index].M_bit = 0;
						PageTable[index].R_bit = 1;
						PageTable[index].frame_num = frame_list[frames_used];
						frames_used++;
						if (frames_used > frame_list.size()-1){
							frames_used = frames_used - frames_used;
						}
					}
					else if (vbits == 1){
						PageTable[index].M_bit = 0;
						PageTable[index].R_bit = 1;
					}
					else if (vbits == 0 && clock_allocated.size() > 0){
						fault_count++;
						pair<int,int> first_out_pair = clock_allocated.back();
						int first_out = first_out_pair.first;
						pair<int,int> new_pair = make_pair(index,1);
						
						if(clock_allocated.size() < number_of_frames){
							clock_allocated.push_front(new_pair);
							PageTable[index].V_bit = 1;
							PageTable[index].M_bit = 0;
							PageTable[index].R_bit = 1;
							PageTable[index].frame_num = frame_list[frames_used];
							frames_used++;
						}
						
						if(clock_allocated.size() == number_of_frames){ 
							if (reverse_flag == 0){
								reverse(clock_allocated.begin(), clock_allocated.end());
								reverse_flag = 1;
							}
							for(int i=0;i<clock_allocated.size();i++){
								int old_index = clock_allocated[i].first;
								int clock_tick = clock_allocated[i].second;
								if(clock_tick == 1){
									// CANNOT CHANGE INDEX
									clock_allocated[i].second = 0;	
								}
								if(clock_tick == 0){
									// Change index
									//cout << "clock tick == 0 -------------------------------- on this index: "<< old_index <<endl;
									int old_frame = PageTable[old_index].frame_num;
									PageTable[index].frame_num = old_frame;
									
									
									//cout << " the old frame is: " << old_frame << endl;
									//cout << " clock tick is zero " << " frame number of " << index << " changed to " << PageTable[old_index].frame_num << endl;
									
									PageTable[old_index].frame_num = 0;
									PageTable[old_index].V_bit = 0;
									PageTable[old_index].R_bit = 0;
									PageTable[old_index].M_bit = 0;
									PageTable[index].V_bit = 1;
									PageTable[index].R_bit = 1;
									PageTable[index].M_bit = 0;
									
									clock_allocated[i] = new_pair;
									goto Done1;
								}							
							}
						}
						
						Done1:
						
						while(clock_allocated.size() > number_of_frames){
							clock_allocated.pop_back();
						}
						
						cout << " frames in use list: "<<endl;
						for(auto item:clock_allocated){
							cout << item.first <<endl;
						}
					}
				}
			}
			
			if(line[0] == 'W'){
				write_count++;
				if(algorithm == "FIFO"){
					Table current_entry;
					current_entry = PageTable[index];
					int vbits = current_entry.V_bit;
					if (vbits == 0 && index_of_allocated.size() == 0){
						fault_count++;
						index_of_allocated.push_front(index);
						PageTable[index].V_bit = 1;
						PageTable[index].M_bit = 1;
						PageTable[index].R_bit = 1;
						PageTable[index].frame_num = frame_list[frames_used];
						frames_used++;
						if (frames_used > frame_list.size()-1){
							frames_used = frames_used - frames_used;
						}
						while(index_of_allocated.size() > number_of_frames){
							index_of_allocated.pop_back();
						}
						
					}
					else if (vbits == 1){
						PageTable[index].M_bit = 1;
						PageTable[index].R_bit = 1;
					}
					else if (vbits == 0 && index_of_allocated.size() > 0){
						fault_count++;
						int first_out = index_of_allocated.back();
					
						index_of_allocated.push_front(index);
						
						if (index_of_allocated.size() > number_of_frames){
							PageTable[index].frame_num = PageTable[first_out].frame_num;
							PageTable[first_out].frame_num = 0;
							PageTable[first_out].V_bit = 0;
							PageTable[first_out].R_bit = 0;
							PageTable[first_out].M_bit = 0;
						}
						
						if (frames_used > number_of_frames-1){
							frames_used = frames_used - frames_used;
							index_of_allocated.pop_back();
						}

						
						cout << " frame number being replaced: " << PageTable[index].frame_num << " by: " << frame_list[frames_used] << endl;
						
						PageTable[index].frame_num = frame_list[frames_used];
						frames_used++;
						
						PageTable[index].V_bit = 1;
						PageTable[index].R_bit = 1;
						PageTable[index].M_bit = 1;

						
						while(index_of_allocated.size() > number_of_frames){
							index_of_allocated.pop_back();
						}
						
						cout << " frames in use list: "<<endl;
						for(auto item:index_of_allocated){
							cout << item <<endl;
						}
					}
					
				}
			
				if(algorithm == "LRU"){
					Table current_entry;
					current_entry = PageTable[index];
					int vbits = current_entry.V_bit;
					if (vbits == 0 && index_of_allocated.size() == 0){
						fault_count++;
						index_of_allocated.push_front(index);
						PageTable[index].V_bit = 1;
						PageTable[index].M_bit = 1;
						PageTable[index].R_bit = 1;
						PageTable[index].frame_num = frame_list[frames_used];
						frames_used++;
						if (frames_used > frame_list.size()-1){
							frames_used = frames_used - frames_used;
						}
						while(index_of_allocated.size() > number_of_frames){
							index_of_allocated.pop_back();
						}
						
					}
					else if (vbits == 1){
						
						if(index_of_allocated.size() == number_of_frames){
							for(int i=0; i > index_of_allocated.size(); i++){
								index_of_allocated[i] = index_of_allocated[i+1];
							}
							LRU[-1] = index;
						}
						
						PageTable[index].M_bit = 1;
						PageTable[index].R_bit = 1;
					}
					else if (vbits == 0 && index_of_allocated.size() > 0){
						
						//index_of_allocated.push_front(index);
						
						if(index_of_allocated.size() < number_of_frames){
							cout << "INDEX THATS MESSING UP: " << index <<endl;
							cout << "size of index of allocated before push front  " << index_of_allocated.size() <<endl;
							if(index_of_allocated.size() == number_of_frames-1){
								for(int i=0; i< index_of_allocated.size()-1; i++){
									cout << " spot: " << index_of_allocated[i+1] << "  changed to: " << index_of_allocated[i] <<endl;
									index_of_allocated[i+1] = index_of_allocated[i];
								}
								cout << " size of index of allocated is : " << index_of_allocated.size() <<endl;
								index_of_allocated[0] = index;
								cout << " size of index of allocated is : " << index_of_allocated.size() <<endl;
								//index_of_allocated.insert(index,0);
								//index_of_allocated.insert(index);
							}
							
							if(index_of_allocated.size() == 1){
								index_of_allocated.push_front(index);
							}
							
							
							
							
							// An insertion at either end of the deque invalidates all the iterators to the deque, but has no effect on the validity of references to elements of the deque.
							PageTable[index].V_bit = 1;
							PageTable[index].M_bit = 1;
							PageTable[index].R_bit = 1;
							PageTable[index].frame_num = frame_list[frames_used];
							frames_used++;
						}
						
						if(index_of_allocated.size() == number_of_frames){
							int index_to_deallocate = LRU[0];
							
							index_of_allocated.push_front(index);
							
							// copy everything into LRU
							for(int i=0; i< index_of_allocated.size()-1; i++){
								LRU[i] = index_of_allocated[i];
							}
							
							// Deallocate index_of_allocated[index_to_deallocate] and allocate index_of_allocated[index]
							PageTable[index].frame_num = PageTable[index_to_deallocate].frame_num;
							PageTable[index_to_deallocate].frame_num = 0;
							PageTable[index_to_deallocate].V_bit = 0;
							PageTable[index_to_deallocate].R_bit = 0;
							PageTable[index_to_deallocate].M_bit = 0;
							
							PageTable[index].V_bit = 1;
							PageTable[index].M_bit = 1;
							PageTable[index].R_bit = 1;
							
							
							// making the most recently used the last thing in LRU 
							int index_of_MRU ;
							for (int i = 0; i < sizeof(LRU)-1; i++){
								if(LRU[i]== index){
									index_of_MRU = i;
								} 
							}
							int MRU = LRU[index_of_MRU];
							
							// First thing in LRU is lost so it needs to be deallocated
							int index_out = LRU[0];
							PageTable[index_out].frame_num = 0;
							PageTable[index_out].V_bit = 0;
							PageTable[index_out].R_bit = 0;
							PageTable[index_out].M_bit = 0;
							
							for(int i=0; i<sizeof(LRU)-2; i++){
								LRU[i] = LRU[i+1];
							}
							
							LRU[-1] = MRU;
							
						}
						
						if (frames_used > frame_list.size()-1){
								frames_used = frames_used - frames_used;
							}
						while(index_of_allocated.size() > number_of_frames){
							index_of_allocated.pop_back();
						}	
					}
					cout << " frames in use list: "<<endl;
						for(auto item:index_of_allocated){
							cout << item <<endl;
						}
					
				}
				
				if(algorithm == "Clock"){
					Table current_entry;
					current_entry = PageTable[index];
					int vbits = current_entry.V_bit;
					if (vbits == 0 && clock_allocated.size() == 0){
						fault_count++;
						pair<int,int> new_pair = make_pair(index,1);
						clock_allocated.push_front(new_pair);
						PageTable[index].V_bit = 1;
						PageTable[index].M_bit = 1;
						PageTable[index].R_bit = 1;
						PageTable[index].frame_num = frame_list[frames_used];
						frames_used++;
						if (frames_used > frame_list.size()-1){
							frames_used = frames_used - frames_used;
						}
					}
					else if (vbits == 1){
						PageTable[index].M_bit = 1;
						PageTable[index].R_bit = 1;
					}
					else if (vbits == 0 && clock_allocated.size() > 0){
						fault_count++;
						pair<int,int> first_out_pair = clock_allocated.back();
						int first_out = first_out_pair.first;
						pair<int,int> new_pair = make_pair(index,1);
						
						if(clock_allocated.size() < number_of_frames){
							clock_allocated.push_front(new_pair);
							PageTable[index].V_bit = 1;
							PageTable[index].M_bit = 1;
							PageTable[index].R_bit = 1;
							PageTable[index].frame_num = frame_list[frames_used];
							frames_used++;
						}
						
						if(clock_allocated.size() == number_of_frames){ 
							if (reverse_flag == 0){
								reverse(clock_allocated.begin(), clock_allocated.end());
								reverse_flag = 1;
							}
							for(int i=0;i<clock_allocated.size();i++){
								int old_index = clock_allocated[i].first;
								int clock_tick = clock_allocated[i].second;
								if(clock_tick == 1){
									// CANNOT CHANGE INDEX
									clock_allocated[i].second = 0;	
								}
								if(clock_tick == 0){
									// Change index
									//cout << "clock tick == 0 -------------------------------- on this index: "<< old_index <<endl;
									int old_frame = PageTable[old_index].frame_num;
									PageTable[index].frame_num = old_frame;
									
									
									//cout << " the old frame is: " << old_frame << endl;
									//cout << " clock tick is zero " << " frame number of " << index << " changed to " << PageTable[old_index].frame_num << endl;
									
									PageTable[old_index].frame_num = 0;
									PageTable[old_index].V_bit = 0;
									PageTable[old_index].R_bit = 0;
									PageTable[old_index].M_bit = 0;
									PageTable[index].V_bit = 1;
									PageTable[index].R_bit = 1;
									PageTable[index].M_bit = 1;
									
									clock_allocated[i] = new_pair;
									goto Done;
								}							
							}
						}
						
						Done:
						while(clock_allocated.size() > number_of_frames){
							clock_allocated.pop_back();
						}
						
						cout << " frames in use list: "<<endl;
						for(auto item:clock_allocated){
							cout << item.first <<endl;
						}
					}
				}
			}
			
			int number_arg = stoi(argv[1]);
			
			if((number_arg != 0) && (mem_count % number_arg == 0)){
					header();
					for(int i=0;i<=15;i++){
						printf("%02d",PageTable[i].index );
						cout << "   ";
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
			printf("%02d",PageTable[i].index );
			cout << "   ";
			cout << PageTable[i].V_bit << "  ";
			cout << PageTable[i].R_bit << "  ";
			cout << PageTable[i].M_bit << "  ";
			cout << PageTable[i].frame_num << endl;
		}
		cout << endl;
	}
	
	// Final print of information 
	cout << algorithm << "  " << frame_list.size() << endl;
	cout << "Memory references: "<< mem_count << endl;
	cout << "Read count: "<< read_count << endl;
	cout << "Write count: "<< write_count << endl;
	
}
