#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "server.h"
#include <iostream>


using std::cout;
using std::string;


ClientInformation GameServer::AcceptSocket[GameServer::MAX_NUM];

GameServer::GameServer()
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)   //判断一下有没有这个版本
	{
		cout << "无法加载套接字协议栈.\n";
		fflush(stdout);
		return;
	}

	//设置监听套接字
	ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //IPv4 STREAM TCP

	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "套接字初始化错误，错误号：" << WSAGetLastError() << "\n";
		fflush(stdout);
		return;
	}

	int port = 1236;

	//设置服务器的信息
	Server.sin_family = PF_INET;
	Server.sin_port = htons(port);
	Server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int errornum;
	//绑定
	bind(ListenSocket, (LPSOCKADDR)& Server, sizeof(Server));
	while ((errornum = WSAGetLastError()) == 10048)
	{
		port++;
		Server.sin_port = htons(port);
		int retval = bind(ListenSocket, (LPSOCKADDR)& Server, sizeof(Server));
		cout << "绑定错误，错误号10048\n";
		fflush(stdout);

	}
	cout << "服务器的IP为:" << Server.sin_addr.S_un.S_addr
		<< " 端口号为: " << port << "\n";
	fflush(stdout);
	//监听   有listen就有accept
	if (listen(ListenSocket, 5) == SOCKET_ERROR)
	{
		cout << "监听出错，错误号：" << WSAGetLastError() << "\n";
		fflush(stdout);
		return;
	}

	//将所有信息初始化
	for (int i = 0; i < MAX_NUM; i++)
		AcceptSocket[i].ClientSock = INVALID_SOCKET;
	cout << "网络初始化成功\n";
	fflush(stdout);
	return;
}

GameServer::~GameServer()
{
	if (ListenSocket != INVALID_SOCKET)
		closesocket(ListenSocket);
	cout << "服务器关闭\n";
	fflush(stdout);
	WSACleanup();
	return;
}

int GameServer::CheckSocket()
{
	for (int i = 0; i < MAX_NUM; i++)
	{
		if (AcceptSocket[i].ClientSock == INVALID_SOCKET)
			return i;
	}
	return -1;
}

int GameServer::ProcessGameServer()
{
	while (true)
	{
		int index = CheckSocket();
		sockaddr_in Client;

		int ClntLen = sizeof(Client);

		if (index != -1) //玩家未满
		{

			//对应前面的listen，这里是对应操作accept
			cout << "等待Client连接...\n";
			fflush(stdout);
			AcceptSocket[index].ClientSock = accept(
				ListenSocket,
				(struct sockaddr*) & AcceptSocket[index].Client,
				&ClntLen);
			AcceptSocket[index].ID = index;       //记录这个Client的ID啊，以后要寻找它
			AcceptSocket[index].Active = false;

			if (AcceptSocket[index].ClientSock == INVALID_SOCKET)
			{
				cout << "连接错误\n";
				fflush(stdout);
				break;
			}
			cout << "连接成功\n";
			fflush(stdout);
			//至此client与server连接成功,欢呼
			cout << "新玩家加入，IP地址为：" << inet_ntoa(AcceptSocket[index].Client.sin_addr)
				<< "  端口号为：" << ntohs(AcceptSocket[index].Client.sin_port) << "\n";
			fflush(stdout);
			//创建接受者线程
			int ThreadID;     //线程ID
			//把刚刚连接成功的Client建立一个新的线程
			ThreadID = (int)CreateThread(NULL, 0,
				(LPTHREAD_START_ROUTINE)(GameServer::ListenThread), //线程点函数
				(LPVOID)& AcceptSocket[index], 0,              //参数
				&AcceptSocket[index].RecvThreadID          //系统分配的ID
			);

			if (!ThreadID)
			{
				cout << "创建线程错误\n";
				fflush(stdout);
				ExitThread(AcceptSocket[index].RecvThreadID);
			}

			//至此，新的线程创建成功，可以传输数据了

			cout << "新玩家" << index << "的接受线程创建成功\n";
			fflush(stdout);
		}
		else   //玩家已满
		{
			SOCKET Accept = accept(ListenSocket,
				(struct sockaddr*) & Client, &ClntLen);

			if (Accept == INVALID_SOCKET)
			{
				cout << "连接错误\n";
				fflush(stdout);
				break;
			}


			cout << "非法请求的玩家的IP的地址为:" << inet_ntoa(Client.sin_addr)
				<< " 端口号为 :" << ntohs(Client.sin_port) << "\n";
			fflush(stdout);
			send(Accept, "当前用户已满", strlen("当前用户已满") + sizeof(char), 0);

			closesocket(Accept);

			cout << "非法连接玩家已断开\n";
			fflush(stdout);
			break;
		}
	}

	return 0;
}

DWORD WINAPI GameServer::ListenThread(void* data) //传进来具体哪个AcceptSocket[xx]的地址
{

	ClientInformation* GameSocket = (ClientInformation*)data;

	while (true)
	{

		fflush(stdout);

		//接收命令 

//		char recvBuf[1024];
		char recvBuf[110];
		fflush(stdout);

		fd_set Read;//基于select模式对IO进行管理  

		fflush(stdout);

		FD_ZERO(&Read);    //初始化为0
		FD_SET(GameSocket->ClientSock, &Read); //将ClientSock加入队列

		fflush(stdout);

		//we only care read
		select(0, &Read, NULL, NULL, NULL);

		fflush(stdout);

		if (FD_ISSET(GameSocket->ClientSock, &Read))
		{
			//接受客户端的数据
			int result = recv(GameSocket->ClientSock, recvBuf, sizeof(recvBuf), 0);
			cout << "recv 返回值: " << result << "\n";
			fflush(stdout);
			if (result > 0)
			{
				recvBuf[result] = 0;
				cout << "玩家" << GameSocket->ID << "发送了消息:"
					<< recvBuf << "\n";
				fflush(stdout);
			}
		}

		//发送命令 
//		char sendBuf[1024];
		char sendBuf[110];
		fd_set write;//基于select模式对IO进行管理  
		FD_ZERO(&write);    //初始化为0
		FD_SET(GameSocket->ClientSock, &write); //将ClientSock加入队列
		//we only care read
		select(0, NULL, &write, NULL, NULL);

		if (FD_ISSET(GameSocket->ClientSock, &write))
		{
			//接受客户端的数据
			strcpy(sendBuf, recvBuf);
			SendMessageToAllClient(sendBuf, GameSocket->ID);
		}

	}
	return 1;
}

int GameServer::SendMessageToOneClient(int ID, const string  str)
{
	if (ID < 0)
		return 0;

	int oneSend = send(AcceptSocket[ID].ClientSock,
		str.c_str(), str.length() + sizeof(char), 0);
	cout << "向玩家" << ID << "发送了" << oneSend << "个bits的消息\n";
	fflush(stdout);
	if (oneSend == SOCKET_ERROR)
	{
		cout << "向玩家" << ID << "发送消息失败\n";
		fflush(stdout);
		return 0;
	}

	return 1;
}

void GameServer::SendMessageToAllClient(const string  str, int ID)
{

	bool flag = true;
	for (int i = 0; i < MAX_NUM; i++)
	{
	if (ID != i)
		{
			if (AcceptSocket[i].ClientSock != INVALID_SOCKET &&
				!SendMessageToOneClient(i, str))
				flag = false;
		}
	}
	if (flag) {
		cout << "服务器向全体(除了玩家" << ID << ")发送了消息:"
			<< str << "\n";
	}
	else {
		cout << "发送存在问题\n";
	}
	fflush(stdout);
}

void GameServer::CleanSocket(int ID)
{
	if (ID < 0)
		return;

	char send[20];
	sprintf(send, "#DD i%d*", ID);
	SendMessageToAllClient(send, ID);


	cout << " 玩家" << ID << "退出游戏\n";
	fflush(stdout);

	AcceptSocket[ID].Active = false;
	closesocket(AcceptSocket[ID].ClientSock);
	AcceptSocket[ID].ClientSock = INVALID_SOCKET;

	cout << "正在关闭其接受线程:" << AcceptSocket[ID].RecvThreadID << "\n";
	fflush(stdout);
	ExitThread(AcceptSocket[ID].RecvThreadID);

	cout << "关闭成功!\n";
	fflush(stdout);
}