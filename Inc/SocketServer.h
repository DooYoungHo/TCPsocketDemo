/*
 * Socket.h
 *
 *  Created on: Jan 29, 2024
 *      Author: youngho
 */


#ifndef _SOCKET_H_SOCKET_H_
#define _SOCKET_H_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

//---------Threshold---------//
#define DATA_BUFFER_SIZE			1024
#define RECV_BUFFER_SIZE			256
#define SEND_BUFFER_SIZE			256

#define SOCKET_FAIL					 0
#define SOCKET_OK					 1

#define LISTEN_TO_MAXIUM_CLIENTS	10

#define READ_SHUTDOWN				SHUT_RD
#define WRITE_SHUTDOWN				SHUT_WR
#define ALL_SHUTDOWN				SHUT_RDWR

//----------include---------------//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>


//---------typedef-----------//
typedef struct
{
	int   iSocket;
	int   iServerAddressReuse;
	char* pchPort;
	char* pchIp;
	struct sockaddr_in ServerAddr;
} ServerSocketInfo;



/*-----임시-------*/
typedef struct
{
	char *pchIp[LISTEN_TO_MAXIUM_CLIENTS];
	// 추가 필요한 정보가 있을지 몰름,,,,
} ConnectClientInfo;


typedef struct
{
	//ConnectClientInfo *ConnectClients;
	int				  iMaxClients;
	int				  iCurrentClients;
} ConnectClientData;


typedef struct
{
	int iSocket;
	int iClientSize;
	struct sockaddr_in ClientAddr;
} ClientSocketInfo;


//-----------Functions--------------------//
//-----------TCP---------------------------------//
bool bStartTCPserver(ServerSocketInfo *serverInfo);
void vBindSocketServer(ServerSocketInfo *serverInfo);
void vListenSocketServer(ServerSocketInfo *serverInfo);
void vConnectNewClient(ServerSocketInfo *serverInfo, ClientSocketInfo *clientInfo);
//----------------------------------------------//




//-----------UDP--------------------------------//
bool bStartUDPserver(ServerSocketInfo *serverInfo);




//----------Common------------------------------//
bool bHalfCloseSocket(ClientSocketInfo *clientInfo, int iHowto);
void vCloseServerSocket(ServerSocketInfo *serverInfo);
void vCloseClientSocket(ClientSocketInfo *clientInfo);




//------------Option Functions------------------//
char* chCheckClientAddress(ClientSocketInfo *clientInfo);
bool bInputAddressInClientInfo(ConnectClientInfo *clientInfo, char* pchAddress);
bool bSocketKeepAlive(ServerSocketInfo *serverInfo, int iSendFirstPacket, int iInterval, int iCnt);
bool bReuseServerSocket(ServerSocketInfo *serverInfo);
void vFreeConnectClientData(ConnectClientData *connectClient);
//----------------------------------------------//
#endif /* _SOCKET_H_SOCKET_H_ */
