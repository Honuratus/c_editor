#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Displayer.h"



void displayLines(char* bufferContent){
	unsigned int y = 0;

	char* start = bufferContent;
	char* end = start;
	while ((end = strchr(start, '\n')) != NULL){
		unsigned int length = end - start;
		char line[length+1];
		strncpy(line, start, length);
		line[length] = '\0';
		mvprintw(y, 0, line);
		y++;
		start = end + 1;
	}

	if(*start == '\0'){
		mvprintw(y,0,start);
	}
	move(0,0);
}

	

