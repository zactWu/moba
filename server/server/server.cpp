#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "server.h"
#include <iostream>


using std::cout;
using std::string;


ClientInformation GameServer::AcceptSocket[GameServer::MAX_NUM];
/*
���ƣ����캯��
����������Socket��ʼ��
*/

GameServer::GameServer()
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)   //�ж�һ����û������汾
	{
		cout << "�޷������׽���Э��ջ.\n";
		fflush(stdout);
		return;
	}

	//���ü����׽���
	ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //IPv4 STREAM TCP

	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "�׽��ֳ�ʼ�����󣬴���ţ�" << WSAGetLastError() << "\n";
		fflush(stdout);
		return;
	}

	int port = 1236;

	//���÷���������Ϣ
	Server.sin_family = PF_INET;
	Server.sin_port = htons(port);
	Server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int errornum;
	//��
	bind(ListenSocket, (LPSOCKADDR)& Server, sizeof(Server));
	while ((errornum = WSAGetLastError()) == 10048)
	{
		port++;
		Server.sin_port = htons(port);
		int retval = bind(ListenSocket, (LPSOCKADDR)& Server, sizeof(Server));
		cout << "�󶨴��󣬴����10048\n";
		fflush(stdout);

	}
	cout << "��������IPΪ:" << Server.sin_addr.S_un.S_addr
		<< " �˿ں�Ϊ: " << port << "\n";
	fflush(stdout);
	//����   ��listen����accept
	if (listen(ListenSocket, 5) == SOCKET_ERROR)
	{
		cout << "������������ţ�" << WSAGetLastError() << "\n";
		fflush(stdout);
		return;
	}

	//��������Ϣ��ʼ��
	for (int i = 0; i < MAX_NUM; i++)
		AcceptSocket[i].ClientSock = INVALID_SOCKET;

	cout << "�����ʼ���ɹ�\n";
	fflush(stdout);
	return;
}

/*
���ƣ���������
���ã�������Դ�ͷ�
*/
GameServer::~GameServer()
{
	if (ListenSocket != INVALID_SOCKET)
		closesocket(ListenSocket);
	cout << "�������ر�\n";
	fflush(stdout);
	WSACleanup();
	return;
}


/*
���ƣ����ID
���������ڼ�⵱ǰû���õ�ID��
*/
int GameServer::CheckSocket()
{
	for (int i = 0; i < MAX_NUM; i++)
	{
		if (AcceptSocket[i].ClientSock == INVALID_SOCKET)
			return i;
	}
	return -1;
}

/*
���ƣ��̴߳���
������Ϊÿһ������Ҵ���һ�������߳�
	  ��������ﵽ���ޣ��;ܾ�
*/
int GameServer::ProcessGameServer()
{
	while (true)
	{
		int index = CheckSocket();
		sockaddr_in Client;

		int ClntLen = sizeof(Client);

		if (index != -1) //���δ��
		{

			//��Ӧǰ���listen�������Ƕ�Ӧ����accept

			cout << "�ȴ�Client����...\n";
			fflush(stdout);
			AcceptSocket[index].ClientSock = accept(
				ListenSocket,
				(struct sockaddr*) & AcceptSocket[index].Client,
				&ClntLen);
			//cout << "#1 AcceptSocket[index].ClientSock: " << AcceptSocket[index].ClientSock << "\n";
			AcceptSocket[index].ID = index;       //��¼���Client��ID�����Ժ�ҪѰ����
			AcceptSocket[index].Active = false;

			if (AcceptSocket[index].ClientSock == INVALID_SOCKET)
			{
				cout << "���Ӵ���\n";
				fflush(stdout);
				break;
			}
			//cout << "#2 AcceptSocket[index].ClientSock: " << AcceptSocket[index].ClientSock << "\n";
			cout << "���ӳɹ�\n";
			fflush(stdout);
			//����client��server���ӳɹ�,����

			cout << "����Ҽ��룬IP��ַΪ��" << inet_ntoa(AcceptSocket[index].Client.sin_addr)
				<< "  �˿ں�Ϊ��" << ntohs(AcceptSocket[index].Client.sin_port) << "\n";
			fflush(stdout);
			//�����������߳�
			int ThreadID;     //�߳�ID

			//�Ѹո����ӳɹ���Client����һ���µ��߳�
			ThreadID = (int)CreateThread(NULL, 0,
				(LPTHREAD_START_ROUTINE)(GameServer::ListenThread), //�̵߳㺯��
				(LPVOID)& AcceptSocket[index], 0,              //����
				&AcceptSocket[index].RecvThreadID          //ϵͳ�����ID
			);

			if (!ThreadID)
			{
				cout << "�����̴߳���\n";
				fflush(stdout);
				ExitThread(AcceptSocket[index].RecvThreadID);
			}

			//���ˣ��µ��̴߳����ɹ������Դ���������

			cout << "�����" << index << "�Ľ����̴߳����ɹ�\n";
			fflush(stdout);
		}
		else   //�������
		{
			SOCKET Accept = accept(ListenSocket,
				(struct sockaddr*) & Client, &ClntLen);

			if (Accept == INVALID_SOCKET)
			{
				cout << "���Ӵ���\n";
				fflush(stdout);
				break;
			}


			cout << "�Ƿ��������ҵ�IP�ĵ�ַΪ:" << inet_ntoa(Client.sin_addr)
				<< " �˿ں�Ϊ :" << ntohs(Client.sin_port) << "\n";
			fflush(stdout);
			send(Accept, "��ǰ�û�����", strlen("��ǰ�û�����") + sizeof(char), 0);

			closesocket(Accept);

			cout << "�Ƿ���������ѶϿ�\n";
			fflush(stdout);
			break;
		}
	}

	return 0;
}

/*
���ƣ������߳�
������Selectģʽ
	�����жϸ��߳��Ƿ�ɶ�������ɶ��Ͷ�ȡ���ϵ���Ϣ
*/
DWORD WINAPI GameServer::ListenThread(void* data) //�����������ĸ�AcceptSocket[xx]�ĵ�ַ
{

	ClientInformation* GameSocket = (ClientInformation*)data;

	while (true)
	{

		fflush(stdout);

		//�������� 

		char recvBuf[1024];
//		char recvBuf[40];
		fflush(stdout);

		fd_set Read;//����selectģʽ��IO���й���  

		fflush(stdout);

		FD_ZERO(&Read);    //��ʼ��Ϊ0
		FD_SET(GameSocket->ClientSock, &Read); //��ClientSock�������

		fflush(stdout);

		//we only care read
		select(0, &Read, NULL, NULL, NULL);

		fflush(stdout);

		if (FD_ISSET(GameSocket->ClientSock, &Read))
		{
			//���ܿͻ��˵�����
			int result = recv(GameSocket->ClientSock, recvBuf, sizeof(recvBuf), 0);
			cout << "recv ����ֵ: " << result << "\n";
			fflush(stdout);
			if (result > 0)
			{
				recvBuf[result] = 0;
				cout << "���" << GameSocket->ID << "��������Ϣ:"
					<< recvBuf << "\n";
				fflush(stdout);
			}
		}

		//�������� 
		char sendBuf[1024];
//		char sendBuf[40];
		fd_set write;//����selectģʽ��IO���й���  
		FD_ZERO(&write);    //��ʼ��Ϊ0
		FD_SET(GameSocket->ClientSock, &write); //��ClientSock�������
		//we only care read
		select(0, NULL, &write, NULL, NULL);

		if (FD_ISSET(GameSocket->ClientSock, &write))
		{
			//���ܿͻ��˵�����
			strcpy(sendBuf, recvBuf);
			//cout << "Just Before Enter TOALL func, AcceptSocket[ID].ClientSock: " << GameSocket->ClientSock << "\n";
			SendMessageToAllClient(sendBuf, GameSocket->ID);
		}

	}
	return 1;
}

/*
���ƣ�������Ϣ��һ��client
������������Ϣ��һ��client
*/
int GameServer::SendMessageToOneClient(int ID, const string  str)
{
	if (ID < 0)
		return 0;

	int oneSend = send(AcceptSocket[ID].ClientSock,
		str.c_str(), str.length() + sizeof(char), 0);
	cout << "�����" << ID << "������" << oneSend << "��bits����Ϣ\n";
	fflush(stdout);
	if (oneSend == SOCKET_ERROR)
	{
		cout << "�����" << ID << "������Ϣʧ��\n";
		fflush(stdout);
		return 0;
	}

	return 1;
}

/*
���ƣ�ȫ����
������������Client������Ϣ
*/
void GameServer::SendMessageToAllClient(const string  str, int ID)
{

	bool flag = true;
	for (int i = 0; i < MAX_NUM; i++)
	{
//	if (ID != i)
//		{
			if (AcceptSocket[i].ClientSock != INVALID_SOCKET &&
				!SendMessageToOneClient(i, str))
				flag = false;
//		}
	}
	if (flag) {
		cout << "��������ȫ��(�������" << ID << ")��������Ϣ:"
			<< str << "\n";
	}
	else {
		cout << "���ʹ�������\n";
	}
	fflush(stdout);
}

/*
���ƣ�����Socket
�����������˳���Ϸ���߳�
*/
void GameServer::CleanSocket(int ID)
{
	if (ID < 0)
		return;

	char send[20];
	sprintf(send, "#DD i%d*", ID);
	SendMessageToAllClient(send, ID);


	cout << " ���" << ID << "�˳���Ϸ\n";
	fflush(stdout);

	AcceptSocket[ID].Active = false;
	closesocket(AcceptSocket[ID].ClientSock);
	AcceptSocket[ID].ClientSock = INVALID_SOCKET;

	cout << "���ڹر�������߳�:" << AcceptSocket[ID].RecvThreadID << "\n";
	fflush(stdout);
	ExitThread(AcceptSocket[ID].RecvThreadID);



	cout << "�رճɹ�!\n";
	fflush(stdout);
}