/*
*  FILE			  	: serverHeader.c
*  PROJECT		  	: SENG2030 - Assignment#4
*  PROGRAMMER		: Zijia Cao & Yutong Ji
*  FIRST VERSION	: 2022-03-29
*  DECRIPTION		:
*	    This file contains the struct and the prototype  also the define for server.
*/
#include <stdio.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> //atoi()
#include <pthread.h>

#define MAX_CLIENT 10
#define BUFFERSIZE 81
#define MAX_FORMATED_MESSAGE_LENGTH 79
#define MAX_INCOME_MESSAGE_LENGTH 41
#define MAX_USER_NAME_LENGTH 6
#define IP_LENGTH 15
#define TRUE 1
#define FALSE 0
#define ERROR -1
#define INIT -1
#define WAIT_FOR_CONNECT -1
#define EXIT -99

typedef struct message
{
	char msg1[MAX_FORMATED_MESSAGE_LENGTH];
    char msg2[MAX_FORMATED_MESSAGE_LENGTH];
    int sent;
    int sourceID;
	struct message *next;
}Node;

typedef struct
{
    int TAG;
    int ID;
    char userName[MAX_USER_NAME_LENGTH];
    char msg1[MAX_INCOME_MESSAGE_LENGTH];
    char msg2[MAX_INCOME_MESSAGE_LENGTH];
}packet;

typedef struct {
	int numberOfClients;
	int ClientID[MAX_CLIENT];
    int ClientSocket[MAX_CLIENT];
    char ClientIP[MAX_CLIENT][30];
    char ClientName[MAX_CLIENT][6];
    int mainSocket[2];
}MasterList;

struct threadArgs{
    int socket;
    int mlIndex;
    MasterList *ml;
    Node *head;
};

int handShake(int socket,int newID,char *userName);
void sorting(MasterList *ml);
void initPacket(packet *pkt);
void *recvMsg(void *arg);
void *sendMsg(void *arg);
void *linkedListMananger(void *arg);
void formatting(char* msg,int ID,char* IP,char *userName,struct tm t,char* returnMsgIn,char* returnMsgOut);
Node *deleteNode(Node *head);
Node *insert(Node *head, Node *pNewNode);
Node *createLL();