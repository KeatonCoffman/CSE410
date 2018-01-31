#
#	Keaton Coffman
#	Project03 Makefile
#	
proj03: proj03.student.o 
		g++ -std=c++11 proj03.student.o -o proj03

proj03.student.o: proj03.student.c
		g++ -std=c++11 -c proj03.student.c
