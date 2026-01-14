#include "mylib.h"

int isAlphaNumeric(const char *str){
    if (!str || strlen(str) == 0) return 0;
    for (int i = 0; str[i]; i++){
        if (!isalnum(str[i])) return 0;
    }
    return 1;
}

int isOnlyAlpha(const char *str){
    if (!str || strlen(str) == 0) return 0;
    for (int i = 0; str[i]; i++){
        if (!isalpha(str[i]) && str[i] != ' ') return 0;
    }
    return 1;
}

int isValidID(const char *id, StudentDatabase *db){
    if (!isAlphaNumeric(id)) return 0;

    for (int i = 0; i< db->count; i++){
        if (strcmp(db->students[i].id, id) == 0) return 0;
    }

    return 1;
}


int isValidName(const char *name){
    return isOnlyAlpha(name);
}

int isValidMarks(float mark){
    return (mark >=0 && mark <= 100);
}
int isValidSubjectPass(const Student *s){
    for(int i = 0; i < SUBJECTS; i++){
        float subject_total = s->minor_marks[i] + s->major_marks[i];
        float subject_percentage = (subject_total / 100) * 100;
        if (subject_percentage < MIN_PASSING_MARKS){
            return 0;
        }
    }

    return 1;
}

void calculateTotalAndPercentage(Student *s){
    s->total = 0;
    for (int i = 0; i < SUBJECTS; i++){
        s->total_marks[i] = s->minor_marks[i] + s->major_marks[i];
        s->total += s->total_marks[i];
    }
    s->percentage = s->total /5.0;
}


char assignGrade(float percentage){
    if (percentage >= 90) return 'O';
    if (percentage >= 85) return '+';
    if (percentage >= 75) return 'A';
    if (percentage >= 65) return 'B';
    if (percentage >= 60) return 'C';
    if (percentage >= 55) return 'D';
    if (percentage >= 50) return 'E';
    return 'F';
}


void calculateCGPA(Student *s){
    if (s->percentage >= 90) s->cgpa = 10.0;
    else if(s->percentage >= 85) s->cgpa = 9.0;
    else if(s->percentage >= 75) s->cgpa = 8.0;
    else if(s->percentage >= 65) s->cgpa = 7.0;
    else if(s->percentage >= 60) s->cgpa = 6.0;
    else if(s->percentage >= 55) s->cgpa = 5.0;
    else if(s->percentage >= 50) s->cgpa = 4.0;
    else s->cgpa = 0.0;
}


float calculateClassAverage(StudentDatabase *db){
    if (db->count == 0) return 0;
    float sum = 0;
    for (int i = 0; i < db->count; i++){
        sum += db->students[i].percentage;
    }
    return sum / db->count;
}
void findHighestLowest(StudentDatabase *db, float *highest, float *lowest){
    if (db->count == 0){
        *highest = 0;
        *lowest = 0;
        return;
    }

    *highest = db->students[0].percentage;
    *lowest = db->students[0].percentage;

    for (int i = 1; i < db->count; i++){
        if (db->students[i].percentage > *highest){
            *highest = db->students[i].percentage;
        }

        if (db->students[i].percentage < *lowest){
            *lowest = db->students[i].percentage;
        }
    }
}
void countGrades(StudentDatabase *db, int *grade_count){
    for (int i = 0; i< 7; i++) grade_count[i] = 0;

    for (int i = 0; i < db->count; i++){
        if (db->students[i].percentage>=90) grade_count[0]++;
        else if (db->students[i].percentage >= 85) grade_count[1]++;
        else if (db->students[i].percentage >= 75) grade_count[2]++;
        else if (db->students[i].percentage >= 65) grade_count[3]++;
        else if (db->students[i].percentage >= 60) grade_count[4]++;
        else if (db->students[i].percentage >= 55) grade_count[5]++;
        else if (db->students[i].percentage >= 50) grade_count[6]++;
        else grade_count[7]++;
    }
}

int readStudentsFromFile(const char *filename, StudentDatabase *db){
    FILE *fp = fopen(filename, "r");
    if (!fp){
        fprintf(stderr, "Error: Cannot open file%s\n", filename);
        return 0;
    }

    char line[512];
    db->count = 0;

    while(fgets(line, sizeof(line), fp) && db->count < MAX_STUDENTS){
        char id[MAX_ID_LEN], name[MAX_NAME_LEN];
        float m[10];

        if(sscanf(line, "%s %s %f %f %f %f %f %f %f %f %f %f", id, name, &m[0], &m[1], &m[2], &m[3], &m[4], &m[5], &m[6], &m[7], &m[8], &m[9]) != 12){
            fprintf(stderr, "Warning: Invalid format in line\n");
            continue;
        }

        if (!isValidID(id, db)){
            fprintf(stderr, "Warning: Invalid or duplicate ID %s\n", id);
            continue;
        }

        if (!isValidName(name)){
            fprintf(stderr, "Warning: Invalid name %s\n", name);
            continue;
        }

        int valid = 1;
        for (int i = 0; i < 10; i++){
            if (!isValidMarks(m[i])){
                fprintf(stderr, "Warning: Invalid marks for student %s\n", id);
                valid = 0;
                break;
            }
        }
        if (!valid) continue;

        strcpy(db->students[db->count].id, id);
        strcpy(db->students[db->count].name, name);

        for (int i = 0; i < SUBJECTS; i++){
            db->students[db->count].minor_marks[i] = m[i*2];
            db->students[db->count].major_marks[i] = m[i*2 + 1];
        }

        calculateTotalAndPercentage(&db->students[db->count]);
        db->students[db->count].grade = assignGrade(db->students[db->count].percentage);
        calculateCGPA(&db->students[db->count]);

        db->count++;
    }

    fclose(fp);
    return db->count;
}

void writeReportToFile(const char *filename, StudentDatabase *db){
    FILE *fp = fopen(filename, "w");
    if (!fp){
        fprintf(stderr, "Error: Cannot create output file %s\n", filename);
        return;
    }

    fprintf(fp, "------------------------------------------\n");
    fprintf(fp, "STUDENT RESULT REPORT\n");
    fprintf(fp, "------------------------------------------\n\n");

    fprintf(fp, "%-15s %-15s ", "Student ID", "Name");
    for(int i = 0; i < SUBJECTS; i++){
        fprintf(fp, " S%d-Minor    S%d-Major ", i+1, i+1);
    }
    fprintf(fp, "%-8s %-10s %-7s %-6s\n", "   Total", "  Percent", " Grade", "   CGPA");

    fprintf(fp, "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for(int i = 0; i < db->count; i++){
        fprintf(fp, "%-15s %-20s ", db->students[i].id, db->students[i].name);

        for(int j = 0; j < SUBJECTS; j++){
            fprintf(fp, "%-10.0f %-10.0f ", db->students[i].minor_marks[j], db->students[i].major_marks[j]);
        }

        fprintf(fp, "%-6.0f %-10.2f %-7c %-6.2f\n",
            db->students[i].total,
            db->students[i].percentage,
            db->students[i].grade,
            db->students[i].cgpa);
    }

    fprintf(fp, "\n\n--------------\n");
    fprintf(fp, "STATISTICS\n");
    fprintf(fp, "--------------\n\n");

    float class_avg = calculateClassAverage(db);
    float highest, lowest;
    findHighestLowest(db, &highest, &lowest);
    int grade_count[8] = {0};
    countGrades(db, grade_count);

    fprintf(fp, "Total Students: %d\n", db->count);
    fprintf(fp, "Class Average Percentage: %.2f%%\n", class_avg);
    fprintf(fp, "Highest Percentage: %.2f%%\n", highest);
    fprintf(fp, "Lowest Percentage: %.2f%%\n\n", lowest);

    fprintf(fp, "Grade Distribution: \n");
    fprintf(fp, " O  (>= 90): %d students\n", grade_count[0]);
    fprintf(fp, " A+ (85-90): %d students\n", grade_count[1]);
    fprintf(fp, " A  (75-85): %d students\n", grade_count[2]);
    fprintf(fp, " B+ (65-75): %d students\n", grade_count[3]);
    fprintf(fp, " B  (60-65): %d students\n", grade_count[4]);
    fprintf(fp, " C  (55-60): %d students\n", grade_count[5]);
    fprintf(fp, " D  (50-55): %d students\n", grade_count[6]);
    fprintf(fp, " F  (< 50 ): %d students\n", grade_count[7]);

    fprintf(fp, "\n------------------------------------------\n");

    fclose(fp);

    printf("Report successfully written to %s\n", filename);
}