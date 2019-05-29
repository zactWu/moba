#include "client.h"
#include <fstream>
#include <cocos2d.h>
#include "GameScene.h"
#include <UnitClass.h>
#include <mutex>
#define PORTS 1236

std::mutex l;
bool GameClient::init(Unit* h)
{
	hero = h;
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

	//	ServerAddr.sin_addr.S_un.S_addr = inet_pton(AF_INET, "127.0.0.1", &ServerAddr.sin_addr.S_un.S_addr);
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
	HANDLE  RecvThread = nullptr, SendThread = nullptr;
	if ((SendThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Send, this, 0, &SendThreadID)) == nullptr) {
//		cocos2d::log("sendThread wrong");
		cocos2d::log("wrong sendThreadId is %d", SendThreadID);
	}
	if (nullptr != SendThread) {
//		cocos2d::log("sendThreadId is %d", SendThreadID);
		CloseHandle(SendThread);
	}

	if ((RecvThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Receive, this, 0, &RecvThreadID)) == nullptr) {
//		cocos2d::log("receiveThread wrong");
		cocos2d::log("wrong recvThreadId is %d", SendThreadID);
	}
	if (nullptr != RecvThread) {
//		cocos2d::log("recvThreadId is %d", SendThreadID);
		CloseHandle(RecvThread);
	}
}

DWORD __stdcall GameClient::Send(LPVOID lpParam)
{
	GameClient* Client = (GameClient*)lpParam;

//	while (true)
//	{
		if (Client->ClientSocket != NULL)
		{
			char sendbuf[1024] = { 0 };
			strcpy(sendbuf, Client->SendBuf);
			if ('\0'==sendbuf[0]) {
				return 0;
			}
			int iSend = send(Client->ClientSocket, sendbuf, strlen(sendbuf), 0);

			ZeroMemory(Client->SendBuf, 1024);

			cocos2d::log("send message :%s\n", sendbuf);
//
//			if (iSend <= 0)
//			{
//				if (Client->ClientSocket != NULL)
//					closesocket(Client->ClientSocket);
//				Client->ClientSocket = NULL;
////				printf("�����̹߳ر�\n");
////				printf("����[%d]\n����[%d]\n", Client->SendThreadID, Client->RecvThreadID);
//				cocos2d::log("sendTread close");
//				cocos2d::log("send[%d] receive[%d]", Client->SendThreadID, Client->RecvThreadID);
//				ExitThread(Client->SendThreadID);
//				ExitThread(Client->RecvThreadID);
////				printf("�����̹߳ر�\n");
//				return 0;
//			}
		}
//	}
	return 0;
}

DWORD __stdcall GameClient::Receive(LPVOID lpParam)
{
	GameClient* Client = (GameClient*)lpParam;

//	while (true)
//	{
		fd_set  Read;
		int ret;
		FD_ZERO(&Read);
		FD_SET(Client->ClientSocket, &Read);

		ret = select(0, &Read, NULL, NULL, NULL);

		auto soc = Client->ClientSocket;

		if (FD_ISSET(Client->ClientSocket, &Read) && Client->ClientSocket != NULL)
		{
			//ͨ������������  
			char recvbuf[1024] = { 0 };
			ZeroMemory(recvbuf, 1024);
			int iLen = recv(Client->ClientSocket, recvbuf, 1024, 0);
			if (iLen <= 0) {
				return 0;
			}
			ZeroMemory(Client->RecvBuf, 1024);
			strcpy(Client->RecvBuf, recvbuf);
			cocos2d::log("receive message: %s ", Client->RecvBuf);
			char c;
			float x, y;
			sscanf(recvbuf, "%f%c%f", &x, &c, &y);
			auto move = MoveTo::create(0.5, Vec2(x, y));
			Client->hero->runAction(move);
//			Client->hero->moveTo_directly(Vec2(x, y));
/*			if (iLen == SOCKET_ERROR || iLen <= 0 )//|| iLen != (int)strlen(recvbuf) + 1
			{
				cocos2d::log("iLen is %d", iLen);
				cocos2d::log("(int)strlen(recvbuf) is %d", (int)strlen(recvbuf));
				cocos2d::log("recvbuf is %s", recvbuf);
				//				printf("���ճ��ִ����:%d\n", WSAGetLastError());
				cocos2d::log("receive number wrong:%d", WSAGetLastError());
				if (Client->ClientSocket != NULL)
					closesocket(Client->ClientSocket);
				Client->ClientSocket = NULL;
				//				printf("�����̹߳ر�\n");
				//				printf("����[%d]\n����[%d]\n", Client->SendThreadID, Client->RecvThreadID);
				cocos2d::log("receive thread close");
				cocos2d::log("send[%d] receive[%d]", Client->SendThreadID, Client->RecvThreadID);
				ExitThread(Client->RecvThreadID);
				ExitThread(Client->SendThreadID);
				//				printf("�����̹߳ر�\n");
				return 0;
			}*/

		}
//	}
	return 0;
}


int GameClient::AddBuf(char c, int tag, float x, float y)
{

	return 0;
}

void  GameClient::CleanUp()
{
	closesocket(ClientSocket);
	WSACleanup();
}
