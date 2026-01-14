#ifndef MYLIB_H
#define MYLIB_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LEN 50
#define MAX_ID_LEN 20
#define MIN_PASSING_MARKS 50
#define SUBJECTS 5
#define MARKS_PER_SUBJECT 2

typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    float minor_marks[SUBJECTS];
    float major_marks[SUBJECTS];
    float total_marks[SUBJECTS];
    float total;
    float percentage;
    char grade;
    float cgpa;
} Student;

typedef struct {
    Student students[MAX_STUDENTS];
    int count;
} StudentDatabase;

int isValidID(const char *id, StudentDatabase *db);
int isValidName(const char *name);
int isValidMarks(float mark);
int isValidSubjectPass(const Student *s);

void calculateTotalAndPercentage(Student *s);
char assignGrade(float percentage);
void calculateCGPA(Student *s);
float calculateClassAverage(StudentDatabase *db);
void findHighestLowest(StudentDatabase *db, float *highest, float *lowest);
void countGrades(StudentDatabase *db, int *grade_count);

int readStudentsFromFile(const char *filename, StudentDatabase *db);
void writeReportToFile(const char *filename, StudentDatabase *db);

int isAlphanumeric(const char *str);
int isOnlyAlpha(const char *str);

#endif