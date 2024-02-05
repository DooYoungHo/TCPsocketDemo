/* User Include ... */
#include "/home/pi/Json/Inc/SocketServer.h"



bool bStartTCPserver(ServerSocketInfo *serverInfo)
{
	serverInfo->iSocket = socket(PF_INET, SOCK_STREAM, 0);
	if ( serverInfo->iSocket < 0 )
	{
		perror(" TCP 서버를 열 수 없습니다...!\n");
		return SOCKET_FAIL;
	}
	return SOCKET_OK;	
}


bool bStartUDPserver(ServerSocketInfo *serverInfo)
{
	serverInfo->iSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if ( serverInfo->iSocket < 0 )
	{
		perror(" UDP 서버를 열 수 없습니다...!\n");
		return SOCKET_FAIL;
	}
	return SOCKET_OK;
}


void vBindSocketServer(ServerSocketInfo *serverInfo)
{
	int iPortNum = 0;
	
	iPortNum = atoi(serverInfo->pchPort);
	memset(&serverInfo->ServerAddr, 0, sizeof(serverInfo->ServerAddr));
	serverInfo->ServerAddr.sin_family = AF_INET;
	serverInfo->ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverInfo->ServerAddr.sin_port = htons(iPortNum);
	
	if ( bind (serverInfo->iSocket, (struct sockaddr *)&serverInfo->ServerAddr,
				sizeof(serverInfo->ServerAddr)) < 0 )
		{
			perror(" Bind 에러 발생했습니다...!\n");
			vCloseServerSocket(serverInfo);
		}
}


void vListenSocketServer(ServerSocketInfo *serverInfo)
{
	if ( listen(serverInfo->iSocket, 5) < 0 )
	{
		perror(" Listen 에러가 발생했습니다...!\n");
		vCloseServerSocket(serverInfo);
	}
}


void vConnectNewClient(ServerSocketInfo *serverInfo, ClientSocketInfo *clientInfo)
{
	clientInfo->iClientSize = sizeof(clientInfo->ClientAddr);
	
	clientInfo->iSocket = accept(serverInfo->iSocket, (struct sockaddr *)&clientInfo->ClientAddr, (socklen_t *)&clientInfo->iClientSize);
	
	if ( clientInfo->iSocket < 0 )
	{
		perror(" Accpet 과정에서 에러가 발생했습니다...!\n");
		vCloseServerSocket(serverInfo);						// 자체적으로 소켓을 닫아주는게 맞는것일까? return값을 주고 사용자가 처리하는 방식이 맞지 않을까
	}
}


bool bSocketKeepAlive(ServerSocketInfo *serverInfo, int iSendFirstPacket, int iInterval, int iCnt)
{
	int iKeepAlive = 1;

	/* KeepAlive On */
	if ( setsockopt(serverInfo->iSocket, SOL_SOCKET, SO_KEEPALIVE, &iKeepAlive, sizeof(iKeepAlive)) < 0 )
	{
		perror(" KeepAlive 설정 오류가 발생했습니다...!\n");
		return SOCKET_FAIL;
	}
		
	/* 유휴상태가 얼마나 지속되면 Alive패킷을 보낼지 */
	if ( setsockopt(serverInfo->iSocket, SOL_TCP, TCP_KEEPIDLE, &iSendFirstPacket, sizeof(iSendFirstPacket)) < 0 )
	{
		perror(" TCP_KEEPIDLE 옵션 설정 중 오류가 발생했습니다...!\n");
		return SOCKET_FAIL;
	}
	
	/* 전송 주기는 얼마로 정할지 */
	if ( setsockopt(serverInfo->iSocket, SOL_TCP, TCP_KEEPINTVL, &iInterval, sizeof(iInterval)) < 0 )
	{
		perror(" TCP_KEEPINTVL 옵션 설정 중 오류가 발생했습니다...!\n");
		return SOCKET_FAIL;
	}
	
	/* 몇 번 전송 시도를 할 지 */
	if ( setsockopt(serverInfo->iSocket, SOL_TCP, TCP_KEEPCNT, &iCnt, sizeof(iCnt)) < 0 )
	{
		perror(" TCP_KEEPCNT 옵션 설정 중 오류가 발생했습니다...!\n");
		return SOCKET_FAIL;
	}
	
	return SOCKET_OK;
}


bool bReuseServerSocket(ServerSocketInfo *serverInfo)
{
	int iReuse = 1;
	
	/* 사용했던 주소 재사용 On */
	serverInfo->iServerAddressReuse = setsockopt(serverInfo->iSocket, SOL_SOCKET, SO_REUSEADDR, &iReuse, sizeof(iReuse));
	if ( serverInfo->iServerAddressReuse < 0 )
	{
		perror(" Reuse 설정 오류가 발생했습니다...!\n");
		return SOCKET_FAIL;
	}
	return SOCKET_OK;
}



void vSendPacket(ClientSocketInfo *clientInfo)
{
	// 패킷만 보내면 될라나? 혹시 모르니까 다른 것도 생각해보자..
}


int iRecvPacket(ClientSocketInfo *clientInfo)
{
	return 0;
}


// 여기서는 그냥 buf를 반환해주고, 올바른 주소인지 체크하는 함수가 따로 있어야 할 것 같고, 배열에 넣는 함수가 따로 있어야할 것 같다????
char* chCheckClientAddress(ClientSocketInfo *clientInfo)
{
	static char buf[INET_ADDRSTRLEN];
	
	if ( inet_ntop(AF_INET, &clientInfo->ClientAddr.sin_addr, buf, INET_ADDRSTRLEN) == NULL )
	{
		return NULL;
	}
	
	return buf;
}




bool bInputAddressInClientInfo(ConnectClientInfo *clientInfo, char* pchAddress)
{
	int iSize = sizeof(clientInfo->pchIp) / sizeof(clientInfo);
	
	for ( int i = 0; i < iSize; i++ )
	{
		if ( clientInfo->pchIp[i] != NULL && strcmp(clientInfo->pchIp[i], pchAddress) == 0 )
		{
			printf("이미 %s의 주소가 존재합니다.\n", pchAddress);
			return false;
		}
		
		if ( clientInfo->pchIp[i] == NULL )
		{
			clientInfo->pchIp[i] = pchAddress;
			printf("[%d]번째 클라이언트 자리에 %s의 주소를 가진 클라이언트가 입장하였습니다.\n", i, pchAddress);
			break;
		}
	}
	
	return true;
}



void vOutputAddressInClientInfo(ConnectClientInfo *clientInfo, char* pchAddress)
{
	int iSize = sizeof(clientInfo->pchIp) / sizeof(clientInfo)
	
	for ( int i = 0; i < iSize; i++ )
	{
		if ( strcmp(clientInfo->pchIp[i], pchAddress) == 0 )
		{
			clientInfo->pchIp[i] = NULL;
			printf("[%d]번째 클라이언트 자리에 %s의 주소를 가진 클라이언트가 퇴장하셨습니다.\n", i, pchAddress);
			break;
		}
	}
}



bool bHalfCloseSocket(ClientSocketInfo *clientInfo, int iHowto)
{
	if ( shutdown(clientInfo->iSocket, iHowto) < 0 )
	{
		perror(" Half Close 도중 에러가 발생했습니다...!\n");
		return SOCKET_FAIL;
	}
	
	return SOCKET_OK;
}


void vCloseServerSocket(ServerSocketInfo *serverInfo)
{
	close(serverInfo->iSocket);
	printf(" Socket 서버가 닫혔습니다...!\n");
}


void vCloseClientSocket(ClientSocketInfo *clientInfo)
{
	close(clientInfo->iSocket);
	printf(" Client Socket이 닫혔습니다...!\n");
}


//accept

//send

//recv

//process

//버퍼 비우기

// block nonblock?

// 주기적으로 연결되었는지 확인 (솔직히 이건 keepAlive 설정을 했는데 필요할지 조금 의문..)
