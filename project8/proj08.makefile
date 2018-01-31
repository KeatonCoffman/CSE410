#
#	Keaton Coffman
#	Project08 Makefile
#	
proj08: proj08.student.o 
		g++ -std=c++11 proj08.student.o -o proj08

proj08.student.o: proj08.student.c
		g++ -std=c++11 -c proj08.student.c
