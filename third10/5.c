#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    SUCCESS = 0,
    INCORRECT_INPUT,
    OVERFLOW_ERROR,
    MALLOC_ERROR,
    FILE_OPENING_ERROR,
    FILE_READING_ERROR,
    UNKNOWN_ERROR
} ErrorCode;

static const char* errorMessages[] = {
    "Всё хорошо, можно идти пить чай ☕",
    "Некорректный ввод, попробуйте ещё раз 🤨",
    "Произошло переполнение, ой 🤯",
    "Проблемы с выделением памяти, грустно 😐",
    "Не удалось открыть файл, грустно 😥",
    "Файл прочитан не полностью, грустно 😿",
    "Неизвестная ошибка, что-то пошло не так 🫢"
};

#define BUFFER_SIZE 256

typedef struct{
    unsigned int id;
    char firstName[50];
    char lastName[50];
    char group[50];
    unsigned char *marks;
} Student;

char* getInput(char buffer[BUFFER_SIZE], FILE *file) {
    memset(buffer, '\0', BUFFER_SIZE);
    return fgets(buffer, BUFFER_SIZE+1, file);
}

void closeFiles(FILE *fileIn, FILE *fileOut) {
    if (fileIn != NULL)
        fclose(fileIn);
    if (fileOut != NULL)
        fclose(fileOut);
}

int compare(const void *x, const void *y) {
    Student S1 = *(Student*)x;
    Student S2 = *(Student*)y;

    if (S1.id - S2.id != 0)
        return S1.id - S2.id;

    if (strcmp(S1.lastName, S2.lastName) != 0)
        return strcmp(S1.lastName, S2.lastName);
    
    if (strcmp(S1.firstName, S2.firstName) != 0)
        return strcmp(S1.firstName, S2.firstName);

    return strcmp(S1.group, S2.group);
}

int cmpID(const void *x, const void *y) {
    Student S1 = *(Student*)x;
    Student S2 = *(Student*)y;
    return S1.id - S2.id;
}

int cmpLastName(const void *x, const void *y) {
    Student S1 = *(Student*)x;
    Student S2 = *(Student*)y;
    return strcmp(S1.lastName, S2.lastName);
}

int cmpName(const void *x, const void *y) {
    Student S1 = *(Student*)x;
    Student S2 = *(Student*)y;
    return strcmp(S1.firstName, S2.firstName);
}

int cmpGroup(const void *x, const void *y) {
    Student S1 = *(Student*)x;
    Student S2 = *(Student*)y;
    return strcmp(S1.group, S2.group);
}

int validateMarks(unsigned char arr[5]) {
    if (!arr)
        return 0;
    for (int i = 0; i < 5; ++i) {
        if (arr[i] >= '2' && arr[i] <= '5')
            continue;
        return 0;
    }
    return 1;
}

void putData(FILE *fileOut, Student *students, int studCnt) {
    if (!fileOut || !students || 0 >= studCnt)
        return;

    for (int i = 0; i < studCnt; ++i) {
        Student student = *(students+i);
        if (!student.marks)
            return;
        fprintf(fileOut, "%u %s %s %s %c %c %c %c %c\n", 
            student.id, 
            student.firstName, 
            student.lastName, 
            student.group,
            student.marks[0],
            student.marks[1],
            student.marks[2],
            student.marks[3],
            student.marks[4]);
    }
    fflush(fileOut);
}

void putIdData(FILE *fileOut, Student *students, int studCnt) {
    if (!fileOut || !students || 0 >= studCnt)
        return;

    for (int i = 0; i < studCnt; ++i) {
        Student student = *(students+i);
        if (!student.marks)
            return;
        double localAverage = 0;
        for (int j = 0; j < 5; ++j) 
            localAverage += student.marks[j]-'0';
        localAverage /= 5;

        // fprintf(fileOut, "%u %s %s %s %c %c %c %c %c\n", 
        //     student.id, 
        //     student.firstName, 
        //     student.lastName, 
        //     student.group,
        //     student.marks[0],
        //     student.marks[1],
        //     student.marks[2],
        //     student.marks[3],
        //     student.marks[4]);
        fprintf(fileOut, "%s %s %s %f\n", 
            student.firstName, 
            student.lastName, 
            student.group,
            localAverage);
    }
    fflush(fileOut);
}

void putHigherThanAverageData(FILE *fileOut, Student *students, int studCnt) {
    if (!fileOut || !students || 0 >= studCnt)
        return;

    double average = 0;
    for (int i = 0; i < studCnt; ++i) {
        Student student = *(students+i);
        if (!student.marks)
            return;
        double localAverage = 0;
        for (int j = 0; j < 5; ++j) 
            localAverage += student.marks[j]-'0';
        localAverage /= 5;

        average += localAverage;
    }
    average /= studCnt;

    for (int i = 0; i < studCnt; ++i) {
        Student student = *(students+i);
        if (!student.marks)
            return;
        double localAverage = 0;
        for (int j = 0; j < 5; ++j) 
            localAverage += student.marks[j]-'0';
        localAverage /= 5;

        if (localAverage <= average)
            continue;
        // fprintf(fileOut, "%u %s %s %s %c %c %c %c %c\n", 
        //     student.id, 
        //     student.firstName, 
        //     student.lastName, 
        //     student.group,
        //     student.marks[0],
        //     student.marks[1],
        //     student.marks[2],
        //     student.marks[3],
        //     student.marks[4]);
        fprintf(fileOut, "%s %s\n",
            student.firstName, 
            student.lastName);
        
    }
    fflush(fileOut);
}

ErrorCode searchData(char search[], char mode, Student *database, int dataSize, Student **answ, int *answSize) {
    if (!database || 0 >= dataSize || !answ || !answSize || !search)
        return INCORRECT_INPUT;
    
    int boolEq;
    unsigned int id;
    if (mode == 'i') {
        char *endptr;
        id = strtoul(search, &endptr, 10);
        if (*endptr != '\0')
            return INCORRECT_INPUT;
    }

    Student *students = (Student*)malloc(1);
    if (students == NULL) {
        return MALLOC_ERROR;
    }
    int studCnt = 0;

    for (int i = 0; i < dataSize; ++i) {
        // Student stud = database[i];

        switch (mode) {
            default:
                boolEq = 0;
                break;
            case 'i':
                boolEq = database[i].id == id;
                break;
            case 'l':
                boolEq = (strcmp(database[i].lastName, search) == 0);
                break;
            case 'f':
                boolEq = (strcmp(database[i].firstName, search) == 0);
                break;
            case 'g':
                boolEq = (strcmp(database[i].group, search) == 0);
                break;
        }

        if (boolEq) {
            Student *tempPtr = (Student*)realloc(students, sizeof(Student) * ++studCnt);
            if (tempPtr == NULL) {
                --studCnt;
                if (students != NULL)
                    free(students);
                return MALLOC_ERROR;
            }
            students = tempPtr;

            students[studCnt-1] = database[i];
            // students[studCnt-1] = stud;
        }
    }

    if (studCnt == 0) {
        free(students);
        students = NULL;
    }
    *answ = students;
    *answSize = studCnt;
    // putData(stdout, *students, studCnt);
    return SUCCESS;
}

ErrorCode getData(FILE *fileIn, Student **database, int *dataSize) {
    if (!fileIn || !database || !dataSize)
        return INCORRECT_INPUT;

    Student *students = (Student*)malloc(1);
    if (students == NULL) {
        return MALLOC_ERROR;
    }
    int studCnt = 0;
    char buffer[BUFFER_SIZE] = "";
    while (getInput(buffer, fileIn) != NULL) {
        if (buffer[BUFFER_SIZE-1] != '\0') {
            for (int i = 0; i < studCnt; ++i)
                free(students[i].marks);
            free(students);
            return OVERFLOW_ERROR;
        }

        Student student;
        memset(student.firstName, '\0', sizeof(student.firstName));
        memset(student.lastName, '\0', sizeof(student.lastName));
        memset(student.group, '\0', sizeof(student.group));
        student.marks = (unsigned char*)malloc(5 * sizeof(unsigned char));
        if (student.marks == NULL) {
            if (students != NULL) {
                for (int i = 0; i < studCnt; ++i)
                    free(students[i].marks);
                free(students);
            }
            return MALLOC_ERROR;
        }
        char empty;
        if (sscanf(buffer, "%u %49s %49s %49s %c %c %c %c %c %c", 
            &student.id, 
            student.firstName, 
            student.lastName,
            student.group,
            &student.marks[0],
            &student.marks[1],
            &student.marks[2],
            &student.marks[3],
            &student.marks[4],
            &empty) != 9 || validateMarks(student.marks) == 0) 
        {
            printf("%s\n", errorMessages[INCORRECT_INPUT]);
            printf("%s", buffer);
            fflush(stdout);
            continue;
        }

        Student *tempPtr = (Student*)realloc(students, sizeof(Student) * ++studCnt);
        if (tempPtr == NULL) {
            --studCnt;
            if (students != NULL) {
                for (int i = 0; i < studCnt; ++i)
                    free(students[i].marks);
                free(students);
            }
            return MALLOC_ERROR;
        }
        students = tempPtr;

        students[studCnt-1] = student;
    }

    *database = students;
    *dataSize = studCnt;
    return SUCCESS;
}
    
int startCmdSession(Student *students, int studCnt, FILE *fileOut)
{
    /*
    0 - ошибка
    1 - успешное завершение
    */
    Student *answ;
    int answSize;
    while (1)
    {
        printf("> ");

        char buffer[BUFFER_SIZE];
        for (int i = 0; i < BUFFER_SIZE - 1; ++i) buffer[i] = '\0';
        if (!fgets(buffer, sizeof(buffer), stdin)) 
        {
            printf("- переполнение буфера\n");
            return 0;
        }
        // Очищаем буфер ввода
        if (!(buffer[BUFFER_SIZE - 2] == '\0' || buffer[BUFFER_SIZE - 2] == '\n'))
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }

        // для разделения Howmuch date -s нужен пробел
        char* command = strtok(buffer, " \n");
        if (command == NULL) 
        {

        }
        else if (strcmp(command, "Logout") == 0)
        {
            printf("Выход...\n\n");
            return 1;
        }
        else if (strcmp(command, "WriteBestStudents") == 0)
        {
            putHigherThanAverageData(fileOut, students, studCnt);
        } 
        else if (strcmp(command, "SortID") == 0)
        {   
            qsort(students, studCnt, sizeof(Student), cmpID);
            putData(stdout, students, studCnt);
        } 
        else if (strcmp(command, "SortSurname") == 0)
        {   
            qsort(students, studCnt, sizeof(Student), cmpLastName);
            putData(stdout, students, studCnt);
        } 
        else if (strcmp(command, "SortName") == 0)
        {   
            qsort(students, studCnt, sizeof(Student), cmpName);
            putData(stdout, students, studCnt);
        } 
        else if (strcmp(command, "SortGroup") == 0)
        {   
            qsort(students, studCnt, sizeof(Student), cmpGroup);
            putData(stdout, students, studCnt);
        } 
        else if (strcmp(command, "WriteIdStudent") == 0)
        {   
            // подходит для поискаID 37\n
            char* argum = strtok(NULL, " \n");

            if (argum == NULL) {
                printf(" аргумент: <id>\n");
                continue;
            }

            ErrorCode code = searchData(argum, 'i', students, studCnt, &answ, &answSize);
            if (code != SUCCESS)
                printf(" %s\n", errorMessages[code]);
            else if (answSize == 0)
                printf(" студент не найден\n");
            else {
                putIdData(fileOut, answ, answSize);
                free(answ);
            }
        } 
        else if (strcmp(command, "SearchID") == 0)
        {   
            char* argum = strtok(NULL, " \n");

            if (argum == NULL) {
                printf(" аргумент: <id>\n");
                continue;
            }

            ErrorCode code = searchData(argum, 'i', students, studCnt, &answ, &answSize);
            if (code != SUCCESS)
                printf(" %s\n", errorMessages[code]);
            else if (answSize == 0)
                printf(" студент не найден\n");
            else {
                putData(stdout, answ, answSize);
                free(answ);
            }
        } 
        else if (strcmp(command, "SearchName") == 0)
        {   
            char* argum = strtok(NULL, " \n");

            if (argum == NULL) {
                printf(" аргумент: <first_name>\n");
                continue;
            }

            ErrorCode code = searchData(argum, 'f', students, studCnt, &answ, &answSize);
            if (code != SUCCESS)
                printf(" %s\n", errorMessages[code]);
            else if (answSize == 0)
                printf(" студент не найден\n");
            else {
                putData(stdout, answ, answSize);
                free(answ);
            }
        } 
        else if (strcmp(command, "SearchSurname") == 0)
        {   
            char* argum = strtok(NULL, " \n");

            if (argum == NULL) {
                printf(" аргумент: <last_name>\n");
                continue;
            }

            ErrorCode code = searchData(argum, 'l', students, studCnt, &answ, &answSize);
            if (code != SUCCESS)
                printf(" %s\n", errorMessages[code]);
            else if (answSize == 0)
                printf(" студент не найден\n");
            else {
                putData(stdout, answ, answSize);
                free(answ);
            }
        } 
        else if (strcmp(command, "SearchGroup") == 0)
        {   
            char* argum = strtok(NULL, " \n");

            if (argum == NULL) {
                printf(" аргумент: <group_name>\n");
                continue;
            }

            ErrorCode code = searchData(argum, 'g', students, studCnt, &answ, &answSize);
            if (code != SUCCESS)
                printf(" %s\n", errorMessages[code]);
            else if (answSize == 0)
                printf(" группа не найдена\n");
            else {
                putData(stdout, answ, answSize);
                free(answ);
            }
        } 
        else if (strcmp(command, "Help") == 0)
        {   
            printf(" возможные команды: SearchGroup, SearchSurname, WriteBestStudents, SortName, SortID, Logout...\n");
        } 
        else 
        {
            printf(" ❗ команда не найдена\n");
        }
    }
    printf("Выход...\n\n");
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("%s <входной файл> <выходной файл>\n", argv[0]);
        return INCORRECT_INPUT;
    }

    if (strcmp(argv[1], argv[2]) == 0) {
        printf("<входной файл> совпадает с <выходной файл>\n");
        return INCORRECT_INPUT;
    }

    FILE *fileIn = fopen(argv[1], "r");
    FILE *fileOut = fopen(argv[2], "w");
    if (fileIn == NULL || fileOut == NULL) {
        printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
        fflush(stdout);
        closeFiles(fileIn, fileOut);
        return FILE_OPENING_ERROR;
    }

    Student *students;
    int studCnt;
    ErrorCode code = getData(fileIn, &students, &studCnt);
    switch (code) {
        case SUCCESS:
            break;
        default:
            printf("%s\n", errorMessages[code]);
            fflush(stdout);
            closeFiles(fileIn, fileOut);
            return code;
    }

    printf("\nВведите команду или 'Help':\n");
    fflush(stdout);
    
    int code1 = startCmdSession(students, studCnt, fileOut);
    
    // putData(fileOut, students, studCnt);
    // putHigherThanAverageData(stdout, students, studCnt);
    // Student *answer;
    // int answSize;
    // searchData("PqusQwQO", 'g', students, studCnt, &answer, &answSize);
    // if (answSize != 0)
    //     putData(stdout, answer, answSize);

    free(students);
    closeFiles(fileIn, fileOut);
    if (code1)
        return SUCCESS;
    else
        return UNKNOWN_ERROR;
}