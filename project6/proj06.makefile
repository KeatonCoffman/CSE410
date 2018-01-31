#
#	Keaton Coffman
#	Project06 Makefile
#	
proj06: proj06.student.o 
		g++ -std=c++11 proj06.student.o -o proj06

proj06.student.o: proj06.student.c
		g++ -std=c++11 -c proj06.student.c
