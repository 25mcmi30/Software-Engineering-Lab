#include <stdio.h>

const char* grade_calculate(int n){
    if (n < 50){
        return "F";
    }
    else if (n >= 90){
        return "O";
    } else if (n >= 85){
        return "A+";
    } else if (n >= 75){
        return "A";
    } else if (n >= 65){
        return "B+";
    } else if (n >= 60){
        return "B";
    } else if (n >= 55){
        return "C";
    } else if (n >= 50){
        return "D";
    } else {
        return "F";
    }
}

typedef struct student{
    char name[50];
    char rollno[10];
    int marks[5][2];
    char grades[5][3];
} student;

void main(void){

    FILE* fp = fopen("input.txt", "r");
    char buffer[100];
    student s[10];
    int scount = 0;

    while(fgets(buffer, sizeof(buffer), fp)){
        student k;
        sscanf(buffer, "%s %s %d %d %d %d %d %d %d %d %d %d", k.rollno, k.name, &k.marks[0][0], &k.marks[0][1], &k.marks[1][0], &k.marks[1][1], &k.marks[2][0], &k.marks[2][1], &k.marks[3][0], &k.marks[3][1], &k.marks[4][0], &k.marks[4][1]);
        s[scount] = k;
        scount++;
    }

    fclose(fp);
    printf("Name   Roll no  sub_1 sub_2 sub_3 sub_4 sub_5\n");

    for (int i = 0; i  < scount; i++){
        printf("%s %s ", s[i].name, s[i].rollno);
        for (int j = 0; j < 5; j++){
            const char* g = grade_calculate(s[i].marks[j][0]+s[i].marks[j][1]);
            snprintf(s[i].grades[j], 3, "%s", g);
            printf("%s ", s[i].grades[j]);
        }
        printf("\n");
    }
}