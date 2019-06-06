#include "client.h"
#include <fstream>
#include <cocos2d.h>
#include "GameScene.h"
#include <UnitClass.h>
#include <mutex>
#include <cstring>
#include "Hero.h"
#define PORTS 1236

extern int InformationNumber;
extern GameClient client;
std::mutex gameLock;

extern std::queue<information> receiveQueue;

bool GameClient::init(Unit* h)
{
	hero = h;
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

	//	ServerAddr.sin_addr.S_un.S_addr = inet_pton(AF_INET, "127.0.0.1", &ServerAddr.sin_addr.S_un.S_addr);
	ServerAddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.104");
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

	HANDLE sendThread, recvThread, controlThread;
	sendThread = CreateThread(NULL, 0,
		static_cast<LPTHREAD_START_ROUTINE>(GameClient::Send),
		static_cast<LPVOID>(this), 0,
		NULL
	);
	if (nullptr == sendThread) {
		DWORD k=GetLastError();
		cocos2d::log("send wrong, error is %d", k);
//		cocos2d::log("send thread wrong");
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
//		cocos2d::log("receive thread wrong");
	}
	else {
		CloseHandle(recvThread);
	}
	
	controlThread = CreateThread(NULL, 0,
		static_cast<LPTHREAD_START_ROUTINE>(GameClient::control),
		static_cast<LPVOID>(this), 0,
		NULL
	);
	if (nullptr == controlThread) {
		DWORD k = GetLastError();
		cocos2d::log("send wrong, error is %d", k);
		//		cocos2d::log("send thread wrong");
	}
	else {
		CloseHandle(controlThread);
	}
	

//	HANDLE  RecvThread = nullptr, SendThread = nullptr;
//	if ((SendThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Send, this, 0, &SendThreadID)) == nullptr) {
////		cocos2d::log("sendThread wrong");
//		cocos2d::log("wrong sendThreadId is %d", SendThreadID);
//	}
//	if (nullptr != SendThread) {
////		cocos2d::log("sendThreadId is %d", SendThreadID);
//		CloseHandle(SendThread);
//	}
//
//	if ((RecvThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Receive, this, 0, &RecvThreadID)) == nullptr) {
////		cocos2d::log("receiveThread wrong");
//		cocos2d::log("wrong recvThreadId is %d", SendThreadID);
//	}
//	if (nullptr != RecvThread) {
////		cocos2d::log("recvThreadId is %d", SendThreadID);
//		CloseHandle(RecvThread);
//	}
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
				char sendbuf[30 * 100] = { 0 };
				strcpy(sendbuf, Client->SendBuf);

				int const iSend = send(Client->ClientSocket, sendbuf, strlen(sendbuf), 0);

				ZeroMemory(Client->SendBuf, 30 * 100);
				client.InfNum = 0;
				cocos2d::log("send message :%s\n", sendbuf);
			}

			if ('\0' != Client->ChatBuf[0]) {
				char chatbuf[100] = { 0 };
				strcpy(chatbuf, Client->ChatBuf);
				int const chatSend = send(Client->ClientSocket, chatbuf, 100, 0);
				ZeroMemory(Client->ChatBuf, 100);
			}
//			if (iSend <= 0)
//			{
//				if (Client->ClientSocket != NULL)
//					closesocket(Client->ClientSocket);
//				Client->ClientSocket = NULL;
////				printf("发送线程关闭\n");
////				printf("发送[%d]\n接收[%d]\n", Client->SendThreadID, Client->RecvThreadID);
//				cocos2d::log("sendTread close");
//				cocos2d::log("send[%d] receive[%d]", Client->SendThreadID, Client->RecvThreadID);
//				ExitThread(Client->SendThreadID);
//				ExitThread(Client->RecvThreadID);
////				printf("发送线程关闭\n");
//				return 0;
//			}
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
			char recvbuf[30*100] = { 0 };
			ZeroMemory(recvbuf, 30*100);
			gameLock.lock();
			int iLen = recv(Client->ClientSocket, recvbuf, 30*100, 0);
			if (iLen <= 0) {
				gameLock.unlock();
				return 0;
			}
			ZeroMemory(Client->RecvBuf, 30*100);
			strcpy(Client->RecvBuf, recvbuf);
			cocos2d::log("receive message: %s ", recvbuf);

			if (':' == recvbuf[0]) {			//'C'代表聊天信息
				cocos2d::log("%s", recvbuf);
				strcpy(Client->ChattingInfirmationFromTheOther, recvbuf);
				Client->UpdateChatMessage = true;  //现在需要更新聊天信息
			}
			else {								//如果不是聊天信息
				for (int i = 0; i < 100; i++) {
					if ('\0' == recvbuf[i * 30]) {
						break;
					}
					char c, tag;
					float x, y;
					sscanf(&recvbuf[i * 30], "%c%d#%f#%f", &c, &tag, &x, &y);
					information inf;
					inf.c = c;
					inf.tag = tag;
					inf.x = x;
					inf.y = y;
					cocos2d::log("inf is %c %d %f %f", inf.c, inf.tag, inf.x, inf.y);
					//			static float xx = 10.0;
					//			static float yy = 10.0;
					//			cocos2d::log("push");
					receiveQueue.push(inf);
				}
			}
			
//			information temp = receiveQueue.front();
//			cocos2d::log("%f %f", temp.x, temp.y);
//			auto move = MoveTo::create(0.5, Vec2(x, y));
//			Client->hero->runAction(move);
//			Client->hero->moveTo_directly(Vec2(x, y));
			gameLock.unlock();
//			xx += 10;
//			yy += 10;
			//if (iLen == SOCKET_ERROR || iLen <= 0 )//|| iLen != (int)strlen(recvbuf) + 1
			//{
			//	cocos2d::log("iLen is %d", iLen);
			//	cocos2d::log("(int)strlen(recvbuf) is %d", (int)strlen(recvbuf));
			//	cocos2d::log("recvbuf is %s", recvbuf);
			//	//				printf("接收出现错误号:%d\n", WSAGetLastError());
			//	cocos2d::log("receive number wrong:%d", WSAGetLastError());
			//	if (Client->ClientSocket != NULL)
			//		closesocket(Client->ClientSocket);
			//	Client->ClientSocket = NULL;
			//	//				printf("接收线程关闭\n");
			//	//				printf("发送[%d]\n接收[%d]\n", Client->SendThreadID, Client->RecvThreadID);
			//	cocos2d::log("receive thread close");
			//	cocos2d::log("send[%d] receive[%d]", Client->SendThreadID, Client->RecvThreadID);
			//	ExitThread(Client->RecvThreadID);
			//	ExitThread(Client->SendThreadID);
			//	//				printf("接收线程关闭\n");
			//	return 0;
			//}

		}
	}
	return 0;
}

DWORD __stdcall GameClient::control(LPVOID lpParam)
{
	GameClient* Client = static_cast<GameClient*>(lpParam);
	if (nullptr == Client) {
		return 0;
	}
	while (true)
	{
		information temp{};
		
		gameLock.lock();
		if (!receiveQueue.empty()) {
			static clock_t _last_time = 0;
			float pass_time = clock() - _last_time;
			
			temp = receiveQueue.front();
			receiveQueue.pop();
			if (pass_time > 170) {
				_last_time = clock();

				switch (temp.c)
				{
				case 'M':		//移动
					Client->MoveControl(Client, &temp);
					break;
				case 'Q':		//Q技能
					Client->Q_Skill(Client, &temp);
					break;
				case 'W':		//W技能
					Client->W_Skill(Client, &temp);
					break;
				case 'E':		//E技能
					Client->E_Skill(Client, &temp);
					break;
				default:
					break;
				}
				
			}
			gameLock.unlock();
		}
		else {
			gameLock.unlock();
			continue;
		}


	}

	return 0;
}



bool GameClient::MoveControl(GameClient* Client, const information* temp)
{
	order od;
	od.kind = 1;
	od.pos.x = temp->x;
	od.pos.y = temp->y;
	if (Client->hero->order_list.size() < 3) {
		Client->hero->order_list.push_back(od);
		return true;
	}
}

//技能QWE对应123
bool GameClient::Q_Skill(GameClient* Client, const information* temp)
{
	order od;
	od.kind = 1;
	od.pos.x = temp->x;
	od.pos.y = temp->y;
	od.tag = temp->tag;
	if (Client->hero->order_list.size() < 3) {
		Client->hero->order_list.push_back(od);
	}
	return true;
}

bool GameClient::W_Skill(GameClient* Client, const information* temp)
{
	order od;
	od.kind = 2;
	od.pos.x = temp->x;
	od.pos.y = temp->y;
	od.tag = temp->tag;
	if (Client->hero->order_list.size() < 3) {
		Client->hero->order_list.push_back(od);
	}
	return true;
}

bool GameClient::E_Skill(GameClient* Client, const information* temp)
{
	order od;
	od.kind = 3;
	od.pos.x = temp->x;
	od.pos.y = temp->y;
	od.tag = temp->tag;
	if (Client->hero->order_list.size() < 3) {
		Client->hero->order_list.push_back(od);
	}
	return true;
}





int GameClient::AddBuf(char c, int tag, float x, float y)
{
	gameLock.lock();
	char inf[30];
	sprintf_s(inf, "%c%d#%f#%f", c, tag, x, y);
//	sprintf_s(SendBuf, "%s", inf);
	strcat(&client.SendBuf[client.InfNum*30], inf);
	client.InfNum++;
	cocos2d::log("sendbuf is %s", client.SendBuf);
	gameLock.unlock();
	return 0;
}

void  GameClient::CleanUp()
{
	closesocket(ClientSocket);
	WSACleanup();
}
