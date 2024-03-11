#ifndef INCRPOLY_INCRPOLY_HPP
#define INCRPOLY_INCRPOLY_HPP

class IncrPoly {
    float a;
    float b;
    float c;

    static int floor(float y) {
        return y >= 0.0f ? (int) y : (int) y - 1;
    }

    static int round(float y) {
        return floor(y + 0.5f);
    }

public:
    IncrPoly(float a, float b, float c) : a(a), b(b), c(c) {}

    float polyItem(float coeff, int base, int order) {
        float value = 0;
        if (order == 0) return coeff;
        if (order == 1) {
            for (int i = 0; i < base; ++i) {
                value += coeff;
            }
            return value;
        }
        return polyItem(polyItem(coeff, base, order - 1), base, 1);
    }

    float getValue(int x) {
        float value = c;
        value += polyItem(b, x, 1);
        value += polyItem(a, x, 2);
        return value;
    }

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
        // a*(x+1)**2 + b*(x+1) + c - a*x**2 - b*x - c = 2*a*x+a+b
        float incr = polyItem(two_a, x, 1) + a + b;
        for (; x <= x_e; ++x) {
            *y_output++ = round(y);
            y += incr;
            // 2*a*(x+1)+a+b-2*a*x-a-b=2*a
            incr += two_a;
        }
    }

    virtual ~IncrPoly() = default;
};


#endif //INCRPOLY_INCRPOLY_HPP
