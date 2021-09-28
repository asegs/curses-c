//
// Created by arctic on 9/24/21.
//

/*
 * To do:
 * Store text as buffer
 * Change position when text is rendered and any move function is called
 * Use buffer function to allow highlighting of text blocks
 * Allow responses directly to different key inputs from user, like arrow keys and enter
 * Limit writing on certain protected coords
 *
 */
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

const char * RESET = "\033[0m";

struct Style {
    char * text;
    int size;
};

const int WIDTH = 210;
const int HEIGHT = 50;

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
    if (n > 0){
        printf("\033[%dD",n);
    }
}

void move_right(int n){
    if (n > 0){
        printf("\033[%dC",n);
    }
}

void move_down(int n){
    if (n > 0){
        printf("\033[%dB",n);
    }
}

void move_up(int n){
    if (n > 0){
        printf("\033[%dA",n);
    }
}

void render(){
    fflush(stdout);
}

void print_render(char * message,int m_len){
    printf("%s",message);
    render();
    move_left(m_len);
}

void move_to(int x, int y){
    if (x - X_POS > 0){
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

void wipe_n_tiles(int n){
    printf("%s",RESET);
    while (n-- >= 0){
        putchar(' ');
    }
    fflush(stdout);
}

void place_at(char * message,int x,int y,int m_len){
    move_to(x,y);
    print_render(message,m_len);
}

void wipe_at(int n, int x, int y) {
    move_to(x,y);
    wipe_n_tiles(n);
}

void place_st_at(struct Style * st,char * message,int x, int y){
    char * to_place = malloc(strl(st->text) + strl(message));
    sprintf(to_place,st->text,message);
    place_at(to_place,x,y,strl(message));
    free(to_place);
}

/*
 * Style builder functions
 */

void safe_cat (struct Style * st, char * new_message){
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

struct Style * init_style(){
    char * style_buffer = malloc(STYLE_BUFFER_SIZE);
    strcpy(style_buffer,"\033[");
    struct Style * st = malloc(sizeof (struct Style));
    st->text = style_buffer;
    st->size = STYLE_BUFFER_SIZE;
    return st;
}

void add_simple_style(struct Style * st,int style_const){
    char * style_buf = malloc(16);
    sprintf(style_buf,"%d;",style_const);
    safe_cat(st,style_buf);
}

void add_rgb_style_fg(struct Style * st,int r, int g, int b){
    char * style_code = malloc(32);
    sprintf(style_code,"38;2;%d;%d;%d;",r,g,b);
    safe_cat(st,style_code);
    free(style_code);
}

void add_rgb_style_bg(struct Style * st,int r,int g,int b){
    char * style_code = malloc(32);
    sprintf(style_code,"48;2;%d;%d;%d;",r,g,b);
    safe_cat(st,style_code);
    free(style_code);
}

struct Style * finish(struct Style * st){
    st->text[strl(st->text)-1] = 'm';
    safe_cat(st,"%s\033[0m");
    return st;
}

void tile(int odd,int r,int g,int bl){
    struct Style * red = init_style();
    struct Style * green = init_style();
    add_rgb_style_bg(green,r,g,bl);
    add_rgb_style_bg(red,255-r,255-g,255-bl);
    finish(green);
    finish(red);
    int i,b = 0;
    for (i = 0;i<HEIGHT;i++){
        for (b = 0;b<WIDTH;b++){
            if (odd == 0){
                place_st_at(green," ",b,i);
            } else {
                place_st_at(red," ",b,i);
            }
            odd = !odd;
        }
        if (WIDTH % 2 == 0){
            odd = !odd;
        }
    }
}

int main(){
    init();
    struct Style * st = init_style();
    add_rgb_style_fg(st,0,0,0);
    add_simple_style(st,UL);
    add_simple_style(st,BOLD);
    add_simple_style(st,ITAL);
    add_rgb_style_bg(st,255,255,255);
    finish(st);
//    place_st_at(st," Hello friends! ",0,0);
//    place_st_at(st," Welcome home. ",5,5);
//    place_st_at(st," And again. ",10,10);
//    wipe_at(4,10,10);
    int odd = 0;
    int ct = 0;
    for (int r = 0;r<256;r++){
        for (int g = 0; g < 256; g++){
            for (int b = 0; b< 256;b++){
                tile(odd,r,g,b);
                ct++;
            }
            for (int b = 255;b>=0;b--){
                tile(odd,r,g,b);
                ct++;
            }
        }
    }
}