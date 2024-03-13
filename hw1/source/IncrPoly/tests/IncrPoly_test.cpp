#include <gtest/gtest.h>
#include "../IncrPoly.hpp"

TEST(IncrPolyTest, TestPolyItem) {
    IncrPoly poly(-1, -1, -1);
    float a = 1.2;
    int x = 8;
    EXPECT_FLOAT_EQ(poly.polyItem(a, x, 2), a * x * x);
}

TEST(IncrPolyTest, TestGetValue) {
    float a = 1.2, b = 3.4, c = 5.6;
    IncrPoly poly(a, b, c);
    int x = 8;
    EXPECT_FLOAT_EQ(poly.getValue(x), a * x * x + b * x + c);
}

TEST(IncrPolyTest, TestGetRange1) {
    float a = 1.2, b = 3.4, c = 5.6;
    IncrPoly poly(a, b, c);
    int x_b = -500, x_e = 500;
    int* y = new int[x_e - x_b + 1];
    poly.getRangeValue(x_b, x_e, y);
    int* yptr = y;
    float y_real;
    for (int x = x_b; x <= x_e; ++x) {
        y_real = a * x * x + b * x + c;
        EXPECT_NEAR(y_real, *yptr++, 1);
    }
}

TEST(IncrPolyTest, TestGetRange2) {
    float a = 1.2, b = 3.4, c = 5.6;
    IncrPoly poly(a, b, c);
    int x_b = -700, x_e = 700;
    int* y = new int[x_e - x_b + 1];
    poly.getRangeValue(x_b, x_e, y);
    int* yptr = y;
    float y_real;
    for (int x = x_b; x <= x_e; ++x) {
        y_real = a * x * x + b * x + c;
        EXPECT_NEAR(y_real, *yptr++, 2);
    }
}