//	Keaton Coffman
//	Cse410
//	Project02
//
//

#include <iostream>
using std::cin; using std::cout; using std::endl;
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <iomanip>
#include <sstream>


int main(int argc, char* argv[]){
	
	char cmdbuffer[512];
	int title_flag = 0;
	int lines_flag = 0;
	char number_buff[5];
	
	// Loops through each argument
	for(int i=1; i<argc; i++){

	        // Sets a flag for titles
		if(strcmp(argv[i],"-t")==0){
			title_flag = 1;
		}

		// Sets a flag for numbered lines
		else if(strcmp(argv[i],"-n")==0){
			lines_flag = 1;
		}
		
		// Writes a help message to the command line
		else if(strcmp(argv[i],"-h")==0){
			write(2,"COMMAND HELP\n -t prints a title \n -n prints numbered lines \n -h prints help menu \n",83);
		}

		// Reading and writing of the file to output
		else{
			int open_flag = open(argv[i],O_RDONLY);
			int file_n_length = strlen(argv[i]);
			std::string colons;

			// Appends colons to string according to name length
			for(int j=0;j<file_n_length;j++){
				colons+=":";
			}

			// If title flag is set print a title using colons and length of name
			if(title_flag == 1){
				write(1,"\n",2);
				write(1,colons.data(),file_n_length);
				write(1,"\n",2);
				write(1,argv[i],file_n_length);
				write(1,"\n",2);
				write(1,colons.data(),file_n_length);
				write(1,"\n",2);
			}	
			
		     
			// Write error to output if file not found
			if(open_flag == -1){
				write(2,"\nError file not found!\n",23);
			}

			int num_of_bytes;
			int prev_line = 0;
			int counter=1;
			
			
			// Read the file and write to the output if -n was not found
			if(lines_flag == 0){
				while((num_of_bytes=read(open_flag,cmdbuffer,512)) > 0){			  
					write(1,cmdbuffer,num_of_bytes);
					num_of_bytes = read(open_flag,cmdbuffer,512);
				}
			}
			  
			// Print numbered lines with output  
			if(lines_flag == 1){
			        int curr_char = 0;
				while (read(open_flag,cmdbuffer+curr_char,1) > 0) {
				        if(curr_char == 511){
					  write(1,cmdbuffer,curr_char+1);
					  curr_char = 0;
				        }
					if(cmdbuffer[curr_char] == '\n'){	
					  counter++;
					  sprintf(number_buff,"%04d ",counter-1);
					  write(1,number_buff,5);
					  write(1,cmdbuffer,curr_char+1);
					  curr_char = -1;
				        }
				curr_char++;
				}
			 }
			
			close(open_flag);
		}
			  
	    }
	  return 0;
	}

