#include "Utilities/General/interface/precomputed_value_sort.h"

#include <vector>
#include <iterator>
#include <iostream>

#include "DataFormats/GeometryVector/interface/Phi.h"

using namespace std;

// A fake class
class Point;

typedef Geom::Phi<double> Phi;

class Point {
 public: 
  Point(float x=0, float y=0);
  Point(const Point& p);
  float r() const {return sqrt(X*X + Y*Y);}
  Phi phi() const {return Phi(atan2(Y,X));}
  float X, Y;
};


ostream & operator<<(ostream & o, const Point& p) {
  return o << "[p=(" << p.X << "," << p.Y << "); r=" << p.r()
	   << " phi=" << p.phi() << "]";
}

ostream & operator<<(ostream & o, const Point* p) {
  return o << *p;
}

Point::Point(float x, float y) : X(x), Y(y) {
  cout << "New Point" << *this <<endl;
}

Point::Point(const Point& p): X(p.X), Y(p.Y){
  cout << "New Point (copy)" << *this <<endl;  
}



// A trivial operation on Point
struct ExtractR1 : public std::unary_function<const Point&, float> {
  float operator()(const Point& p) const {return p.r();}
};

// Same, but on Point*
struct ExtractR2 : public std::unary_function<const Point*, float> {
  float operator()(const Point* p) const {return p->r();}
};

// Extract phi on Point*
struct ExtractPhi2 : public std::unary_function<const Point*, Phi> {
  Phi operator()(const Point* p) const {return p->phi();}
};


// A trivial (useless!) binary predicate
struct LessR {
  bool operator()(const double& a, const double& b) const {
    return a<b;
  }
};
 
// A less trivial example: sorts angles 
// within any range SMALLER THAN PI "counter-clockwise" 
// even if the angles cross the pi boundary.
// The result is undefined if the input values cover a range larger than pi!!!
struct LessDPhi {
  bool operator()(const Phi& a, const Phi& b) const {
    return b - a > 0.; // note: Phi handles periodicity...
  }
};


int main() {

  // Create a vector with some random Points
  vector<Point> v1;
  v1.reserve(6);
  v1.push_back(Point(-1.343, 2.445));
  v1.push_back(Point(-1.566, 1.678));
  v1.push_back(Point(-1.678, 1.569));
  v1.push_back(Point(-3.138, 5.321));
  v1.push_back(Point(-5.12, 0.321));
  v1.push_back(Point(-5.12, -0.321));

  // A vector of pointer to Points
  vector<Point*> v2;
  for (vector<Point>::iterator i = v1.begin(); i!= v1.end(); ++i) {
    v2.push_back(&(*i));
  }

  // Copy it
  vector<Point*> v3 = v2;

  cout << "Original vector:" << endl;
  copy(v1.begin(), v1.end(), ostream_iterator<Point>(cout, "\n"));
  cout << endl;

  // Sort v1
  precomputed_value_sort(v1.begin(), v1.end(), ExtractR1());
  cout << "Sorted with ExtractR1 : " << endl;
  copy(v1.begin(), v1.end(), ostream_iterator<Point>(cout, "\n"));
  cout << endl;

  // Sort v2
  cout << "Sorted with ExtractR2: " << endl;  
  precomputed_value_sort(v2.begin(), v2.end(), ExtractR2());
  copy(v2.begin(), v2.end(), ostream_iterator<Point*>(cout, "\n"));
  cout << endl;

  // Sort v3 using a BinaryPredicate
  cout << "Sort with LessR: " << endl;  
  precomputed_value_sort(v3.begin(), v3.end(), ExtractR2(), LessR());
  copy(v3.begin(), v3.end(), ostream_iterator<Point* >(cout, "\n"));
  cout << endl;
  
  // Sort v3 using phi
  cout << "Sort with ExtractPhi2: " << endl;  
  precomputed_value_sort(v3.begin(), v3.end(), ExtractPhi2());
  copy(v3.begin(), v3.end(), ostream_iterator<Point* >(cout, "\n"));
  cout << endl;


  // Sort v3 using a BinaryPredicate
  cout << "Sort with LessDPhi: " << endl;  
  precomputed_value_sort(v3.begin(), v3.end(), ExtractPhi2(), LessDPhi());
  copy(v3.begin(), v3.end(), ostream_iterator<Point* >(cout, "\n"));
  cout << endl;

  return 0;
}
