#ifndef INCRPOLY_INCRPOLY_HPP
#define INCRPOLY_INCRPOLY_HPP

class IncrPoly {
    float a;
    float b;
    float c;

    static int floor(float y) { return y >= 0.0f ? (int) y : (int) y - 1; }

    static int round(float y) { return floor(y + 0.5f); }

public:
    IncrPoly(float a, float b, float c) : a(a), b(b), c(c) {}

    // Calculate $\text{coeff}*\text{base}^\text{order}$.
    float polyItem(float coeff, int base, int order) {
        float value = 0;
        if (order == 0) return coeff;
        if (order == 1) {
            if (base >= 0) {
                for (int i = 0; i < base; ++i) value += coeff;
            } else {
                for (int i = 0; i < -base; ++i) value -= coeff;
            }
            return value;
        }
        return polyItem(polyItem(coeff, base, order - 1), base, 1);
    }

    // Calculate $ax^2 + bx + c$ for the initial value.
    float getValue(int x) {
        float value = c;
        value += polyItem(b, x, 1);
        value += polyItem(a, x, 2);
        return value;
    }

    // Calculate $ax^2 + bx + c$ for the range $[x_b, x_e]$,
    // store the result into the array *y_output = int[$x_e-x_b+1$].
    void getRangeValue(int x_b, int x_e, int *y_output) {
        if (x_b > x_e) {
            // swap the value when the left is bigger than the right.
            int x_tmp = x_b;
            x_b = x_e;
            x_e = x_tmp;
        }
        int x = x_b;
        float y = getValue(x);
        const float two_a = a + a;
        // $a(x+1)^2 + b(x+1) + c - ax^2 - bx - c = 2ax+a+b$
        float incr = polyItem(two_a, x, 1) + a + b;
        for (; x <= x_e; ++x) {
            *y_output++ = round(y);
            y += incr;
            // $2a(x+1)+a+b-2ax-a-b=2a$
            incr += two_a;
        }
    }

    virtual ~IncrPoly() = default;
};


#endif //INCRPOLY_INCRPOLY_HPP
