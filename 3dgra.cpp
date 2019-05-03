#include <algorithm>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h> // Simple Directmedia Layer lib
using namespace std;

struct Polar {
    double x,y,z,d; // 3 angles + distance
    Polar(): x(0.0), y(0.0), z(0.0), d(0.0) {}
    Polar(double X, double Y, double Z, double D): x(X), y(Y), z(Z), d(D) {}
    Polar& rotateX(double a){ x+=a; return *this; }
    Polar& rotateY(double a){ y+=a; return *this; }
    Polar& rotateZ(double a){ z+=a; return *this; }
    Polar operator+(const Polar& rhs) const { return Polar(x+rhs.x, y+rhs.y, z+rhs.z, d+rhs.d); }
};
  
struct Point {
    double x,y,z;
    Point(): x(0.0), y(0.0), z(0.0) {}
    Point(double X, double Y, double Z): x(X), y(Y), z(Z) {}
};

// create a point structure with redX, blueX, y
struct iPoint{
    int x,y,z;
    iPoint(): x(0), y(0), z(0) {}
    iPoint(int X, int Y, int Z): x(X), y(Y), z(Z) {}
};
bool lessZ(const iPoint& lhs, const iPoint& rhs){ return lhs.z > rhs.z; } // less Z axis
//bool lessZ(const iPoint& lhs, const iPoint& rhs){ return lhs.redx-lhs.bluex < rhs.redx-rhs.bluex; }

struct Node {
    Point pt;
//    int id;
//    string label;
    Node(double X, double Y, double Z): pt(X,Y,Z) {}
};

struct Edge{
    int from, to;
    Edge(): from(0), to(0) {}
    Edge(int fromID, int toID): from(fromID), to(toID) {}
};

const double eyeDist = 1000.0; // distance from the screen to your eye
const double eye2eye = 100.0;  // distance between eyes

// project Point to screen coordinates iPoint
void project(const Polar& screen, const Point& point, iPoint& pt){
    const double screenX = 1000.0;
    const double screenY = 1000.0;
    pt.x = point.x;
    pt.y = point.y;
    pt.z = point.z;
}


struct SDLRect {
    SDL_Rect rect;
    SDL_Rect* operator()(){ return &rect; }
    SDLRect(){ rect.x=0; rect.y=0; rect.w=0; rect.h=0; }
    SDLRect(int x, int y, int w, int h){ rect.x=x; rect.y=y; rect.w=w; rect.h=h; }
};

void drawPoint(SDL_Renderer* rend, const iPoint& pt){ // circle at pt.x, pt.y of size pt.z
    int screenZ = (pt.z+eyeDist)*(eye2eye/eyeDist)*2;
//    int screenZ =  pt.z > 0 ? eye2eye*pt.z/(eyeDist+pt.z) : eye2eye*pt.z/eyeDist; // before or behind screen?
    screenZ = min(screenZ, 70);   // infinity
    screenZ = max(screenZ, -70); // too close

    SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE); // blue
    SDLRect rect(pt.x-screenZ-3, pt.y-3, 7, 7);
    SDL_RenderFillRect(rend, rect()); 

    SDL_SetRenderDrawColor(rend, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE); // red
    rect.rect.x = pt.x+screenZ-3;
    SDL_RenderFillRect(rend, rect()); 
}

void drawEdge(SDL_Renderer* rend, const iPoint& from, const iPoint& to){ // line from .. to ..
//    SDL_SetRenderDrawColor(rend, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE);
//    SDL_RenderDrawLine(rend, from.x, from.y , to.x, to.y);
}

double r(){ return (double) (rand() % 2000) - 1000.0; }



int main(int argc, char* argv[]){
    vector<Node> points;
    vector<Edge> edges;
    vector<iPoint> xy;

    const int POINT_COUNT = 100;
    const int EDGE_COUNT = 300;
    const double DX = 10.0;         // defines how delta changes
    Polar delta (0.0, 0.0, 0.0, 0.0); // defines rotation of the screen
    Polar screen(0.0, 0.0, 0.0, 1000.0); // screen plane is orthogonal to this vector and is located screen.d distance from origin

    xy.resize(POINT_COUNT); // match points[]
    for(int i = 0; i < POINT_COUNT; ++i){
        points.push_back(Node(r(),r(),r()));
    }
    for(int i = 0; i < EDGE_COUNT; ++i){
        edges.push_back( Edge(rand()%100, rand()%100) );
    }

    /************************* INIT SDL ****************************/
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        cerr << "SDL could not initialized! SDL_Error: " << SDL_GetError() << endl;
	return 1;
    }
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

//    SDL_Window* window = NULL;
//    SDL_Renderer* renderer = NULL;
//    if( SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) != 0 ) {

    SDL_Window* window=SDL_CreateWindow("3dgra", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    if( 0==window || 0==renderer){
        cerr << "Window & renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
	return 2;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP); // SDL_WINDOW_FULLSCREEN for different resolution
    SDL_Event e;
    bool run = true;

    while(run){
        SDL_PollEvent( &e );
	if(e.type == SDL_QUIT){ break; }
	if(e.type == SDL_KEYDOWN){
            switch(e.key.keysym.sym){
	        case SDLK_q:  run = false;       break;
	        case SDLK_PLUS:  screen.d +=DX;  break;
                case SDLK_MINUS: screen.d -=DX;  break;
	        case SDLK_LEFT:  delta.x  -=DX;  break;
	        case SDLK_RIGHT: delta.x  +=DX;  break;
	        case SDLK_UP:    delta.y  -=DX;  break;
	        case SDLK_DOWN:  delta.y  +=DX;  break;
	    }
        }
	screen = screen + delta;

        for(int i=0; i< POINT_COUNT; ++i){
	    project(screen, points[i].pt, xy[i]);
	}
	std::sort(xy.begin(), xy.end(), lessZ);

        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_ADD);

	for(int i=0; i< EDGE_COUNT; ++i){
	    drawEdge(renderer, xy[edges[i].from], xy[edges[i].to]);
	}
        for(int i=0; i< POINT_COUNT; ++i){
            drawPoint(renderer, xy[i]);
	}

        SDL_RenderPresent(renderer);
        SDL_Delay( 50 );
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}
