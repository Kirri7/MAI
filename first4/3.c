#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);


size_t getFileSize(FILE* file);

int main(int argc, char *argv[]) {  
	if (argc != 3)
	{
        printf("Программа.exe /folder/file.in /folder/file.out\n");
        return 0;
	}

    FILE* fileIn = fopen(argv[1], "rb"); // запись в бинарном режиме

    if (fileIn == NULL)
    {
        printf("Не удалось открыть файл, грустно 😥\n");
        return 1;
    }

    FILE* fileOut = fopen(argv[2], "wb");
    
    if (fileOut == NULL)
    {
        printf("Не удалось создать файл, грустно 😥\n");
        return 1;
    }

    size_t fileSize = getFileSize(fileIn);
    unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char) * fileSize);

    if (buffer == NULL)
    {
        printf("Проблемы с памятью, грустно 😥\n");
        return 1;
    }

    if (fread(buffer, sizeof(char), fileSize, fileIn) != fileSize)
    {
        printf("Файл прочитан не полностью, грустно 😥\n");
        return 1;
    }

    if (fwrite(buffer, sizeof(char), fileSize, fileOut) != fileSize)
    {
        printf("Файл записан не полностью, грустно 😥\n");
        return 1;
    }
    printf("На этом всё, хорошего дня 👋");

    free(buffer);
    fclose(fileIn);
    fflush(fileOut);
    fclose(fileOut);
    return 0;
}

size_t getFileSize(FILE* file) {
    size_t currentPosition = ftell(file); // сохраняем, где остановились
    fseek(file, 0, SEEK_END); // идём в конец

    size_t fileSize = ftell(file); // конечная позиция = размер
    fseek(file, currentPosition, SEEK_SET); // блудный сын идёт домой

    return fileSize;
}
