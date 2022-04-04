/*
*  FILE			  	: Main.c
*  PROJECT		  	: SENG2030 - Assignment#4
*  PROGRAMMER		: Zijia Cao & Yutong Ji
*  FIRST VERSION	: 2022-03-29
*  DECRIPTION		:
*	    This contains the server's main() function, which will piggyback on the MasterList,
*       prepare the parameters needed by each thread, prepare the socket,
*       receive the connection and transfer the connection to the thread in charge.
*
*       The server uses sockaddr_in on line 104 of the file to get the client IP and save it to the MasterList.
*       The user ID is received during the handshake between the client and the server and is also stored in the MasterList. (in Utility.c file handShake() function)
*       The data structure packet (located in the serverHeader.h) is used during the handshake with the client.
*       After handshake, the socket is saved to the MasterList, at this point the message is passed directly, the client passes the 80 charasters msg and the server returns the formatted msg
*       The ">>bye<<" check  before the client's incoming msg being formatted.
*       After detecting ">>bye<<", the thread cleans up the records related to that client, such as ID, userName, IP, and closes the socket.
*       After the cleanup is complete, the sort() function is called and it adjusts the order within the MasterList for the next connection.
*       When all clients exit, the numberOfClients in the MasterList will be set to -99,
*       and the thread used to clean up the Linked list will start closing the socket used to receive new clients and call the exit() function after it detects -99
*
*
*       numberOfClients for how many client connected,          ClientID[MAX_CLIENT] for the client's id,       ClientSocket[MAX_CLIENT] for the socket that connect to client.
*       ClientIP[MAX_CLIENT][30] for client's ipaddress,        ClientName[MAX_CLIENT][6] for Client's name.
*       mainSocket[2] for the socket that accept new connection.
*
*       This is how the MasterList organized.
*       Not as good as the MasterList in assigenment3, which we didn't realize at first had so many things to put in it. 
*       By the time we felt something was wrong, the unorganized MasterList was already being put everywhere.
*/
#include "../inc/serverHeader.h"

int main(int argc,char *argv[]){

    int port=19971;
    int newClient =INIT;
    
    //=====Setting socket
    int socketServer = socket(AF_INET,SOCK_STREAM,0);
    if(socketServer==ERROR){
        printf("Create socket failed.\n");
        return -1;
    }

    struct sockaddr_in server_addr = {0};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

    int ret=bind(socketServer,(struct sockaddr *)&server_addr,sizeof(server_addr));
    if(ret == ERROR){
        printf("Bind port failed.\n");
        return -1;
	}

    ret = listen(socketServer,MAX_CLIENT);
    if(ret == ERROR){
        printf("Listen failed.\n");
        return -1;
	}

    printf("Server up.\n");

    //=====Setup MasterList for client managing 
    MasterList *ml=(MasterList*)malloc(sizeof(MasterList));
    ml->numberOfClients=WAIT_FOR_CONNECT;
    for(int i=0;i<=MAX_CLIENT;i++){
        ml->ClientID[i]=WAIT_FOR_CONNECT;
        strcpy(ml->ClientIP[i],"0");
        ml->ClientSocket[i]=WAIT_FOR_CONNECT;
    }

    Node *head = createLL();    //create LinkedList

    //=====Prepare & launch the sendThread
    pthread_t sendThread;
    struct threadArgs tASend;
    tASend.ml=ml;
    tASend.head=head;
    ret=pthread_create(&sendThread,NULL,sendMsg,(void*)&tASend);

    //=====Preparing the LinklistManangment thread, it will delete the sent message.
    pthread_t linkedListThread;
    struct threadArgs tAManager;
    tAManager.ml=ml;
    tAManager.head=head;
    tAManager.ml->mainSocket[0]=socketServer;
    tAManager.ml->mainSocket[1]=newClient;
    ret=pthread_create(&linkedListThread,NULL,linkedListMananger,(void*)&tAManager);

    //=====This part is used to accept connections, assign IDs, and start the recv threads
    while (TRUE)
    {
        //=====Setup socket
        struct sockaddr_in client_addr={0};
	    int len = sizeof(client_addr);
        //Wait for connection
        newClient = accept(socketServer,(struct sockaddr*)&client_addr,&len);
        //Setup the srand() for generating ID
        srand(time(NULL));
        //Assign ID to Client
        if(ml->numberOfClients<=MAX_CLIENT){
            int clientIndex = 0;
            int randomID=rand()%999999999;
        
            //avoid having same ID
            for(int i=0;i<=MAX_CLIENT;i++){
                if(ml->ClientID[i]==randomID){
                    //if have a same ID, gererate a new one
                    randomID=rand()%999999999;
                    //reset i to 0 for check the new ID
                    i=0;
                }
            }
            
            char userName[6]={0};
            if(handShake(newClient,randomID,userName)!=ERROR){
                //Hand shake completed, start assign ID
                //And record socket id and IP 
                ml->numberOfClients++;
                clientIndex=ml->numberOfClients;
                ml->ClientID[clientIndex]=randomID;
                ml->ClientSocket[clientIndex]=newClient;
                strcpy(ml->ClientIP[clientIndex],inet_ntoa(client_addr.sin_addr));
                strcpy(ml->ClientName[clientIndex],userName);

                //=====Prepare and launch the recvThread
                struct threadArgs tARecv;
                tARecv.socket=newClient;
                tARecv.ml=ml;
                tARecv.head=head;
                tARecv.mlIndex=clientIndex;
                pthread_t recvThread;
                ret=pthread_create(&recvThread,NULL,recvMsg,(void*)&tARecv);
            }
            else{
                //Hand shake failed, close socket
                close(newClient);
            }
        }
        
    }

}


