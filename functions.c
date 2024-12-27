#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

/*
######
the prepare function is meant for turning our data stored in a text file formatted in CSV file shape
the data is in a format of lines of data, each line represent a student and each student have data fields
separated by a semicolon ';'
the prepare function takes each line sequentially; separates each string ending by ';' and fill it
in the proper struct filed (instance)
######
 */
int prepare() {
    FILE *input_file = fopen("data.txt", "r"); // oppening files
    FILE *output_file = fopen("ready.txt", "wb");

    if (input_file == NULL || output_file == NULL) {
        return 0;  // File opening failed
    }

    char line[200];
    student stud;

    while (fgets(line, sizeof(line), input_file)) { // reading text file sequentially
        // Parse the line into the student struct
        char *token = strtok(line, ";");
        strcpy(stud.id, token); // strtok separates string based on a separator

        token = strtok(NULL, ";");
        strcpy(stud.name, token);

        token = strtok(NULL, ";");
        stud.birthYear = atoi(token);

        token = strtok(NULL, ";");
        strcpy(stud.group, token);

        token = strtok(NULL, ";");
        strcpy(stud.sfsd, token);

        token = strtok(NULL, ";");
        strcpy(stud.oop, token);

        token = strtok(NULL, ";");
        strcpy(stud.math, token);

        token = strtok(NULL, ";");
        strcpy(stud.algebra, token);

        token = strtok(NULL, ";");

        // this is not necessary but in my machine the decimal part of a float
        // is separated by a comma ',' which caused me some troubles in casting
        // notes and average to a float
        for (int i = 0; token[i] != '\0'; i++) {
            if (token[i] == '.') {  // Change dot to comma
                token[i] = ',';
            }
        }

        // Convert to float and log the result after casting
        stud.average = strtof(token, NULL); // this function is similar to atof
        token = strtok(NULL, ";");
        stud.flag = atoi(token);

        // Write the student struct to the binary file
        fwrite(&stud, sizeof(stud), 1, output_file);
    }

    fclose(input_file);
    fclose(output_file);

    return 1;  // Success
}

/*
 ######
 search function takes an id string and search it sequentially through the binary
 file that we prepared, and it has two other parameters a string and a size of string,
 these two are used to store the searching result to display it in the UI, this method is used
 to share data between functions in different files
 ######
 */
int search(const char* id, char *result, size_t result_size) {
    FILE *sfp = fopen("ready.txt", "rb"); // opening binary file
    if (sfp == NULL) {
        printf("Input file not found\n");
        return 0; // failed to open file
    }

    student student;
    while (fread(&student, sizeof(student), 1, sfp)) {
        if (strcmp(student.id, id) == 0) {
            // Create the result text using snprintf to avoid buffer overflow
            char *temp_math = strtok(student.math, ",");
            char *temp_alg = strtok(student.algebra, ",");
            char *temp_sfsd = strtok(student.sfsd, ",");
            char *temp_oop = strtok(student.oop, ",");

            snprintf(result, result_size, "Found Student:\nID: %s\n Name: %s\n Group: %s\n BirthYear: %d\n Sfsd: %s\n OOP: %s\n Math: %s\n Algebra: %s\n Average: %.2f\n Flag: %d\n",
                     student.id, student.name, student.group, student.birthYear, temp_sfsd,temp_oop, temp_math, temp_alg, student.average, student.flag);
            printf("%s\n", student.group);
            fclose(sfp);  // Close the file before returning
            return 1;  // Student found
        }
    }

    fclose(sfp);
    return 0;  // Student not found
}

// a small function used to calculate the average of students
float prepareNote(char* sfsd, char* oop, char* math, char* algebra) {
    // Predefined weights for each subject
    float result = (atof(sfsd)*4 + atof(oop)*3 + atof(math)*2 + atof(algebra)*5)/14;
    return result;
}

// a fucntion used to prapare all inputs for handling by
// fillin them in a student variable in case of inserting a new student
student prepareInput(const char* id, const char* name, const char* birthYear, const char* group,const char *sfsd,const char *oop,const char *math,const char *alg, const char* flag ) {

    student stu;
    // checking the validity of inputs is not necessary here since we already
    // did that in the main file but in case condition failure or what ever happened
    // data will be safe in each case
    if (atof(sfsd) > 20 || atof(sfsd) < 0) {
        strcpy(stu.sfsd,"00");
    }
    else {
        strcpy(stu.sfsd, sfsd);
    }
    if (atof(oop) > 20 || atof(oop) < 0) {
        strcpy(stu.oop,"00");
    }
    else {
        strcpy(stu.oop, oop);
    }
    if (atof(math) > 20 || atof(math) < 0) {
        strcpy(stu.math,"00");
    }
    else {
        strcpy(stu.math, math);
    }
    if (atof(alg) > 20 || atof(alg) < 0) {
        strcpy(stu.algebra,"00");
    }
    else {
        strcpy(stu.algebra, alg);
    }
    strncpy(stu.id, id, 4);
    strcpy(stu.name, name);
    stu.birthYear = atoi(birthYear);
    strncpy(stu.group, group, 3);
    stu.average = 0;
    stu.flag = atoi(flag);
    return stu;
}

// this function is used to insert a new student to our binary file
int insert(student* student) {
    FILE *fps = fopen("ready.txt", "a+b"); // opening file
    if (fps == NULL) {
        printf("Input file not found\n");
        return 0; // failure of opening the file
    }

    if (strlen(student->id) == 0 || strlen(student->name) == 0) {
        fclose(fps);
        return -1; // name and ID should never be empty strings
    }
    char a[1];
    // Check if student already exists
    if (!search(student->id, a, sizeof(a))) { // If the student doesn't exist
        student->average = prepareNote(student->sfsd,student->oop, student->math, student->algebra);
        strcat(student->sfsd, ",4");
        strcat(student->oop, ",3");
        strcat(student->math, ",2");
        strcat(student->algebra, ",5");
        student->group[2] = '\0';
        fseek(fps, 0, SEEK_END);
        fwrite(student, sizeof(*student), 1, fps); // Write the struct
        fclose(fps);
        return 1; // Student successfully added
    }

    // Student already exists
    printf("Student with ID %s already exists.\n", student->id);
    fclose(fps);
    return 0; // Duplicate entry
}

// a function to modify the flag of a student and either mark him as deleted or remove his mark
// this function works like a toggle that turns the status of student from 0 to 1 if it was 0
// or from 1 to 0 if it was 1
int deleteLogically(const char* id) {
    FILE *sfp = fopen("ready.txt", "r+b");  // Open file in read-write binary mode
    if (sfp == NULL) {
        printf("Input file not found\n");
        return 0; // opening file fails
    }
    student stu;
    while (fread(&stu, sizeof(student), 1, sfp)) {
        if (strcmp(stu.id, id) == 0) {
            if (stu.flag == 1) {
                stu.flag = 0;// remove the deleted mark
                fseek(sfp, -(long)sizeof(student), SEEK_CUR);  // Move the file pointer back to the start of the current student record
                fwrite(&stu, sizeof(student), 1, sfp);  // Write the updated student data back to the file
                fclose(sfp);
                return 2;  // Successfully marked the student as deleted
            }
            else {
                stu.flag = 1;  // Mark the student as deleted by setting the flag to 1
                fseek(sfp, -(long)sizeof(student), SEEK_CUR);  // Move the file pointer back to the start of the current student record
                fwrite(&stu, sizeof(student), 1, sfp);  // Write the updated student data back to the file
                fclose(sfp);
                return 1;  // Successfully marked the student as deleted
            }

        }
    }

    fclose(sfp);
    return 0;  // Student with the given ID not found
}

// the reset function simply removes all the flagged student by creating a copy
// of the binary file, fill it with the non deleted students, closes it and rename
// it ready.txt and removes the old ready.txt
// the searching of deleted students is sequential
int reset() {
    FILE *sfp = fopen("ready.txt", "rb");
    if (sfp == NULL) {
        printf("Input file not found\n");
        return 0;
    }

    FILE *sfp2 = fopen("readycpy.txt", "wb");
    if (sfp2 == NULL) {
        printf("Output file not found\n");
        fclose(sfp);  // Close the input file if output fails
        return 0;
    }

    student student;
    // Copy all students whose flag is 0 to the new file
    while (fread(&student, sizeof(student), 1, sfp)) {
        if (student.flag == 0) {  // Only write students with flag == 0 (not deleted)
            fwrite(&student, sizeof(student), 1, sfp2);
        }
    }

    fclose(sfp);
    fclose(sfp2);

    // Delete the original file and rename the copied file to the original file's name
    if (remove("ready.txt") != 0) {
        printf("Error deleting the original file\n");
        return 0;
    }

    if (rename("readycpy.txt", "ready.txt") != 0) {
        printf("Error renaming the copied file\n");
        return 0;
    }

    return 1;  // Successfully deleted the marked students
}

// the modify function works similarly to insert but it doesn't change
// the content of data if the input was empty, this way the user will modify
// only the data he wants to modify while preserving the data that he kept
// their entry texts empty while clicking the modify button
int modify(student* stu) {
    FILE *sfp = fopen("ready.txt", "r+b");  // Open file in read and write mode
    if (sfp == NULL) {
        printf("Input file not found\n");
        return 0;
    }

    student buf;
    int modified = 0;

    // Read the file record by record
    while (fread(&buf, sizeof(student), 1, sfp)) {
        // Find the student with the matching ID
        if (strcmp(buf.id, stu->id) == 0) { // ID used for finding the student user wants to modify
            // the ID and Average and Flag can't be modified by this function
            // Modify only the fields that are not empty in the input
            // rather if notes were modified the new average will be calculated

            if (strcmp(stu->name, "") != 0) {
                strcpy(buf.name, stu->name);
            }
            if (stu->birthYear != 0) {
                buf.birthYear = stu->birthYear;
            }
            if (strcmp(stu->group, "") != 0) {
                strcpy(buf.group, stu->group);
            }
            if (strcmp(stu->sfsd, "") != 0) {
                strcpy(buf.sfsd, stu->sfsd);
            }
            if (strcmp(stu->oop, "") != 0) {
                strcpy(buf.oop, stu->oop);
            }
            if (strcmp(stu->math, "") != 0) {
                strcpy(buf.math, stu->math);
            }
            if (strcmp(stu->algebra, "") != 0) {
                strcpy(buf.algebra, stu->algebra);
            }
            buf.average = prepareNote(buf.sfsd,buf.oop, buf.math, buf.algebra);


            // Update the record in the file
            fseek(sfp, -(long)sizeof(student), SEEK_CUR);
            fwrite(&buf, sizeof(student), 1, sfp);
            modified = 1;
            break;  // No need to continue reading after modification
        }
    }

    fclose(sfp);

    // If no student was modified, inform the user
    if (!modified) {
        printf("Student with ID %s not found.\n", stu->id);
        return 0;
    }

    printf("Student data modified successfully.\n");
    return 1;
}

// the extract function works similarly to search, it search for students
// from a given group sequentially and fill them into a string in a formatted way
// to display on the screen
int extract(const char* grp, char* name, size_t size) {
    FILE *sfp = fopen("ready.txt", "rb");
    if (sfp == NULL) {
        printf("Input file not found\n");
        return 0;  // Exit if the file cannot be opened
    }
    student stu;

    // Initialize name buffer (empty string)
    name[0] = '\0';  // Ensure the buffer is empty before concatenation

    while (fread(&stu, sizeof(student), 1, sfp)) {
        if (strcmp(stu.group, grp) == 0) {  // Check if groups match
            // Simply append the name and newline
            strcat(name, stu.name);
            strcat(name, "\n");
        }
    }
    fclose(sfp);

    return 1;
}

// this fucntion is meant for saving the data in the data.txt text file after
// finishing using the application, it simply takes all the binary data and
// stores it in a formated way in our data.txt which will be overwritten
int overWrite() {
    FILE *input_file = fopen("ready.txt", "rb");
    FILE *output_file = fopen("data.txt", "w");

    if (input_file == NULL || output_file == NULL) {
        return 0;  // File opening failed
    }

    student stud;
    while (fread(&stud, sizeof(stud), 1, input_file)) {
        strcat(stud.sfsd, ",4");
        strcat(stud.oop, ",3");
        strcat(stud.math, ",2");
        strcat(stud.algebra, ",5");
        // Write data in the original text format with a proper float precision for average
        fprintf(output_file, "%s;%s;%d;%s;%s;%s;%s;%s;%.1f;%d\n",
                stud.id, stud.name, stud.birthYear, stud.group,
                stud.sfsd, stud.oop, stud.math, stud.algebra, stud.average, stud.flag);
    }

    fclose(input_file);
    fclose(output_file);

    return 1;  // Success
}
