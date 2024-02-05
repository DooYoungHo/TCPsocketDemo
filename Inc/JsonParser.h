/*
 * JsonParser.h
 *
 *  Created on: Jan 29, 2024
 *      Author: youngho
 */


#ifndef _JSONPARSER_H_JSONPARSER_H_
#define _JSONPARSER_H_JSONPARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

// all of your legacy C code here

#ifdef __cplusplus
}
#endif

//---------Include---------//
#include "Types.h"
#include "cJSON.h"
#include "ReadJsonFile.h"
#include <stdlib.h>
#include <string.h>


//----------define------------//
#define TCP_IP          "TCP/IP"
#define LORA            "Lora"
#define SERIAL          "Serial"

#define JSON_FAIL       0
#define JSON_OK         1

//---------typedef---------//
//----json parser----//
typedef struct
{
    char *pchContents;
    char *pchType;
    char *pchIp;
    char *pchPort;
    int  iBaudrate;
} NetworkInfo;

typedef struct
{
    NetworkInfo *pNetworkArray;
    int          iContentsSize;
    int          iJsonSize;
} NetworkData;


//---------Functions---------//
cJSON* CreateJsonFromDoc(char* doc);

NetworkData ConvertJsonToNetworkData(cJSON *json, const char* pchKey, const char* pchContents);

void vFreeNetworkData(NetworkData *data);

void vPrintJsonFile(const char* pchLabel, NetworkData netdata);
bool bIsClientAddressInNetworkData(NetworkData networkdata, char *pchIpAddress);
#endif /* JSONPARSER_H_JSONPARSER_H_ */

//InternalData ReadAndParseInternal(cJSON *json);
//void vFreeInternalData(InternalData *data);

