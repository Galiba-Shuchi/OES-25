#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <conio.h> 
#include <windows.h>


#define ADMIN_FILE "admin.txt"
#define TEACHER_FILE "teacher.txt"
#define USER_FILE "user.txt"
#define VIDEO_FILE "video_resources.txt"
#define PDF_FILE "pdf_resources.txt"
#define WEBPAGE_FILE "webpage_resources.txt"
#define COUNSELING_FILE "counseling.txt"
#define MCQ_FILE "mcq.txt"
#define WRONG_ANSWERS_FILE "wrong_answers.txt"
#define EXAM_RESULT_FILE "exam_results.txt"
#define WIDTH 50
// ANSI color codes
#define BLUE_TEXT    "\033[1;34m"
#define RED_TEXT     "\033[1;31m"
#define RESET_COLOR  "\033[0m"
#define GREEN_TEXT   "\033[1;32m"
#define YELLOW_TEXT  "\033[1;33m"

char loggedInStudentID[20] = "";
int examDurationMinutes = 0; 


// Structures
typedef struct
{
    char student_id[20];
    char name[50];
    char password[50];
    char student_role[10];
} user;

typedef struct
{
    char teacher_id[20];
    char name[50];
    char password[50];
    char subject[30];
} teacher;

struct teacher2
{
    char teacher_id[20];
    char name[50];
    char password[50];
    char subject[30];
};
struct teacher2 prevAllTeachers[100];

typedef struct admin {
    char admin_id[20];
    char name[50];
    char password[50];
    char admin_role[20];
} admin;

typedef struct
{
    char course_name[100];
    char course_code[20];
} Course;

typedef struct
{
    char course_code[20];
    char question[300];
    char option_a[100];
    char option_b[100];
    char option_c[100];
    char option_d[100];
    char check;
    char correct_answer;
    int duration_minutes;
} MCQ;

typedef struct
{
    char question[300];
    char correct_answer;
    char student_answer;
} WrongAnswer;

typedef struct
{
    char student_id[20];
    char course_code[20];
    int score;
} ExamResult;

typedef struct
{
    int record_id;
    char student_id[20];
    char teacher_id[20];
    char date[20];
    char topic[100];
    char notes[300];
} Counseling;

typedef struct
{
    char title[100];
    char course_code[20];
    char url[200];
} VideoResource;

typedef struct
{
    char title[100];
    char course_code[20];
    char url[200];
} PDFResource;

typedef struct
{
    char title[100];
    char course_code[20];
    char url[200];
} WebpageResource;



// Function Declarations

// log in Registration
void registerUser();
int loginUser();
void displayUser(const user *u);
void updateUser(user *u);
void deleteUser(user *u);
void registerTeacher();
int loginTeacher();
void displayTeacher(const teacher *t);
void updateTeacher(teacher *t);
void deleteTeacher(teacher *t);
void registerAdmin();
int loginAdmin();
// course
void addCourse();
void showCourses();
void updateCourse();
void deleteCourse();
// question
void addQuestion();
void showQuestionsForAdmin();
void updateQuestion();
void deleteQuestion();
// exam
int loadQuestions(const char *course_code, MCQ questions[]);
void showStudentResults(const char *student_id);
void showAllResults(const char *student_id);
void addExamResult(ExamResult *res);


// counseling
void addCounseling(const Counseling *c);
void showCounselings(const char *student_id);
void updateCounseling(const char *student_id, const char *date);
void deleteCounseling(const char *student_id, const char *date);
void listAllCounselings();
// video resources
void addVideoResource(const VideoResource *v);
void showVideoResources(const char *course_code);
void updateVideoResource(const char *course_code, const char *title);
void deleteVideoResource(const char *course_code, const char *title);
// Pdf & Book Resource
void addPdfResource(const PDFResource *p);
void showPdfResources(const char *course_code);
void updatePdfResource(const char *course_code, const char *title);
void deletePdfResource(const char *course_code, const char *title);
// Webpage Resource
void addWebpageResource(const WebpageResource *w);
void showWebpageResources(const char *course_code);
void updateWebpageResource(const char *course_code, const char *title);
void deleteWebpageResource(const char *course_code, const char *title);
//menu
void adminMenu();
void teacherMenu();
void studentMenu();
void clearInputBuffer();


//Logo

void print_exam_bunny_menu(void)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    puts("                     (\\__/)");
    puts("                     (> . <)          ");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("                    ExamBunny\n");

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    puts("                     /     \\  ");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    puts("              Check. Learn. Repeat.                  ");

    puts("   ");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}


// Buffer clear function
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void trimNewline(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
}

void getPassword(char *password, int maxLength) {
    int index = 0;
    char ch;

    while (1) {
        ch = getch(); 

        if (ch == 13) { 
            password[index] = '\0';
            printf("\n");
            break;
        } 
        else if (ch == 8) {
            if (index > 0) {
                index--;
                printf("\b \b"); 
            }
        } 
        else if (index < maxLength - 1) {
            password[index++] = ch;
            printf("*"); 
        }
    }
}

//Registration and log in functions

// admin registration function
void registerAdmin()
{
    admin a;

    printf("Enter Admin ID: ");
    scanf("%19s", a.admin_id);
    printf("Enter Name: ");
    scanf(" %[^\n]", a.name);
    printf("Enter Password: ");
getPassword(a.password, 50);
    printf("Enter Role: ");
    scanf(" %[^\n]", a.admin_role);

    FILE *fp = fopen(ADMIN_FILE, "a");
    if (!fp)
    {
        perror("Error opening admin file");
        return;
    }
    fprintf(fp, "%s %s %s|%s\n", a.admin_id, a.name, a.password, a.admin_role);
    fclose(fp);

    printf("Admin registered successfully!\n");
}
// ADMIN login function
int loginAdmin()
{
    char id[20], password[50];
    printf("Enter Admin ID: ");
    scanf("%19s", id);
    printf("Enter Password: ");
getPassword(password, 50);


    id[strcspn(id, "\n")] = 0;
    password[strcspn(password, "\n")] = 0;

    FILE *fp = fopen(ADMIN_FILE, "r");
    if (!fp)
    {
        perror("Error opening admin file");
        return 0;
    }

    admin a;
    char line[200];
    int found = 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        for (int i = 0; line[i]; i++)
        {
            if (line[i] == '|')
            {
                line[i] = ' ';
            }
        }

        if (sscanf(line, "%19s %49s %49s %9s", a.admin_id, a.name, a.password, a.admin_role) == 4)
        {
            if (strcmp(id, a.admin_id) == 0 && strcmp(password, a.password) == 0)
            {
                found = 1;
                break;
            }
        }
    }

    fclose(fp);

    if (found)
    {
        printf(" Admin login successful. Welcome, %s! Role: %s\n", a.name, a.admin_role);
        return 1;
    }
    else
    {
        printf("Login failed. Invalid ID or password.\n");
        return 0;
    }
}


// Student registration function
void registerUser()
{
    user u;

    printf("Enter Student ID: ");
    scanf("%19s", u.student_id);
    printf("Enter Name: ");
    scanf(" %[^\n]", u.name);
    printf("Enter Password: ");
getPassword(u.password, 50);


    FILE *fp = fopen(USER_FILE, "a");
    if (!fp)
    {
        perror("Error opening user file");
        return;
    }
    fprintf(fp, "%s %s %s\n", u.student_id, u.name, u.password);
    fclose(fp);

    printf("Student registered successfully!\n");
}

int loginUser() {
    char id[20], password[50];
    printf("Enter User ID: ");
    scanf("%19s", id);
    getchar(); // newline consume

    printf("Enter Password: ");
    getPassword(password, 50);  // mask password input

    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) { perror("Error opening user file"); return 0; }

    user u;
    int found = 0;
    while (fscanf(fp, "%s %s %s\n", u.student_id, u.name, u.password) != EOF) {
        if (strcmp(id, u.student_id) == 0 && strcmp(password, u.password) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found) {
        printf("User login successful. Welcome, %s!\n", u.name);
        strcpy(loggedInStudentID, u.student_id);
        return 1;
    } else {
        printf("Login failed. Invalid ID or password.\n");
        return 0;
    }
}



// Teacher registration function
void registerTeacher()
{
    teacher t;

    printf("Enter Teacher ID: ");
    scanf("%19s", t.teacher_id);

    FILE *fp;
    char teacher_id2[20];
    int found = 0;
    fp = fopen("teacherData.txt", "r");
    int index = 0;
    while (fscanf(fp, "%s %s %s %s", teacher_id2, t.name, t.password, t.subject) != EOF)
    {
        if (strcmp(t.teacher_id, teacher_id2) == 0)
        {
            found = 1;
            break;
        }
        index = index + 1;
    }
    fclose(fp);

    if (found)
    {
        printf("Teacher with ID %s exists. Input Info....\n", t.teacher_id);
        printf("Enter Name: ");
        scanf(" %[^\n]", t.name);
        printf("Enter Password: ");
getPassword(t.password, 50);

        printf("Enter Subject: ");
        scanf(" %[^\n]", t.subject);

        fp = fopen(TEACHER_FILE, "a");
        if (!fp)
        {
            perror("Error opening teacher file");
            return;
        }

        fprintf(fp, "%s|%s|%s|%s\n", t.teacher_id, t.name, t.password, t.subject);
        fclose(fp);

        printf("Teacher registered successfully!\n");
        return;
    }
    else
    {
        printf("Teacher with ID %s does not exist.\n", teacher_id2);
    }
}
// Teacher login function
int loginTeacher()
{
    char id[20], password[50];
    printf("Enter Teacher ID: ");
    scanf("%19s", id);
   printf("Enter Password: ");
getPassword(password, 50);


    FILE *fp = fopen(TEACHER_FILE, "r");
    if (!fp)
    {
        perror("Error opening teacher file");
        return 0; // failed login
    }

    teacher t;
    int found = 0;
    while (fscanf(fp, "%19[^|]|%49[^|]|%49[^|]|%29[^\n]\n", t.teacher_id, t.name, t.password, t.subject) == 4)
    {
        if (strcmp(id, t.teacher_id) == 0 && strcmp(password, t.password) == 0)
        {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found)
    {
        printf(" Teacher login successful. Welcome, %s!\n", t.name);
        return 1; // success
    }
    else
    {
        printf(" Login failed. Invalid ID or password.\n");
        return 0; // fail
    }
}



//Admin access

// Update user function
void updateUser(user *u)
{
    system("cls");

    if (u == NULL)
    {
        printf("Invalid user pointer.\n");
        return;
    }

    FILE *fp = fopen(USER_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp)
    {
        perror("File error");
        if (fp)
            fclose(fp);
        if (temp)
            fclose(temp);
        return;
    }

    user tempUser;
    int found = 0;

    while (fscanf(fp, "%19[^|]|%49[^|]|%49[^|]|%9[^\n]\n", tempUser.student_id, tempUser.name, tempUser.password, tempUser.student_role) == 4)
    {
        if (strcmp(tempUser.student_id, u->student_id) == 0)
        {
            found = 1;
            // Write updated user info passed in 'u'
            fprintf(temp, "%s|%s|%s|%s\n", u->student_id, u->name, u->password, u->student_role);
        }
        else
        {
            // Write existing user info as-is
            fprintf(temp, "%s|%s|%s|%s\n", tempUser.student_id, tempUser.name, tempUser.password, tempUser.student_role);
        }
    }

    fclose(fp);
    fclose(temp);

    if (!found)
    {
        printf("User  with ID %s not found.\n", u->student_id);
        remove("temp.txt");
        return;
    }

    remove(USER_FILE);
    rename("temp.txt", USER_FILE);
    printf("User  updated successfully.\n");
}
// Delete user function
void deleteUser(user *u)
{
    system("cls");


    if (u == NULL)
    {
        printf("Invalid user pointer.\n");
        return;
    }

    FILE *fp = fopen(USER_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp)
    {
        perror("File error");
        if (fp)
            fclose(fp);
        if (temp)
            fclose(temp);
        return;
    }

    user tempUser;
    int found = 0;

    while (fscanf(fp, "%19[^|]|%49[^|]|%49[^|]|%9[^\n]\n", tempUser.student_id, tempUser.name, tempUser.password, tempUser.student_role) == 4)
    {
        if (strcmp(tempUser.student_id, u->student_id) == 0)
        {
            found = 1;
            // skip writing this user to delete
            continue;
        }
        fprintf(temp, "%s|%s|%s|%s\n", tempUser.student_id, tempUser.name, tempUser.password, tempUser.student_role);
    }

    fclose(fp);
    fclose(temp);

    if (!found)
    {
        printf("User  with ID %s not found.\n", u->student_id);
        remove("temp.txt");
        return;
    }

    remove(USER_FILE);
    rename("temp.txt", USER_FILE);
    printf("User  deleted successfully.\n");
}
//view teacher information(Admin)
void viewTeachers()
{
    system("cls");

    FILE *fp = fopen("teacher.txt", "r");
    if (!fp)
    {
        printf("No teacher data found.\n");
        return;
    }

    teacher t;
    char line[200];

    printf("=== Teacher Information ===\n");
    printf("----------------------------\n");

    while (fgets(line, sizeof(line), fp))
    {
        trimNewline(line);

        
        if (sscanf(line, "%19[^|]|%49[^|]|%49[^|]|%29s", t.teacher_id, t.name, t.password, t.subject) == 4)
        {
            printf("Teacher ID : %s\n", t.teacher_id);
            printf("Name       : %s\n", t.name);
            printf("Password   : %s\n", t.password);
            printf("Subject    : %s\n", t.subject);
            printf("----------------------------\n");
        }
    }
    fclose(fp);
}
//view student information (admin)
void viewStudents()
{
    system("cls");

    FILE *fp = fopen("user.txt", "r");
    if (!fp)
    {
        printf("No student data found.\n");
        return;
    }

    user s;
    char line[256];
    printf("=== Student Information ===\n");
    printf("----------------------------\n");

    while (fgets(line, sizeof(line), fp))
    {
        trimNewline(line);

        int numFields = sscanf(line, "%19s %49s %49s %9s",
                              s.student_id, s.name, s.password, s.student_role);

        if (numFields >= 3)
        {
            if (numFields == 3)
                strcpy(s.student_role, "(empty)");

            printf("Student ID   : %s\n", s.student_id);
            printf("Name         : %s\n", s.name);
            printf("Password     : %s\n", s.password);
            printf("Student Role : %s\n", s.student_role);
            printf("----------------------------\n");
        }
        else
        {
            printf("Invalid line format: %s\n", line);
        }
    }
    fclose(fp);
}
// Update teacher function
void updateTeacher(teacher *t)
{
    system("cls");

    if (t == NULL)
    {
        printf("Invalid teacher pointer.\n");
        return;
    }

    FILE *fp = fopen("teacher.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp)
    {
        perror("File error");
        if (fp)
            fclose(fp);
        if (temp)
            fclose(temp);
        return;
    }

    teacher tempTeacher;
    int found = 0;

    while (fscanf(fp, "%19[^|]|%49[^|]|%49[^|]|%29[^\n]\n", tempTeacher.teacher_id, tempTeacher.name, tempTeacher.password, tempTeacher.subject) == 4)
    {
        if (strcmp(tempTeacher.teacher_id, t->teacher_id) == 0)
        {
            found = 1;
            // Write updated teacher info
            fprintf(temp, "%s|%s|%s|%s\n", t->teacher_id, t->name, t->password, t->subject);
        }
        else
        {
            fprintf(temp, "%s|%s|%s|%s\n", tempTeacher.teacher_id, tempTeacher.name, tempTeacher.password, tempTeacher.subject);
        }
    }

    fclose(fp);
    fclose(temp);

    if (!found)
    {
        printf("Teacher with ID %s not found.\n", t->teacher_id);
        remove("temp.txt");
        return;
    }

    remove("teacher.txt");
    rename("temp.txt", "teacher.txt");

    printf("Teacher updated successfully.\n");
}
// Delete teacher function
void deleteTeacher(teacher *t)
{
    system("cls");

    if (t == NULL) return;

    FILE *fp = fopen("teacher.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp)
    {
        perror("File error");
        if(fp) fclose(fp);
        if(temp) fclose(temp);
        return;
    }

    teacher tempTeacher;
    int found = 0;

    while(fscanf(fp, "%19[^|]|%49[^|]|%49[^|]|%29[^\n]\n",
                tempTeacher.teacher_id,
                tempTeacher.name,
                tempTeacher.password,
                tempTeacher.subject) == 4)
    {
        if(strcmp(tempTeacher.teacher_id, t->teacher_id) == 0)
        {
            found = 1;
            continue; // skip to delete
        }
        fprintf(temp, "%s|%s|%s|%s\n",
                tempTeacher.teacher_id,
                tempTeacher.name,
                tempTeacher.password,
                tempTeacher.subject);
    }

    fclose(fp);
    fclose(temp);

    if(!found)
    {
        printf("Teacher with ID %s not found.\n", t->teacher_id);
        remove("temp.txt");
        return;
    }

    remove("teacher.txt");
    rename("temp.txt", "teacher.txt");
    printf("Teacher deleted successfully.\n");
}

void addTeacher()
{
    system("cls");

    teacher t;

    printf("Enter Teacher ID: ");
    fgets(t.teacher_id, sizeof(t.teacher_id), stdin);
    trimNewline(t.teacher_id);

    printf("Enter Teacher Name: ");
    fgets(t.name, sizeof(t.name), stdin);
    trimNewline(t.name);

    printf("Enter Teacher Password: ");
    fgets(t.password, sizeof(t.password), stdin);
    trimNewline(t.password);

    printf("Enter Teacher Subject: ");
    fgets(t.subject, sizeof(t.subject), stdin);
    trimNewline(t.subject);

    // Open teacher.txt in append mode
    FILE *fp = fopen("teacher.txt", "a");
    if (!fp)
    {
        perror("Error opening teacher file");
        return;
    }

    // Write teacher info in same format
    fprintf(fp, "%s|%s|%s|%s\n", t.teacher_id, t.name, t.password, t.subject);

    fclose(fp);

    printf("Teacher added successfully.\n");
}



//Course Part

// Add Course Function
void addCourse()
{
    system("cls");
    Course c;

    printf("Enter course name: ");
    fgets(c.course_name, sizeof(c.course_name), stdin);
    trimNewline(c.course_name);

    printf("Enter course code: ");
    fgets(c.course_code, sizeof(c.course_code), stdin);
    trimNewline(c.course_code);

    FILE *fp = fopen("course.txt", "a");
    if (!fp)
    {
        perror("File open error");
        return;
    }

    fprintf(fp, "%s,%s\n", c.course_name, c.course_code);
    fclose(fp);

    printf("Course added successfully.\n");
}
// Show Course Function
void showCourses()
{
    system("cls");
    FILE *fp = fopen("course.txt", "r");
    if (!fp)
    {
        printf("No courses found.\n");
        return;
    }

    Course c;
    char line[256];
    printf("Course List:\n");
    printf("---------------\n");

    while (fgets(line, sizeof(line), fp))
    {
        trimNewline(line);
        if (sscanf(line, "%99[^,],%19s", c.course_name, c.course_code) == 2)
        {
            printf("Course Name: %s\n", c.course_name);
            printf("Course Code: %s\n", c.course_code);
            printf("---------------\n");
        }
    }
    fclose(fp);
}
// Update Course Function
void updateCourse()
{
    system("cls");
    char code[20];
    printf("Enter course code to update: ");
    scanf("%19s", code);
    clearInputBuffer();

    FILE *fp = fopen("course.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp)
    {
        perror("File error");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    Course c;
    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), fp))
    {
        trimNewline(line);
        if (sscanf(line, "%99[^,],%19s", c.course_name, c.course_code) == 2)
        {
            if (strcmp(c.course_code, code) == 0)
            {
                found = 1;
                printf("Enter new course name: ");
                fgets(c.course_name, sizeof(c.course_name), stdin);
                trimNewline(c.course_name);

                printf("Enter new course code: ");
                scanf("%19s", c.course_code);
                clearInputBuffer();
            }
            fprintf(temp, "%s,%s\n", c.course_name, c.course_code);
        }
    }

    fclose(fp);
    fclose(temp);

    if (!found)
    {
        printf("Course with code %s not found.\n", code);
        remove("temp.txt");
        return;
    }

    remove("course.txt");
    rename("temp.txt", "course.txt");
    printf("Course updated successfully.\n");
}
// Delete Course Function
void deleteCourse()
{
    system("cls");

    char code[20];
    printf("Enter course code to delete: ");
    scanf("%19s", code);
    clearInputBuffer();

    FILE *fp = fopen("course.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp)
    {
        perror("File error");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    Course c;
    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), fp))
    {
        trimNewline(line);

        if (sscanf(line, "%99[^,],%19s", c.course_name, c.course_code) == 2)
        {
            if (strcmp(c.course_code, code) == 0)
            {
                found = 1;
                
                continue;
            }
            fprintf(temp, "%s,%s\n", c.course_name, c.course_code);
        }
    }

    fclose(fp);
    fclose(temp);

    if (!found)
    {
        printf("Course with code %s not found.\n", code);
        remove("temp.txt");
        return;
    }

    remove("course.txt");
    rename("temp.txt", "course.txt");
    printf("Course deleted successfully.\n");
}



//Question part

// Add Question Function
void addQuestion()
{
    system("cls");

    MCQ q;
    printf("Enter course code: ");
    scanf("%19s", q.course_code);
    clearInputBuffer();

    printf("Enter question: ");
    fgets(q.question, sizeof(q.question), stdin);
    q.question[strcspn(q.question, "\n")] = '\0';

    printf("Enter option A: ");
    fgets(q.option_a, sizeof(q.option_a), stdin);
    q.option_a[strcspn(q.option_a, "\n")] = '\0';

    printf("Enter option B: ");
    fgets(q.option_b, sizeof(q.option_b), stdin);
    q.option_b[strcspn(q.option_b, "\n")] = '\0';

    printf("Enter option C: ");
    fgets(q.option_c, sizeof(q.option_c), stdin);
    q.option_c[strcspn(q.option_c, "\n")] = '\0';

    printf("Enter option D: ");
    fgets(q.option_d, sizeof(q.option_d), stdin);
    q.option_d[strcspn(q.option_d, "\n")] = '\0';

    printf("Enter correct answer (a/b/c/d): ");
    scanf(" %c", &q.check);
    q.check = tolower(q.check);

    FILE *fp = fopen("mcq.txt", "a");
    if (!fp)
    {
        perror("File open error");
        return;
    }

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%c\n",
            q.course_code, q.question,
            q.option_a, q.option_b,
            q.option_c, q.option_d,
            q.check);

    fflush(fp);
    fclose(fp);

    printf("Question added successfully.\n");
}
void trim(char *str)
{
    char *start = str;
    while (*start == ' ')
        start++; // leading spaces
    char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ')
    {
        *end = '\0';
        end--;
    }
    if (start != str)
    {
        memmove(str, start, strlen(start) + 1);
    }
}
// Update Question Function
void updateQuestion()
{
    system("cls");

    FILE *fp = fopen("mcq.txt", "r");
    if (!fp)
    {
        perror("File open error");
        return;
    }

    MCQ questions[500];
    int count = 0;

    // Read all questions
    while (fscanf(fp, "%19[^,],%299[^,],%99[^,],%99[^,],%99[^,],%99[^,],%c\n",
                  questions[count].course_code,
                  questions[count].question,
                  questions[count].option_a,
                  questions[count].option_b,
                  questions[count].option_c,
                  questions[count].option_d,
                  &questions[count].check) == 7)
    {
        trim(questions[count].course_code);
        trim(questions[count].question);
        trim(questions[count].option_a);
        trim(questions[count].option_b);
        trim(questions[count].option_c);
        trim(questions[count].option_d);
        count++;
    }
    fclose(fp);

    if (count == 0)
    {
        printf("No questions found.\n");
        return;
    }

    // Show list with serial numbers
    for (int i = 0; i < count; i++)
    {
        printf("%d) %s\n", i + 1, questions[i].question);
    }

    int serial;
    printf("Enter serial number of question to update: ");
    scanf("%d", &serial);
    clearInputBuffer();

    if (serial < 1 || serial > count)
    {
        printf("Invalid serial number.\n");
        return;
    }

    int idx = serial - 1; // array index

    printf("Enter new question: ");
    fgets(questions[idx].question, sizeof(questions[idx].question), stdin);
    questions[idx].question[strcspn(questions[idx].question, "\n")] = '\0';
    trim(questions[idx].question);

    printf("Enter option A: ");
    fgets(questions[idx].option_a, sizeof(questions[idx].option_a), stdin);
    questions[idx].option_a[strcspn(questions[idx].option_a, "\n")] = '\0';
    trim(questions[idx].option_a);

    printf("Enter option B: ");
    fgets(questions[idx].option_b, sizeof(questions[idx].option_b), stdin);
    questions[idx].option_b[strcspn(questions[idx].option_b, "\n")] = '\0';
    trim(questions[idx].option_b);

    printf("Enter option C: ");
    fgets(questions[idx].option_c, sizeof(questions[idx].option_c), stdin);
    questions[idx].option_c[strcspn(questions[idx].option_c, "\n")] = '\0';
    trim(questions[idx].option_c);

    printf("Enter option D: ");
    fgets(questions[idx].option_d, sizeof(questions[idx].option_d), stdin);
    questions[idx].option_d[strcspn(questions[idx].option_d, "\n")] = '\0';
    trim(questions[idx].option_d);

    printf("Enter correct answer (a/b/c/d): ");
    scanf(" %c", &questions[idx].check);
    questions[idx].check = tolower(questions[idx].check);

    // Write all questions back
    fp = fopen("mcq.txt", "w");
    if (!fp)
    {
        perror("File open error");
        return;
    }
    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "%s,%s,%s,%s,%s,%s,%c\n",
                questions[i].course_code,
                questions[i].question,
                questions[i].option_a,
                questions[i].option_b,
                questions[i].option_c,
                questions[i].option_d,
                questions[i].check);
    }
    fclose(fp);

    printf("Question updated successfully.\n");
}

// Delete Question Function
void deleteQuestion()
{
    system("cls");

    FILE *fp = fopen("mcq.txt", "r");
    if (!fp)
    {
        perror("File open error");
        return;
    }

    MCQ questions[500];
    int count = 0;

    while (fscanf(fp, "%19[^,],%299[^,],%99[^,],%99[^,],%99[^,],%99[^,],%c\n",
                  questions[count].course_code,
                  questions[count].question,
                  questions[count].option_a,
                  questions[count].option_b,
                  questions[count].option_c,
                  questions[count].option_d,
                  &questions[count].check) == 7)
    {
        trim(questions[count].course_code);
        trim(questions[count].question);
        trim(questions[count].option_a);
        trim(questions[count].option_b);
        trim(questions[count].option_c);
        trim(questions[count].option_d);
        count++;
    }
    fclose(fp);

    if (count == 0)
    {
        printf("No questions found.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        printf("%d) %s\n", i + 1, questions[i].question);
    }

    int serial;
    printf("Enter serial number of question to delete: ");
    scanf("%d", &serial);
    clearInputBuffer();

    if (serial < 1 || serial > count)
    {
        printf("Invalid serial number.\n");
        return;
    }

    int idx = serial - 1; 

    for (int i = idx; i < count - 1; i++)
    {
        questions[i] = questions[i + 1];
    }
    count--; 

    fp = fopen("mcq.txt", "w");
    if (!fp)
    {
        perror("File open error");
        return;
    }
    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "%s,%s,%s,%s,%s,%s,%c\n",
                questions[i].course_code,
                questions[i].question,
                questions[i].option_a,
                questions[i].option_b,
                questions[i].option_c,
                questions[i].option_d,
                questions[i].check);
    }
    fclose(fp);

    printf("Question deleted successfully!\n");
}

// Show Questions for Admin Function
void showQuestionsForAdmin()
{
    system("cls");

    FILE *fp = fopen("mcq.txt", "r");
    if (!fp)
    {
        printf("No questions found.\n");
        return;
    }

    MCQ q;
    printf("MCQ Questions with answers (Admin view):\n");
    printf("---------------------------------------\n");
    while (fscanf(fp, "%19[^,],%299[^,],%99[^,],%99[^,],%99[^,],%99[^,], %c\n",
                  q.course_code, q.question,
                  q.option_a, q.option_b,
                  q.option_c, q.option_d, &q.check) == 7)
    {

        trim(q.course_code);
        trim(q.question);
        trim(q.option_a);
        trim(q.option_b);
        trim(q.option_c);
        trim(q.option_d);

        printf("Course Code: %s\nQuestion: %s\n", q.course_code, q.question);
        printf("A) %s\nB) %s\nC) %s\nD) %s\n", q.option_a, q.option_b, q.option_c, q.option_d);
        printf("Correct Answer: %c\n", q.check);
        printf("---------------------------------------\n");
    }
    fclose(fp);
}
// Show Questions for Teacher Function
void showQuestionsForTeacher(const char *course_code)
{
    system("cls");

    FILE *fp = fopen("mcq.txt", "r");
    if (!fp)
    {
        printf("No questions available.\n");
        return;
    }

    MCQ q;
    int found = 0;

    printf("Questions for course %s:\n\n", course_code);
    while (fscanf(fp, "%19[^,],%299[^,],%99[^,],%99[^,],%99[^,],%99[^,], %c\n",
                  q.course_code, q.question, q.option_a, q.option_b, q.option_c, q.option_d, &q.check) == 7)
    {

        trim(q.course_code);
        trim(q.question);
        trim(q.option_a);
        trim(q.option_b);
        trim(q.option_c);
        trim(q.option_d);

        if (strcmp(q.course_code, course_code) == 0)
        {
            found = 1;
            printf("Q: %s\n", q.question);
            printf("A) %s\n", q.option_a);
            printf("B) %s\n", q.option_b);
            printf("C) %s\n", q.option_c);
            printf("D) %s\n", q.option_d);
            printf("Correct answer: %c\n", q.check);
            printf("------------------------------\n");
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("No questions found for this course.\n");
    }
}


// Exam Function

// Load questions from file
int loadQuestions(const char *course_code, MCQ questions[])
{
    FILE *fp = fopen(MCQ_FILE, "r");
    if (!fp) return 0;
    int count = 0;
    MCQ temp;
    char line[512];
    while (fgets(line, sizeof(line), fp))
    {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%c",
               temp.course_code, temp.question,
               temp.option_a, temp.option_b, temp.option_c, temp.option_d, &temp.check);
        if (strcmp(temp.course_code, course_code) == 0)
            questions[count++] = temp;
    }
    fclose(fp);
    return count;
}

// Save exam result to file
void addExamResult(ExamResult *res)
{
    FILE *fp = fopen(EXAM_RESULT_FILE, "a");
    if (!fp)
    {
        printf("Error opening exam result file.\n");
        return;
    }
    fprintf(fp, "%s|%s|%d\n", res->student_id, res->course_code, res->score);
    fclose(fp);
    printf("Exam result saved.\n");
}

// Student Exam function with countdown timer
void studentExam(const char *student_id, const char *course_code, int examDurationMinutes)
{
    system("cls");

    int totalSeconds = examDurationMinutes * 60;
    int score = 0;

    MCQ questions[50];
    int totalQuestions = loadQuestions(course_code, questions);
    if (totalQuestions == 0)
    {
        printf("No questions found for this course.\n");
        return;
    }

    printf("Starting exam for course: %s\n", course_code);
    printf("Total questions: %d\n", totalQuestions);

    for (int i = 0; i < totalQuestions; i++)
    {
        if (totalSeconds <= 0) break;

        printf("\nQ%d: %s\n", i + 1, questions[i].question);
        printf("a) %s\nb) %s\nc) %s\nd) %s\n", 
               questions[i].option_a, questions[i].option_b, 
               questions[i].option_c, questions[i].option_d);

        char ans = 0;
        time_t startTime = time(NULL);

        while (1)
        {
            // Show remaining time
            int minutes = totalSeconds / 60;
            int seconds = totalSeconds % 60;
            printf("\rTime Remaining: %02d:%02d  ", minutes, seconds);
            fflush(stdout);

            if (kbhit())
            {
                ans = getch();
                printf("%c\n", ans);
                break;
            }

            Sleep(1000); // wait 1 second
            time_t now = time(NULL);
            totalSeconds -= (int)(now - startTime);
            startTime = now;

            if (totalSeconds <= 0)
            {
                printf("\nTime is up! Exam ended.\n");
                goto endExam;
            }
        }

        ans = tolower(ans);
        if (ans == questions[i].check) score++;
    }

endExam:
    printf("\nYour score: %d/%d\n", score, totalQuestions);

    // Save result
    ExamResult res;
    strcpy(res.student_id, student_id);
    strcpy(res.course_code, course_code);
    res.score = score;

    addExamResult(&res);  // make sure addExamResult() is a separate function
}

//how only student's previous results 
void showStudentResults(const char *student_id)
{
    system("cls");

    FILE *fp = fopen(EXAM_RESULT_FILE, "r");
    if (!fp) {
        printf("No exam results found.\n");
        return;
    }

    ExamResult res;
    int found = 0;
    printf("\nPrevious Exam Results for Student ID: %s\n", student_id);
    printf("-----------------------------------\n");
    while (fscanf(fp, "%19[^|]|%19[^|]|%d\n", res.student_id, res.course_code, &res.score) == 3)
    {
        if (strcmp(res.student_id, student_id) == 0)
        {
            found = 1;
            printf("Course: %s | Score: %d\n", res.course_code, res.score);
        }
    }
    fclose(fp);

    if (!found)
        printf("No previous exams found.\n");
}

//Show all results (admin/teacher)
void showAllResults(const char *student_id)
{
    system("cls");

    FILE *fp = fopen(EXAM_RESULT_FILE, "r");
    if (!fp)
    {
        printf("No exam results found.\n");
        return;
    }

    ExamResult result;
    printf("\n=== Exam Results ===\n");
    while (fscanf(fp, "%19[^|]|%19[^|]|%d\n",
                  result.student_id, result.course_code, &result.score) == 3)
    {
        if (student_id[0] == '\0' || strcmp(result.student_id, student_id) == 0)
        {
            printf("Student ID: %s | Course: %s | Score: %d\n",
                   result.student_id, result.course_code, result.score);
        }
    }

    fclose(fp);
}

//rules
void examRules()
{
    system("cls");

    printf("Exam Rules:\n");
    printf("1. No preview of questions before exam.\n");
    printf("2. Answer all questions carefully.\n");
    printf("3. No cheating allowed.\n");
    printf("4. Exam will be timed (if applicable).\n");
    printf("5. Once exam is finished, your answers will be submitted automatically.\n");
    printf("6. You cannot go back and change answers after submission.\n");
    printf("7. Review your score and feedback (if provided) after the exam.\n");
    printf("8. Contact your teacher or admin for any discrepancies in results.\n");
    printf("9. Ensure you have a stable internet connection (if online).\n");
    printf("10. Take a screenshot or note your score for your own record.\n");
    printf("11. Good luck!\n");
}




//Counseling
int getNextRecordID() {
    FILE *fp = fopen(COUNSELING_FILE, "r");
    if (!fp) return 1; // First record

    Counseling c;
    int maxID = 0;

    while (fscanf(fp, "%d %19s %19s %19s %99s %299[^\n]",
                  &c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes) == 6) {
        if (c.record_id > maxID)
            maxID = c.record_id;
    }
    fclose(fp);
    return maxID + 1;
}

//  Add Counseling 
void addCounseling(const Counseling *c) {
    system("cls");

    FILE *fp = fopen(COUNSELING_FILE, "a");
    if (!fp) {
        printf("Error opening file for writing.\n");
        return;
    }
    fprintf(fp, "%d %s %s %s %s %s\n",
            c->record_id, c->student_id, c->teacher_id, c->date, c->topic, c->notes);
    fclose(fp);
    printf("Counseling record added successfully.\n");
}

//  Show Counseling for a Student 
void showCounselings(const char *student_id) {
    system("cls");
    FILE *fp = fopen(COUNSELING_FILE, "r");
    if (!fp) {
        printf("Error opening file for reading.\n");
        return;
    }

    Counseling c;
    int found = 0;
    printf("Counseling records for student ID: %s\n", student_id);

    while (fscanf(fp, "%d %19s %19s %19s %99s %299[^\n]",
                  &c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes) == 6) {
        if (strcmp(c.student_id, student_id) == 0) {
            found = 1;
            printf("\nRecord ID: %d\nTeacher ID: %s\nDate: %s\nTopic: %s\nNotes: %s\n",
                   c.record_id, c.teacher_id, c.date, c.topic, c.notes);
        }
    }
    if (!found) {
        printf("No counseling records found for this student.\n");
    }
    fclose(fp);
}

//  Update Counseling 
void updateCounseling(const char *student_id, const char *date) {
    system("cls");

    FILE *fp = fopen(COUNSELING_FILE, "r");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    Counseling c;
    int found = 0;

    while (fscanf(fp, "%d %19s %19s %19s %99s %299[^\n]",
                  &c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes) == 6) {
        if (strcmp(c.student_id, student_id) == 0 && strcmp(c.date, date) == 0) {
            found = 1;
            printf("Updating counseling record (Record ID: %d)\n", c.record_id);

            printf("Enter new topic: ");
            getchar();
            fgets(c.topic, sizeof(c.topic), stdin);
            trimNewline(c.topic);

            printf("Enter new notes: ");
            fgets(c.notes, sizeof(c.notes), stdin);
            trimNewline(c.notes);
        }
        fprintf(temp, "%d %s %s %s %s %s\n",
                c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(COUNSELING_FILE);
        rename("temp.txt", COUNSELING_FILE);
        printf("Counseling record updated successfully.\n");
    } else {
        remove("temp.txt");
        printf("Record not found for update.\n");
    }
}

// Delete Counseling 
void deleteCounseling(const char *student_id, const char *date) {
    system("cls");

    FILE *fp = fopen(COUNSELING_FILE, "r");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    Counseling c;
    int found = 0;

    while (fscanf(fp, "%d %19s %19s %19s %99s %299[^\n]",
                  &c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes) == 6) {
        if (strcmp(c.student_id, student_id) == 0 && strcmp(c.date, date) == 0) {
            found = 1;
        } else {
            fprintf(temp, "%d %s %s %s %s %s\n",
                    c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(COUNSELING_FILE);
        rename("temp.txt", COUNSELING_FILE);
        printf("Counseling record deleted successfully.\n");
    } else {
        remove("temp.txt");
        printf("Record not found for deletion.\n");
    }
}

// List All Counseling
void listAllCounselings() {
    system("cls");

    FILE *fp = fopen(COUNSELING_FILE, "r");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    Counseling c;
    int count = 0;
    printf("All Counseling Records:\n");

    while (fscanf(fp, "%d %19s %19s %19s %99s %299[^\n]",
                  &c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes) == 6) {
        count++;
        printf("\nRecord ID: %d\nStudent ID: %s\nTeacher ID: %s\nDate: %s\nTopic: %s\nNotes: %s\n",
               c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes);
    }
    if (count == 0) {
        printf("No counseling records found.\n");
    }
    fclose(fp);
}



//Resourses
// Add Video Resource
void addVideoResource(const VideoResource *v)
{
    system("cls");

    FILE *fp = fopen(VIDEO_FILE, "a");
    if (!fp)
    {
        printf("Error opening video resource file.\n");
        return;
    }
    fprintf(fp, "%s %s %s\n", v->title, v->course_code, v->url);
    fclose(fp);
    printf("Video resource added successfully.\n");
}
// SHOW VIDEO RESOURCES
void showVideoResources(const char *course_code)
{
    system("cls");

    FILE *fp = fopen(VIDEO_FILE, "r");
    if (!fp)
    {
        printf("No video resources found.\n");
        return;
    }
    VideoResource v;
    int found = 0;

    printf("Video Resources for course %s:\n--------------------------\n", course_code);
    while (fscanf(fp, "%99s %19s %199s\n", v.title, v.course_code, v.url) == 3)
    {
        trim(v.course_code);
        if (strcmp(v.course_code, course_code) == 0)
        {
            found = 1;
            printf("Title: %s\nURL: %s\n--------------------------\n", v.title, v.url);
        }
    }
    fclose(fp);
    if (!found)
    {
        printf("No video resources found for this course.\n");
    }
}
// UPDATE VIDEO RESOURCE
void updateVideoResource(const char *course_code, const char *title)
{
    system("cls");

    FILE *fp = fopen(VIDEO_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp)
    {
        printf("Error opening files.\n");
        if (fp)
            fclose(fp);
        if (temp)
            fclose(temp);
        return;
    }
    VideoResource v;
    int updated = 0;

    while (fscanf(fp, "%99s %19s %199s\n", v.title, v.course_code, v.url) == 3)
    {
        trim(v.course_code);
        if (strcmp(v.course_code, course_code) == 0 && strcmp(v.title, title) == 0)
        {
            updated = 1;
            printf("Current URL: %s\nEnter new URL (press Enter to keep current): ", v.url);
            char newUrl[200];
            fgets(newUrl, sizeof(newUrl), stdin);
            newUrl[strcspn(newUrl, "\n")] = '\0';
            if (strlen(newUrl) > 0)
            {
                strcpy(v.url, newUrl);
            }
        }
        fprintf(temp, "%s %s %s\n", v.title, v.course_code, v.url);
    }
    fclose(fp);
    fclose(temp);

    if (updated)
    {
        remove(VIDEO_FILE);
        rename("temp.txt", VIDEO_FILE);
        printf("Video resource updated successfully.\n");
    }
    else
    {
        remove("temp.txt");
        printf("No matching video resource found.\n");
    }
}
// DELETE VIDEO RESOURCE
void deleteVideoResource(const char *course_code, const char *title)
{
    system("cls");

    FILE *fp = fopen(VIDEO_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp)
    {
        printf("Error opening files.\n");
        if (fp)
            fclose(fp);
        if (temp)
            fclose(temp);
        return;
    }
    VideoResource v;
    int found = 0;

    while (fscanf(fp, "%99s %19s %199s\n", v.title, v.course_code, v.url) == 3)
    {
        trim(v.course_code);
        if (strcmp(v.course_code, course_code) == 0 && strcmp(v.title, title) == 0)
        {
            found = 1;
            continue; // skip writing this record to delete it
        }
        fprintf(temp, "%s %s %s\n", v.title, v.course_code, v.url);
    }
    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove(VIDEO_FILE);
        rename("temp.txt", VIDEO_FILE);
        printf("Video resource deleted successfully.\n");
    }
    else
    {
        remove("temp.txt");
        printf("No matching video resource found.\n");
    }
}


// Add PDF Resource
void addPDFResource(const PDFResource *p)
{
    system("cls");

    FILE *fp = fopen(PDF_FILE, "a");
    if (!fp)
    {
        printf("Error opening PDF resource file.\n");
        return;
    }
    fprintf(fp, "%s %s %s\n", p->title, p->course_code, p->url);
    fclose(fp);
    printf("PDF resource added successfully.\n");
}
// Show PDF Resources
void showPDFResources(const char *course_code)
{
    system("cls");

    FILE *fp = fopen(PDF_FILE, "r");
    if (!fp)
    {
        printf("No PDF resources found.\n");
        return;
    }
    PDFResource p;
    int found = 0;

    printf("PDF Resources for course %s:\n--------------------------\n", course_code);
    while (fscanf(fp, "%99s %19s %199s\n", p.title, p.course_code, p.url) == 3)
    {
        trim(p.course_code);
        if (strcmp(p.course_code, course_code) == 0)
        {
            found = 1;
            printf("Title: %s\nURL: %s\n--------------------------\n", p.title, p.url);
        }
    }
    fclose(fp);
    if (!found)
    {
        printf("No PDF resources found for this course.\n");
    }
}
// UPDATE PDF RESOURCE
void updatePDFResource(const char *course_code, const char *title)
{
    system("cls");

    FILE *fp = fopen(PDF_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp)
    {
        printf("Error opening files.\n");
        if (fp)
            fclose(fp);
        if (temp)
            fclose(temp);
        return;
    }
    PDFResource p;
    int updated = 0;

    while (fscanf(fp, "%99s %19s %199s\n", p.title, p.course_code, p.url) == 3)
    {
        trim(p.course_code);
        if (strcmp(p.course_code, course_code) == 0 && strcmp(p.title, title) == 0)
        {
            updated = 1;
            printf("Current URL: %s\nEnter new URL (press Enter to keep current): ", p.url);
            char newUrl[200];
            fgets(newUrl, sizeof(newUrl), stdin);
            newUrl[strcspn(newUrl, "\n")] = '\0';
            if (strlen(newUrl) > 0)
            {
                strcpy(p.url, newUrl);
            }
        }
        fprintf(temp, "%s %s %s\n", p.title, p.course_code, p.url);
    }
    fclose(fp);
    fclose(temp);

    if (updated)
    {
        remove(PDF_FILE);
        rename("temp.txt", PDF_FILE);
        printf("PDF resource updated successfully.\n");
    }
    else
    {
        remove("temp.txt");
        printf("No matching PDF resource found.\n");
    }
}
// DELETE PDF RESOURCE
void deletePDFResource(const char *course_code, const char *title)
{
    system("cls");

    FILE *fp = fopen(PDF_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp)
    {
        printf("Error opening files.\n");
        if (fp)
            fclose(fp);
        if (temp)
            fclose(temp);
        return;
    }
    PDFResource p;
    int found = 0;

    while (fscanf(fp, "%99s %19s %199s\n", p.title, p.course_code, p.url) == 3)
    {
        trim(p.course_code);
        if (strcmp(p.course_code, course_code) == 0 && strcmp(p.title, title) == 0)
        {
            found = 1;
            continue; // skip writing this record to delete
        }
        fprintf(temp, "%s %s %s\n", p.title, p.course_code, p.url);
    }
    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove(PDF_FILE);
        rename("temp.txt", PDF_FILE);
        printf("PDF resource deleted successfully.\n");
    }
    else
    {
        remove("temp.txt");
        printf("No matching PDF resource found.\n");
    }
}


// Add Webpage Resource
void addWebpageResource(const WebpageResource *w)
{
    system("cls");

    FILE *fp = fopen(WEBPAGE_FILE, "a");
    if (!fp)
    {
        printf("Error opening webpage resource file.\n");
        return;
    }
    fprintf(fp, "%s %s %s\n", w->title, w->course_code, w->url);
    fclose(fp);
    printf("Webpage resource added successfully.\n");
}
// Show Webpage Resources
void showWebpageResources(const char *course_code)
{
    system("cls");

    FILE *fp = fopen(WEBPAGE_FILE, "r");
    if (!fp)
    {
        printf("No webpage resources found.\n");
        return;
    }
    WebpageResource w;
    int found = 0;

    printf("Webpage Resources for course %s:\n--------------------------\n", course_code);
    while (fscanf(fp, "%99s %19s %199s\n", w.title, w.course_code, w.url) == 3)
    {
        trim(w.course_code);
        if (strcmp(w.course_code, course_code) == 0)
        {
            found = 1;
            printf("Title: %s\nURL: %s\n--------------------------\n", w.title, w.url);
        }
    }
    fclose(fp);
    if (!found)
    {
        printf("No webpage resources found for this course.\n");
    }
}
// Update Webpage Resource
void updateWebpageResource(const char *course_code, const char *title)
{
    system("cls");

    FILE *fp = fopen(WEBPAGE_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp)
    {
        printf("Error opening files.\n");
        if (fp)
            fclose(fp);
        if (temp)
            fclose(temp);
        return;
    }
    WebpageResource w;
    int updated = 0;

    while (fscanf(fp, "%99s %19s %199s\n", w.title, w.course_code, w.url) == 3)
    {
        trim(w.course_code);
        if (strcmp(w.course_code, course_code) == 0 && strcmp(w.title, title) == 0)
        {
            updated = 1;
            printf("Current URL: %s\nEnter new URL (press Enter to keep current): ", w.url);
            char newUrl[200];
            fgets(newUrl, sizeof(newUrl), stdin);
            newUrl[strcspn(newUrl, "\n")] = '\0';
            if (strlen(newUrl) > 0)
            {
                strcpy(w.url, newUrl);
            }
        }
        fprintf(temp, "%s %s %s\n", w.title, w.course_code, w.url);
    }
    fclose(fp);
    fclose(temp);

    if (updated)
    {
        remove(WEBPAGE_FILE);
        rename("temp.txt", WEBPAGE_FILE);
        printf("Webpage resource updated successfully.\n");
    }
    else
    {
        remove("temp.txt");
        printf("No matching webpage resource found.\n");
    }
}
// DELETE WEBPAGE RESOURCE
void deleteWebpageResource(const char *course_code, const char *title)
{
    system("cls");

    FILE *fp = fopen(WEBPAGE_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp)
    {
        printf("Error opening files.\n");
        if (fp)
            fclose(fp);
        if (temp)
            fclose(temp);
        return;
    }
    WebpageResource w;
    int found = 0;

    while (fscanf(fp, "%99s %19s %199s\n", w.title, w.course_code, w.url) == 3)
    {
        trim(w.course_code);
        if (strcmp(w.course_code, course_code) == 0 && strcmp(w.title, title) == 0)
        {
            found = 1;
            continue; 
        }
        fprintf(temp, "%s %s %s\n", w.title, w.course_code, w.url);
    }
    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove(WEBPAGE_FILE);
        rename("temp.txt", WEBPAGE_FILE);
        printf("Webpage resource deleted successfully.\n");
    }
    else
    {
        remove("temp.txt");
        printf("No matching webpage resource found.\n");
    }
}


//Menus
//Admin menu
void adminMenu() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int choice;

    while (1) {
        printf("\n");

        // Blue Title
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("%*s\n", (WIDTH + (int)strlen("=== ADMIN MENU ==="))/2, "=== ADMIN MENU ===");

        // Green Menu Items
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("%*s\n", (WIDTH + (int)strlen("1. Add Course"))/2, "1. Add Course");
        printf("%*s\n", (WIDTH + (int)strlen("2. View Courses"))/2, "2. View Courses");
        printf("%*s\n", (WIDTH + (int)strlen("3. Update Course"))/2, "3. Update Course");
        printf("%*s\n", (WIDTH + (int)strlen("4. Delete Course"))/2, "4. Delete Course");
        printf("%*s\n", (WIDTH + (int)strlen("5. View Teacher Information"))/2, "5. View Teacher Information");
        printf("%*s\n", (WIDTH + (int)strlen("6. View Student Information"))/2, "6. View Student Information");
        printf("%*s\n", (WIDTH + (int)strlen("7. Counseling Information"))/2, "7. Counseling Information");
        printf("%*s\n", (WIDTH + (int)strlen("8. Show Questions"))/2, "8. Show Questions");
        printf("%*s\n", (WIDTH + (int)strlen("9. Show All Results"))/2, "9. Show All Results");
        printf("%*s\n", (WIDTH + (int)strlen("10. Add Teacher Information"))/2, "10. Add Teacher Information");
        printf("%*s\n", (WIDTH + (int)strlen("11. Delete Teacher Information"))/2, "11. Delete Teacher Information");
        printf("%*s\n", (WIDTH + (int)strlen("12. Update Teacher Information"))/2, "12. Update Teacher Information");

        // Red for Logout
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("%*s\n", (WIDTH + (int)strlen("13. Logout"))/2, "13. Logout");

        // Green for prompt
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("\n%*s", (WIDTH + (int)strlen("Enter your choice: "))/2, "Enter your choice: ");

        scanf(" %d", &choice); // space before %d to skip leftover newline
        clearInputBuffer();

        // Reset color to default
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        switch (choice) {
            case 1: addCourse(); break;
            case 2: showCourses(); break;
            case 3: updateCourse(); break;
            case 4: deleteCourse(); break;
            case 5: viewTeachers(); break;
            case 6: viewStudents(); break;
            case 7: listAllCounselings(); break;
            case 8: showQuestionsForAdmin(); break;
            case 9: showAllResults(""); break;
            case 10: addTeacher(); break;
            case 11: {
                teacher t;
                printf("Enter Teacher ID to delete: ");
                fgets(t.teacher_id, sizeof(t.teacher_id), stdin);
                trimNewline(t.teacher_id);
                deleteTeacher(&t);
                break;
            }
            case 12: {
                teacher t;
                printf("Enter Teacher ID to update: ");
                fgets(t.teacher_id, sizeof(t.teacher_id), stdin);
                trimNewline(t.teacher_id);

                printf("Enter new Teacher Name: ");
                fgets(t.name, sizeof(t.name), stdin);
                trimNewline(t.name);

                printf("Enter new Teacher Password: ");
                fgets(t.password, sizeof(t.password), stdin);
                trimNewline(t.password);

                printf("Enter new Teacher Subject: ");
                fgets(t.subject, sizeof(t.subject), stdin);
                trimNewline(t.subject);

                updateTeacher(&t);
                break;
            }
            case 13:
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("\nLogged out successfully.\n");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}


// Student Menu
void studentMenu() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int choice;

    while (1) {
        printf("\n");

        // Blue Title
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("%*s\n", (WIDTH + (int)strlen("=== STUDENT MENU ==="))/2, "=== STUDENT MENU ===");

        // Green menu items
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("%*s\n", (WIDTH + (int)strlen("1. View Courses"))/2, "1. View Courses");
        printf("%*s\n", (WIDTH + (int)strlen("2. Take Exam"))/2, "2. Take Exam");
        printf("%*s\n", (WIDTH + (int)strlen("3. Exam Instructions"))/2, "3. Exam Instructions");
        printf("%*s\n", (WIDTH + (int)strlen("4. View Previous Results"))/2, "4. View Previous Results");
        printf("%*s\n", (WIDTH + (int)strlen("5. View Counseling"))/2, "5. View Counseling");
        printf("%*s\n", (WIDTH + (int)strlen("6. View Video Resources"))/2, "6. View Video Resources");
        printf("%*s\n", (WIDTH + (int)strlen("7. View PDF Resources"))/2, "7. View PDF Resources");
        printf("%*s\n", (WIDTH + (int)strlen("8. View Webpage Resources"))/2, "8. View Webpage Resources");

        // Red for Logout
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("%*s\n", (WIDTH + (int)strlen("9. Log Out"))/2, "9. Log Out");

        // Green for prompt
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("\n%*s", (WIDTH + (int)strlen("Enter your choice: "))/2, "Enter your choice: ");

        scanf(" %d", &choice); // space before %d
        clearInputBuffer();

        // Reset color
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        switch(choice) {
            case 1: showCourses(); break;
            case 2: {
                char course_code[20];
                int duration = 1; // example duration
                printf("Enter course code to take exam: ");
                fgets(course_code, sizeof(course_code), stdin);
                trimNewline(course_code);
                studentExam(loggedInStudentID, course_code, duration);
                break;
            }
            case 3: examRules(); break;
            case 4: showStudentResults(loggedInStudentID); break;
            case 5: showCounselings(loggedInStudentID); break;
            case 6: {
                char course_code[20];
                printf("Enter course code to view video resources: ");
                fgets(course_code, sizeof(course_code), stdin);
                trimNewline(course_code);
                showVideoResources(course_code);
                break;
            }
            case 7: {
                char course_code[20];
                printf("Enter course code to view PDF resources: ");
                fgets(course_code, sizeof(course_code), stdin);
                trimNewline(course_code);
                showPDFResources(course_code);
                break;
            }
            case 8: {
                char course_code[20];
                printf("Enter course code to view webpage resources: ");
                fgets(course_code, sizeof(course_code), stdin);
                trimNewline(course_code);
                showWebpageResources(course_code);
                break;
            }
            case 9:
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("\nLogged out successfully.\n");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                return;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }
}


// Teacher Menu
void teacherMenu() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int choice;
    char course_code[20];
    char title[100];

    while (1) {
        printf("\n");

        // Blue Title
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("%*s\n", (WIDTH + (int)strlen("=== TEACHER MENU ==="))/2, "=== TEACHER MENU ===");

        // Green menu options 1–19
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("%*s\n", (WIDTH + (int)strlen("1. Add Question"))/2, "1. Add Question");
        printf("%*s\n", (WIDTH + (int)strlen("2. Delete Question"))/2, "2. Delete Question");
        printf("%*s\n", (WIDTH + (int)strlen("3. Update Question"))/2, "3. Update Question");
        printf("%*s\n", (WIDTH + (int)strlen("4. Add Video Resource"))/2, "4. Add Video Resource");
        printf("%*s\n", (WIDTH + (int)strlen("5. Update Video Resource"))/2, "5. Update Video Resource");
        printf("%*s\n", (WIDTH + (int)strlen("6. Delete Video Resource"))/2, "6. Delete Video Resource");
        printf("%*s\n", (WIDTH + (int)strlen("7. Add PDF Resource"))/2, "7. Add PDF Resource");
        printf("%*s\n", (WIDTH + (int)strlen("8. Update PDF Resource"))/2, "8. Update PDF Resource");
        printf("%*s\n", (WIDTH + (int)strlen("9. Delete PDF Resource"))/2, "9. Delete PDF Resource");
        printf("%*s\n", (WIDTH + (int)strlen("10. Add Webpage Resource"))/2, "10. Add Webpage Resource");
        printf("%*s\n", (WIDTH + (int)strlen("11. Update Webpage Resource"))/2, "11. Update Webpage Resource");
        printf("%*s\n", (WIDTH + (int)strlen("12. Delete Webpage Resource"))/2, "12. Delete Webpage Resource");
        printf("%*s\n", (WIDTH + (int)strlen("13. View Student Results"))/2, "13. View Student Results");
        printf("%*s\n", (WIDTH + (int)strlen("14. Add Counseling Message"))/2, "14. Add Counseling Message");
        printf("%*s\n", (WIDTH + (int)strlen("15. Update Counseling Message"))/2, "15. Update Counseling Message");
        printf("%*s\n", (WIDTH + (int)strlen("16. Delete Counseling Message"))/2, "16. Delete Counseling Message");
        printf("%*s\n", (WIDTH + (int)strlen("17. Show Counseling for Student"))/2, "17. Show Counseling for Student");
        printf("%*s\n", (WIDTH + (int)strlen("18. Show Questions"))/2, "18. Show Questions");
        printf("%*s\n", (WIDTH + (int)strlen("19. Set Timer"))/2, "19. Set Timer");

        // Red for Log Out (20)
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("%*s\n", (WIDTH + (int)strlen("20. Log Out"))/2, "20. Log Out");

        // Green prompt
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("\n%*s", (WIDTH + (int)strlen("Enter your choice: "))/2, "Enter your choice: ");
        scanf(" %d", &choice);
        clearInputBuffer();

        // Reset color
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        switch(choice) {
            case 1: addQuestion(); break;
            case 2: deleteQuestion(); break;
            case 3: updateQuestion(); break;
            case 4: {
                VideoResource v;
                printf("Enter course code: ");
                fgets(course_code, sizeof(course_code), stdin); trimNewline(course_code);
                printf("Enter title: ");
                fgets(v.title, sizeof(v.title), stdin); trimNewline(v.title);
                printf("Enter URL: ");
                fgets(v.url, sizeof(v.url), stdin); trimNewline(v.url);
                strcpy(v.course_code, course_code);
                addVideoResource(&v);
                break;
            }
            case 5: {
                printf("Enter course code: "); fgets(course_code, sizeof(course_code), stdin); trimNewline(course_code);
                printf("Enter title to update: "); fgets(title, sizeof(title), stdin); trimNewline(title);
                updateVideoResource(course_code, title);
                break;
            }
            case 6: {
                printf("Enter course code: "); fgets(course_code, sizeof(course_code), stdin); trimNewline(course_code);
                printf("Enter title to delete: "); fgets(title, sizeof(title), stdin); trimNewline(title);
                deleteVideoResource(course_code, title);
                break;
            }
            case 7: {
                PDFResource p;
                printf("Enter course code: "); fgets(course_code, sizeof(course_code), stdin); trimNewline(course_code);
                printf("Enter title: "); fgets(p.title, sizeof(p.title), stdin); trimNewline(p.title);
                printf("Enter URL: "); fgets(p.url, sizeof(p.url), stdin); trimNewline(p.url);
                strcpy(p.course_code, course_code);
                addPDFResource(&p);
                break;
            }
            case 8: {
                printf("Enter course code: "); fgets(course_code, sizeof(course_code), stdin); trimNewline(course_code);
                printf("Enter title to update: "); fgets(title, sizeof(title), stdin); trimNewline(title);
                updatePDFResource(course_code, title);
                break;
            }
            case 9: {
                printf("Enter course code: "); fgets(course_code, sizeof(course_code), stdin); trimNewline(course_code);
                printf("Enter title to delete: "); fgets(title, sizeof(title), stdin); trimNewline(title);
                deletePDFResource(course_code, title);
                break;
            }
            case 10: {
                WebpageResource w;
                printf("Enter course code: "); fgets(course_code, sizeof(course_code), stdin); trimNewline(course_code);
                printf("Enter title: "); fgets(w.title, sizeof(w.title), stdin); trimNewline(w.title);
                printf("Enter URL: "); fgets(w.url, sizeof(w.url), stdin); trimNewline(w.url);
                strcpy(w.course_code, course_code);
                addWebpageResource(&w);
                break;
            }
            case 11: {
                printf("Enter course code: "); fgets(course_code, sizeof(course_code), stdin); trimNewline(course_code);
                printf("Enter title to update: "); fgets(title, sizeof(title), stdin); trimNewline(title);
                updateWebpageResource(course_code, title);
                break;
            }
            case 12: {
                printf("Enter course code: "); fgets(course_code, sizeof(course_code), stdin); trimNewline(course_code);
                printf("Enter title to delete: "); fgets(title, sizeof(title), stdin); trimNewline(title);
                deleteWebpageResource(course_code, title);
                break;
            }
            case 13: showAllResults(""); break;
            case 14: {
                Counseling c; static int next_record_id=1;
                printf("Enter student ID: "); fgets(c.student_id,sizeof(c.student_id),stdin); trimNewline(c.student_id);
                printf("Enter teacher ID: "); fgets(c.teacher_id,sizeof(c.teacher_id),stdin); trimNewline(c.teacher_id);
                printf("Enter date: "); fgets(c.date,sizeof(c.date),stdin); trimNewline(c.date);
                printf("Enter topic: "); fgets(c.topic,sizeof(c.topic),stdin); trimNewline(c.topic);
                printf("Enter notes: "); fgets(c.notes,sizeof(c.notes),stdin); trimNewline(c.notes);
                c.record_id=next_record_id++;
                addCounseling(&c);
                break;
            }
            case 15: {
                char sid[20], date[20];
                printf("Enter student ID to update: "); fgets(sid,sizeof(sid),stdin); trimNewline(sid);
                printf("Enter date: "); fgets(date,sizeof(date),stdin); trimNewline(date);
                updateCounseling(sid, date);
                break;
            }
            case 16: {
                char sid[20], date[20];
                printf("Enter student ID to delete: "); fgets(sid,sizeof(sid),stdin); trimNewline(sid);
                printf("Enter date: "); fgets(date,sizeof(date),stdin); trimNewline(date);
                deleteCounseling(sid, date);
                break;
            }
            case 17: {
                char sid[20];
                printf("Enter student ID: "); fgets(sid,sizeof(sid),stdin); trimNewline(sid);
                showCounselings(sid);
                break;
            }
            case 18: {
                printf("Enter course code: "); fgets(course_code,sizeof(course_code),stdin); trimNewline(course_code);
                showQuestionsForTeacher(course_code);
                break;
            }
            case 19:
                printf("Set Exam Duration (minutes): "); scanf("%d",&examDurationMinutes); clearInputBuffer();
                printf("Exam duration set to %d minutes.\n", examDurationMinutes);
                break;
            case 20:
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("\nLogged out successfully.\n");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                return;
            default: printf("\nInvalid choice. Please try again.\n");
        }
    }
}


// MAIN MENU
int main(void)
{

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int user_choice, action_choice;

    while (1)
    {      
         system("cls"); 
         
    print_exam_bunny_menu();
    
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("\n%*s\n", (WIDTH + (int)strlen("Choose user type:"))/2, "Choose user type:");
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("%*s\n", (WIDTH + (int)strlen("1. Student"))/2, "1. Student");
    printf("%*s\n", (WIDTH + (int)strlen("2. Teacher"))/2, "2. Teacher");
    printf("%*s\n", (WIDTH + (int)strlen("3. Admin"))/2, "3. Admin");
SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("%*s\n", (WIDTH + (int)strlen("4. Exit"))/2, "4. Exit");


    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    printf("\n%*s", (WIDTH + (int)strlen("Enter your choice: "))/2, "Enter your choice: ");
    scanf("%d", &user_choice);

    

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("\nYou chose: %d\n", user_choice);

    getchar();

    if (user_choice == 4) { 
        
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("Exiting system.\n");
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        const char *role = (user_choice == 1) ? "student" : (user_choice == 2) ? "teacher"
                                                        : (user_choice == 3)   ? "admin"
                                                                               : NULL;

        if (!role)
        {
            printf("Invalid user type.\n");
            continue;
        }

printf("\n%*s\n", (WIDTH + (int)strlen("-------------------"))/2, "-------------------");
 system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("\n%*s\n", (WIDTH + (int)strlen("Choose action:")) / 2, "Choose action:");

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("%*s\n", (WIDTH + (int)strlen("1. Register")) / 2, "1. Register");

    printf("%*s\n", (WIDTH + (int)strlen("2. Login")) / 2, "2. Login");

    printf("\n%*s", (WIDTH + (int)strlen("Enter your choice: ")) / 2, "Enter your choice: ");
    scanf("%d", &action_choice);
    getchar();

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);


        switch (user_choice)
        {
        case 1: // Student
            if (action_choice == 1)
            {
                registerUser();
            }
            else if (action_choice == 2)
            {
                if (loginUser())
                system("cls");
                    studentMenu();
            }
            else
            {
                printf("Invalid action choice.\n");
            }
            break;

        case 2: // Teacher
            if (action_choice == 1)
            {
                registerTeacher();
            }
            else if (action_choice == 2)
            {
                if (loginTeacher())
                system("cls");
                    teacherMenu();
            }
            else
            {
                printf("Invalid action choice.\n");
            }
            break;

        case 3: // Admin
            if (action_choice == 1)
            {
                registerAdmin();
            }
            else if (action_choice == 2)
            {
                if (loginAdmin())
                system("cls");
                    adminMenu();
            }
            else
            {
                printf("Invalid action choice.\n");
            }
            break;
        }
    }

    return 0;
}