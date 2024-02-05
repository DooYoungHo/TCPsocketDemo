/* User Include ... */
#include "/home/pi/Json/Inc/JsonParser.h"

cJSON* CreateJsonFromDoc(char* doc)
{  
    if ( doc == NULL )
    {
        fprintf(stderr, " File reading Error ...\n");
        return NULL;
    }
    
    cJSON* json = cJSON_Parse(doc);
    /* 동적 메모리 할당 해제 */
    free(doc);
    
    if ( json == NULL )
    {
        const char* error_ptr = cJSON_GetErrorPtr();
        if ( error_ptr != NULL )
        {
            fprintf(stderr, " Error in Parse : %s\n", error_ptr);
        }
        return NULL;
    }
    
    return json;
}


NetworkData ConvertJsonToNetworkData(cJSON *json, const char* pchKey, const char* pchContents)
{
    /* 구조체 선언 및 초기화 */
    NetworkData result;
    result.iJsonSize = 0;
    result.iContentsSize = 0;
    result.pNetworkArray = NULL;
    
    cJSON *Network = cJSON_GetObjectItem(json, pchKey);
    if ( !cJSON_IsArray(Network) )
    {
        /* 반환 값으로 NULL 반환 */
        return result;
    }
    
    int iArraySize = cJSON_GetArraySize(Network);
    result.pNetworkArray = malloc(iArraySize * sizeof(NetworkInfo));
    if ( result.pNetworkArray == NULL )
    {
        /* 메모리 할당 실패 처리 */
        return result;
    }
    
    result.iJsonSize = iArraySize;
    
    for ( int i = 0; i < iArraySize; i++ )
    {
        cJSON *item = cJSON_GetArrayItem(Network, i);
        cJSON *contents = cJSON_GetObjectItem(item, "Contents");
        cJSON *type = cJSON_GetObjectItem(item, "Type");
        cJSON *ip = cJSON_GetObjectItem(item, "IP");
        cJSON *port = cJSON_GetObjectItem(item, "Port");
        cJSON *baudrate = cJSON_GetObjectItem(item, "Baudrate");
        
        if ( contents != NULL && cJSON_IsString(contents) &&
             type != NULL && cJSON_IsString(type) &&
             ip != NULL && cJSON_IsString(ip) &&
             port != NULL && cJSON_IsString(port) )
        {
            
            if ( strcmp(contents -> valuestring, pchContents) == 0 )
            {
                result.pNetworkArray[i].pchContents = strdup(contents -> valuestring);
                result.pNetworkArray[i].pchType = strdup(type -> valuestring);
                result.pNetworkArray[i].pchIp = strdup(ip -> valuestring);
                result.pNetworkArray[i].pchPort = strdup(port -> valuestring);
                
                if ( baudrate != NULL && cJSON_IsNumber(baudrate) )
                {
                    result.pNetworkArray[i].iBaudrate = baudrate -> valueint;
                }
                result.iContentsSize++;
            }
        }
        
        else
        {
            /* 유효하지 않은 항목 처리
             * 현재까지 유효한 데이터 크기로 조정 */
            result.iJsonSize = i;
            break;
        }
    }
        
    return result;
}



void vFreeNetworkData(NetworkData *data)
{
    if (data != NULL && data -> pNetworkArray != NULL)
    {
        for (int i = 0; i < data -> iJsonSize; i++)
        {
            free(data->pNetworkArray[i].pchType);
            free(data->pNetworkArray[i].pchIp);
        }
        free(data->pNetworkArray);
        // 구조체 내 포인터를 NULL로 설정하여 이중 해제(double free) 방지
        data->pNetworkArray = NULL;
    }
    data->iJsonSize = 0; // 요소 개수를 0으로 설정
    data->iContentsSize = 0;
}




void vPrintJsonFile(const char* pchLabel, NetworkData netdata)
{
    printf("%s\n", pchLabel);
    for ( int i = 0; i < netdata.iJsonSize; i++ )
    {
        printf(" Contents : %s\n", netdata.pNetworkArray[i].pchContents);
        printf(" Types : %s\n", netdata.pNetworkArray[i].pchType);
        printf(" Ip : %s\n", netdata.pNetworkArray[i].pchIp);
        printf(" Port : %s\n", netdata.pNetworkArray[i].pchPort);
        printf(" BaudRate : %d\n", netdata.pNetworkArray[i].iBaudrate);
        printf("---------------------------\n");
    }
}



bool bIsClientAddressInNetworkData(NetworkData networkdata, char *pchIpAddress)
{
    for ( int i = 0; i < networkdata.iJsonSize; i++ )
    {
        if ( strcmp(networkdata.pNetworkArray[i].pchIp, pchIpAddress) == 0 )
        {
            return true;
        }
    }
    
    return false;
}
