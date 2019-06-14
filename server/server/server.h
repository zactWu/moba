#pragma once
#include <winsock2.h>
#include <string>
#pragma comment(lib,"ws2_32.lib")

struct  ClientInformation           //Client 相关信息
{
	SOCKET ClientSock;       //套接字
	sockaddr_in Client;      //Clinet IP
	int ID;                   //Server分配的ID号码
	DWORD RecvThreadID;        //Server分配接收的线程号
	DWORD SendThreadID;			//Server分配发送的线程号
	bool Active;
};

class GameServer
{
protected:
	enum {
		MAX_NUM = 2  //最大上限人数  
	};
public:
	GameServer();
	~GameServer();
	int ProcessGameServer();    //线程处理
	int CheckSocket();     //检测当前可用的ID号
	static int SendMessageToOneClient(int ID, const std::string  str);  //向某一个Client发送信息
	static void SendMessageToAllClient(const std::string  str, int ID = -1); //向所有Client发送信息
	void CleanSocket(int ID); //清空ID号的套接字

	//Socket 相关
public:
	static DWORD WINAPI ListenThread(void* data); //接受线程


protected:
	SOCKET ListenSocket;       //等待接受数据的socket
	static ClientInformation AcceptSocket[MAX_NUM];  //Client的相关信息
	sockaddr_in Server;        //绑定地址

};

