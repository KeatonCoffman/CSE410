#
#	Keaton Coffman
#	Project07 Makefile
#	
proj07: proj07.student.o 
		g++ -std=c++11 proj07.student.o -o proj07

proj07.student.o: proj07.student.c
		g++ -std=c++11 -c proj07.student.c
