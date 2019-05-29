#ifndef CLIENT
#define CLIENT

#include <winsock2.h>
#include <process.h>               //C�е��߳� 
#include <string>
#include <cocos2d.h>
#include <UnitClass.h>
#pragma comment(lib,"ws2_32.lib")

class GameClient
{
private:
	Unit* hero;

public:
	bool virtual init(Unit* h);
	void ClientProcess();    //�����ͻ�����
	static DWORD WINAPI Send(LPVOID lpParam);    //�����߳�
	static DWORD WINAPI Receive(LPVOID lpParam);  //�����߳�
	int AddBuf(char c, int tag, float x, float y);
	void CleanUp();      //��Դ����
	//SOCKET���
	SOCKET ClientSocket;
	struct sockaddr_in ServerAddr;

	DWORD SendThreadID;
	DWORD RecvThreadID;

	char RecvBuf[1024];
	char SendBuf[1024];

};

#endif // !CLIENT
