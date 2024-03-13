#ifndef INCRPOLY_OPENGL_INCRPOLY_CLI_H
#define INCRPOLY_OPENGL_INCRPOLY_CLI_H

#include <iostream>

void prepare_input(float &a, float &b, float &c, int &x_b, int &x_e) {
    std::cout << "Incremental algorithm for calculating a*x^2+b*x+c "
                 "within range [x_b,x_e]." << std::endl;
    std::cout << "Please input the coefficients (could be decimals); "
                 "split by whitespace and hit Enter [a b c]: ";
    std::cin >> a >> b >> c;

    float _x_b, _x_e;
    std::cout << "Please input the range boundaries (make sure they are integers); "
                 "split by whitespace and hit Enter [x_b x_e]: ";
    std::cin >> _x_b >> _x_e;
    if (_x_b > _x_e) {
        float _x_tmp = _x_b;
        _x_b = _x_e;
        _x_e = _x_tmp;
    }
    x_b = (int) _x_b;
    x_e = (int) _x_e;
}

void output_result(int *y, int length) {
    std::cout << "The polynomial result are (rounded, "
                 "from the left boundary "
                 "to the right boundary): ";
    for (int i = 0; i < length; ++i) {
        std::cout << *y++ << ' ';
    }
    std::cout << std::endl;
}

#endif //INCRPOLY_OPENGL_INCRPOLY_CLI_H
