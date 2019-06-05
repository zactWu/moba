#ifndef CLIENT
#define CLIENT

#include <winsock2.h>
#include <process.h>               //C�е��߳� 
#include <string>
#include <cocos2d.h>
#include <UnitClass.h>
#include <queue>
#pragma comment(lib,"ws2_32.lib")

static int InformationNumber = 0;


struct information {
	char c;
	char tag;
	float x;
	float y;
};
static std::queue<information> receiveQueue;

class GameClient
{
private:
	Unit* hero;
	char RecvBuf[30 * 100];
	char SendBuf[30 * 100];

public:

	bool virtual init(Unit* h);
	void ClientProcess();    //�����ͻ�����
	static DWORD WINAPI Send(LPVOID lpParam);    //�����߳�
	static DWORD WINAPI Receive(LPVOID lpParam);  //�����߳�
	static DWORD WINAPI control(LPVOID lpParam);	//����
	static int AddBuf(char c, int tag, float x, float y);
	void CleanUp();      //��Դ����
	//SOCKET���
	SOCKET ClientSocket;
	struct sockaddr_in ServerAddr;

	DWORD SendThreadID;
	DWORD RecvThreadID;



};

#endif // !CLIENT
