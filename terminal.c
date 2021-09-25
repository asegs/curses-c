//
// Created by arctic on 9/24/21.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StyledText {
    char * text;
    int size;
};


const int WIDTH = 90;
const int HEIGHT = 20;

const int STYLE_BUFFER_SIZE = 64;

int X_POS = 0;
int Y_POS = 0;

void init(){
    for (int i = 0;i<HEIGHT;i++){
        for (int b = 0;b<WIDTH;b++){
            printf(" ");
        }
        printf("\n");
    }
    X_POS = 0;
    Y_POS = HEIGHT+1;
}

int strl(const char * c){
    int ct = 0;
    int idx = 0;
    while (c[idx] != 0){
        ct++;
        idx++;
    }
    return ct;
}


void move_left(int n){
    printf("\033[%dD",n);
}

void move_right(int n){
    printf("\033[%dC",n);
}

void move_down(int n){
    printf("\033[%dB",n);
}

void move_up(int n){
    printf("\033[%dA",n);
}

void render(){
    fflush(stdout);
}

void print_render(char * message){
    printf("%s",message);
    render();
    move_left(strl(message));
}

void move_to(int x, int y){
    if (x - X_POS >= 0){
        move_right(x-X_POS);
    }else{
        move_left((x - X_POS) * -1);
    }

    if (y - Y_POS >= 0){
        move_down(y - Y_POS);
    }else{
        move_up((y - Y_POS) * -1);
    }
    X_POS = x;
    Y_POS = y;
}

void place_at(char * message,int x,int y){
    move_to(x,y);
    print_render(message);
}

/*
 * Style builder functions
 */

struct StyledText * init_style(){
    char * style_buffer = malloc(STYLE_BUFFER_SIZE);
    strcpy(style_buffer,"\\033[");
    struct StyledText * st = malloc(sizeof (struct StyledText));
    st->text = style_buffer;
    st->size = STYLE_BUFFER_SIZE;
    return st;
}

void safe_cat (struct StyledText * st, char * new_message){
    while (strl(st->text) + strl(new_message) >= st->size){
        printf("Reallocating, size: %d\n",st->size);
        char * new_mem = malloc(st->size * 2);
        strcpy(new_mem,st->text);
        free(st->text);
        st->text = new_mem;
        st->size *= 2;
    }
    strcat(st->text,new_message);
}

struct StyledText * finish(struct StyledText * st){
    safe_cat(st,"\\03[0");
    return st;
}






int main(){
    init();
    for (int i = 1;i<HEIGHT;i++){
        place_at("Hello bae",i,i);
    }
    for (int i = HEIGHT;i>=1;i--){
        place_at("Hello bae",HEIGHT + (HEIGHT - i),i);
    }

    printf("%s\n",finish(init_style())->text);
    while(1){
        ;
    }
}