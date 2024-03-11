#include <iostream>
#include "IncrPoly.hpp"

void output_array(int *y, int length) {
    for (int i = 0; i < length; ++i) {
        std::cout << *y++ << ' ';
    }
}

int main() {
    float a, b, c;
    std::cout << "Incremental algorithm for calculating a*x^2+b*x+c "
                 "within range [x_b,x_e]." << std::endl;
    std::cout << "Please input the coefficients (could be decimals); "
                 "split by whitespace and hit Enter [a b c]: ";
    std::cin >> a >> b >> c;
    IncrPoly poly(a, b, c);

    float _x_b, _x_e;
    std::cout << "Please input the range boundaries (make sure they are integers); "
                 "split by whitespace and hit Enter [x_b x_e]: ";
    std::cin >> _x_b >> _x_e;
    if (_x_b > _x_e) {
        float _x_tmp = _x_b;
        _x_b = _x_e;
        _x_e = _x_tmp;
    }
    int x_b = (int) _x_b, x_e = (int) _x_e;

    int length = x_e - x_b + 1;
    int *y = new int[length];
    poly.getRangeValue(x_b, x_e, y);

    std::cout << "The polynomial result are (rounded, "
                 "from the left boundary " << x_b << " " <<
                 "to the right boundary " << x_e << "): ";
    output_array(y, length);
    std::cout << std::endl;

    delete[] y;
    return 0;
}
