#
#	Keaton Coffman
#	Project10 Makefile
#	
proj10: proj10.student.o 
		g++ -std=c++11 proj10.student.o -o proj10

proj10.student.o: proj10.student.c
		g++ -std=c++11 -c proj10.student.c
