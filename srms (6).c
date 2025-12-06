/* SRM STUDENT SYSTEM - STRICT COMPATIBILITY MODE
   Fixed for Dev-C++ (C89 Standard)
   Updated: Added Student Login (ID + Phone Validation)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>   
#include <windows.h> 

/* --- Settings --- */
#define DB_FILE "univ_records.dat"
#define ADMIN_PIN "1234"
#define STAFF_PIN "5678"

/* --- Data Structure --- */
typedef struct {
    char id[20];
    char name[50];
    char branch[10];
    char phone[15];
    float cgpa;
} Student;

/* --- Prototypes --- */
void init_default_data();
void main_menu();
void admin_panel();
void staff_panel();
void student_login(); /* NEW FUNCTION */

/* Features */
void add_student();
void list_students();
void find_student();
void remove_student();

/* Helpers */
void header(char *text);
void pause_screen();
void mask_input(char *pass);
void clear_buffer();

/* --- MAIN --- */
int main() {
    /* Set Color */
    system("COLOR 0B"); 
    
    /* Load Data */
    init_default_data(); 
    
    while(1) {
        main_menu();
    }
    return 0;
}

/* --- DATA LOADER --- */
void init_default_data() {
    /* ERROR FIX: All variables declared at top */
    FILE *fp;
    Student s1; 
    Student s2; 
    Student s3; 
    
    fp = fopen(DB_FILE, "rb");
    
    if (fp != NULL) {
        fclose(fp);
        return;
    }

    /* Manually assign data to avoid C89 struct init issues */
    strcpy(s1.id, "101"); strcpy(s1.name, "Alice smith"); strcpy(s1.branch, "CSE"); strcpy(s1.phone, "1112223333"); s1.cgpa = 9.2;
    strcpy(s2.id, "102"); strcpy(s2.name, "Suresh");   strcpy(s2.branch, "ECE"); strcpy(s2.phone, "4445556666"); s2.cgpa = 7.8;
    strcpy(s3.id, "103"); strcpy(s3.name, "Ramesh");   strcpy(s3.branch, "MECH");strcpy(s3.phone, "7778889999"); s3.cgpa = 8.5;

    fp = fopen(DB_FILE, "wb");
    if (fp != NULL) {
        fwrite(&s1, sizeof(Student), 1, fp);
        fwrite(&s2, sizeof(Student), 1, fp);
        fwrite(&s3, sizeof(Student), 1, fp);
        fclose(fp);
        printf(">> System Initialized: Default data loaded.\n");
        Sleep(1000);
    }
}

/* --- MENUS --- */
void main_menu() {
    int opt;
    char pin[20];

    system("cls");
    printf("\n====================================\n");
    printf("      UNIVERSITY PORTAL v3.1        \n");
    printf("====================================\n");
    printf(" [1] Admin Login (Full Access)\n");
    printf(" [2] Staff Login (View Only)\n");
    printf(" [3] Student Login (View My Profile)\n");
    printf(" [4] Exit\n");
    printf("------------------------------------\n");
    printf(" Enter Option: ");
    
    if(scanf("%d", &opt) != 1) {
        clear_buffer();
        opt = 0;
    }

    switch(opt) {
        case 1:
            printf(" Enter Admin PIN: ");
            mask_input(pin);
            if(strcmp(pin, ADMIN_PIN) == 0) admin_panel();
            else { printf("\n (!) Wrong PIN"); pause_screen(); }
            break;
        case 2:
            printf(" Enter Staff PIN: ");
            mask_input(pin);
            if(strcmp(pin, STAFF_PIN) == 0) staff_panel();
            else { printf("\n (!) Wrong PIN"); pause_screen(); }
            break;
        case 3:
            student_login(); /* CALLS NEW FUNCTION */
            break;
        case 4:
            exit(0);
        default:
            printf("\n (!) Invalid Option");
            pause_screen();
    }
}

void admin_panel() {
    int choice;
    do {
        header("ADMIN CONTROL PANEL");
        printf(" 1. Register New Student\n");
        printf(" 2. List All Students\n");
        printf(" 3. Search Student\n");
        printf(" 4. Remove Student\n");
        printf(" 5. Logout\n");
        printf("\n Action: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: add_student(); break;
            case 2: list_students(); break;
            case 3: find_student(); break;
            case 4: remove_student(); break;
            case 5: return;
        }
    } while(choice != 5);
}

void staff_panel() {
    int choice;
    do {
        header("STAFF VIEW MODE");
        printf(" 1. List All Students\n");
        printf(" 2. Search Student\n");
        printf(" 3. Logout\n");
        printf("\n Action: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: list_students(); break;
            case 2: find_student(); break;
            case 3: return;
        }
    } while(choice != 3);
}

/* --- NEW FEATURE: STUDENT LOGIN --- */
void student_login() {
    char input_id[20];
    char input_phone[20];
    Student s;
    int found = 0;
    FILE *fp;

    header("STUDENT SECURE LOGIN");
    printf(" Note: Your Password is your registered Phone Number.\n\n");
    
    printf(" Enter Student ID: ");
    scanf("%s", input_id);

    printf(" Enter Password (Phone): ");
    mask_input(input_phone);

    fp = fopen(DB_FILE, "rb");
    if(!fp) {
        printf("\n (!) Database Error.\n");
        pause_screen();
        return;
    }

    while(fread(&s, sizeof(Student), 1, fp)) {
        /* Check if ID AND Phone match */
        if(strcmp(s.id, input_id) == 0 && strcmp(s.phone, input_phone) == 0) {
            found = 1;
            system("cls");
            printf("\n ===================================\n");
            printf("      WELCOME, %s \n", s.name);
            printf(" ===================================\n");
            printf("  Student ID : %s\n", s.id);
            printf("  Branch     : %s\n", s.branch);
            printf("  Phone      : %s\n", s.phone);
            printf("  CGPA       : %.2f\n", s.cgpa);
            printf(" ===================================\n");
            break;
        }
    }
    
    fclose(fp);

    if(!found) {
        printf("\n\n (!) Login Failed. Invalid ID or Password.\n");
    }
    pause_screen();
}

/* --- CORE FUNCTIONS --- */

void add_student() {
    Student s;
    FILE *fp = fopen(DB_FILE, "ab");
    
    header("ADD STUDENT");
    
    printf(" ID Number : "); scanf("%s", s.id);
    
    printf(" Full Name : "); 
    clear_buffer();
    scanf("%[^\n]s", s.name); 
    
    printf(" Branch    : "); scanf("%s", s.branch);
    printf(" Phone No  : "); scanf("%s", s.phone);
    printf(" CGPA      : "); scanf("%f", &s.cgpa);

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
    printf("\n >> Success! Student added.\n");
    pause_screen();
}

void list_students() {
    Student s;
    FILE *fp = fopen(DB_FILE, "rb");
    
    if(!fp) { printf("\n (!) No Data Found."); pause_screen(); return; }

    header("STUDENT DATABASE");
    printf(" %-10s %-20s %-8s %-12s %-5s\n", "ID", "NAME", "BRANCH", "PHONE", "CGPA");
    printf(" -------------------------------------------------------------\n");

    while(fread(&s, sizeof(Student), 1, fp)) {
        printf(" %-10s %-20s %-8s %-12s %-5.2f\n", s.id, s.name, s.branch, s.phone, s.cgpa);
    }
    printf(" -------------------------------------------------------------\n");
    pause_screen();
    fclose(fp);
}

void find_student() {
    Student s;
    char target[20];
    int found = 0;
    FILE *fp = fopen(DB_FILE, "rb");

    if(!fp) { printf("\n (!) Database empty."); pause_screen(); return; }

    printf(" Enter ID to Search: ");
    scanf("%s", target);

    while(fread(&s, sizeof(Student), 1, fp)) {
        if(strcmp(s.id, target) == 0) {
            found = 1;
            printf("\n [ RECORD FOUND ]\n");
            printf(" Name   : %s\n", s.name);
            printf(" Branch : %s\n", s.branch);
            printf(" Phone  : %s\n", s.phone);
            printf(" CGPA   : %.2f\n", s.cgpa);
        }
    }
    if(!found) printf("\n (!) Student Not Found.\n");
    
    fclose(fp);
    pause_screen();
}

void remove_student() {
    Student s;
    char target[20];
    int found = 0;
    FILE *fp, *temp;
    
    header("DELETE STUDENT");
    printf(" Enter ID to Delete: ");
    scanf("%s", target);

    fp = fopen(DB_FILE, "rb");
    temp = fopen("temp.dat", "wb");
    
    if(!fp) { printf("\n (!) Error opening file."); return; }

    while(fread(&s, sizeof(Student), 1, fp)) {
        if(strcmp(s.id, target) == 0) {
            found = 1;
        } else {
            fwrite(&s, sizeof(Student), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if(found) {
        remove(DB_FILE);
        rename("temp.dat", DB_FILE);
        printf("\n >> Student deleted successfully.\n");
    } else {
        remove("temp.dat");
        printf("\n (!) ID not found.\n");
    }
    pause_screen();
}

/* --- UTILS --- */

void header(char *text) {
    system("cls");
    printf(" :: %s ::\n", text);
    printf(" ------------------------\n");
}

void pause_screen() {
    printf("\n Press any key to continue...");
    getch();
}

void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void mask_input(char *pass) {
    int i = 0;
    char ch;
    while(1) {
        ch = getch();
        if(ch == 13) { /* Enter */
            pass[i] = '\0';
            break;
        } else if(ch == 8) { /* Backspace */
            if(i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            pass[i++] = ch;
            printf("*");
        }
    }
}
