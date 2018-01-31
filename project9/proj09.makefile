#
#	Keaton Coffman
#	Project09 Makefile
#	
proj09: proj09.student.o 
		g++ -std=c++11 proj09.student.o -o proj09

proj09.student.o: proj09.student.c
		g++ -std=c++11 -c proj09.student.c
