CC = gcc
CFG = -Wall -Wextra -std=c11

all: student_database

student_database: student_database.o
	$(CC) $(CFG) -o student_database student_database.o

student_database.o: student_database.c
	$(CC) $(CFG) -c student_database.c

clean:
	rm -f *.o student_database