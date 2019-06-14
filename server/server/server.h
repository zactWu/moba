#pragma once
#include <winsock2.h>
#include <string>
#pragma comment(lib,"ws2_32.lib")

struct  ClientInformation           //Client �����Ϣ
{
	SOCKET ClientSock;       //�׽���
	sockaddr_in Client;      //Clinet IP
	int ID;                   //Server�����ID����
	DWORD RecvThreadID;        //Server������յ��̺߳�
	DWORD SendThreadID;			//Server���䷢�͵��̺߳�
	bool Active;
};

class GameServer
{
protected:
	enum {
		MAX_NUM = 2  //�����������  
	};
public:
	GameServer();
	~GameServer();
	int ProcessGameServer();    //�̴߳���
	int CheckSocket();     //��⵱ǰ���õ�ID��
	static int SendMessageToOneClient(int ID, const std::string  str);  //��ĳһ��Client������Ϣ
	static void SendMessageToAllClient(const std::string  str, int ID = -1); //������Client������Ϣ
	void CleanSocket(int ID); //���ID�ŵ��׽���

	//Socket ���
public:
	static DWORD WINAPI ListenThread(void* data); //�����߳�


protected:
	SOCKET ListenSocket;       //�ȴ��������ݵ�socket
	static ClientInformation AcceptSocket[MAX_NUM];  //Client�������Ϣ
	sockaddr_in Server;        //�󶨵�ַ

};

