#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "Buffer.h"


#define ctrl(x) ((x) & 0x1f)

#define BACKSPACE 263
#define ESCAPE 27
#define TWO_DOT 58
#define ENTER 10

#define UP_ARROW 259
#define DOWN_ARROW 258
#define LEFT_ARROW 260
#define RIGHT_ARROW 261



typedef enum{
    NORMAL,
    INSERT,
    COMMAND
}Mode;

Mode mode = NORMAL;

char* getModeName(){
    switch(mode){
        case NORMAL:
            return "NORMAL";
            break;
        case INSERT:
            return "INSERT";
            break;
        case COMMAND:
            return "COMMAND";
            break;
        default:
            "Unknown Mode";
            break;
    }
    return "Error";
}


void changeMode(char* filename){
        int x, y;
        int row;
        getyx(stdscr, y, x);
        row = getmaxy(stdscr);
        mvprintw(row-2, 0, getModeName());
        printw(" | ");
        printw(filename);        
}

int getLineEnd(int row){
    int maximumX;
    char ch;
    int x = 0;

    maximumX = getmaxx(stdscr);

    for(int i = 0; i<maximumX; i++){
        move(row, i);
        ch = inch() & A_CHARTEXT;
        if(ch != '\0' && ch != ' ' && ch != '\n'){
            x = i; 
        }
       
    }
    return x+1;
}

int main(int argc, char** argv){
    int ch; 
    int row, col;


    FILE* file = fopen(argv[1], "w");

    TextBuffer* tb = initTextBuffer();


    // initialization
    initscr();
    raw();
    keypad(stdscr, TRUE); // activating the keyboard 
    noecho();  // for not printing out my chracter when writing
 
    set_escdelay(0); // for no escape chracter delay 
  
    // this three lines are for the bottom info of the file and modes stuff
    getmaxyx(stdscr, row, col);
    changeMode(argv[1]);
    move(0,0);

    // cordinates variables
    int x = 0,y = 0;
    while((ch = getch()) != ctrl('q')){    
        switch(mode){
            case NORMAL:
                if(ch == 'i' || ch == 'a'){
                    mode = INSERT;
                    changeMode(argv[1]);
                    move(y,x);
                }
                else if (ch == TWO_DOT){
                    mode = COMMAND;
                    changeMode(argv[1]);
                    move(y, x);
                    mvprintw(row-1,0,":");
                }
                else if (ch == UP_ARROW){
                    y = y-1;
                    move(y,x);
                }
                else if(ch == DOWN_ARROW){
                    y = y+1;
                    move(y,x);
                }
                else if(ch == LEFT_ARROW){
                    x = x - 1;
                    move(y,x);
                }
                else if (ch == RIGHT_ARROW){
                    x = x + 1;
                    move(y,x);
                }

                break;
            case INSERT:
                getyx(stdscr,y,x);
                if(ch == BACKSPACE){
                    if(x > 0){
                        deleteChar(tb, (y * getmaxx(stdscr)) + (x-1));
                        move(y,x-1);
                        delch();
                        
                    }
                    else if (y > 0){
                        y = y-1;
                        x = getLineEnd(y);
                        move(y,x);
                    }
                }
                else if (ch == ESCAPE){
                    mode = NORMAL;
                    changeMode(argv[1]);
                    move(y, x);
                }else if (ch == UP_ARROW){
                    if (y > 0)
                        y = y-1;
                    move(y,x);
                }
                else if(ch == DOWN_ARROW){
                   if (y < getmaxy(stdscr) - 1)
                        y = y+1;
                   move(y,x);
                }
                else if(ch == LEFT_ARROW){
                    if (x > 0) x = x - 1;
                    move(y,x);
                }
                else if (ch == RIGHT_ARROW){
                    if (x < getmaxx(stdscr) - 1) x = x + 1;
                    move(y,x);
                }
                else if(ch == 10 || ch == '\r' || ch == KEY_ENTER || ch == '\n'){
                    int pos = (y * getmaxx(stdscr)) + x;
                    insertChar(tb, pos, '\n');
                    y = y+1;
                    x = 0;
                    move(y,x);
                }
                else{
                    int pos = (y * getmaxx(stdscr)) + x;
                    insertChar(tb, pos, ch);
                    addch(ch);
                }
                break;
            case COMMAND:
                if(ch == ESCAPE){
                    int commandX, commandY;
                    mode = NORMAL;
                    getyx(stdscr, commandY, commandX);
                    for (int i = commandX-1; i>=0; i--){
                        move(commandY, i);
                        delch();
                    }
                    changeMode(argv[1]);
                    move(y, x);
                    break;
                }
                else if(ch == ENTER){
                    // for now it's enter to normal mode but it's gonna execute the command
                    int commandX, commandY;
                    mode = NORMAL;
                    getyx(stdscr, commandY, commandX);
                    for (int i = commandX-1; i>=0; i--){
                        move(commandY, i);
                        delch();
                    }
                    changeMode(argv[1]);
                    move(y, x);
                    break;
                }
                else if (ch == BACKSPACE){ 
                    int commandX, commandY;
                    getyx(stdscr, commandY, commandX);
                    if (commandX > 1){                
                        move(commandY, commandX-1);
                        delch();
                    }
                }
                else{
                    addch(ch);         
                }
                break;
                
        }
        if (mode != COMMAND)
            getyx(stdscr, y, x);
        
    }



    
    fwrite(tb->content, tb->length, 1, file);

    fclose(file);

    refresh();
    endwin();  
    
    return 0;
}
