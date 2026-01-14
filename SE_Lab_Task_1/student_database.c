#include "mylib.h"

int main(){
    StudentDatabase db;
    db.count = 0;

    char input_file[100] = "students.txt";
    char output_file[100] = "result_report.txt";

    int records = readStudentsFromFile(input_file, &db);

    if(records == 0){
        printf("No valid student records found or filr not accessible.\n");
        return 1;
    }

    writeReportToFile(output_file, &db);

    printf("\nProceesing Complete!\n");

    return 0;
}