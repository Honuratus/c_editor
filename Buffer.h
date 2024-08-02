#ifndef BUFFER_H
#define BUFFER_H

#define INITIAL_CAPACITY 4096 // 4kb
#define G_FACTOR 2 // growth factor

typedef struct{
    char* content;
    int length;
    int capacity;
}TextBuffer;

TextBuffer* initTextBuffer();
void insertChar(TextBuffer* tb, int position, char ch);
void deleteChar(TextBuffer* tb, int position);
void displayBuffer(TextBuffer* tb, int x, int y);
void addNlToBuffer(TextBuffer* tb);

int xyToIndex(int x, int y, char* bufferContent);
int getBufferContent(char* filename, TextBuffer* tb);

#endif
