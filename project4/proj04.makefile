#
#	Keaton Coffman
#	Project04 Makefile
#	
proj04: proj04.student.o 
		g++ -std=c++11 proj04.student.o -o proj04

proj04.student.o: proj04.student.c
		g++ -std=c++11 -c proj04.student.c
