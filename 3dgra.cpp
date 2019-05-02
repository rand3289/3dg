#include <algorithm>
#include <vector>
#include <iostream>
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


struct iPoint{
    int x,y,z;
    iPoint(): x(0), y(0), z(0) {}
    iPoint(int X, int Y, int Z): x(X), y(Y), z(Z) {}
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

void project(const Polar& screen, const Point& point, iPoint& pt){
    const double screenX = 1000.0;
    const double screenY = 1000.0;
    const double eyeDist = 1000.0;    // distance from the screen to your eye
} 

iPoint red(const iPoint& pt){}
iPoint blue(const iPoint& pt){}

void drawPoint(const iPoint& pt){ // circle at pt.x, pt.y of size pt.z
}

void drawEdge(const iPoint& from, const iPoint& to){ // line from .. to ..
}

double r(){ return (double) (rand() % 2000) - 1000.0; }

bool lessZ(const iPoint& lhs, const iPoint& rhs){ return lhs.z > rhs.z; } // less Z axis


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

    for(;;){
        for(int i=0; i< POINT_COUNT; ++i){
	    project(screen, points[i].pt, xy[i]); // in xy: x,y are screen coordinates of a point and z is the depth	    
	}
	std::sort(xy.begin(), xy.end(), lessZ);
        for(int i=0; i< POINT_COUNT; ++i){
            drawPoint(xy[i]);
	}
	for(int i=0; i< EDGE_COUNT; ++i){
	    drawEdge( xy[edges[i].from], xy[edges[i].to]);
	}

	int key = 0;
// DEBUGGING:
	const int LEFT = 1;
	const int RIGHT = 2;
	const int UP = 3;
	const int DOWN = 4;
        switch(key){
	    case '+':   screen.d+=DX; break;
            case '-':   screen.d-=DX; break;
	    case LEFT:  delta.x-=DX;  break;
	    case RIGHT: delta.x+=DX;  break;
	    case UP:    delta.y-=DX;  break;
	    case DOWN:  delta.y+=DX;  break;
	}
	screen = screen + delta;
    }
}
