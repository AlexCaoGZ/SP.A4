/*
*  FILE			  	  : Windows.c
*  PROJECT		  	: SENG2030 - Assignment#4
*  PROGRAMMER		  : Zijia Cao & Yutong Ji
*  FIRST VERSION	: 2022-03-29
*  DECRIPTION		  :
*	    This file contains all functions related to window
*/
#include "../inc/clientHeader.h"

/*
*Function   :	display_win()
*Description:	This function came from the demo
*Parameters :	WINDOW *boderWin,WINDOW *win, char *word, int whichRow,int msgWinHeight
*Returns    :	none
*/
void display_win(WINDOW *boderWin,WINDOW *win, char *word, int whichRow,int msgWinHeight)
{
    wmove(win, (whichRow+2), 1);    //Set cursor
    if(whichRow>=(msgWinHeight-1)){ 
        //Sometime it will interferes with the border,
        //So thats why this space is here.
        char word2[BUFSIZ]=" ";
        strcat(word2,word);
        strcat(word2,"\n");
        //print the text to window
        //mvwaddstr(win, 0, 0, word2);
        //printf("%s",word2);
        wprintw(win, word2);
    }else{
        strcat(word,"\n");
        wprintw(win, word);
    }
    //Redraw the box
    box(boderWin, 0, 0);
    //show the screen
    //wbkgd(boderWin,COLOR_PAIR(3));
    wrefresh(boderWin);
    //wbkgd(win,COLOR_PAIR(3));
    wrefresh(win);
}

/*
*Function   :	create_newwin()
*Description:	This function came from the demo
*Parameters :	int height, int width, int starty, int startx
*Returns    :	WINDOW *
*/
WINDOW *create_newwin(int height, int width, int starty, int startx)
{       
  WINDOW *local_win;
     
  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0);               
  wmove(local_win, 1, 1);            
  wrefresh(local_win);     
  return local_win;
}

/*
*Function   :	input_win()
*Description:	This function came from the demo, but switched to wgetnstr() to get userINput
*Parameters :	WINDOW *win, char *word
*Returns    :	void
*/
void input_win(WINDOW *win, char *word)
{
  int i, ch;
  int maxrow, maxcol, row = 1, col = 0;
     
  blankWin(win);                         
  getmaxyx(win, maxrow, maxcol);          
  bzero(word, BUFSIZ);
  wmove(win, 1, 1);                      
  wgetnstr(win,word,MAX_LENGTH_ALLOWED);
} 

/*
*Function   :	blankWin()
*Description:	This function came from the demo
*Parameters :	WINDOW *win
*Returns    :	void
*/
void blankWin(WINDOW *win)
{
  int i;
  int maxrow, maxcol;
     
  getmaxyx(win, maxrow, maxcol);
  for (i = 1; i < maxcol-2; i++)  
  {
    wmove(win, i, 1);
    refresh();
    wclrtoeol(win);
    wrefresh(win);
  }
  box(win, 0, 0);             
  wrefresh(win);
} 