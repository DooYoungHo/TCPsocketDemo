/*
 * ReadJsonFile.h
 *
 *  Created on: Jan 29, 2024
 *      Author: youngho
 */


#ifndef _READJSONFILE_C_READJSONFILE_C_
#define _READJSONFILE_C_READJSONFILE_C_

#ifdef __cplusplus
extern "C" {
#endif

// all of your legacy C code here

#ifdef __cplusplus
}
#endif

/* System Include ... */
#include <stdio.h>      /* 파일 처리 함수가 포함된 헤더 파일 */
#include <stdlib.h>     /* malloc, free 함수가 포함된 헤더 파일 */
#include <stdbool.h>    /* bool, true, false가 정의된 헤더 파일 */
#include <string.h>     /* strchr, memset, memcpy가 포함된 헤더 파일 */


/* Functions ... */
char *readFile(const char *filename, int *readSize);

#endif /* _READJSONFILE_C_READJSONFILE_C_ */
