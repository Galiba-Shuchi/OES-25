#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ADMIN_FILE "admin.txt"
#define TEACHER_FILE "teacher.txt"
#define USER_FILE "user.txt"
#define VIDEO_FILE "video_resources.txt"
#define PDF_FILE "pdf_resources.txt"
#define WEBPAGE_FILE "webpage_resources.txt"
#define COUNSELING_FILE "counseling.txt"
#define COMPLAINT_FILE "complaints.txt"

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

typedef struct
{
    char student_id[20];
    char student_name[50];
    char date[20];
    char subject[100];
    char details[300];
} Complaint;

// Function Declarations
// log in Registration
void registerUser();
int loginUser();
void displayUser(const user *u);
void updateUser(user *u);
void deleteUser(user *u);
void registerTeacher();
int loginTeacher();
// update
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
void studentExam(const char *student_id, const char *course_code);
void examRules();
void showAllResults(const char *student_id);
void addExamResult(ExamResult *result);
void displayExamResult(const ExamResult *result);
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

void print_exam_bunny_logo(void)
{
    puts("  (\\__/)");
    puts("  (> . <)          ExamBunny");
    puts("  /     \\   \"Check. Learn. Repeat.\"");
    puts(""); /* blank line */
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



// admin registration function
void registerAdmin()
{
    admin a;

    printf("Enter Admin ID: ");
    scanf("%19s", a.admin_id);
    printf("Enter Name: ");
    scanf(" %[^\n]", a.name);
    printf("Enter Password: ");
    scanf("%49s", a.password);
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
    scanf("%49s", password);

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


// STUDENT registration function
void registerUser()
{
    user u;

    printf("Enter Student ID: ");
    scanf("%19s", u.student_id);
    printf("Enter Name: ");
    scanf(" %[^\n]", u.name);
    printf("Enter Password: ");
    scanf("%49s", u.password);

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

// USER login function
int loginUser()
{
    char id[20], password[50];
    printf("Enter User ID: ");
    scanf("%19s", id);
    printf("Enter Password: ");
    scanf("%49s", password);

    FILE *fp = fopen(USER_FILE, "r");
    if (!fp)
    {
        perror("Error opening user file");
        return 0; // failed login
    }

    user u;
    int found = 0;
    while (fscanf(fp, "%s %s %s\n", u.student_id, u.name, u.password) != EOF)
    {

        if ((strcmp(id, u.student_id) == 0) && (strcmp(password, u.password) == 0))
        {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found)
    {
        printf(" User login successful. Welcome, %s!\n", u.name);
        return 1; // success
    }
    else if (found == 0)
    {
        printf(" Login failed. Invalid ID or password.\n");
        return 0; // fail
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
    // store all sales data to allSalesProduct function
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
        scanf("%49s", t.password);
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
    scanf("%49s", password);

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

// Update user function
void updateUser(user *u)
{
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
            // skip writing to delete
            continue;
        }
        fprintf(temp, "%s|%s|%s|%s\n", tempTeacher.teacher_id, tempTeacher.name, tempTeacher.password, tempTeacher.subject);
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

    printf("Teacher deleted successfully.\n");
}


// Add Course Function
void addCourse()
{
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



// Add Question Function
void addQuestion()
{
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
    char courseCode[20];
    printf("Enter course code of the question to update: ");
    scanf("%19s", courseCode);
    clearInputBuffer();

    char oldQuestion[300];
    printf("Enter the exact question text to update: ");
    fgets(oldQuestion, sizeof(oldQuestion), stdin);
    oldQuestion[strcspn(oldQuestion, "\n")] = '\0';
    trim(oldQuestion);

    FILE *fp = fopen("mcq.txt", "r");
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

    MCQ q;
    int found = 0;
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

        char qQuestionTrimmed[300];
        strcpy(qQuestionTrimmed, q.question);
        trim(qQuestionTrimmed);

        if (strcmp(q.course_code, courseCode) == 0 && strcmp(qQuestionTrimmed, oldQuestion) == 0)
        {
            found = 1;

            printf("Enter new question text: ");
            fgets(q.question, sizeof(q.question), stdin);
            q.question[strcspn(q.question, "\n")] = '\0';
            trim(q.question);

            printf("Enter option A: ");
            fgets(q.option_a, sizeof(q.option_a), stdin);
            q.option_a[strcspn(q.option_a, "\n")] = '\0';
            trim(q.option_a);

            printf("Enter option B: ");
            fgets(q.option_b, sizeof(q.option_b), stdin);
            q.option_b[strcspn(q.option_b, "\n")] = '\0';
            trim(q.option_b);

            printf("Enter option C: ");
            fgets(q.option_c, sizeof(q.option_c), stdin);
            q.option_c[strcspn(q.option_c, "\n")] = '\0';
            trim(q.option_c);

            printf("Enter option D: ");
            fgets(q.option_d, sizeof(q.option_d), stdin);
            q.option_d[strcspn(q.option_d, "\n")] = '\0';
            trim(q.option_d);

            printf("Enter correct answer (a/b/c/d): ");
            scanf(" %c", &q.check);
            q.check = tolower(q.check);
            clearInputBuffer();
        }

        fprintf(temp, "%s,%s,%s,%s,%s,%s,%c\n",
                q.course_code, q.question,
                q.option_a, q.option_b,
                q.option_c, q.option_d,
                q.check);
    }

    fclose(fp);
    fclose(temp);

    if (!found)
    {
        printf("Question not found.\n");
        remove("temp.txt");
        return;
    }

    remove("mcq.txt");
    rename("temp.txt", "mcq.txt");
    printf("Question updated successfully.\n");
}

// Delete Question Function
void deleteQuestion()
{
    char courseCode[20];
    printf("Enter course code of the question to delete: ");
    scanf("%19s", courseCode);
    clearInputBuffer();

    char questionText[300];
    printf("Enter the exact question text to delete: ");
    fgets(questionText, sizeof(questionText), stdin);
    questionText[strcspn(questionText, "\n")] = '\0';
    trim(questionText);

    FILE *fp = fopen("mcq.txt", "r");
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

    MCQ q;
    int found = 0;
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

        char qQuestionTrimmed[300];
        strcpy(qQuestionTrimmed, q.question);
        trim(qQuestionTrimmed);

        if (strcmp(q.course_code, courseCode) == 0 && strcmp(qQuestionTrimmed, questionText) == 0)
        {
            found = 1;
            // skip writing to delete question
            continue;
        }

        fprintf(temp, "%s,%s,%s,%s,%s,%s,%c\n",
                q.course_code, q.question,
                q.option_a, q.option_b,
                q.option_c, q.option_d,
                q.check);
    }

    fclose(fp);
    fclose(temp);

    if (!found)
    {
        printf("Question not found.\n");
        remove("temp.txt");
        return;
    }

    remove("mcq.txt");
    rename("temp.txt", "mcq.txt");
    printf("Question deleted successfully.\n");
}

// Show Questions for Admin Function
void showQuestionsForAdmin()
{
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



// Function to conduct exam
void studentExam(const char *student_id, const char *course_code)
{
    FILE *fp = fopen("mcq.txt", "r");
    if (!fp)
    {
        printf("No questions available.\n");
        return;
    }

    MCQ questions[100];
    int total_questions = 0;

    // Load only questions for the requested course_code
    while (fscanf(fp, "%19[^|]|%299[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|%c\n",
                  questions[total_questions].course_code,
                  questions[total_questions].question,
                  questions[total_questions].option_a,
                  questions[total_questions].option_b,
                  questions[total_questions].option_c,
                  questions[total_questions].option_d,
                  &questions[total_questions].check) == 7)
    {
        if (strcmp(questions[total_questions].course_code, course_code) == 0)
        {
            total_questions++;
            if (total_questions >= 100)
                break;
        }
    }
    fclose(fp);

    if (total_questions == 0)
    {
        printf("No questions found for course %s.\n", course_code);
        return;
    }

    printf("Starting exam for course: %s\n", course_code);
    printf("Total questions: %d\n", total_questions);

    int score = 0;
    WrongAnswer wrongAnswers[100];
    int wrongCount = 0;

    for (int i = 0; i < total_questions; i++)
    {
        printf("\nQ%d: %s\n", i + 1, questions[i].question);
        printf("a) %s\n", questions[i].option_a);
        printf("b) %s\n", questions[i].option_b);
        printf("c) %s\n", questions[i].option_c);
        printf("d) %s\n", questions[i].option_d);

        char ans;
        do
        {
            printf("Enter your answer (a/b/c/d): ");
            scanf(" %c", &ans);
            ans = tolower(ans);
        } while (ans != 'a' && ans != 'b' && ans != 'c' && ans != 'd');

        if (ans == questions[i].check)
        {
            score++;
        }
        else
        {
            // Store wrong answer details
            strcpy(wrongAnswers[wrongCount].question, questions[i].question);
            wrongAnswers[wrongCount].correct_answer = questions[i].check;
            wrongAnswers[wrongCount].student_answer = ans;
            wrongCount++;
        }
    }

    printf("\nExam finished! Your score: %d out of %d\n", score, total_questions);

    // Save exam result
    FILE *resfp = fopen("exam_results.txt", "a");
    if (resfp)
    {
        fprintf(resfp, "%s|%s|%d\n", student_id, course_code, score);
        fclose(resfp);
    }
    else
    {
        printf("Failed to save exam result.\n");
    }

    // Save wrong answers for review later
    FILE *wfp = fopen("wrong_answers.txt", "a");
    if (wfp)
    {
        for (int i = 0; i < wrongCount; i++)
        {
            fprintf(wfp, "%s|%s|%c|%c\n", student_id, wrongAnswers[i].question, wrongAnswers[i].correct_answer, wrongAnswers[i].student_answer);
        }
        fclose(wfp);
    }

    // Show review option
    char choice;
    printf("Do you want to review your wrong answers? (y/n): ");
    scanf(" %c", &choice);
    if (tolower(choice) == 'y')
    {
        printf("\nWrong answers review:\n");
        for (int i = 0; i < wrongCount; i++)
        {
            printf("Q: %s\n", wrongAnswers[i].question);
            printf("Your answer: %c | Correct answer: %c\n", wrongAnswers[i].student_answer, wrongAnswers[i].correct_answer);
            printf("-----------------------------\n");
        }
    }
}

// exam instructtion
void examRules()
{
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

// function to add exam result
void addExamResult(ExamResult *result)
{
    FILE *fp = fopen("exam_results.txt", "a");
    if (!fp)
    {
        printf("Error: Unable to open results file.\n");
        return;
    }
    fprintf(fp, "%s|%s|%d\n", result->student_id, result->course_code, result->score);
    fclose(fp);
}

// display result
void displayExamResult(const ExamResult *result)
{
    printf("Student ID: %s\n", result->student_id);
    printf("Course Code: %s\n", result->course_code);
    printf("Score: %d\n", result->score);
    printf("-------------------------\n");
}

// function to show all result
void showAllResults(const char *student_id)
{
    FILE *fp = fopen("exam_results.txt", "r");
    if (!fp)
    {
        printf("No exam results found.\n");
        return;
    }

    ExamResult result;
    int found = 0;

    printf("Exam Results for Student ID: %s\n", student_id);
    printf("------------------------------\n");

    while (fscanf(fp, "%19[^|]|%19[^|]|%d\n", result.student_id, result.course_code, &result.score) == 3)
    {
        if (strcmp(result.student_id, student_id) == 0)
        {
            displayExamResult(&result);
            found = 1;
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("No exam results found for this student.\n");
    }
}

// add counseling
void addCounseling(const Counseling *c)
{
    FILE *fp = fopen(COUNSELING_FILE, "a");
    if (!fp)
    {
        printf("Error opening file for writing.\n");
        return;
    }
    // স্পেস দিয়ে আলাদা করে লেখাঃ
    fprintf(fp, "%d %s %s %s %s %s\n", c->record_id, c->student_id, c->teacher_id, c->date, c->topic, c->notes);
    fclose(fp);
    printf("Counseling record added successfully.\n");
}

// show counsiling
void showCounselings(const char *student_id)
{
    FILE *fp = fopen(COUNSELING_FILE, "r");
    if (!fp)
    {
        printf("Error opening file for reading.\n");
        return;
    }

    Counseling c;
    int found = 0;

    printf("Counseling records for student ID: %s\n", student_id);

    while (fscanf(fp, "%d %19s %19s %19s %99s %299[^\n]", &c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes) == 6)
    {
        if (strcmp(c.student_id, student_id) == 0)
        {
            found = 1;
            printf("Record ID: %d\nTeacher ID: %s\nDate: %s\nTopic: %s\nNotes: %s\n\n", c.record_id, c.teacher_id, c.date, c.topic, c.notes);
        }
    }
    if (!found)
    {
        printf("No counseling records found for this student.\n");
    }
    fclose(fp);
}

// update counseling
void updateCounseling(const char *student_id, const char *date)
{
    FILE *fp = fopen(COUNSELING_FILE, "r");
    if (!fp)
    {
        printf("Error opening file.\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    if (!temp)
    {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    Counseling c;
    int found = 0;

    while (fscanf(fp, "%d %19s %19s %19s %99s %299[^\n]", &c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes) == 6)
    {
        if (strcmp(c.student_id, student_id) == 0 && strcmp(c.date, date) == 0)
        {
            found = 1;
            printf("Updating counseling record (Record ID: %d)\n", c.record_id);

            printf("Enter new topic: ");
            getchar(); // leftover newline
            fgets(c.topic, sizeof(c.topic), stdin);
            c.topic[strcspn(c.topic, "\n")] = 0;

            printf("Enter new notes: ");
            fgets(c.notes, sizeof(c.notes), stdin);
            c.notes[strcspn(c.notes, "\n")] = 0;

            fprintf(temp, "%d %s %s %s %s %s\n", c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes);
        }
        else
        {
            fprintf(temp, "%d %s %s %s %s %s\n", c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove(COUNSELING_FILE);
        rename("temp.txt", COUNSELING_FILE);
        printf("Counseling record updated successfully.\n");
    }
    else
    {
        remove("temp.txt");
        printf("Record not found for update.\n");
    }
}

// delete counseling
void deleteCounseling(const char *student_id, const char *date)
{
    FILE *fp = fopen(COUNSELING_FILE, "r");
    if (!fp)
    {
        printf("Error opening file.\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    if (!temp)
    {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    Counseling c;
    int found = 0;

    while (fscanf(fp, "%d %19s %19s %19s %99s %299[^\n]", &c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes) == 6)
    {
        if (strcmp(c.student_id, student_id) == 0 && strcmp(c.date, date) == 0)
        {
            found = 1; // skip writing to delete
        }
        else
        {
            fprintf(temp, "%d %s %s %s %s %s\n", c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove(COUNSELING_FILE);
        rename("temp.txt", COUNSELING_FILE);
        printf("Counseling record deleted successfully.\n");
    }
    else
    {
        remove("temp.txt");
        printf("Record not found for deletion.\n");
    }
}

// all counsil
void listAllCounselings()
{
    FILE *fp = fopen(COUNSELING_FILE, "r");
    if (!fp)
    {
        printf("Error opening file.\n");
        return;
    }

    Counseling c;
    int count = 0;

    printf("All Counseling Records:\n");
    while (fscanf(fp, "%d %19s %19s %19s %99s %299[^\n]", &c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes) == 6)
    {
        count++;
        printf("Record ID: %d\nStudent ID: %s\nTeacher ID: %s\nDate: %s\nTopic: %s\nNotes: %s\n\n",
               c.record_id, c.student_id, c.teacher_id, c.date, c.topic, c.notes);
    }

    if (count == 0)
    {
        printf("No counseling records found.\n");
    }

    fclose(fp);
}

// add compliment
void addComplaint()
{
    Complaint c;
    FILE *fp = fopen(COMPLAINT_FILE, "a");
    if (!fp)
    {
        printf("Error opening complaint file.\n");
        return;
    }

    printf("Enter your Student ID: ");
    fgets(c.student_id, sizeof(c.student_id), stdin);
    c.student_id[strcspn(c.student_id, "\n")] = '\0';

    printf("Enter your Name: ");
    fgets(c.student_name, sizeof(c.student_name), stdin);
    c.student_name[strcspn(c.student_name, "\n")] = '\0';

    printf("Enter Date (DD/MM/YYYY): ");
    fgets(c.date, sizeof(c.date), stdin);
    c.date[strcspn(c.date, "\n")] = '\0';

    printf("Enter Subject of Complaint: ");
    fgets(c.subject, sizeof(c.subject), stdin);
    c.subject[strcspn(c.subject, "\n")] = '\0';

    printf("Enter Complaint Details: ");
    fgets(c.details, sizeof(c.details), stdin);
    c.details[strcspn(c.details, "\n")] = '\0';

    fprintf(fp, "%s|%s|%s|%s|%s\n", c.student_id, c.student_name, c.date, c.subject, c.details);
    fclose(fp);

    printf("Complaint submitted successfully.\n");
}

// Show complaints by student ID (for student)
void showComplaints(const char *student_id)
{
    Complaint c;
    int found = 0;
    FILE *fp = fopen(COMPLAINT_FILE, "r");
    if (!fp)
    {
        printf("No complaints found.\n");
        return;
    }

    printf("\nYour Complaints:\n");
    printf("-----------------------------\n");

    while (fscanf(fp, "%19[^|]|%49[^|]|%19[^|]|%99[^|]|%299[^\n]\n",
                  c.student_id, c.student_name, c.date, c.subject, c.details) == 5)
    {
        if (strcmp(c.student_id, student_id) == 0)
        {
            found = 1;
            printf("Date: %s\nSubject: %s\nDetails: %s\n-----------------------------\n",
                   c.date, c.subject, c.details);
        }
    }

    fclose(fp);
    if (!found)
    {
        printf("No complaints found for your ID.\n");
    }
}

// Show complaints by student ID and date (for admin)
void adminViewComplaint()
{
    char student_id[20], date[20];
    printf("Enter Student ID: ");
    fgets(student_id, sizeof(student_id), stdin);
    student_id[strcspn(student_id, "\n")] = '\0';

    printf("Enter Date (DD/MM/YYYY): ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = '\0';

    Complaint c;
    int found = 0;
    FILE *fp = fopen(COMPLAINT_FILE, "r");
    if (!fp)
    {
        printf("No complaints found.\n");
        return;
    }

    printf("\nComplaint details for Student ID %s on %s:\n", student_id, date);
    printf("--------------------------------------------\n");

    while (fscanf(fp, "%19[^|]|%49[^|]|%19[^|]|%99[^|]|%299[^\n]\n",
                  c.student_id, c.student_name, c.date, c.subject, c.details) == 5)
    {
        if (strcmp(c.student_id, student_id) == 0 && strcmp(c.date, date) == 0)
        {
            found = 1;
            printf("Name: %s\nSubject: %s\nDetails: %s\n", c.student_name, c.subject, c.details);
            break;
        }
    }

    fclose(fp);
    if (!found)
    {
        printf("No complaint found for this student ID and date.\n");
    }
}

// Add Video Resource
void addVideoResource(const VideoResource *v)
{
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
            continue; // skip writing this record to delete
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

// Admin Menu
void adminMenu()
{
    int choice;
    char admin_id[20] = "";

    while (1)
    {
        printf("\n=== ADMIN MENU ===\n");
        printf("1. Add Course\n");
        printf("2. View Courses\n");
        printf("3. Update Course\n");
        printf("4. Delete Course\n");
        printf("5. View Teacher Information\n");
        printf("6. View Student Information\n");
        printf("7. Counsiling Information\n");
        printf("8.View Complains\n");
        printf("9. Show Questions\n");
        printf("10. Show All Results\n");
        printf("11. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            addCourse();
            break;
        case 2:
            showCourses();
            break;
        case 3:
            updateCourse();
            break;
        case 4:
            deleteCourse();
            break;
        
            case 5:
            // View Teacher Information
            viewTeachers();
            break;
        case 6:
            // View Student Information
            viewStudents();
            break;
        case 7:
            // Counselling Information
            listAllCounselings();
            break;
        case 8:
            // View Complains
            adminViewComplaint();
            break;
        case 9:
            showQuestionsForAdmin();
            break;
        case 10:
            showAllResults("");
            break;
        case 11:
            printf("Logged out successfully.\n");
            return;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

// Student Menu
void studentMenu()
{
    int choice;
    char student_id[20] = "";

    while (1)
    {
        printf("\n=== STUDENT MENU ===\n");
        printf("1. View Courses\n");
        printf("2. Take Exam\n");
        printf("3. Exam Instructions\n");
        printf("4. View Previous Results\n");
        printf("5. View Counseling\n");
        printf("6. View Video Resources\n");
        printf("7. View PDF Resources\n");
        printf("8. View Webpage Resources\n");
        printf("9. Submit Complaint\n");    // add
        printf("10. View My Complaints\n"); // view
        printf("11. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            showCourses();
            break;
        case 2:
            if (strlen(student_id) > 0)
            {
                char course_code[20];
                printf("Enter course code to take exam: ");
                fgets(course_code, sizeof(course_code), stdin);
                course_code[strcspn(course_code, "\n")] = '\0';
                studentExam(student_id, course_code);
            }
            else
            {
                printf("Please login first.\n");
            }
            break;
        case 3:
            examRules();
            break;
        case 4:
            if (strlen(student_id) > 0)
            {
                showAllResults(student_id);
            }
            else
            {
                printf("Please login first.\n");
            }
            break;
        case 5:
            if (strlen(student_id) > 0)
            {
                showCounselings(student_id);
            }
            else
            {
                printf("Please login first.\n");
            }
            break;

            break;

        case 6:
        {
            char course_code[20];
            printf("Enter course code to view Video Resources: ");
            fgets(course_code, sizeof(course_code), stdin);
            course_code[strcspn(course_code, "\n")] = '\0';
            showVideoResources(course_code);
            break;
        }
        case 7:
        {
            char course_code[20];
            printf("Enter course code to view PDF Resources: ");
            fgets(course_code, sizeof(course_code), stdin);
            course_code[strcspn(course_code, "\n")] = '\0';
            showPDFResources(course_code);
            break;
        }
        case 8:
        {
            char course_code[20];
            printf("Enter course code to view Webpage Resources: ");
            fgets(course_code, sizeof(course_code), stdin);
            course_code[strcspn(course_code, "\n")] = '\0';
            showWebpageResources(course_code);
            break;
        }
        case 9:
            if (strlen(student_id) > 0)
            {
                addComplaint();
            }
            else
            {
                printf("Please login first.\n");
            }
            break;
        case 10:
            if (strlen(student_id) > 0)
            {
                showComplaints(student_id);
            }
            else
            {
                printf("Please login first.\n");
            }
            break;
        case 11:
            printf("Logged out successfully.\n");
            return;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

// Teacher Menu
void teacherMenu()
{
    int choice;
    char course_code[20];
    char title[100];

    while (1)
    {
        printf("\n=== TEACHER MENU ===\n");
        printf("1. Add Question\n");
        printf("2. Delete Question\n");
        printf("3. Update Question\n");
        printf("4. Add Video Resource\n");
        printf("5. Update Video Resource\n");
        printf("6. Delete Video Resource\n");
        printf("7. Add PDF Resource\n");
        printf("8. Update PDF Resource\n");
        printf("9. Delete PDF Resource\n");
        printf("10. Add Webpage Resource\n");
        printf("11. Update Webpage Resource\n");
        printf("12. Delete Webpage Resource\n");
        printf("13. View Student Results\n");
        printf("14. Add Counseling Message\n");
        printf("15. Update Counseling Message\n");
        printf("16. Delete Counseling Message\n");
        printf("17. Show Counseling for Student\n");
        printf("18. Show Questions\n");
        printf("19. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            addQuestion();
            break;

        case 2:
            deleteQuestion();
            break;

        case 3:
            updateQuestion();
            break;

        case 4:
        {
            VideoResource v;
            printf("Enter course code: ");
            fgets(course_code, sizeof(course_code), stdin);
            trimNewline(course_code);

            printf("Enter title: ");
            fgets(v.title, sizeof(v.title), stdin);
            trimNewline(v.title);

            printf("Enter URL: ");
            fgets(v.url, sizeof(v.url), stdin);
            trimNewline(v.url);

            strcpy(v.course_code, course_code);
            addVideoResource(&v);
            break;
        }

        case 5:
        {
            printf("Enter course code: ");
            fgets(course_code, sizeof(course_code), stdin);
            trimNewline(course_code);

            printf("Enter title of the Video Resource to update: ");
            fgets(title, sizeof(title), stdin);
            trimNewline(title);

            updateVideoResource(course_code, title);
            break;
        }

        case 6:
        {
            printf("Enter course code: ");
            fgets(course_code, sizeof(course_code), stdin);
            trimNewline(course_code);

            printf("Enter title of the Video Resource to delete: ");
            fgets(title, sizeof(title), stdin);
            trimNewline(title);

            deleteVideoResource(course_code, title);
            break;
        }

        case 7:
        {
            PDFResource p;
            printf("Enter course code: ");
            fgets(course_code, sizeof(course_code), stdin);
            trimNewline(course_code);

            printf("Enter title: ");
            fgets(p.title, sizeof(p.title), stdin);
            trimNewline(p.title);

            printf("Enter URL: ");
            fgets(p.url, sizeof(p.url), stdin);
            trimNewline(p.url);

            strcpy(p.course_code, course_code);
            addPDFResource(&p);
            break;
        }

        case 8:
        {
            printf("Enter course code: ");
            fgets(course_code, sizeof(course_code), stdin);
            trimNewline(course_code);

            printf("Enter title of the PDF Resource to update: ");
            fgets(title, sizeof(title), stdin);
            trimNewline(title);

            updatePDFResource(course_code, title);
            break;
        }

        case 9:
        {
            printf("Enter course code: ");
            fgets(course_code, sizeof(course_code), stdin);
            trimNewline(course_code);

            printf("Enter title of the PDF Resource to delete: ");
            fgets(title, sizeof(title), stdin);
            trimNewline(title);

            deletePDFResource(course_code, title);
            break;
        }

        case 10:
        {
            WebpageResource w;
            printf("Enter course code: ");
            fgets(course_code, sizeof(course_code), stdin);
            trimNewline(course_code);

            printf("Enter title: ");
            fgets(w.title, sizeof(w.title), stdin);
            trimNewline(w.title);

            printf("Enter URL: ");
            fgets(w.url, sizeof(w.url), stdin);
            trimNewline(w.url);

            strcpy(w.course_code, course_code);
            addWebpageResource(&w);
            break;
        }

        case 11:
        {
            printf("Enter course code: ");
            fgets(course_code, sizeof(course_code), stdin);
            trimNewline(course_code);

            printf("Enter title of the Webpage Resource to update: ");
            fgets(title, sizeof(title), stdin);
            trimNewline(title);

            updateWebpageResource(course_code, title);
            break;
        }

        case 12:
        {
            printf("Enter course code: ");
            fgets(course_code, sizeof(course_code), stdin);
            trimNewline(course_code);

            printf("Enter title of the Webpage Resource to delete: ");
            fgets(title, sizeof(title), stdin);
            trimNewline(title);

            deleteWebpageResource(course_code, title);
            break;
        }

        case 13:
            showAllResults(""); 
            break;

    

        case 14:
        {
            Counseling c;
            static int next_record_id = 1; 

            printf("Enter student ID: ");
            fgets(c.student_id, sizeof(c.student_id), stdin);
            trimNewline(c.student_id);

            printf("Enter teacher ID: ");
            fgets(c.teacher_id, sizeof(c.teacher_id), stdin);
            trimNewline(c.teacher_id);

            printf("Enter date (DD/MM/YYYY): ");
            fgets(c.date, sizeof(c.date), stdin);
            trimNewline(c.date);

            printf("Enter topic: ");
            fgets(c.topic, sizeof(c.topic), stdin);
            trimNewline(c.topic);

            printf("Enter notes: ");
            fgets(c.notes, sizeof(c.notes), stdin);
            trimNewline(c.notes);

            c.record_id = next_record_id++; 

            addCounseling(&c);
            break;
        }

        case 15:
        {
            char student_id[20], date[20];
            printf("Enter student ID to update: ");
            fgets(student_id, sizeof(student_id), stdin);
            trimNewline(student_id);

            printf("Enter date (DD/MM/YYYY): ");
            fgets(date, sizeof(date), stdin);
            trimNewline(date);

            updateCounseling(student_id, date);
            break;
        }

        case 16:
        {
            char student_id[20], date[20];
            printf("Enter student ID to delete: ");
            fgets(student_id, sizeof(student_id), stdin);
            trimNewline(student_id);

            printf("Enter date (DD/MM/YYYY): ");
            fgets(date, sizeof(date), stdin);
            trimNewline(date);

            deleteCounseling(student_id, date);
            break;
        }

        case 17:
        {
            char student_id[20];
            printf("Enter student ID to view counseling messages: ");
            fgets(student_id, sizeof(student_id), stdin);
            trimNewline(student_id);

            showCounselings(student_id);
            break;
        }

        case 18:
        {
            printf("Enter course code to show questions: ");
            fgets(course_code, sizeof(course_code), stdin);
            trimNewline(course_code);

            showQuestionsForTeacher(course_code);
            break;
        }

        case 19:
            printf("Logged out successfully.\n");
            return;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

// MAIN MENU
int main(void)
{

    print_exam_bunny_logo();
    int user_choice, action_choice;

    while (1)
    {
        printf("\nChoose user type:\n");
        printf("1. Student\n");
        printf("2. Teacher\n");
        printf("3. Admin\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &user_choice);
        getchar();

        if (user_choice == 4)
        {
            printf("Exiting system.\n");
            break;
        }

        const char *role = (user_choice == 1) ? "student" : (user_choice == 2) ? "teacher"
                                                        : (user_choice == 3)   ? "admin"
                                                                               : NULL;

        if (!role)
        {
            printf("Invalid user type.\n");
            continue;
        }

        printf("\n----------%s---------\n", (user_choice == 1) ? "Student" : (user_choice == 2) ? "Teacher"
                                                                                                : "Admin");
        printf("\nChoose action:\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("Enter your choice: ");
        scanf("%d", &action_choice);
        getchar();

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