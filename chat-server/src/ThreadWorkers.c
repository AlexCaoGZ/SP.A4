/*
*  FILE			  	: ThreadWorkers.c
*  PROJECT		  	: SENG2030 - Assignment#4
*  PROGRAMMER		: Zijia Cao & Yutong Ji
*  FIRST VERSION	: 2022-03-29
*  DECRIPTION		:
*	    This file contains 3 workers.
*/
#include "../inc/serverHeader.h"

/*
*Function   :	recvMsg()
*Description:	This function will recv the msg from client
*Parameters :	struct threadArgs
*Returns    :	void *
*/
void *recvMsg(void *arg){
    //=====load the arguments
    struct threadArgs *Args = (struct threadArgs*)arg;
    int socket =Args->socket;
    int Index = Args->mlIndex;
    int myID = Args->ml->ClientID[Index];
    MasterList *ml=Args->ml;

    char msg[BUFFERSIZE] = { 0 };
    while (recv(socket, msg, sizeof(msg), 0))
	{
        Node *head = Args->head;
        //setup timestamp
        time_t rawtime;
        struct tm t;

        rawtime = time (&rawtime);
        localtime_r(&rawtime,&t);

        //check if the index go error since every recv
        for(int i=0;i<=MAX_CLIENT;i++){
            if(myID==ml->ClientID[i]){
                Index=i;
                break;
            }
        }
        
        //for client exit
        if(strcmp(msg,">>bye<<")==0){
            //clean the data for this thread
            ml->ClientID[Index]=-1;
            ml->ClientSocket[Index]=-1;
            //if this is the last client/thread switch the status to exit
            //late on another thread will exit the whole peogram
            if(ml->numberOfClients==0){
                ml->numberOfClients=EXIT;
            }
            else{
                //if not 0,just delete this client's record
                ml->numberOfClients--;
            }
            strcpy(ml->ClientIP[Index],"0");    //clear the ip record
            sorting(ml);                        //sort the record
            close(socket);                      //close socket
            break;                              //break the while loop
        }
        else if(strlen(msg)>MAX_INCOME_MESSAGE_LENGTH-1){   //if the msg's size bugger than 40
            char buffer1[MAX_FORMATED_MESSAGE_LENGTH]={0};  //spilt to 2 message
            char buffer2[MAX_FORMATED_MESSAGE_LENGTH]={0};
            strncpy(buffer1,msg,MAX_INCOME_MESSAGE_LENGTH);
            strncpy(buffer2,msg+MAX_INCOME_MESSAGE_LENGTH,MAX_INCOME_MESSAGE_LENGTH);

            char buffer3[MAX_FORMATED_MESSAGE_LENGTH]={0};
            char buffer4[MAX_FORMATED_MESSAGE_LENGTH]={0};

            //Here a very silly way is used, the messages are divided into msg1 and msg2 and the ID of the sender is stored 
            //in the linkedlist, and we choose whether to send msg1 or msg2 based on the ID
            //that's how we to show the directionality of the message
            formatting( buffer1, ml->ClientID[Index],ml->ClientIP[Index],ml->ClientName[Index],t,buffer3,buffer4);
            Node *new1=createLL();
            strncpy(new1->msg1,buffer3,MAX_FORMATED_MESSAGE_LENGTH);
            strncpy(new1->msg2,buffer4,MAX_FORMATED_MESSAGE_LENGTH);
            new1->sourceID=ml->ClientID[Index];
            new1->sent=FALSE;
            insert(head,new1);
            head->sourceID=ml->ClientID[Index];

            //I forget why there is a sleep() here, but there must be a reason.
            sleep(0.1);
            //UPDATE: In early testing it was found that if two messages are sent in a very short time, 
            //there is a possibility that the order will be wrong, so there is a sleep() here.

            //the message that exceeds 40 characters is processed here
            formatting( buffer2, ml->ClientID[Index],ml->ClientIP[Index],ml->ClientName[Index],t,buffer3,buffer4);
            Node *new2=createLL();
            strncpy(new2->msg1,buffer3,MAX_FORMATED_MESSAGE_LENGTH);
            strncpy(new2->msg2,buffer4,MAX_FORMATED_MESSAGE_LENGTH);
            new2->sourceID=ml->ClientID[Index];
            new2->sent=FALSE;
            insert(head,new2);
        }
        else{
            //less than 40 characters
            char buffer1[MAX_FORMATED_MESSAGE_LENGTH]={0};
            char buffer2[MAX_FORMATED_MESSAGE_LENGTH]={0};
            formatting( msg, ml->ClientID[Index],ml->ClientIP[Index],ml->ClientName[Index],t,buffer1,buffer2);
            Node *new=createLL();
            strncpy(new->msg1,buffer1,MAX_FORMATED_MESSAGE_LENGTH);
            strncpy(new->msg2,buffer2,MAX_FORMATED_MESSAGE_LENGTH);
            new->sourceID=ml->ClientID[Index];
            new->sent=FALSE;
            insert(head,new);
            head->sourceID=ml->ClientID[Index];
        }
        //reset msg
		memset(msg, 0, BUFFERSIZE);

	}
    //when the while loop been break, exit this thread.
    pthread_exit(0);

}

/*
*Function   :	sendMsg()
*Description:	This function will send the msg to clients
*Parameters :	struct threadArgs
*Returns    :	void *
*/
void *sendMsg(void *arg){
    //=====get arguments
    struct threadArgs *Args = (struct threadArgs*)arg;
    MasterList *ml=Args->ml;

    while(ml->numberOfClients==-1){
        //no client yet, a empty loop
    }

    while(TRUE){
        //Basically it iterates through the linked list, sending all the ones with sent==0 to each client in order
        Node *head = Args->head;

        Node *p=head;
        while(p!=NULL){
            if(p->sent!=TRUE&&p->sourceID!=0){
                for(int i=0;i<=(ml->numberOfClients);i++){
                    if(ml->ClientSocket[i]!=-1){
                        int socket = ml->ClientSocket[i];
                        char buffer[100]={0};
                        if(p->sourceID==ml->ClientID[i]){//Decide whether to send >> or << based on the sourceID
                            strncpy(buffer,p->msg2,MAX_FORMATED_MESSAGE_LENGTH);
                            send(socket,buffer,MAX_FORMATED_MESSAGE_LENGTH,0);
                        }
                        else{
                            strncpy(buffer,p->msg1,MAX_FORMATED_MESSAGE_LENGTH);
                            send(socket,buffer,MAX_FORMATED_MESSAGE_LENGTH,0);
                        }
                        
                    }
               }
                p->sent=TRUE;
                
            }
            if(p->sent==TRUE) p=p->next;
        }
            
        
    }
}


/*
*Function   :	linkedListMananger()
*Description:	This function will delete some useless nodes and moniting if the program need to exit
*Parameters :	struct threadArgs
*Returns    :	void *
*/
void *linkedListMananger(void *arg){
    struct threadArgs *Args = (struct threadArgs*)arg;
    Node* head = Args->head;
    MasterList *ml=Args->ml;
    while(1){
        //delete the sent Node
        head = deleteNode(head);
        //and ready for exit
        if(ml->numberOfClients==EXIT){
            printf("All Client exited.\n");
            close((ml->mainSocket[0]));
            close((ml->mainSocket[1]));
            exit(0);
        }
    }
}