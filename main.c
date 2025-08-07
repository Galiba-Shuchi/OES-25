#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void add_course();
void show_courses();
void show_question();
void add_question();
void delete_question();
void update_question();
void admin_menu();
void student_menu();
void exam_rules();
void student_exam();
void show_all_results();

typedef struct
{
    char student_id[20];
    char name[50]; 
    char password[50]; 
    char role[10];
    
} user;

typedef struct
{ 
    char course_name[100];
    char course_code[20];
    
} Course;

typedef struct
{
    char course_code[20];
    char question[300];
    char optionA[100];
    char optionB[100]; 
    char optionC[100]; 
    char optionD[100]; 
    char check;
} MCQ;

typedef struct
{
char student_id[20];
char course_code[20]; 
int score;
} ExamResult;



// MAIN FUNCTION
int main() {
    int user_choice, action_choice;

    while (1) {
        printf("\n=====================================\n");
        printf("        🖥️  ONLINE EXAM SYSTEM\n");
        printf("=====================================\n");
        printf("Choose user type:\n");
        printf("  1. Student 👨‍🎓\n");
        printf("  2. Admin 🧑‍💼\n");
        printf("  3. Exit 🚪\n");
        printf("Enter your choice (1-3): ");
        scanf("%d", &user_choice);
        getchar();  // consume newline

        if (user_choice == 3) {
            printf("\nThank you for using the Online Exam System.\n");
            break;
        }

        if (user_choice != 1 && user_choice != 2) {
            printf("\n❌ Invalid user type. Please try again.\n");
            continue;
        }

        const char *role = (user_choice == 1) ? "student" : "admin";

        printf("\n=====================================\n");
        printf("        👤 %s Portal\n", role);
        printf("=====================================\n");
        printf("Choose action:\n");
        printf("  1. Register 📝\n");
        printf("  2. Login 🔐\n");
        printf("Enter your choice (1-2): ");
        scanf("%d", &action_choice);
        getchar();

        switch (action_choice) {
            case 1:
                register_user(role);
                break;

            case 2:
                if (login_user(role)) {
                    if (strcmp(role, "student") == 0)
                        student_menu();
                    else
                        admin_menu();
                } else {
                    printf("\n❌ Login failed. Please try again.\n");
                }
                break;

            default:
                printf("\n❌ Invalid action choice.\n");
        }
    }

    return 0;
}