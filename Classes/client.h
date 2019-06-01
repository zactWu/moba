#ifndef CLIENT
#define CLIENT

#include <winsock2.h>
#include <process.h>               //C中的线程 
#include <string>
#include <cocos2d.h>
#include <UnitClass.h>
#include <queue>
#pragma comment(lib,"ws2_32.lib")

struct information {
	float x;
	float y;
};
static std::queue<information> receiveQueue;

class GameClient
{
private:
	Unit* hero;


public:

	bool virtual init(Unit* h);
	void ClientProcess();    //启动客户处理
	static DWORD WINAPI Send(LPVOID lpParam);    //发送线程
	static DWORD WINAPI Receive(LPVOID lpParam);  //接受线程
	static DWORD WINAPI control(LPVOID lpParam);	//控制
	int AddBuf(char c, int tag, float x, float y);
	void CleanUp();      //资源回收
	//SOCKET相关
	SOCKET ClientSocket;
	struct sockaddr_in ServerAddr;

	DWORD SendThreadID;
	DWORD RecvThreadID;

	char RecvBuf[1024];
	char SendBuf[1024];

};

#endif // !CLIENT
