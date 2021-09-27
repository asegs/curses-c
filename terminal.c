//
// Created by arctic on 9/24/21.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int DISABLE = 0;
const int BOLD = 1;
const int FAINT = 2;
const int ITAL = 3;
const int UL = 4;
const int BLINK_ON = 5;
const int SWAP = 7;
const int INVIS = 8;
const int STRIKE = 9;
const int STD = 10;
const int DOUBLE_UL = 21;
const int BLINK_OFF = 25;
const int SWAP_OFF = 27;
const int VIS = 28;
const int GRAY_FG = 30;
const int RED_FG = 31;
const int LB_FG = 32;
const int PINK_FG = 33;
const int BLUE_FG = 34;
const int MAG_FG = 35;
const int GREEN_FG = 36;
const int SALMON_FG = 37;
const int RES_FG = 39;
const int DARK_GREEN_BG = 40;
const int RED_BG = 41;
const int LB_BG = 42;
const int WHITE_BG = 43;
const int BLUE_BG = 44;
const int MAG_BG = 45;
const int GREEN_BG = 46;
const int BEIGE_BG = 47;
const int RES_BG = 49;
const int OL = 53;
const int RES_OL = 55;

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

struct StyledText * init_style(){
    char * style_buffer = malloc(STYLE_BUFFER_SIZE);
    strcpy(style_buffer,"\033[");
    struct StyledText * st = malloc(sizeof (struct StyledText));
    st->text = style_buffer;
    st->size = STYLE_BUFFER_SIZE;
    return st;
}

struct StyledText *  add_simple_style(struct StyledText * st,int style_const){
    char * style_buf = malloc(16);
    sprintf(style_buf,"%d;",style_const);
    safe_cat(st,style_buf);
    return st;
}

struct StyledText * add_rgb_style_fg(struct StyledText * st,int r, int g, int b){
    char * style_code = malloc(32);
    sprintf(style_code,"38;2;%d;%d;%d;",r,g,b);
    safe_cat(st,style_code);
    free(style_code);
    return st;
}

struct StyledText * add_rgb_style_bg(struct StyledText * st,int r,int g,int b){
    char * style_code = malloc(32);
    sprintf(style_code,"48;2;%d;%d:%d;",r,g,b);
    safe_cat(st,style_code);
    free(style_code);
    return st;
}

struct StyledText * add_message(struct StyledText * st,char * message){
    st->text[strl(st->text)-1] = 'm';
    safe_cat(st,message);
    return st;
}

struct StyledText * terminate(struct StyledText * st){
    safe_cat(st,"\033[0m");
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
    struct StyledText * st = init_style();
    st = add_rgb_style_fg(st,0,0,0);
    st = add_simple_style(st,UL);
    st = add_message(st,"Aaron!");
    st = terminate(st);


    printf("%s",st->text);
    fflush(stdout);
    while(1){
        ;
    }
}