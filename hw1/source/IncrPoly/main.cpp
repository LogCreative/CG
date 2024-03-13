#include "IncrPoly-cli.h"
#include "IncrPoly.hpp"

int main() {
    float a, b, c;
    int x_b, x_e;
    prepare_input(a, b, c, x_b, x_e);
    IncrPoly poly(a, b, c);
    int length = x_e - x_b + 1;
    int *y = new int[length];
    poly.getRangeValue(x_b, x_e, y);
    output_result(y, length);
    delete[] y;
    return 0;
}
