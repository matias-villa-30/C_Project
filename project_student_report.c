#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE_SUBJECT 32
#define MAX_SIZE_NAME 100

typedef struct student {
    char subject[MAX_SIZE_SUBJECT];
    int score;
    int grade;
    struct student *next;
} student_;

// Function prototypes
void getStudentName(char *student_name);
int getNumberOfSubjects();
student_ *createStudentList(int number_grades);
void printReport(const char *student_name, student_ *head, int number_grades);
void saveReportToFile(const char *student_name, student_ *head, int number_grades);
void freeMemory(student_ *head);
int getValidScore(const char *subject_name);
int calculateGrade(int score);
void generateFileName(char *filename, const char *student_name);



int main() {
    char student_name[MAX_SIZE_NAME];
    getStudentName(student_name);

    int number_grades = getNumberOfSubjects();

    student_ *head = createStudentList(number_grades);

    printReport(student_name, head, number_grades);
    saveReportToFile(student_name, head, number_grades);

    freeMemory(head);

    return 0;
}

// Function to get student name
void getStudentName(char *student_name) {
    printf("Please enter your name: ");
    fgets(student_name, MAX_SIZE_NAME, stdin);
    student_name[strcspn(student_name, "\n")] = 0; // Remove newline
}

// Function to get the number of subjects
int getNumberOfSubjects() {
    int num;
    printf("How many subjects do you want to calculate grades for? ");
    scanf("%d", &num);
    getchar(); // Consume newline left in buffer
    return num;
}

// Function to create a linked list of subjects and grades
student_ *createStudentList(int number_grades) {
    student_ *head = NULL;
    student_ *tail = NULL;

    for (int i = 1; i <= number_grades; i++) {
        student_ *nuevo = malloc(sizeof(student_));
        if (nuevo == NULL) {
            printf("Memory allocation failed\n");
            exit(1);
        }

        printf("Enter subject %d name: ", i);
        fgets(nuevo->subject, MAX_SIZE_SUBJECT, stdin);
        nuevo->subject[strcspn(nuevo->subject, "\n")] = 0; // Remove newline

        nuevo->score = getValidScore(nuevo->subject);
        nuevo->grade = calculateGrade(nuevo->score);
        nuevo->next = NULL;

        // Insert into linked list
        if (head == NULL) {
            head = nuevo;
            tail = nuevo;
        } else {
            tail->next = nuevo;
            tail = nuevo;
        }
    }
    return head;
}

// Function to get a valid score
int getValidScore(const char *subject_name) {
    int temp_grades;
    while (1) {
        printf("Enter your score for %s (0-100): ", subject_name);
        if (scanf("%d", &temp_grades) != 1) { // Check for non-integer input
            printf("Invalid input, please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        if (temp_grades < 0 || temp_grades > 100) {
            printf("Wrong grade, try again\n");
            continue;
        }
        getchar(); // Consume newline after scanf
        return temp_grades;
    }
}

// Function to calculate grade based on score
int calculateGrade(int score) {
    if (score < 50) return 0;
    if (score < 60) return 1;
    if (score < 70) return 2;
    if (score < 80) return 3;
    if (score < 90) return 4;
    return 5;
}

// Function to print student report
void printReport(const char *student_name, student_ *head, int number_grades) {
    float total_grades = 0.0;
    int count = 0;

    printf("\n-------------------------------------------------\n");
    
    printf("Student: %s\n", student_name);
    printf("-------------------------------------------------\n");
    printf("%-20s %-10s %-6s\n", "Subject", "Score", "Grade");
    printf("-------------------------------------------------\n");

    student_ *temp = head;
    while (temp != NULL) {
        printf("%-20s %3d%%      %6d\n", temp->subject, temp->score, temp->grade);
        total_grades += temp->grade;
        count++;
        temp = temp->next;
    }

    float average = count > 0 ? total_grades / count : 0;

    
    printf("\nAverage grade: %.2f\n", average);
    printf("-------------------------------------------------\n");

    // Print report to file

}

// Function to save the student report to a file
void saveReportToFile(const char *student_name, student_ *head, int number_grades) {
    char filename[MAX_SIZE_NAME + 20];
    generateFileName(filename, student_name);

    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error creating file.\n");
        return;
    }

    float total = 0;
    int count = 0;

    
    
    fprintf(file, "-------------------------------------------------\n");
    fprintf(file, "Student: %s\n", student_name);
    fprintf(file, "-------------------------------------------------\n");
    fprintf(file, "%-20s %-10s %-6s\n", "Subject", "Score", "Grade");
    fprintf(file, "-------------------------------------------------\n");

    student_ *temp = head;
    while (temp) {
        fprintf(file, "%-20s %3d%%      %6d\n", temp->subject, temp->score, temp->grade);
        total += temp->grade;
        count++;
        temp = temp->next;
    }

    
    fprintf(file, "\nAverage grade: %.2f\n", count > 0 ? total / count : 0);
    fprintf(file, "-------------------------------------------------\n");

    fclose(file);
    printf("Report saved as: %s\n", filename);
}


// Function to free allocated memory
void freeMemory(student_ *head) {
    student_ *temp = head;
    while (temp != NULL) {
        student_ *next = temp->next;
        free(temp);
        temp = next;
    }
}

void generateFileName(char *filename, const char *student_name) {
    strcpy(filename, student_name);
    for (int i = 0; filename[i]; i++) {
        if (filename[i] == ' ') {
            filename[i] = '_'; // Replace spaces with underscores
        }
    }
    strcat(filename, "_report.txt");
}