//#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
//#include <wait.h>
#include <sys/wait.h>
#include <time.h>

#define DEBUG 0

#ifdef DEBUG
#define DEBUG_PRINT(format, ...) printf("Debug: "); printf(format, ##__VA_ARGS__); fflush(stdout)
#else
#define DEBUG_PRINT(msg)
#endif

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);

const int LOGIN_LIMIT = 6;
const int PASSWORD_LIMIT = 6;
const int SANCTIONS_LIMIT = 6;
const int BUFFER_SIZE = 24;

void printHelp();
int validateCh(char ch);
int validateAuthData(const char login[], const char password[]);
int auth(const char login[], const char password[], int* sanctions);
int reg(const char login[], const char password[]);
int login(int* sanctions);
int startCmdSession(int sanctions);
int setSanctions(const char login[], int sanctions);

int main(int argc, char *argv[]) 
{
    while (1) 
    {
        int sanctions = 0;
        if (!login(&sanctions)) return 1;

        int pid = fork();

        if (pid == 0)
        {
            if (!startCmdSession(sanctions))
            {
                return 1;
            } else 
            {
                return 0;
            }
        }
        else 
        {
            // ждём завершение дочернего процесса
            int status;
            waitpid(pid, &status, 0);
            if (status != 0) 
            {
                printf("⚠️ Сессия закончилась аварийно, грустно\n")
            }
            DEBUG_PRINT("Child process stat = %d\n", status);
        }
    }

    return 0;
}

void printHelp() 
{
    printf("Примитивная оболочка командной строки\n"\
    "Time -> запрос текущего времени в стандартном формате чч:мм:сс"\
    "Date -> запрос текущей даты в стандартном формате дд:мм:гггг"\
    "Howmuch дд:мм:гггг -flag -> запрос прошедшего времени с указанной даты в s,m,h,y"\
    "Logout -> выйти в меню авторизации"\
    "Sanctions username 0 -> ввод санкций на сеанс для пользователя"
    );
}

int validateCh(char ch) 
{
    if ('0' <= ch && ch <= '9')
        return 1;
    if ('a' <= ch && ch <= 'z')
        return 1;
    if ('A' <= ch && ch <= 'Z')
        return 1;
    return 0;
}

int validateAuthData(const char login[], const char password[]) {
    if (login == NULL || password == NULL)
        return 0;
    if (login[0] == '\0' || password[0] == '\0')
        return 0;
    for (int i = 0; i <= LOGIN_LIMIT; ++i) 
    {
        if (login[i] == '\0') break;
        if (i == LOGIN_LIMIT) return 0; // слишком длинный логин
        if (!validateCh(login[i])) return 0;
    }
    for (int i = 0; i <= PASSWORD_LIMIT; ++i) 
    {
        if (password[i] == '\0') break;
        if (i == PASSWORD_LIMIT) return 0; // слишком длинный пароль
        if (!isdigit(password[i])) return 0;
    }
    int intPassword = (int)strtol(password, NULL, 10);
    if (!(0 <= intPassword && intPassword <= 100000)) return 0;
    return 1;
}

int auth(const char login[], const char password[], int* sanctions) 
{   /*
    0 - пользователь не найден
    1 - успешная авторизация
    2 - неверный пароль
    3 - проблемы с открытием файла
    */
    FILE *fSanct;
    char filename[LOGIN_LIMIT + 15];
    for (int i = 0; i < LOGIN_LIMIT + 15; ++i) filename[i] = '\0';
    strcat(filename, login);
    strcat(filename, "-sanctions.txt");
    fSanct = fopen(filename, "r");
    if (fSanct == NULL) 
    {
        *sanctions = 0;
    }
    else 
    {
        char buffer[BUFFER_SIZE];
        for (int i = 0; i < BUFFER_SIZE; ++i) buffer[i] = '\0';
        if (fgets(buffer, sizeof(buffer), fSanct) != NULL) 
        {
            *sanctions = atoi(buffer);
        }
        else 
        {
            *sanctions = 0;
        }
        fclose(fSanct);
    }

    FILE *file;
    file = fopen("logins.txt", "r");
    if (file == NULL) 
    {
        return 3;
    }

    char buffer[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; ++i) buffer[i] = '\0';
    while (fgets(buffer, sizeof(buffer), file) != NULL) 
    {
        char* bufLogin = strtok(buffer, " ");
        char* bufpassword = strtok(NULL, "\n");
        //char* bufSanct = strtok(NULL, "\n");
        if (bufLogin == NULL || bufpassword == NULL)
        {
            fclose(file);
            return 3;
        }
        else if (strcmp(login, bufLogin) == 0) 
        {
            if (strcmp(password, bufpassword) == 0) 
            {
                //*sanctions = atoi(bufSanct);
                fclose(file);
                return 1;
            } else 
            {
                fclose(file);
                return 2;
            }
        }
    }
    fclose(file);
    return 0;
}

int reg(const char login[], const char password[]) 
{   /*
    0 - пользователь зарегистрирован
    1 - проблемы с открытием файла
    */
    FILE *file;

    file = fopen("logins.txt", "a");
    if (file == NULL) 
    {
        return 0;
    }

    fprintf(file, "%s %s\n", login, password);
    fflush(file);
    fclose(file);
    return 1;
}

int login(int* sanctions) 
{
    /*
    1 - успешный вход
    0 - аварийное завершение
    */
    while (1) {
        printf("👀 Введите 'логин пароль'\n: ");

        char buffer[BUFFER_SIZE];
        for (int i = 0; i < BUFFER_SIZE; ++i) buffer[i] = '\0';
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

        char* login = strtok(buffer, " ");
        char* password = strtok(NULL, "\n");

        if (!validateAuthData(login, password)) 
        {
            printf("Введены некорректные данные, грустно 🤕\n")
        } 
        else if (auth(login, password, sanctions) == 1) 
        {
            printf("🚪 Добро пожаловать, %s!\n", login);
            break;
        } 
        else if (auth(login, password, sanctions) == 2) {
            printf("🔐 Неверный пароль для %s\n", login);
        } 
        else if (auth(login, password, sanctions) == 3) {
            printf("Не удалось открыть файл логинов, грустно 😥\n");
            return 0;
        } 
        else 
        {
            printf("🔎 Пользователя %s не существует\n", login);
            printf("Хотите зарегистрироваться (y/n)?\n");
            char answer[2] = "n";
            if (!fgets(answer, 2, stdin)) {
                printf("- переполнение буфера\n");
                return 0;
            }
            if (answer[0] == 'y') {
                if (reg(login, password)) {
                    printf("📂 Пользователь %s зарегистрирован, пароль: %s\n", login, password);
                } else {
                    printf("📂 Регистрация не удалась\n");
                }
            }
            // очистка буфера
            int tmp;
            while ((tmp = getchar()) != '\n' && tmp != EOF);
        }
    }
    return 1;
}

int startCmdSession(int sanctions)
{
    /*
    0 - ошибка
    1 - успешное завершение
    */
    sanctions = sanctions ? sanctions : -1;
    DEBUG_PRINT("sanctc: %d\n", sanctions);
    while (sanctions) 
    {
        sanctions = sanctions > 0 ? sanctions-1 : -1;
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
            ++sanctions;
        }
        else if (strcmp(command, "Logout") == 0)
        {
            printf("Выход...\n\n");
            return 1;
        }
        else if (strcmp(command, "Time") == 0)
        {
            time_t rawtime = time(NULL);
            struct tm now = *localtime(&rawtime);
            printf("🕐 Время сейчас: %02d:%02d:%4d\n", now.tm_hour, now.tm_mon, now.tm_year + 1900);
        } 
        else if (strcmp(command, "Howmuch") == 0)
        {   
            // Howmuch 00:00:0000 -f\n
            char* dayStr = strtok(NULL, ":");
            char* monthStr = strtok(NULL, ":");
            char* yearStr = strtok(NULL, " ");
            char* flag = strtok(NULL, " \n");

            int dateIsValide = 0;
            struct tm pastTm = {0};
            if (dayStr != NULL && monthStr != NULL && yearStr != NULL && flag != NULL)
            {
                dateIsValide = 1;
                int day = atoi(dayStr);
                int month = atoi(monthStr);
                int year = atoi(yearStr);

                if (!(1 <= day && day <= 31)) dateIsValide = 0;
                if (!(1 <= month && month <= 12)) dateIsValide = 0;
                if (year == 0) dateIsValide = 0; // поимка сразу двух зайцев
                if (year < 0) ++year;
                
                pastTm.tm_mday = day;
                pastTm.tm_mon = month - 1;
                pastTm.tm_year = year - 1900;
            }
            // struct tm -> time_t
            time_t now = time(NULL);
            time_t past = mktime(&pastTm);

            if (past == -1 || !dateIsValide) 
            {
                printf("Ошибка при преобразовании введённой даты\n");
            }
            else if (past > now)
            {
                printf("Введённая дата ещё не наступила\n");
            }
            else 
            {
                // разница в секундах
                time_t secondDiff = difftime(now, past);

                if (strcmp(flag, "-s") == 0) 
                {
                    printf("Разница в секундах: %ld\n", secondDiff);
                }
                else if (strcmp(flag, "-m") == 0) 
                {
                    printf("Разница в минутах: %ld\n", secondDiff/60);
                }
                else if (strcmp(flag, "-h") == 0) 
                {
                    printf("Разница в часах: %ld\n", secondDiff/60/60);
                }
                else if (strcmp(flag, "-y") == 0) 
                {
                    double yearDiff = secondDiff/365.25/60/60/24;
                    printf("Разница в годах: %ld\n", (long)yearDiff);
                }
                else
                {
                    printf("flag не распознан\n");
                }
            }
        } 
        else if (strcmp(command, "Sanctions") == 0)
        {
            printf("Подтвердите действие ('12345'): ");

            char buffer2[BUFFER_SIZE];
            for (int i = 0; i < BUFFER_SIZE - 1; ++i) buffer2[i] = '\0';
            if (!fgets(buffer2, sizeof(buffer2), stdin)) 
            {
                printf("- переполнение буфера\n");
                return 0;
            }
            // Очищаем буфер ввода
            if (!(buffer2[BUFFER_SIZE - 2] == '\0' || buffer2[BUFFER_SIZE - 2] == '\n'))
            {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
            
            {
                if (strcmp(buffer2, "12345\n") == 0)
                {
                    printf(" ✅\n");
                }
                else 
                {
                    printf(" ❌\n");
                    continue;
                }
            }

            // Sanctions login 123456\n
            char* login = strtok(NULL, " ");
            char* amountStr = strtok(NULL, " \n");
            
            int sanctIsValide = 0;
            int amount = 0;
            if (login != NULL && amountStr != NULL)
            {
                sanctIsValide = 1;
                amount = atoi(amountStr);
                if (!(0 <= amount && amount <= 100000)) 
                {
                    printf("Количество санкций [0,100000]\n");
                    sanctIsValide = 0;
                }
            }

            if (!sanctIsValide) 
            {
                printf("Ошибка при чтении команды\n");
            }
            else if (!setSanctions(login, amount))
            {
                printf("Не удалось установить санкции\n");
            }
            else 
            {
                printf("Санкции установлены\n");
            }
        }
        else 
        {
            printf(" ❗ команда не найдена\n")
            ++sanctions;
        }
    }
    printf("Выход...\n\n");
    return 1;
}

int setSanctions(const char login[], int sanctions)
{   /*
    1 - санкции установлены
    0 - проблемы с открытием файла
    */
    FILE *file;

    char filename[LOGIN_LIMIT + 15];
    for (int i = 0; i < LOGIN_LIMIT + 15; ++i) filename[i] = '\0';
    strcat(filename, login);
    strcat(filename, "-sanctions.txt");
    
    file = fopen(filename, "w");
    if (file == NULL) 
    {
        return 0;
    }

    fprintf(file, "%d\n", sanctions);
    fflush(file);
    fclose(file);
    return 1;
}