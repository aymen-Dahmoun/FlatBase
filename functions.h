#ifndef FUNCTIONS_H
#define FUNCTIONS_H
/*
 #######
 this file is the header file for our function.c file, it defines the usable function
 in the project folder and the structs used in the project
 #######
 */
#include "gtk/gtk.h"
typedef struct Buffers { // a struct used as a bridge between UI elements and functions used
                        // to handle the data manipulation
    GtkEntryBuffer *idBuffer;
    GtkEntryBuffer *nameBuffer;
    GtkEntryBuffer *birthYearBuffer;
    GtkEntryBuffer *groupBuffer;
    GtkEntryBuffer *averageBuffer;
    GtkEntryBuffer *flagBuffer;
    GtkEntryBuffer *mathBuffer;
    GtkEntryBuffer *algebraBuffer;
    GtkEntryBuffer *sfsdBuffer;
    GtkEntryBuffer *oopBuffer;
} Buffers;

typedef struct student { // student struct
    char id[4];
    char name[30];
    int birthYear;
    char group[3];
    char sfsd[10];
    char oop[10];
    char math[10];
    char algebra[10];
    float average;
    int flag;
} student;
// definition of all our functions
int prepare();
int search(const char *idc, char *result, size_t result_size);
int insert(student* student);
int deleteLogically(const char* id);
int reset();
int modify(student* stu);
void fill(const char *field, int index, student *stu);
student prepareInput(const char* id, const char* name, const char* birthYear, const char* group,const char *sfsd,const char *oop,const char *math,const char *alg, const char* flag );
int extract(const char* grp, char* name, size_t size);
int overWrite();
#endif
