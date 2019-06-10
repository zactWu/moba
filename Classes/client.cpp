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
bool GameClient::init(Hero* h,Hero* e)
{
	hero = h;
	en_hero = e;
	std::ofstream outfile;
	outfile.open("log2.txt");

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		outfile << "�޷������׽���Э��ջ\n";
		return false;
	}

	ServerAddr.sin_family = PF_INET;
	ServerAddr.sin_port = htons(PORTS);


	ServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	ClientSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (ClientSocket == INVALID_SOCKET)
	{
		outfile << "�׽��ֳ�ʼ������\n";
		WSACleanup(); //�ͷ��׽�����Դ  
		return false;
	}

	if (connect(ClientSocket, (LPSOCKADDR)& ServerAddr,
		sizeof(ServerAddr)) == INVALID_SOCKET)
	{
		outfile << "���ӷ��������ִ���\n";
		closesocket(ClientSocket); //�ر��׽���  
		WSACleanup(); //�ͷ��׽�����Դ  
		return false;
	}

	outfile << "���ӷ������ɹ�\n";
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
			//ͨ������������  
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
			if (':' == recvbuf[0]) {			//'C'����������Ϣ
				cocos2d::log("recv chat %s", recvbuf);
				strcpy(Client->ChattingInfirmationFromTheOther, recvbuf);
				Client->UpdateChatMessage = true;  //������Ҫ����������Ϣ
			}
			else if ('$' == recvbuf[0]) {		//װ��
				char num = recvbuf[1];
				cocos2d::log("shop %c", num);
				switch (num)
				{
				case '1':
					client.en_hero->_speed += 30;//����Ь��û��Ӧ
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
			else {								//�������������Ϣ
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