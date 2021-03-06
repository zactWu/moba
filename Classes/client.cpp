#include "client.h"
#include <fstream>
#include <cocos2d.h>
#include "GameScene.h"
#include <UnitClass.h>
#include <mutex>
#include <cstring>
#include "Hero.h"
#define PORTS 1236

extern GameClient client;
std::mutex gameLock;
bool fight = false;
extern std::string ipMessage;
bool GameClient::init(Hero* h,Hero* e)
{
	hero = h;
	en_hero = e;
	std::ofstream outfile;
	outfile.open("log2.txt");

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		outfile << "无法加载套接字协议栈\n";
		return false;
	}

	ServerAddr.sin_family = PF_INET;
	ServerAddr.sin_port = htons(PORTS);

	int len = ipMessage.size();
	char ip[100] = { 0 };
	for (int i = 0; i < len; i++) {
		ip[i] = ipMessage[i];
	}
	ServerAddr.sin_addr.S_un.S_addr = inet_addr(ip);
	ClientSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (ClientSocket == INVALID_SOCKET)
	{
		outfile << "套接字初始化错误\n";
		WSACleanup(); //释放套接字资源  
		return false;
	}

	if (connect(ClientSocket, (LPSOCKADDR)& ServerAddr,
		sizeof(ServerAddr)) == INVALID_SOCKET)
	{
		outfile << "连接服务器出现错误\n";
		closesocket(ClientSocket); //关闭套接字  
		WSACleanup(); //释放套接字资源  
		return false;
	}

	outfile << "连接服务器成功\n";
	outfile.close();
	return true;
}

void GameClient::ClientProcess()
{

	HANDLE sendThread, recvThread;
	sendThread = CreateThread(NULL, 0,
		static_cast<LPTHREAD_START_ROUTINE>(GameClient::Send),
		static_cast<LPVOID>(this), 0,
		NULL
	);
	if (nullptr == sendThread) {
		DWORD k=GetLastError();
		cocos2d::log("send wrong, error is %d", k);
	}
	else {
		CloseHandle(sendThread);
	}

	recvThread = CreateThread(NULL, 0,
		static_cast<LPTHREAD_START_ROUTINE>(GameClient::Receive),
		static_cast<LPVOID>(this), 0,
		NULL
	);
	if (nullptr == recvThread) {
		DWORD k = GetLastError();
		cocos2d::log("receive wrong, error is %d", k);
	}
	else {
		CloseHandle(recvThread);
	}
}


DWORD __stdcall GameClient::Send(LPVOID lpParam)
{
	GameClient* Client = (GameClient*)lpParam;

	while (true)
	{
		if (Client->ClientSocket != NULL)
		{

			gameLock.lock();

			if ('\0' != Client->SendBuf[0]) {
				char sendbuf[InformationLength] = { 0 };
				strcpy(sendbuf, Client->SendBuf);

				int const iSend = send(Client->ClientSocket, sendbuf, strlen(sendbuf), 0);

				ZeroMemory(Client->SendBuf, 30 * 100);
				cocos2d::log("send message :%s\n", sendbuf);
			}
			if ('\0' != Client->ChatBuf[0]) {
				char chatbuf[100] = { 0 };
				strcpy(chatbuf, Client->ChatBuf);
				int const chatSend = send(Client->ClientSocket, chatbuf, 100, 0);
				cocos2d::log("chat %s", chatbuf);
				ZeroMemory(Client->ChatBuf, 100);
			}
			gameLock.unlock();
		}
	}
	log("return");
	return 0;
}

DWORD __stdcall GameClient::Receive(LPVOID lpParam)
{
	GameClient* Client = (GameClient*)lpParam;

	while (true)
	{
		fd_set  Read;
		int ret;
		FD_ZERO(&Read);
		FD_SET(Client->ClientSocket, &Read);

		ret = select(0, &Read, NULL, NULL, NULL);

		auto soc = Client->ClientSocket;

		if (FD_ISSET(Client->ClientSocket, &Read) && Client->ClientSocket != NULL)
		{
			//通过缓冲区交互  
			char recvbuf[InformationLength] = { 0 };
			ZeroMemory(recvbuf, InformationLength);
			gameLock.lock();
			int iLen = recv(Client->ClientSocket, recvbuf, InformationLength, 0);
			if (iLen <= 0) {
				gameLock.unlock();
				return 0;
			}
			ZeroMemory(Client->RecvBuf, InformationLength);
			strcpy(Client->RecvBuf, recvbuf);
			cocos2d::log("receive message: %s ", recvbuf);
			if (false == fight) {
				fight = true;
			}
			if (':' == recvbuf[0]) {			//'C'代表聊天信息
				cocos2d::log("recv chat %s", recvbuf);
				strcpy(Client->ChattingInfirmationFromTheOther, recvbuf);
				Client->UpdateChatMessage = true;  //现在需要更新聊天信息
			}
			else if ('$' == recvbuf[0]) {		//装备
				char num = recvbuf[1];
				cocos2d::log("shop %c", num);
				switch (num)
				{
				case '1':
					client.en_hero->_speed += 30;//购买鞋子没反应
					break;
				case '2':
					client.en_hero->_attack += 5;
					break;
				case '3':
					client.en_hero->_defense += 5;
					break;
				case '4':
					client.en_hero->_attackRange += 10;
					break;
				case '5':
					client.en_hero->_attack += 5;
					client.en_hero->_attackRange += 10;
					break;
				case '6':
					client.en_hero->_life_max += 25;
					break;
				case '7 ':
					client.en_hero->_speed += 20;
					client.en_hero->_attack += 3;
					client.en_hero->_defense += 3;
					client.en_hero->_attackRange += 8;
					client.en_hero->_life_max += 20;
					break;
				}
			}
			else {								//如果不是聊天信息
				order od;
				sscanf_s(recvbuf, "%f#%f#%d#%d", &od.pos.x, &od.pos.y, &od.kind, &od.tag);
				client.en_hero->order_list.push_back(od);			
			}
			gameLock.unlock();
		}
	}
	return 0;
}

void  GameClient::CleanUp()
{
	closesocket(ClientSocket);
	WSACleanup();
}
