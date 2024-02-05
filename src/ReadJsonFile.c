/* User Include ... */
#include "/home/pi/Json/Inc/ReadJsonFile.h"


char *readFile(const char *filename, int *readSize)    // 파일을 읽어서 내용을 반환하는 함수
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf("File Not Opend ...! \n");
        return NULL;
    }

    int size;
    char *buffer;

    // 파일 크기 구하기
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // 파일 크기 + NULL 공간만큼 메모리를 할당하고 0으로 초기화
    buffer = malloc(size + 1);
    memset(buffer, 0, size + 1);

    // 파일 내용 읽기
    if (fread(buffer, size, 1, fp) < 1)
    {
        *readSize = 0;
        free(buffer);
        fclose(fp);
        return NULL;
    }

    // 파일 크기를 넘겨줌
    *readSize = size;

    fclose(fp);    // 파일 포인터 닫기

    return buffer;
}
