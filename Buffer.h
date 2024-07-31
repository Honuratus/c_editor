#ifndef BUFFER
#define BUFFER

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

#endif
