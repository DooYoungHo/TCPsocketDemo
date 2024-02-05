/* System Include ... */
#include <stdio.h>      /* 파일 처리 함수가 포함된 헤더 파일 */
#include <stdlib.h>     /* malloc, free 함수가 포함된 헤더 파일 */
#include <stdbool.h>    /* bool, true, false가 정의된 헤더 파일 */
#include <string.h>     /* strchr, memset, memcpy가 포함된 헤더 파일 */
#include <malloc.h>



/* User Include ... */
#include "/home/pi/Json/Inc/JsonParser.h"
#include "/home/pi/Json/Inc/ReadJsonFile.h"
#include "/home/pi/Json/Inc/Types.h"
#include "/home/pi/Json/Inc/cJSON.h"
#include "/home/pi/Json/Inc/SocketServer.h"



/* Global Variables ... */
// Json 파일 관련
int giFileSize = 0;

const char *gFilePath = "/home/pi/Json/socketExample.json";
const char *gKeyOfExternal = "External";
const char *gKeyOfInternal = "Internal";



/* Functions ... */
void vJsonParseTask(cJSON** json, NetworkData *pServer, NetworkData *pClient);

/* ---------------------------------------------- */
/* ----------------- Main 실행문 ----------------- */
/* ---------------------------------------------- */
int main(int argc, char const* argv[])
{
    NetworkData ServerData, ClientData;
    cJSON* json = NULL;
    
    vJsonParseTask(&json, &ServerData, &ClientData);

    //---------------------- Socket Test --------------------------------//
    
    ServerSocketInfo svr;
    ClientSocketInfo cli;
    ConnectClientInfo cci;
    
    char recvBuff[RECV_BUFFER_SIZE];
    
    svr.pchPort = ServerData.pNetworkArray[0].pchPort;
    
    bStartTCPserver(&svr);
    
    if ( bReuseServerSocket(&svr) == SOCKET_FAIL )
    {
        perror(" Setting Option Error ");
    }
    
    vBindSocketServer(&svr);
    vListenSocketServer(&svr);

    while (1)
    {
        vConnectNewClient(&svr, &cli);
        if( cli.iSocket < 0 )
        {
            printf("Error\n");
            vCloseServerSocket(&svr);
            exit(0);
        }
        
        printf(" Client Connected...!\n");
        char *buffer = chCheckClientAddress(&cli);
        if ( bIsClientAddressInNetworkData(ClientData, buffer) )
        {
            bInputAddressInClientInfo(&cci, buffer);
        }
        
        while (1)
        {
            int test = recv(cli.iSocket, recvBuff, sizeof(recvBuff), 0);
            if ( test == -1 )
            {
                perror(" Erro Occured ...! ");
                break;
            }
            else if ( test == 0 )
            {
                perror(" Client Closed ...! ");
                break;
            }
            for ( int i = 0; i < test; i++ )
            {
                printf("%02x ", recvBuff[i]);
            }
            printf("\n");
            fflush(stdout);
        }
        
        vCloseClientSocket(&cli);
    }

    /* n. 사용 후 메모리 해제 */
    vFreeNetworkData(&ServerData);
    vFreeNetworkData(&ClientData);
    cJSON_Delete(json);
    vCloseServerSocket(&svr);

    return 0;
}




void vJsonParseTask(cJSON** json, NetworkData *pServer, NetworkData *pClient)
{
    /* 1. Json파일 읽어서 빈 파일인지 확인하기 */
    char* doc = readFile(gFilePath, &giFileSize);
    if ( doc == NULL || giFileSize == 0 )
    {
        printf(" 빈 파일입니다...!\n");
        exit(EXIT_FAILURE);
    }

    /* 2. Json 객체로 파싱하기 */
    *json = CreateJsonFromDoc(doc);
   
    if ( *json == NULL )
    {
        printf(" Json 파싱에 실패했습니다...!\n");
        exit(EXIT_FAILURE);
    }
    
    /* 3. Json 객체를 'NetworkData' 구조체로 변환 */
    *pServer = ConvertJsonToNetworkData(*json, gKeyOfExternal, TCP_IP);
    *pClient = ConvertJsonToNetworkData(*json, gKeyOfInternal, TCP_IP);
}





void vClearAllData(ServerSocketInfo *server, ClientSocketInfo *client, cJSON *json)
{
    
}
