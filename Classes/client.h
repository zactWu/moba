#ifndef CLIENT
#define CLIENT

#include <winsock2.h>
#include <process.h>               //C中的线程 
#include <string>
#include <cocos2d.h>
#include <UnitClass.h>
#include <queue>
#include "GameScene.h"
#pragma comment(lib,"ws2_32.lib")



struct information {
	char c;
	int tag;
	float x;
	float y;
};
static std::queue<information> receiveQueue;

class GameClient
{
private:
	Unit* hero;


public:

	GameScene* scene;

	bool UpdateChatMessage = false;

	char RecvBuf[30 * 100] = { 0 };
	char SendBuf[30 * 100] = { 0 };
	char ChatBuf[100] = { 0 };
	char ChattingInfirmationFromTheOther[100] = { 0 };

	bool virtual init(Unit* h);
	void ClientProcess();    //启动客户处理
	static DWORD WINAPI Send(LPVOID lpParam);    //发送线程
	static DWORD WINAPI Receive(LPVOID lpParam);  //接受线程
	static DWORD WINAPI control(LPVOID lpParam);	//控制


	bool MoveControl(GameClient* Client, const information* temp);
	bool Q_Skill(GameClient* Client, const information* temp);
	bool W_Skill(GameClient* Client, const information* temp);
	bool E_Skill(GameClient* Client, const information* temp);


	static int AddBuf(char c, int tag, float x, float y);
	void CleanUp();      //资源回收
	//SOCKET相关
	SOCKET ClientSocket;
	struct sockaddr_in ServerAddr;

	DWORD SendThreadID;
	DWORD RecvThreadID;
	 
	int InfNum = 0;


};

#endif // !CLIENT
