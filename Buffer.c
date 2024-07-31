#include "Buffer.h"
#include <stdlib.h>
#include <string.h>
#include <curses.h>


TextBuffer* initTextBuffer(){
    TextBuffer* tb = (TextBuffer*)malloc(sizeof(TextBuffer));
    tb->content = (char*)malloc(sizeof(char) * INITIAL_CAPACITY);
    tb->length = 0;
    tb->capacity = INITIAL_CAPACITY;
    return tb;
}

void insertChar(TextBuffer* tb, int position, char ch){
    if (position < 0 || position > tb->length) return; 
    memmove(&tb->content[position+1], &tb->content[position], tb->length - position);
    tb->content[position] = ch;
    tb->length++;   
}

void deleteChar(TextBuffer* tb, int position){
    if(tb->length <= 0){
        return;

    }
    
    memmove(&tb->content[position], &tb->content[position+1], tb->length - position - 1);
    tb->length--;   
}



