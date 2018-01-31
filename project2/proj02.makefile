#
#	Keaton Coffman
#	Project02 Makefile
#	
proj02: proj02.student.o 
		g++ -std=c++11 proj02.student.o -o proj02

proj02.student.o: proj02.student.c
		g++ -std=c++11 -c proj02.student.c
