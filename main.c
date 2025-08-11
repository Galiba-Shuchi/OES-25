#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#define ADMIN_FILE "admin.txt"    
#define TEACHER_FILE "teacher.txt"
#define USER_FILE "user.txt"
#define COUNSELING_FILE "counseling.txt"
#define MEDIA_FILE "media_links.txt"



// Structures
typedef struct {
    char student_id[20];
    char name[50];
    char password[50];
    char student_role[10];
} user;

typedef struct {
    char teacher_id[20];
    char name[50];
    char password[50];
    char subject[30];
} teacher;

typedef struct {
    char admin_id[20];
    char name[50];
    char password[50];
    char admin_role[10];
} admin;

typedef struct {
    char course_name[100];
    char course_code[20];
} Course;

typedef struct {
    char course_code[20];
    char question[300];
    char option_a[100];
    char option_b[100];
    char option_c[100];
    char option_d[100];
    char check;
} MCQ;

typedef struct {
    char question[300];
    char correct_answer;
    char student_answer;
} WrongAnswer;

typedef struct {
    char student_id[20];
    char course_code[20];
    int score;
} ExamResult;

typedef struct {
    char student_id[20];
    char student_name[50];
    char teacher_id[20];
    char teacher_name[50];
    char date[20];
    char topic[100];
    char notes[300];
} Counseling;

typedef struct {
    char title[100];
    char course_code[20];
    char media_type[20];
    char url[200];
} MediaLink;




//Function Declarations 
void registerUser ();
int loginUser ();
void displayUser (const user *u);
void updateUser (user *u);
void deleteUser (user *u);
void registerTeacher();
int loginTeacher();
void displayTeacher(const teacher *t);
void updateTeacher(teacher *t);
void deleteTeacher(teacher *t);
void registerAdmin();
int loginAdmin();
void displayAdmin(const admin *a);
void addCourse();
void showCourses();
void updateCourse();
void deleteCourse();
void addQuestion();
void showQuestionsForAdmin();
void updateQuestion();
void deleteQuestion();
void studentExam(const char *student_id, const char *course_code);
void examRules();
void showAllResults(const char *student_id);
void addExamResult(ExamResult *result);
void displayExamResult(const ExamResult *result);
void addCounseling(const Counseling *c);
void showCounselings(const char *student_id);
void updateCounseling(const char *student_id, const char *date);
void deleteCounseling(const char *student_id, const char *date);
void addMediaLink(const MediaLink *m);
void showMediaLinks(const char *course_code);
void updateMediaLink(const char *course_code, const char *title);
void deleteMediaLink(const char *course_code, const char *title);
void adminMenu();
void teacherMenu();
void studentMenu();
void clearInputBuffer();


void print_exam_bunny_logo(void) {
    puts("  (\\__/)");
    puts("  (> . <)          ExamBunny");
    puts("  /     \\   \"Check. Learn. Repeat.\"");
    puts(""); /* blank line */
}



// Function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// admin registration function
void registerAdmin() {
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
    if (!fp) {
        perror("Error opening admin file");
        return;
    }
    fprintf(fp, "%s %s %s|%s\n", a.admin_id, a.name, a.password, a.admin_role);
    fclose(fp);

    printf("Admin registered successfully!\n");
}




// ADMIN login function
int loginAdmin() {
    char id[20], password[50];
    printf("Enter Admin ID: ");
    scanf("%19s", id);
    printf("Enter Password: ");
    scanf("%49s", password);

    id[strcspn(id, "\n")] = 0;
    password[strcspn(password, "\n")] = 0;

    FILE *fp = fopen(ADMIN_FILE, "r");
    if (!fp) {
        perror("Error opening admin file");
        return 0;
    }

    admin a;
    char line[200];
    int found = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        for (int i = 0; line[i]; i++) {
            if (line[i] == '|') {
                line[i] = ' ';
            }
        }

        
        if (sscanf(line, "%19s %49s %49s %9s", a.admin_id, a.name, a.password, a.admin_role) == 4) {
            if (strcmp(id, a.admin_id) == 0 && strcmp(password, a.password) == 0) {
                found = 1;
                break;
            }
        }
    }

    fclose(fp);

    if (found) {
        printf(" Admin login successful. Welcome, %s! Role: %s\n", a.name, a.admin_role);
        return 1;
    } else {
        printf("Login failed. Invalid ID or password.\n");
        return 0;
    }
}




// STUDENT registration function
void registerUser () {
    user u;

    printf("Enter Student ID: ");
    scanf("%19s", u.student_id);
    printf("Enter Name: ");
    scanf(" %[^\n]", u.name);
    printf("Enter Password: ");
    scanf("%49s", u.password);
    

    FILE *fp = fopen(USER_FILE, "a");
    if (!fp) {
        perror("Error opening user file");
        return;
    }
    fprintf(fp, "%s %s %s\n", u.student_id, u.name, u.password);
    fclose(fp);

    printf("Student registered successfully!\n");
}




// USER login function
int loginUser() {
    char id[20], password[50];
    printf("Enter User ID: ");
    scanf("%19s", id);
    printf("Enter Password: ");
    scanf("%49s", password);

    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) {
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

    if (found) {
        printf(" User login successful. Welcome, %s!\n", u.name);
        return 1;  // success
    }
     else if(found==0) {
        printf(" Login failed. Invalid ID or password.\n");
        return 0;  // fail
    }
}





// Teacher registration function
void registerTeacher() {
    teacher t;

    printf("Enter Teacher ID: ");
    scanf("%19s", t.teacher_id);
    printf("Enter Name: ");
    scanf(" %[^\n]", t.name);
    printf("Enter Password: ");
    scanf("%49s", t.password);
    printf("Enter Subject: ");
    scanf(" %[^\n]", t.subject);

    FILE *fp = fopen(TEACHER_FILE, "a");
    if (!fp) {
        perror("Error opening teacher file");
        return;
    }
    fprintf(fp, "%s|%s|%s|%s\n", t.teacher_id, t.name, t.password, t.subject);
    fclose(fp);

    printf("Teacher registered successfully!\n");
}





// Teacher login function
int loginTeacher() {
    char id[20], password[50];
    printf("Enter Teacher ID: ");
    scanf("%19s", id);
    printf("Enter Password: ");
    scanf("%49s", password);

    FILE *fp = fopen(TEACHER_FILE, "r");
    if (!fp) {
        perror("Error opening teacher file");
        return 0; // failed login
    }

    teacher t;
    int found = 0;
    while (fscanf(fp, "%19[^|]|%49[^|]|%49[^|]|%29[^\n]\n", t.teacher_id, t.name, t.password, t.subject) == 4) {
        if (strcmp(id, t.teacher_id) == 0 && strcmp(password, t.password) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found) {
        printf(" Teacher login successful. Welcome, %s!\n", t.name);
        return 1;  // success
    } else {
        printf(" Login failed. Invalid ID or password.\n");
        return 0;  // fail
    }
}



// Update user function
void updateUser (user *u) {
    if (u == NULL) {
        printf("Invalid user pointer.\n");
        return;
    }

    FILE *fp = fopen(USER_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        perror("File error");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    user tempUser ;
    int found = 0;

    while (fscanf(fp, "%19[^|]|%49[^|]|%49[^|]|%9[^\n]\n", tempUser .student_id, tempUser .name, tempUser .password, tempUser .student_role) == 4) {
        if (strcmp(tempUser .student_id, u->student_id) == 0) {
            found = 1;
            // Write updated user info passed in 'u'
            fprintf(temp, "%s|%s|%s|%s\n", u->student_id, u->name, u->password, u->student_role);
        } else {
            // Write existing user info as-is
            fprintf(temp, "%s|%s|%s|%s\n", tempUser .student_id, tempUser .name, tempUser .password, tempUser .student_role);
        }
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        printf("User  with ID %s not found.\n", u->student_id);
        remove("temp.txt");
        return;
    }

    remove(USER_FILE);
    rename("temp.txt", USER_FILE);
    printf("User  updated successfully.\n");
}




// Delete user function
void deleteUser (user *u) {
    if (u == NULL) {
        printf("Invalid user pointer.\n");
        return;
    }

    FILE *fp = fopen(USER_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        perror("File error");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    user tempUser ;
    int found = 0;

    while (fscanf(fp, "%19[^|]|%49[^|]|%49[^|]|%9[^\n]\n", tempUser .student_id, tempUser .name, tempUser .password, tempUser .student_role) == 4) {
        if (strcmp(tempUser .student_id, u->student_id) == 0) {
            found = 1;
            // skip writing this user to delete
            continue;
        }
        fprintf(temp, "%s|%s|%s|%s\n", tempUser .student_id, tempUser .name, tempUser .password, tempUser .student_role);
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        printf("User  with ID %s not found.\n", u->student_id);
        remove("temp.txt");
        return;
    }

    remove(USER_FILE);
    rename("temp.txt", USER_FILE);
    printf("User  deleted successfully.\n");
}




// Display teacher function
void displayTeacher(const teacher *t) {
    printf("Teacher ID: %s\n", t->teacher_id);
    printf("Name: %s\n", t->name);
    printf("Subject: %s\n", t->subject);
}




// Update teacher function
void updateTeacher(teacher *t) {
    if (t == NULL) {
        printf("Invalid teacher pointer.\n");
        return;
    }

    FILE *fp = fopen("teacher.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        perror("File error");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    teacher tempTeacher;
    int found = 0;

    while (fscanf(fp, "%19[^|]|%49[^|]|%49[^|]|%29[^\n]\n", tempTeacher.teacher_id, tempTeacher.name, tempTeacher.password, tempTeacher.subject) == 4) {
        if (strcmp(tempTeacher.teacher_id, t->teacher_id) == 0) {
            found = 1;
            // Write updated teacher info
            fprintf(temp, "%s|%s|%s|%s\n", t->teacher_id, t->name, t->password, t->subject);
        } else {
            fprintf(temp, "%s|%s|%s|%s\n", tempTeacher.teacher_id, tempTeacher.name, tempTeacher.password, tempTeacher.subject);
        }
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        printf("Teacher with ID %s not found.\n", t->teacher_id);
        remove("temp.txt");
        return;
    }

    remove("teacher.txt");
    rename("temp.txt", "teacher.txt");

    printf("Teacher updated successfully.\n");
}




// Delete teacher function
void deleteTeacher(teacher *t) {
    if (t == NULL) {
        printf("Invalid teacher pointer.\n");
        return;
    }

    FILE *fp = fopen("teacher.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        perror("File error");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    teacher tempTeacher;
    int found = 0;

    while (fscanf(fp, "%19[^|]|%49[^|]|%49[^|]|%29[^\n]\n", tempTeacher.teacher_id, tempTeacher.name, tempTeacher.password, tempTeacher.subject) == 4) {
        if (strcmp(tempTeacher.teacher_id, t->teacher_id) == 0) {
            found = 1;
            // skip writing to delete
            continue;
        }
        fprintf(temp, "%s|%s|%s|%s\n", tempTeacher.teacher_id, tempTeacher.name, tempTeacher.password, tempTeacher.subject);
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        printf("Teacher with ID %s not found.\n", t->teacher_id);
        remove("temp.txt");
        return;
    }

    remove("teacher.txt");
    rename("temp.txt", "teacher.txt");

    printf("Teacher deleted successfully.\n");
}




// Add Course Function
void addCourse() {
    Course c;
    printf("Enter course name: ");
    clearInputBuffer(); // Clear any leftover input
    fgets(c.course_name, sizeof(c.course_name), stdin);
    c.course_name[strcspn(c.course_name, "\n")] = '\0';

    printf("Enter course code: ");
    scanf("%19s", c.course_code);

    FILE *fp = fopen("course.txt", "a");
    if (!fp) {
        perror("File open error");
        return;
    }

    fprintf(fp, "%s|%s\n", c.course_name, c.course_code);
    fclose(fp);

    printf("Course added successfully.\n");
}




// Show Courses Function
void showCourses() {
    FILE *fp = fopen("course.txt", "r");
    if (!fp) {
        printf("No courses found.\n");
        return;
    }

    Course c;
    printf("Course List:\n");
    printf("---------------\n");
    while (fscanf(fp, "%99[^|]|%19[^\n]\n", c.course_name, c.course_code) == 2) {
        printf("Course Name: %s\n", c.course_name);
        printf("Course Code: %s\n", c.course_code);
        printf("---------------\n");
    }
    fclose(fp);
}




// Update Course Function
void updateCourse() {
    char code[20];
    printf("Enter course code to update: ");
    scanf("%19s", code);

    FILE *fp = fopen("course.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        perror("File error");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    Course c;
    int found = 0;

    while (fscanf(fp, "%99[^|]|%19[^\n]\n", c.course_name, c.course_code) == 2) {
        if (strcmp(c.course_code, code) == 0) {
            found = 1;
            printf("Enter new course name: ");
            clearInputBuffer();
            fgets(c.course_name, sizeof(c.course_name), stdin);
            c.course_name[strcspn(c.course_name, "\n")] = '\0';

            printf("Enter new course code: ");
            scanf("%19s", c.course_code);
        }
        fprintf(temp, "%s|%s\n", c.course_name, c.course_code);
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        printf("Course with code %s not found.\n", code);
        remove("temp.txt");
        return;
    }

    remove("course.txt");
    rename("temp.txt", "course.txt");
    printf("Course updated successfully.\n");
}




// Delete Course Function
void deleteCourse() {
    char code[20];
    printf("Enter course code to delete: ");
    scanf("%19s", code);

    FILE *fp = fopen("course.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        perror("File error");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    Course c;
    int found = 0;

    while (fscanf(fp, "%99[^|]|%19[^\n]\n", c.course_name, c.course_code) == 2) {
        if (strcmp(c.course_code, code) == 0) {
            found = 1;
            // skip writing to delete course
            continue;
        }
        fprintf(temp, "%s|%s\n", c.course_name, c.course_code);
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        printf("Course with code %s not found.\n", code);
        remove("temp.txt");
        return;
    }

    remove("course.txt");
    rename("temp.txt", "course.txt");

    printf("Course deleted successfully.\n");
}




// Add Question Function
void addQuestion() {
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
    if (!fp) {
        perror("File open error");
        return;
    }

    // Write in the new format
    fprintf(fp, "%s, %s, A) %s, B) %s, C) %s, D) %s, %c\n", 
            q.course_code, q.question, 
            q.option_a, 
            q.option_b, 
            q.option_c, 
            q.option_d, 
            q.check);
    fflush(fp);
    fclose(fp);

    printf("Question added successfully.\n");
}



// Show Questions for Admin Function
void showQuestionsForAdmin() {
    FILE *fp = fopen("mcq.txt", "r");
    if (!fp) {
        printf("No questions found.\n");
        return;
    }

    MCQ q;
    printf("MCQ Questions with answers (Admin view):\n");
    printf("---------------------------------------\n");
    while (fscanf(fp, "%19[^|]|%299[^|]|%99[^|]|%99[^|]|%99[^|]|%99[^|]|%c\n",
                  q.course_code, q.question, q.option_a, q.option_b, q.option_c, q.option_d, &q.check) == 7) {
        printf("Course Code: %s\nQuestion: %s\n", q.course_code, q.question);
        printf("A) %s\nB) %s\nC) %s\nD) %s\n", q.option_a, q.option_b, q.option_c, q.option_d);
        printf("Correct Answer: %c\n", q.check);
        printf("---------------------------------------\n");
    }
    fclose(fp);
}




// Update Question Function
void updateQuestion() {
    char courseCode[20];
    printf("Enter course code of the question to update: ");
    scanf("%19s", courseCode);
    clearInputBuffer();

    char oldQuestion[300];
    printf("Enter the exact question text to update: ");
    fgets(oldQuestion, sizeof(oldQuestion), stdin);
    oldQuestion[strcspn(oldQuestion, "\n")] = '\0';

    // Trim whitespace from oldQuestion
    char *ptr = oldQuestion;
    while (*ptr == ' ') ptr++; // Trim leading spaces
    strcpy(oldQuestion, ptr); // Copy trimmed string back

    FILE *fp = fopen("mcq.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        perror("File error");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    MCQ q;
    int found = 0;

    while (fscanf(fp, "%19[^,], %299[^,], %99[^,], %99[^,], %99[^,], %99[^,], %c\n",
                  q.course_code, q.question, q.option_a, q.option_b, q.option_c, q.option_d, &q.check) == 7) {
        // Trim whitespace from q.question
        char *qQuestionTrimmed = q.question;
        while (*qQuestionTrimmed == ' ') qQuestionTrimmed++; // Trim leading spaces

        if (strcmp(q.course_code, courseCode) == 0 && strcmp(qQuestionTrimmed, oldQuestion) == 0) {
            found = 1;
            printf("Enter new question text: ");
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
            clearInputBuffer();
        }
        // Write the updated or unchanged question to the temp file
        fprintf(temp, "%s, %s, A) %s, B) %s, C) %s, D) %s, %c\n", 
                q.course_code, q.question, 
                q.option_a, 
                q.option_b, 
                q.option_c, 
                q.option_d, 
                q.check);
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        printf("Question not found.\n");
        remove("temp.txt");
        return;
    }

    remove("mcq.txt");
    rename("temp.txt", "mcq.txt");
    printf("Question updated successfully.\n");
}



// Delete Question Function
void deleteQuestion() {
    char courseCode[20];
    printf("Enter course code of the question to delete: ");
    scanf("%19s", courseCode);

    char questionText[300];
    printf("Enter the exact question text to delete: ");
    clearInputBuffer();
    fgets(questionText, sizeof(questionText), stdin);
    questionText[strcspn(questionText, "\n")] = '\0';

    // Trim whitespace from questionText
    char *ptr = questionText;
    while (*ptr == ' ') ptr++; // Trim leading spaces
    strcpy(questionText, ptr); // Copy trimmed string back

    FILE *fp = fopen("mcq.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        perror("File error");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    MCQ q;
    int found = 0;

    while (fscanf(fp, "%19[^,], %299[^,], %99[^,], %99[^,], %99[^,], %99[^,], %c\n",
                  q.course_code, q.question, q.option_a, q.option_b, q.option_c, q.option_d, &q.check) == 7) {
        // Trim whitespace from q.question
        char *qQuestionTrimmed = q.question;
        while (*qQuestionTrimmed == ' ') qQuestionTrimmed++; // Trim leading spaces

        if (strcmp(q.course_code, courseCode) == 0 && strcmp(qQuestionTrimmed, questionText) == 0) {
            found = 1;
            // skip writing to delete question
            continue;
        }
        fprintf(temp, "%s, %s, A) %s, B) %s, C) %s, D) %s, %c\n", 
                q.course_code, q.question, 
                q.option_a, 
                q.option_b, 
                q.option_c, 
                q.option_d, 
                q.check);
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        printf("Question not found.\n");
        remove("temp.txt");
        return;
    }

    remove("mcq.txt");
    rename("temp.txt", "mcq.txt");
    printf("Question deleted successfully.\n");
}





// Show Questions for Teacher Function
void showQuestionsForTeacher(const char *course_code) {
    FILE *fp = fopen("mcq.txt", "r");
    if (!fp) {
        printf("No questions available.\n");
        return;
    }

    MCQ q;
    int found = 0;

    printf("Questions for course %s:\n\n", course_code);
    while (fscanf(fp, "%19[^,], %299[^,], %99[^,], %99[^,], %99[^,], %99[^,], %c\n",
                  q.course_code, q.question, q.option_a, q.option_b, q.option_c, q.option_d, &q.check) == 7) {
        if (strcmp(q.course_code, course_code) == 0) {
            found = 1;
            printf("Q: %s\n", q.question);
            printf("A) %s\n", q.option_a); // Updated to show option letters
            printf("B) %s\n", q.option_b);
            printf("C) %s\n", q.option_c);
            printf("D) %s\n", q.option_d);
            printf("Correct answer: %c\n", q.check);
            printf("------------------------------\n");
        }
    }

    fclose(fp);

    if (!found) {
        printf("No questions found for this course.\n");
    }
}



// Function to conduct exam
void studentExam(const char *student_id, const char *course_code) {
    FILE *fp = fopen("mcq.txt", "r");
    if (!fp) {
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
                  &questions[total_questions].check) == 7) {
        if (strcmp(questions[total_questions].course_code, course_code) == 0) {
            total_questions++;
            if (total_questions >= 100) break;
        }
    }
    fclose(fp);

    if (total_questions == 0) {
        printf("No questions found for course %s.\n", course_code);
        return;
    }

    printf("Starting exam for course: %s\n", course_code);
    printf("Total questions: %d\n", total_questions);

    int score = 0;
    WrongAnswer wrongAnswers[100];
    int wrongCount = 0;

    for (int i = 0; i < total_questions; i++) {
        printf("\nQ%d: %s\n", i + 1, questions[i].question);
        printf("a) %s\n", questions[i].option_a);
        printf("b) %s\n", questions[i].option_b);
        printf("c) %s\n", questions[i].option_c);
        printf("d) %s\n", questions[i].option_d);

        char ans;
        do {
            printf("Enter your answer (a/b/c/d): ");
            scanf(" %c", &ans);
            ans = tolower(ans);
        } while (ans != 'a' && ans != 'b' && ans != 'c' && ans != 'd');

        if (ans == questions[i].check) {
            score++;
        } else {
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
    if (resfp) {
        fprintf(resfp, "%s|%s|%d\n", student_id, course_code, score);
        fclose(resfp);
    } else {
        printf("Failed to save exam result.\n");
    }


    // Save wrong answers for review later
    FILE *wfp = fopen("wrong_answers.txt", "a");
    if (wfp) {
        for (int i = 0; i < wrongCount; i++) {
            fprintf(wfp, "%s|%s|%c|%c\n", student_id, wrongAnswers[i].question, wrongAnswers[i].correct_answer, wrongAnswers[i].student_answer);
        }
        fclose(wfp);
    }


    // Show review option
    char choice;
    printf("Do you want to review your wrong answers? (y/n): ");
    scanf(" %c", &choice);
    if (tolower(choice) == 'y') {
        printf("\nWrong answers review:\n");
        for (int i = 0; i < wrongCount; i++) {
            printf("Q: %s\n", wrongAnswers[i].question);
            printf("Your answer: %c | Correct answer: %c\n", wrongAnswers[i].student_answer, wrongAnswers[i].correct_answer);
            printf("-----------------------------\n");
        }
    }
}


//exam instructtion
void examRules() {
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

//function to add exam result
void addExamResult(ExamResult *result) {
    FILE *fp = fopen("exam_results.txt", "a");
    if (!fp) {
        printf("Error: Unable to open results file.\n");
        return;
    }
    fprintf(fp, "%s|%s|%d\n", result->student_id, result->course_code, result->score);
    fclose(fp);
}


//display result
void displayExamResult(const ExamResult *result) {
    printf("Student ID: %s\n", result->student_id);
    printf("Course Code: %s\n", result->course_code);
    printf("Score: %d\n", result->score);
    printf("-------------------------\n");
}


//function to show all result
void showAllResults(const char *student_id) {
    FILE *fp = fopen("exam_results.txt", "r");
    if (!fp) {
        printf("No exam results found.\n");
        return;
    }

    ExamResult result;
    int found = 0;

    printf("Exam Results for Student ID: %s\n", student_id);
    printf("------------------------------\n");

    while (fscanf(fp, "%19[^|]|%19[^|]|%d\n", result.student_id, result.course_code, &result.score) == 3) {
        if (strcmp(result.student_id, student_id) == 0) {
            displayExamResult(&result);
            found = 1;
        }
    }

    fclose(fp);

    if (!found) {
        printf("No exam results found for this student.\n");
    }
}



// Add counseling
void addCounseling(const Counseling *c) {
    FILE *fp = fopen(COUNSELING_FILE, "a");
    if (!fp) {
        printf("Error opening counseling file.\n");
        return;
    }
    fprintf(fp, "%s|%s|%s|%s|%s|%s|%s\n", c->student_id, c->student_name, c->teacher_id, c->teacher_name, c->date, c->topic, c->notes);
    fclose(fp);
    printf("Counseling record added.\n");
}



// Show counseling records for a student
void showCounselings(const char *student_id) {
    FILE *fp = fopen(COUNSELING_FILE, "r");
    if (!fp) {
        printf("No counseling records found.\n");
        return;
    }

    Counseling c;
    int found = 0;
    printf("Counseling records for student ID: %s\n", student_id);
    printf("------------------------------------\n");

    while (fscanf(fp, "%19[^|]|%49[^|]|%19[^|]|%49[^|]|%19[^|]|%99[^|]|%299[^\n]\n",
                  c.student_id, c.student_name, c.teacher_id, c.teacher_name, c.date, c.topic, c.notes) == 7) {
        if (strcmp(c.student_id, student_id) == 0) {
            found = 1;
            printf("Date: %s\nTeacher: %s (%s)\nTopic: %s\nNotes: %s\n--------------------------\n",
                   c.date, c.teacher_name, c.teacher_id, c.topic, c.notes);
        }
    }
    fclose(fp);

    if (!found) {
        printf("No counseling records found for this student.\n");
    }
}



// Update counseling (match by student_id and date)
void updateCounseling(const char *student_id, const char *date) {
    FILE *fp = fopen(COUNSELING_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening files.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    Counseling c;
    int updated = 0;

    while (fscanf(fp, "%19[^|]|%49[^|]|%19[^|]|%49[^|]|%19[^|]|%99[^|]|%299[^\n]\n",
                  c.student_id, c.student_name, c.teacher_id, c.teacher_name, c.date, c.topic, c.notes) == 7) {
        if (strcmp(c.student_id, student_id) == 0 && strcmp(c.date, date) == 0) {
            updated = 1;
            printf("Current Topic: %s\n", c.topic);
            printf("Enter new topic (press Enter to keep current): ");
            char newTopic[100];
            fgets(newTopic, sizeof(newTopic), stdin);
            newTopic[strcspn(newTopic, "\n")] = '\0';
            if (strlen(newTopic) > 0) {
                strcpy(c.topic, newTopic);
            }

            printf("Current Notes: %s\n", c.notes);
            printf("Enter new notes (press Enter to keep current): ");
            char newNotes[300];
            fgets(newNotes, sizeof(newNotes), stdin);
            newNotes[strcspn(newNotes, "\n")] = '\0';
            if (strlen(newNotes) > 0) {
                strcpy(c.notes, newNotes);
            }
        }
        fprintf(temp, "%s|%s|%s|%s|%s|%s|%s\n",
                c.student_id, c.student_name, c.teacher_id, c.teacher_name, c.date, c.topic, c.notes);
    }

    fclose(fp);
    fclose(temp);

    if (updated) {
        remove(COUNSELING_FILE);
        rename("temp.txt", COUNSELING_FILE);
        printf("Counseling record updated successfully.\n");
    } else {
        remove("temp.txt");
        printf("No matching counseling record found.\n");
    }
}



// Delete counseling (match by student_id and date)
void deleteCounseling(const char *student_id, const char *date) {
    FILE *fp = fopen(COUNSELING_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening files.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    Counseling c;
    int deleted = 0;

    while (fscanf(fp, "%19[^|]|%49[^|]|%19[^|]|%49[^|]|%19[^|]|%99[^|]|%299[^\n]\n",
                  c.student_id, c.student_name, c.teacher_id, c.teacher_name, c.date, c.topic, c.notes) == 7) {
        if (strcmp(c.student_id, student_id) == 0 && strcmp(c.date, date) == 0) {
            deleted = 1;
            continue;
        }
        fprintf(temp, "%s|%s|%s|%s|%s|%s|%s\n",
                c.student_id, c.student_name, c.teacher_id, c.teacher_name, c.date, c.topic, c.notes);
    }

    fclose(fp);
    fclose(temp);

    if (deleted) {
        remove(COUNSELING_FILE);
        rename("temp.txt", COUNSELING_FILE);
        printf("Counseling record deleted successfully.\n");
    } else {
        remove("temp.txt");
        printf("No matching counseling record found.\n");
    }
}



// Add media link
void addMediaLink(const MediaLink *m) {
    FILE *fp = fopen(MEDIA_FILE, "a");
    if (!fp) {
        printf("Error opening media file.\n");
        return;
    }
    fprintf(fp, "%s|%s|%s|%s\n", m->title, m->course_code, m->media_type, m->url);
    fclose(fp);
    printf("Media link added successfully.\n");
}



// Show all media links for a course
void showMediaLinks(const char *course_code) {
    FILE *fp = fopen(MEDIA_FILE, "r");
    if (!fp) {
        printf("No media links found.\n");
        return;
    }

    MediaLink m;
    int found = 0;

    printf("Media links for course %s:\n", course_code);
    printf("--------------------------\n");

    while (fscanf(fp, "%99[^|]|%19[^|]|%19[^|]|%199[^\n]\n",
                  m.title, m.course_code, m.media_type, m.url) == 4) {
        if (strcmp(m.course_code, course_code) == 0) {
            found = 1;
            printf("Title: %s\n", m.title);
            printf("Type: %s\n", m.media_type);
            printf("URL: %s\n", m.url);
            printf("--------------------------\n");
        }
    }

    fclose(fp);

    if (!found) {
        printf("No media links found for this course.\n");
    }
}



// Update media link by course_code and title
void updateMediaLink(const char *course_code, const char *title) {
    FILE *fp = fopen(MEDIA_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening files.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    MediaLink m;
    int updated = 0;

    while (fscanf(fp, "%99[^|]|%19[^|]|%19[^|]|%199[^\n]\n",
                  m.title, m.course_code, m.media_type, m.url) == 4) {
        if (strcmp(m.course_code, course_code) == 0 && strcmp(m.title, title) == 0) {
            updated = 1;
            printf("Current Type: %s\n", m.media_type);
            printf("Enter new type (press Enter to keep current): ");
            char newType[20];
            fgets(newType, sizeof(newType), stdin);
            newType[strcspn(newType, "\n")] = '\0';
            if (strlen(newType) > 0) {
                strcpy(m.media_type, newType);
            }

            printf("Current URL: %s\n", m.url);
            printf("Enter new URL (press Enter to keep current): ");
            char newUrl[200];
            fgets(newUrl, sizeof(newUrl), stdin);
            newUrl[strcspn(newUrl, "\n")] = '\0';
            if (strlen(newUrl) > 0) {
                strcpy(m.url, newUrl);
            }
        }
        fprintf(temp, "%s|%s|%s|%s\n", m.title, m.course_code, m.media_type, m.url);
    }

    fclose(fp);
    fclose(temp);

    if (updated) {
        remove(MEDIA_FILE);
        rename("temp.txt", MEDIA_FILE);
        printf("Media link updated successfully.\n");
    } else {
        remove("temp.txt");
        printf("No matching media link found.\n");
    }
}




// Admin Menu
void adminMenu() {
    int choice;
    char admin_id[20] = "";
    
    while(1) {
        printf("\n=== ADMIN MENU ===\n");
        printf("1. Add Course\n");
        printf("2. View Courses\n");
        printf("3. Update Course\n");
        printf("4. Delete Course\n");
        printf("5. View Teacher Information\n");
        printf("6. View Student Information\n");
        printf("7. Delete Teacher\n");
        printf("8. Delete Student\n");
        printf("9. Show Questions\n");
        printf("10. Show All Results\n");
        printf("11. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch(choice) {
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
                // View Teacher Information implementation
                printf("Teacher information functionality\n");
                break;
            case 6:
                // View Student Information implementation
                printf("Student information functionality\n");
                break;
            case 7:
                // Delete Teacher implementation
                printf("Delete teacher functionality\n");
                break;
            case 8:
                // Delete Student implementation
                printf("Delete student functionality\n");
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
void studentMenu() {
    int choice;
    char student_id[20] = "";
    
    while(1) {
        printf("\n=== STUDENT MENU ===\n");
        printf("1. View Courses\n");
        printf("2. Take Exam\n");
        printf("3. Exam Instructions\n");
        printf("4. View Previous Results\n");
        printf("5. View Counseling\n");
        printf("6. View Media Links\n");
        printf("7. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch(choice) {
            case 1:
                showCourses();
                break;
            case 2:
                if(strlen(student_id) > 0) {
                    char course_code[20];
                    printf("Enter course code to take exam: ");
                    fgets(course_code, sizeof(course_code), stdin);
                    course_code[strcspn(course_code, "\n")] = '\0';
                    studentExam(student_id, course_code);
                } else {
                    printf("Please login first.\n");
                }
                break;
            case 3:
                examRules();
                break;
            case 4:
                if(strlen(student_id) > 0) {
                    showAllResults(student_id);
                } else {
                    printf("Please login first.\n");
                }
                break;
            case 5:
                if(strlen(student_id) > 0) {
                    showCounselings(student_id);
                } else {
                    printf("Please login first.\n");
                }
                break;
            case 6: {
                char course_code[20];
                printf("Enter course code to view Media Links: ");
                fgets(course_code, sizeof(course_code), stdin);
                course_code[strcspn(course_code, "\n")] = '\0';
                // Filter for Media Links only
                showMediaLinks(course_code);
                break;
            }
            case 7:
                printf("Logged out successfully.\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}



// Teacher Menu
void teacherMenu() {
    int choice;
    char teacher_id[20] = "";
    
    while(1) {
        printf("\n=== TEACHER MENU ===\n");
        printf("1. Add Question\n");
        printf("2. Delete Question\n");
        printf("3. Update Question\n");
        printf("4. Add Media Link\n");
        printf("5. View Student Results\n");
        printf("6. Counseling Messages\n");
        printf("7. Update Counseling\n");
        printf("8. Delete Counseling\n");
        printf("9. Show Questions\n");
        printf("10. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch(choice) {
            case 1:
                addQuestion();
                break;
            case 2:
                deleteQuestion();
                break;
            case 3:
                updateQuestion();
                break;
            case 4: {
                MediaLink m;
                printf("Enter course code: ");
                fgets(m.course_code, sizeof(m.course_code), stdin);
                m.course_code[strcspn(m.course_code, "\n")] = '\0';
                printf("Enter title: ");
                fgets(m.title, sizeof(m.title), stdin);
                m.title[strcspn(m.title, "\n")] = '\0';
                strcpy(m.media_type, " link ");
                printf("Enter URL: ");
                fgets(m.url, sizeof(m.url), stdin);
                m.url[strcspn(m.url, "\n")] = '\0';
                addMediaLink(&m);
                break;
            }
            case 5:
                showAllResults("");
                break;
            case 6:
                showCounselings("");
                break;
            case 7: {
                char student_id[20], date[20];
                printf("Enter student ID: ");
                fgets(student_id, sizeof(student_id), stdin);
                student_id[strcspn(student_id, "\n")] = '\0';
                printf("Enter date (DD/MM/YYYY): ");
                fgets(date, sizeof(date), stdin);
                date[strcspn(date, "\n")] = '\0';
                updateCounseling(student_id, date);
                break;
            }
            case 8: {
                char student_id[20], date[20];
                printf("Enter student ID: ");
                fgets(student_id, sizeof(student_id), stdin);
                student_id[strcspn(student_id, "\n")] = '\0';
                printf("Enter date (DD/MM/YYYY): ");
                fgets(date, sizeof(date), stdin);
                date[strcspn(date, "\n")] = '\0';
                deleteCounseling(student_id, date);
                break;
            }
            case 9:
                printf("Enter course code to show questions: ");
                char course_code[20];
                fgets(course_code, sizeof(course_code), stdin);
                course_code[strcspn(course_code, "\n")] = '\0';
                showQuestionsForTeacher(course_code);
                break;

            case 10:
                printf("Logged out successfully.\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}





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

        const char *role = (user_choice == 1) ? "student" :
                           (user_choice == 2) ? "teacher" :
                           (user_choice == 3) ? "admin" : NULL;

        if (!role)
        {
            printf("Invalid user type.\n");
            continue;
        }

        printf("\n----------%s---------\n", (user_choice == 1) ? "Student" :
                                              (user_choice == 2) ? "Teacher" :
                                              "Admin");
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