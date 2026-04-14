#include <iostream>
#include <vector>
#include <string>
#include "core_numeric.h"

using namespace std;

// =========================
// CLASE PROPIA
// =========================
class Punto {
public:
    double x, y;

    Punto(double x=0, double y=0):x(x),y(y){}

    Punto operator+(const Punto& o) const { return {x+o.x, y+o.y}; }
    Punto operator-(const Punto& o) const { return {x-o.x, y-o.y}; }
    Punto operator*(const Punto& o) const { return {x*o.x, y*o.y}; }
    Punto operator/(size_t n) const { return {x/n, y/n}; }

    bool operator<(const Punto& o) const {
        return x*x + y*y < o.x*o.x + o.y*o.y;
    }
};

int main() {

    cout << "=== VALIDOS ===\n";

    vector<double> v{7,8,12};
    cout << core_numeric::sum(v) << endl;
    cout << core_numeric::mean(v) << endl;
    cout << core_numeric::variance(v) << endl;
    cout << core_numeric::max(v) << endl;

    cout << core_numeric::sum_variadic(1,2,3,4) << endl;
    cout << core_numeric::mean_variadic(1,2,3,4) << endl;
    cout << core_numeric::variance_variadic(1,2,3,4) << endl;
    cout << core_numeric::max_variadic(1,2,3,4) << endl;

    // =========================
    // CLASE PROPIA
    // =========================
    vector<Punto> vp{{1,2},{2,1},{3,0}};
    core_numeric::sum(vp);
    core_numeric::mean(vp);
    core_numeric::variance(vp);
    core_numeric::max(vp);

    cout << "Clase propia OK\n";

    // =========================
    // NO COMPILAN
    // =========================

    // string NO cumple NumericLike
    // vector<string> vs{"a","b"};
    // core_numeric::variance(vs);

    // string no tiene *, /
    // core_numeric::max(vs);

    // int no es Iterable
    // int x = 5;
    // core_numeric::sum(x);

    // const char* no cumple Addable correctamente
    // core_numeric::sum_variadic("Hola","Mundo");

    cout << "Test completo\n";
}
