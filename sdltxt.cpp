// This code will print a string to SDL_Renderer via SDL_RenderDrawPoint()
#include <SDL2/SDL.h> // Simple Directmedia Layer lib has to be installed
#include <string>

// 7x8 font generated via unix command "banner"
const std::string numbers =
"  ###      #     #####   #####  #       #######  #####  #######  #####   #####  "\
" #   #    ##    #     # #     # #    #  #       #     # #    #  #     # #     # "\
"# #   #  # #          #       # #    #  #       #           #   #     # #     # "\
"#  #  #    #     #####   #####  #######  #####  ######     #     #####   ###### "\
"#   # #    #    #             #      #        # #     #   #     #     #       # "\
" #   #     #    #       #     #      #  #     # #     #   #     #     # #     # "\
"  ###    #####  #######  #####       #   #####   #####    #      #####   #####  ";

const std::string letters = 
"   #    ######   #####  ######  ####### #######  #####  #     #   ###         # #    #  #       #     # #     # ####### ######   #####  ######   #####  ####### #     # #     # #     # #     # #     # ####### "\
"  # #   #     # #     # #     # #       #       #     # #     #    #          # #   #   #       ##   ## ##    # #     # #     # #     # #     # #     #    #    #     # #     # #  #  #  #   #   #   #       #  "\
" #   #  #     # #       #     # #       #       #       #     #    #          # #  #    #       # # # # # #   # #     # #     # #     # #     # #          #    #     # #     # #  #  #   # #     # #       #   "\
"#     # ######  #       #     # #####   #####   #  #### #######    #          # ###     #       #  #  # #  #  # #     # ######  #     # ######   #####     #    #     # #     # #  #  #    #       #       #    "\
"####### #     # #       #     # #       #       #     # #     #    #    #     # #  #    #       #     # #   # # #     # #       #   # # #   #         #    #    #     #  #   #  #  #  #   # #      #      #     "\
"#     # #     # #     # #     # #       #       #     # #     #    #    #     # #   #   #       #     # #    ## #     # #       #    #  #    #  #     #    #    #     #   # #   #  #  #  #   #     #     #      "\
"#     # ######   #####  ######  ####### #        #####  #     #   ###    #####  #    #  ####### #     # #     # ####### #        #### # #     #  #####     #     #####     #     ## ##  #     #    #    ####### ";

const int CW = 8; // Char Width
const int CH = 7; // Char Height

void pchar(SDL_Renderer* renderer, int x, int y, char c){
    const std::string* font = &letters;
    int width = 26*CW; // for letters
    int idx = 0;

    if('0' <=c && c <= '9'){
        font = &numbers;
        width = 10*CW;
        idx = c-'0';
    } else if ('a' <= c && c <= 'z'){
        idx = c-'a';
    } else if ('A' <= c && c <= 'Z'){
        idx = c-'A';
    } else {
        return;
    }

    for(int dy=0; dy<CH; ++dy){
        int offset = dy*width+idx*CW; // TODO: optimize idx, ++y
        for(int dx=0; dx<CW; ++dx){
            if( ' ' != (*font)[offset+dx] ){
                SDL_RenderDrawPoint(renderer, x+dx, y+dy);
            }
        }
    }
}

void pstr(SDL_Renderer* renderer, int x, int y, const std::string& str){
    for(int i = 0; i < str.length(); ++i){
        pchar(renderer, x+i*CW, y, str[i]);
    }
}
