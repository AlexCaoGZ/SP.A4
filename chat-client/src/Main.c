/*
*  FILE				: Main.c
*  PROJECT			: SENG2030 - Assignment#4
*  PROGRAMMER		: Zijia Cao & Yutong Ji
*  FIRST VERSION	: 2022-03-29
*  DECRIPTION		:
*	This is the main function, which prints the useage, renders the window, prepares the socket,
*   and hand shake with the server
*/
#include "../inc/clientHeader.h"

int main(int argc,char *argv[]){

    WINDOW *chat_win, *msgBoder_win, *msg_win,*input_promot_win,*msg_promot_win;
    int chat_startx, chat_starty, chat_width, chat_height;
    int msg_startx, msg_starty, msg_width, msg_height, i;

    int myId=-1;
    int port=19971;

    char userName[6]={0};
    char serverAddress[1024]={0};

    if(argc==3){
        if(strncmp(argv[1],"-user",5)==0 && strncmp(argv[2],"-server",7)==0){
            strncpy(userName,argv[1]+5,5);
            strncpy(serverAddress,argv[2]+7,strlen(argv[2]));
        }
        else{
            printUseage();
            return -1;
        }
    }
    else{
        printUseage();
        return -1;
    }

    initscr();                      
    cbreak();
    refresh();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    //=====Setting window's size
    chat_height = 5;
    chat_width  = COLS - 2;
    chat_startx = 1;        
    chat_starty = LINES - chat_height;        
     
    msg_height = 20 - chat_height - 1;
    msg_width  = COLS ;
    msg_startx = 0;        
    msg_starty = 0; 


    //=====Creating windows
    input_promot_win=newwin(2, chat_width, 17, chat_startx);
    mvwaddstr(input_promot_win, 0, getmaxx(input_promot_win)/2-15, "VV Outgoing message VV\n");
    wbkgd(input_promot_win,COLOR_PAIR(1));
    wrefresh(input_promot_win);

    msg_promot_win=newwin(2, msg_width, 15, msg_startx);
    mvwaddstr(msg_promot_win, 0, getmaxx(msg_promot_win)/2-10, "^^ message ^^\n");
    wbkgd(msg_promot_win,COLOR_PAIR(1));
    wrefresh(msg_promot_win);

    msgBoder_win = create_newwin(msg_height, msg_width, msg_starty, msg_startx);
    scrollok(msgBoder_win, TRUE);

    msg_win = derwin(msgBoder_win, msg_height - 1, msg_width - 2, msg_starty, msg_startx);
    wbkgd(msg_win,COLOR_PAIR(1));
    wrefresh(msg_win);
	scrollok(msg_win, TRUE);

    chat_win = create_newwin(chat_height, chat_width, chat_starty, chat_startx);
    scrollok(chat_win, TRUE);

    


    //=====Setting socket
    int socketClient=socket(AF_INET,SOCK_STREAM,0);
    if(socketClient==-1){
        //socket failed.
    }

    struct sockaddr_in server_addr = {0};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(serverAddress);
    int ret = connect(socketClient, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(ret == -1){
        return -1;
		//connect failed
	}


    //=====Start protocol hand shake
    myId = handShake(socketClient,userName);
    if(myId==-1){
        //Print error message
        char msg[1024]="Hand shake failed.";
        display_win(msgBoder_win,msg_win,msg,0,4);
        sleep(5);
        return -1;
    }


    //=====Create the struct for Theard's arguments
	pthread_t sendTheard;
    struct threadArgs tA;

    tA.socket=socketClient;
    tA.win=chat_win;
    //start thread        
   ret = pthread_create(&sendTheard,NULL,sendMsg,(void*)&tA);


    //======Recive the message from server
   int lines=0;
    while(1){
        char msg[BUFSIZ]={0};
        read(socketClient,msg,sizeof(msg));
        display_win(msgBoder_win,msg_win,msg,lines,(msg_height - 1));
        lines++;
    }

}

/*
*Function:		printUseage()
*Description:	This function will print the useage.
*Parameters:	void
*Returns:		none
*/
void printUseage(){
    printf("\nUsage: chat-client –user<userID> –server<server name>\n");
    printf("userID must be greater than 0 and less than 6 (0 & 6 not included.)\n");
    printf("ie: chat-client –userZCJJ –server192.168.0.1\n");
    printf("ie: chat-client –userJOKEE –serverUbuntu\n");
}

