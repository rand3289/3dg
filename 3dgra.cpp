//#include <algorithm> // sort()
#include <vector>
#include <iostream>
#include <SDL2/SDL.h> // Simple Directmedia Layer lib
using namespace std;

struct Polar {
    double x,y,z,d; // 3 angles + distance
    Polar(): x(0.0), y(0.0), z(0.0), d(0.0) {}
    Polar(double X, double Y, double Z, double D): x(X), y(Y), z(Z), d(D) {}
    Polar operator+(const Polar& rhs) const { return Polar(x+rhs.x, y+rhs.y, z+rhs.z, d+rhs.d); }
    Polar& operator+=(const Polar&rhs){x+=rhs.x; y+=rhs.y; z+=rhs.z; d+=rhs.d; return *this; }
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
//    int id;
//    string label;
    Point pt;
    Node(double X, double Y, double Z): pt(X,Y,Z) {}
};

struct Edge{
    int from, to;
    Edge(): from(0), to(0) {}
    Edge(int fromID, int toID): from(fromID), to(toID) {}
};

// project Point to screen coordinates RBPoint
void project(const Polar& screen, const Point& point, RBPoint& pt, int width, int height){
    const double eyeDist = 1000.0; // distance from the screen to your eye
    const double eye2eye = 100.0;  // distance between eyes

// TODO: use screen.d for depth calculations && screen.x, screen.y for rotation calculations. Avoid screen.z (twist)
// rotate point by screen.x and screen.y then translate by screen.d, and project to two points on the screen
    int screenZ = (point.z+eyeDist)*(eye2eye/eyeDist)*2;
//    int screenZ =  pt.z > 0 ? eye2eye*pt.z/(eyeDist+pt.z) : eye2eye*pt.z/eyeDist; // before or behind screen?
    screenZ = min(screenZ, 70);   // infinity
    screenZ = max(screenZ, -70); // too close

    pt.redx  = point.x + screenZ;
    pt.bluex = point.x - screenZ;
    pt.y = point.y;
// TODO: make sure redx and bluex are on screen (>0 && <width) otherwise set y to -1
}

void drawPoint(SDL_Renderer* rend, const RBPoint& pt){
    if(pt.y < 0) { return; }
    SDL_Rect rect;  rect.x=pt.bluex-3;  rect.y= pt.y-3;  rect.w=7;  rect.h=7;
    SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE); // blue
    SDL_RenderFillRect(rend, &rect); 

    rect.x = pt.redx-3;
    SDL_SetRenderDrawColor(rend, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE); // red
    SDL_RenderFillRect(rend, &rect);
}

void drawEdge(SDL_Renderer* rend, const RBPoint& from, const RBPoint& to){ // line
    if( from.y < 0 && to.y < 0 ){ return; } // if both end points are off the screen, do not draw the edge
    SDL_SetRenderDrawColor(rend, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE); // red
    SDL_RenderDrawLine(rend, from.redx, from.y , to.redx, to.y);
    SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE); // blue
    SDL_RenderDrawLine(rend, from.bluex, from.y , to.bluex, to.y);
}

void loadGraph(vector<Node>& points, vector<Edge>& edges, int width, int height){ // screen width & height
    const int POINT_COUNT = 50;
    const int EDGE_COUNT = 3*POINT_COUNT;
    const int mx = max(width, height);

    for(int i=0; i < POINT_COUNT; ++i){
        points.emplace_back( rand()%width, rand()%height, rand()%(2*mx)-mx );
    }
    for(int i=0; i < EDGE_COUNT; ++i){
        edges.emplace_back( rand()%POINT_COUNT, rand()%POINT_COUNT );
    }
}

void exitSDLerr(){
    cerr << "SDL error: " << SDL_GetError() << endl;
    SDL_Quit();
    exit(1);
}

int main(int argc, char* argv[]){
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const int WINPOS = SDL_WINDOWPOS_CENTERED;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) { exitSDLerr(); }

    SDL_Window* window=SDL_CreateWindow("3dgra", WINPOS, WINPOS, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(0==window){ exitSDLerr(); }
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    if(0==renderer){ exitSDLerr(); }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP); // SDL_WINDOW_FULLSCREEN for different resolution

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);

    vector<RBPoint> xy;
    vector<Node> points;
    vector<Edge> edges;
    loadGraph(points, edges, dm.w, dm.h);
    xy.resize( points.size() );

    Polar screen(0.0, 0.0, 0.0, 1000.0); // screen plane is orthogonal to this vector and is located screen.d distance from origin
    Polar delta (0.0, 0.0, 0.0, 0.0);    // defines rotation of the screen (angular velocity)
    const double DX = 0.01;              // defines how delta changes (angluar acceleration)
    const double ZOOM = 100.0;           // defines how screen.d changes

    SDL_Event e;
    bool run = true;
    while(run){
        while( SDL_PollEvent( &e ) ){
	    if(e.type == SDL_QUIT){ run=false; }
	    else if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
		    case SDLK_ESCAPE:
	            case SDLK_q:     run=false;       break;
	            case SDLK_PLUS:  screen.d +=ZOOM; break;
                    case SDLK_MINUS: screen.d -=ZOOM; break;
	            case SDLK_LEFT:  delta.x  -=DX;   break;
	            case SDLK_RIGHT: delta.x  +=DX;   break;
	            case SDLK_UP:    delta.y  -=DX;   break;
	            case SDLK_DOWN:  delta.y  +=DX;   break;
	        }
            }
	}

	screen += delta;
        for(int i=0; i< xy.size(); ++i){
	    project(screen, points[i].pt, xy[i], dm.w, dm.h);
	}
	// sort xy on depth
//	std::sort(xy.begin(), xy.end(), [](const RBPoint& lhs, const RBPoint& rhs){ return lhs.redx-lhs.bluex < rhs.redx-rhs.bluex; } );

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
