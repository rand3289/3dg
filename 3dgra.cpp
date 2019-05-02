#include <vector>
#include <iostream>
#using namespace std;

namespace x {
/*
struct Angle{
    double angle = 0.0;
    double& operator() { return angle; }
};

enum XYZ { X=0, Y=1, Z=2 };
*/

struct Polar {
    double x,y,z,d; // 3 angles + distance
    Polar(): x(0.0), y(0.0), z(0.0), d(0.0) {}
    Vector& rotateX(double a){ x+=a; return *this; }
    Vector& rotateY(double a){ y+=a; return *this; }
    Vector& rotateZ(double a){ z+=a; return *this; }
    Vector operator+(const Vector& rhs) const { return Vector(x+rhs.x, y+rhs.y, z+rhs.z, d+rhs.d); }
}
  
struct Point {
    double x,y,z;
    Point(): x(0.0), y(0.0), z(0.0) {}
    Point(T X, T Y, T Z): x(X), y(Y), z(Z) {}
};

struct Node {
    Point pt;
    int id;
    string label;
};

struct Edge{
    int fro, to, id;
};

  
} // namespace x
using namespace x;


int main(int argc, char* argv[]){
    vector<Node> points;
    vector<Edge> edges;

//    Point origin(); // 0 0 0
    const double DX = 10.0;         // defines how delta changes
    Polar delta (0.0, 0.0, 0.0, 0.0); // defines rotation of the screen
    Polar screen(0.0, 0.0, 0.0, 1000.0); // screen plane is orthogonal to this vector and is located screen.d distance from origin
    double screenX = 1000.0;
    double screenY = 1000.0;
    double eyeDist = 1000.0;    // distance from the screen to your eye

    for(;;){
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
