/*
*  FILE			  	: clientHeader.c
*  PROJECT		  	: SENG2030 - Assignment#4
*  PROGRAMMER		: Zijia Cao & Yutong Ji
*  FIRST VERSION	: 2022-03-29
*  DECRIPTION		:
*	    This file contains the struct and the prototype  also the define for client.
*/
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> 
#include <pthread.h>

#include <ncurses.h>
#include <stdarg.h>
#include <fcntl.h>

#define MAX_USER_NAME_LENGTH 6
#define MAX_INCOME_MESSAGE_LENGTH 41
#define HAND_SHAKE_SATGE_1 0
#define HAND_SHAKE_SATGE_2 1
#define MAX_LENGTH_ALLOWED 81

typedef struct
{
    int TAG;
    int ID;
    char userName[MAX_USER_NAME_LENGTH];
    char msg1[MAX_INCOME_MESSAGE_LENGTH];
    char msg2[MAX_INCOME_MESSAGE_LENGTH];
}packet;

struct threadArgs
{
    int socket;
    WINDOW *win;

};

void printUseage();
void* sendMsg(void *arg);
int handShake(int socket,char *userName);
void initPacket(packet *pkt);
WINDOW *create_newwin(int height, int width, int starty, int startx);
void input_win(WINDOW *win, char *word);
void blankWin(WINDOW *win);
void display_win(WINDOW *boderWin,WINDOW *win, char *word, int whichRow,int msgWinHeight);