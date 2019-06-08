#ifndef CLIENT
#define CLIENT

#include <winsock2.h>
#include <process.h>      
#include <string>
#include <cocos2d.h>
#include <UnitClass.h>
#include <queue>
#include "GameScene.h"
#pragma comment(lib,"ws2_32.lib")

//000

//struct  order
//{
//	Vec2 pos;
//	int kind = 0;
//	int tag = -1;
//};


class GameClient
{
private:

public:
	enum {
		InformationLength = 40
	};

	Hero* hero;
	Hero* en_hero;

	GameScene* scene;

	bool UpdateChatMessage = false;

	char RecvBuf[InformationLength] = { 0 };
	char SendBuf[InformationLength] = { 0 };
	char ChatBuf[100] = { 0 };
	char ChattingInfirmationFromTheOther[100] = { 0 };

	bool virtual init(Hero* h, Hero* en_hero);
	void ClientProcess();    //�����ͻ�����
	static DWORD WINAPI Send(LPVOID lpParam);    //�����߳�
	static DWORD WINAPI Receive(LPVOID lpParam);  //�����߳�

	void CleanUp();      //��Դ����
	//SOCKET���
	SOCKET ClientSocket;
	struct sockaddr_in ServerAddr;

	DWORD SendThreadID;
	DWORD RecvThreadID;


};

#endif // !CLIENT
