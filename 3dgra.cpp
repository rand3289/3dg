#include <algorithm>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h> // Simple Directmedia Layer lib
using namespace std;

struct Polar {
    double x,y,z,d; // 3 angles + distance
    Polar(): x(0.0), y(0.0), z(0.0), d(0.0) {}
    Polar(double X, double Y, double Z, double D): x(X), y(Y), z(Z), d(D) {}
    Polar operator+(const Polar& rhs) const { return Polar(x+rhs.x, y+rhs.y, z+rhs.z, d+rhs.d); }
//    Polar& rotateX(double a){ x+=a; return *this; }
//    Polar& rotateY(double a){ y+=a; return *this; }
//    Polar& rotateZ(double a){ z+=a; return *this; }
};
  
struct Point {
    double x,y,z;
    Point(): x(0.0), y(0.0), z(0.0) {}
    Point(double X, double Y, double Z): x(X), y(Y), z(Z) {}
};

struct RBPoint{
    int redx, bluex, y;
    RBPoint(): redx(0), bluex(0), y(0) {}
    RBPoint(int redX, int blueX, int Y): redx(redX), bluex(blueX), y(Y) {}
};

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

// project Point to screen coordinates RBPoint
void project(const Polar& screen, const Point& point, RBPoint& pt){
    const double eyeDist = 1000.0; // distance from the screen to your eye
    const double eye2eye = 100.0;  // distance between eyes
    const double screenX = 1000.0;
    const double screenY = 1000.0;

    int screenZ = (point.z+eyeDist)*(eye2eye/eyeDist)*2;
//    int screenZ =  pt.z > 0 ? eye2eye*pt.z/(eyeDist+pt.z) : eye2eye*pt.z/eyeDist; // before or behind screen?
    screenZ = min(screenZ, 70);   // infinity
    screenZ = max(screenZ, -70); // too close

    pt.redx  = point.x + screenZ;
    pt.bluex = point.x - screenZ;
    pt.y = point.y;
}

struct SDLRect {
    SDL_Rect rect;
    SDL_Rect* operator()(){ return &rect; }
    SDLRect(){ rect.x=0; rect.y=0; rect.w=0; rect.h=0; }
    SDLRect(int x, int y, int w, int h){ rect.x=x; rect.y=y; rect.w=w; rect.h=h; }
};

void drawPoint(SDL_Renderer* rend, const RBPoint& pt){
    SDLRect rect(pt.bluex-3, pt.y-3, 7, 7);
    SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE); // blue
    SDL_RenderFillRect(rend, rect()); 

    rect.rect.x = pt.redx-3;
    SDL_SetRenderDrawColor(rend, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE); // red
    SDL_RenderFillRect(rend, rect());
}

void drawEdge(SDL_Renderer* rend, const RBPoint& from, const RBPoint& to){ // line
    SDL_SetRenderDrawColor(rend, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE); // red
    SDL_RenderDrawLine(rend, from.redx, from.y , to.redx, to.y);
    SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE); // blue
    SDL_RenderDrawLine(rend, from.bluex, from.y , to.bluex, to.y);
}

double r(){ return (double) (rand() % 2000 - 1000); }

void exitSDLerr(){
    cerr << "SDL error: " << SDL_GetError() << endl;
    exit(1);
}

int main(int argc, char* argv[]){
    vector<Node> points;
    vector<Edge> edges;
    vector<RBPoint> xy;
    Polar screen(0.0, 0.0, 0.0, 1000.0); // screen plane is orthogonal to this vector and is located screen.d distance from origin
    Polar delta (0.0, 0.0, 0.0, 0.0);    // defines rotation of the screen
    const double DX = 0.01;              // defines how delta changes
    const double ZOOM = 100.0;           // defines how screen.d changes

    const int POINT_COUNT = 100;
    const int EDGE_COUNT = 300;
    xy.resize(POINT_COUNT); // match points[]

    for(int i=0; i < POINT_COUNT; ++i){
        points.emplace_back( r(),r(),r() );
    }
    for(int i=0; i < EDGE_COUNT; ++i){
        edges.emplace_back( rand()%POINT_COUNT, rand()%POINT_COUNT );
    }

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const int winpos = SDL_WINDOWPOS_CENTERED;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) { exitSDLerr(); }

    SDL_Window* window=SDL_CreateWindow("3dgra", winpos, winpos, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(0==window){ exitSDLerr(); }
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    if(0==renderer){ exitSDLerr(); }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP); // SDL_WINDOW_FULLSCREEN for different resolution

    SDL_Event e;
    bool run = true;
    while(run){
        while( SDL_PollEvent( &e ) ){
	    if(e.type == SDL_QUIT){ run=false; }
	    else if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
		    case SDLK_ESCAPE:
	            case SDLK_q:  run = false;        break;
	            case SDLK_PLUS:  screen.d +=ZOOM; break;
                    case SDLK_MINUS: screen.d -=ZOOM; break;
	            case SDLK_LEFT:  delta.x  -=DX;   break;
	            case SDLK_RIGHT: delta.x  +=DX;   break;
	            case SDLK_UP:    delta.y  -=DX;   break;
	            case SDLK_DOWN:  delta.y  +=DX;   break;
	        }
            }
	}

	screen = screen + delta;
        for(int i=0; i< xy.size(); ++i){
	    project(screen, points[i].pt, xy[i]);
	}
	// sort xy on depth    // TODO: do we need this???
	std::sort(xy.begin(), xy.end(), [](const RBPoint& lhs, const RBPoint& rhs){ return lhs.redx-lhs.bluex < rhs.redx-rhs.bluex; } );

        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_ADD);

	for(int i=0; i< edges.size(); ++i){
	    drawEdge(renderer, xy[edges[i].from], xy[edges[i].to]);
	}
        for(int i=0; i< xy.size(); ++i){
            drawPoint(renderer, xy[i]);
	}

        SDL_RenderPresent(renderer);
        SDL_Delay( 16 ); // less than 60fps
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
