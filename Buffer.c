#include "Buffer.h"
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>

int fileExists(char* filename){
	return access(filename, F_OK) != -1;
}

TextBuffer* initTextBuffer(){
    TextBuffer* tb = (TextBuffer*)malloc(sizeof(TextBuffer));
    tb->content = (char*)malloc(sizeof(char) * INITIAL_CAPACITY);
    tb->length = 0;
    tb->capacity = INITIAL_CAPACITY;
	return tb;
}

void insertChar(TextBuffer* tb, int position, char ch){
    if (position < 0) return; 
	if (position > tb->length){
		return;
	}
	if (tb->length >= tb->capacity)
	{
		tb->capacity *= 2;
		tb->content = (char*)realloc(tb->content, tb->capacity * sizeof(char));
	}
    memmove(&tb->content[position+1], &tb->content[position], tb->length - position);
    tb->content[position] = ch;
    tb->length++;   
	tb->content[tb->length] = '\0';
}

void deleteChar(TextBuffer* tb, int position){
    if(tb->length <= 0){
        return;

    }
    
    memmove(&tb->content[position], &tb->content[position+1], tb->length - position);
    tb->length--;   
}

void addNlToBuffer(TextBuffer* tb){
	tb->content[tb->length] = '\n';
	tb->content[tb->length+1] = '\0';
}

int getBufferContent(char* filename, TextBuffer* tb){
	if (!fileExists(filename)){
		return 0;
	}
	FILE* file = fopen(filename, "r");
	if (!file){
		// exit
		return 0;
	}


	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	rewind(file);
	
	// how to update capacity

	tb->length = fileSize;
	tb->content  = (char*)malloc(sizeof(char) * (fileSize + 1));
	
	tb->capacity = fileSize;

	fread(tb->content, sizeof(char), fileSize, file);


	fclose(file);
	tb->content[fileSize] = '\0';
}



int xyToIndex(int x, int y, char* bufferContent){
	int index = 0;
	int currentLine = 0;

	while(currentLine < y && bufferContent[index] != '\0'){
		if (bufferContent[index] == '\n'){
			currentLine++;
		}
		index++;
	}
	
	index += x;
	return index;
}	

