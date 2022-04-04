/*
*  FILE			  	: Network.c
*  PROJECT		  	: SENG2030 - Assignment#4
*  PROGRAMMER		: Zijia Cao & Yutong Ji
*  FIRST VERSION	: 2022-03-29
*  DECRIPTION		:
*	    This file contains all functions related to connection. like handshake, reflesh packet
*/
#include "../inc/clientHeader.h"

/*
*Function   :	initPacket()
*Description:	This function will clear the packet
*Parameters :	packet *pkt
*Returns    :	void
*/
void initPacket(packet *pkt){
    pkt->ID=0;
    memset(pkt->msg1, 0, MAX_INCOME_MESSAGE_LENGTH);
    memset(pkt->msg2, 0, MAX_INCOME_MESSAGE_LENGTH);
    pkt->TAG=0;
    memset(pkt->userName, 0, 6);
}

/*
*Function   :	handShake()
*Description:	This function will proform a handshake will server
*Parameters :	int socket,char *userName
*Returns    :	int
*/
int handShake(int socket,char *userName){

    int myID=-1;
    packet Packet;

    //send the init packet to server
    initPacket(&Packet);
    Packet.TAG=HAND_SHAKE_SATGE_1;
    strncpy(Packet.userName,userName,5);
    send(socket,(char *)&Packet,sizeof(Packet),0);

    //recv the id from server
    initPacket(&Packet);
    char buffer[1024]={0};
    recv(socket,buffer,sizeof(buffer),0);
    memcpy(&Packet,buffer,sizeof(buffer));

    if(Packet.TAG==HAND_SHAKE_SATGE_2){
        myID=Packet.ID;
    }
    //this id had been used to determine the directionality of the message
    //But things changed.
    return myID;
}

/*
*Function   :	sendMsg()
*Description:	This is the thread worker function, for send the msg to server
*Parameters :	void *arg
*Returns    :	void
*/
void* sendMsg(void *arg){
    //Get the arguments from struct threadArgs 
    struct threadArgs *Args = (struct threadArgs*)arg;
    int socketClient=Args->socket;
    WINDOW *win = Args->win;

    //Get userInput and respond to ">>bye<<".
    char buf[BUFSIZ] = {0};
	while(1)
	{
        input_win(win, buf);
		write(socketClient,buf,strlen(buf));
        if(strcmp(buf,">>bye<<")==0){
            exit(0);
        }
    }
}