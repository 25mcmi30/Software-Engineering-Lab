CC = gcc
CFG = -Wall -Wextra -std=c11

all: student_database

student_database: student_database.o mylib.o
	$(CC) $(CFG) -o student_database student_database.o mylib.o

%.o: %.c mylib.h
	$(CC) $(CFG) -c $<

clean:
	rm -f *.o student_database output.txt