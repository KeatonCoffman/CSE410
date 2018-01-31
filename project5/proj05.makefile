#
#	Keaton Coffman
#	Project05 Makefile
#	
proj05: proj05.student.o 
		g++ -std=c++11 proj05.student.o -o proj05

proj05.student.o: proj05.student.c
		g++ -std=c++11 -c proj05.student.c
