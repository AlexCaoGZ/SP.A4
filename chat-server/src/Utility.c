/*
*  FILE			  	: Utility.c
*  PROJECT		  	: SENG2030 - Assignment#4
*  PROGRAMMER		: Zijia Cao & Yutong Ji
*  FIRST VERSION	: 2022-03-29
*  DECRIPTION		:
*	    This file contains functions that are difficult to classify
*/

#include "../inc/serverHeader.h"

/*
*Function   :	sorting()
*Description:	This function will organized the MasterList
*Parameters :	MasterList *ml
*Returns    :	void
*/
void sorting(MasterList *ml){
    for(int i=0;i<=MAX_CLIENT-1;i++){
        if(ml->ClientID[i]==-1){
            for(int o=i;o<=MAX_CLIENT-2;o++){
                ml->ClientID[o]=ml->ClientID[o+1];
                ml->ClientSocket[o]=ml->ClientSocket[o+1];
                strcpy(ml->ClientIP[o],ml->ClientIP[o+1]);
            }
            
        }
    }
}

/*
*Function   :	formatting()
*Description:	This function formatting the msg from client
*Parameters :	char* msg,int ID,char* IP,char *userName,struct tm t,char* returnMsgIn,char* returnMsgOut
*Returns    :	void
*/
void formatting(char* msg,int ID,char* IP,char *userName,struct tm t,char* returnMsgIn,char* returnMsgOut){

    //just a lot of strcpy() and strcat()
    char buffer[BUFFERSIZE]={0};
    char msg1[BUFFERSIZE]={0};
    char msg2[BUFFERSIZE]={0};

    int ipLen = strlen(IP);
    strcpy(buffer,IP);
    
    for(int i=ipLen;i<=IP_LENGTH;i++){
        strcat(buffer," ");
    }

    strcat(buffer,"[");
    int nameLen = strlen(userName);
    char BUF[MAX_USER_NAME_LENGTH]={0};
    strcpy(BUF,userName);
    for(int i=nameLen;i<=4;i++){
        strcat(BUF," ");
    }
    strcat(buffer,BUF);
    strcat(buffer,"] ");

    strcpy(msg1,buffer);
    strcpy(msg2,buffer);

    strcat(msg1,"<< ");
    strcat(msg2,">> ");

    char buffer2[1024]={0};

    int msgLen = strlen(msg);
    strcpy(buffer2,msg);

    
    for(int i=msgLen;i<=MAX_INCOME_MESSAGE_LENGTH-1;i++){   //-1 for '\n'
        strcat(buffer2," ");
    }


    strcat(buffer2," (");

    sprintf(buffer,"%02d:",t.tm_hour);
    strcat(buffer2,buffer);
    sprintf(buffer,"%02d:",t.tm_min);
    strcat(buffer2,buffer);
    sprintf(buffer,"%02d",t.tm_sec);
    strcat(buffer2,buffer);

    strcat(buffer2,")");

    strcat(msg1,buffer2);
    strcat(msg2,buffer2);

    strcpy(returnMsgIn,msg1);
    strcpy(returnMsgOut,msg2);

}

/*
*Function   :	handShake()
*Description:	this function will proform a handshake with the client, it will wait for the client to connect.
*Parameters :	int socket,int newID,char *userName
*Returns    :	int
*/
int handShake(int socket,int newID,char *userName){

    int newClient = FALSE;

    //recv the init packet from client
    packet Packet;
    initPacket(&Packet);
    char buffer[1024]={0};
    recv(socket,buffer,sizeof(buffer),0);
    memcpy(&Packet,buffer,sizeof(buffer));

    if(Packet.TAG==0 && newID !=ERROR ){
        strcpy(userName,Packet.userName);
        newClient = TRUE;
    }
    else{
        printf("Hand shake failed.");
    }  

    initPacket(&Packet);
    
    if(newClient==TRUE){
        Packet.TAG=1;
        Packet.ID=newID;
    }
    else{
        Packet.TAG=ERROR;
    }

    //sent the id to client
    send(socket,(char *)&Packet,sizeof(Packet),0);

    return newClient;
}

/*
*Function   :	initPacket()
*Description:	This function will clear the packet
*Parameters :	packet *pkt
*Returns    :	void
*/
void initPacket(packet *pkt){
    pkt->ID=0;
    memset(pkt->msg1, 0, strlen(pkt->msg1));
    memset(pkt->msg2, 0, strlen(pkt->msg2));
    pkt->TAG=0;
    memset(pkt->userName, 0, 6);
}