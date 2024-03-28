float funcx(float t) { return t; }

float tarr[5] = {0, 1, 3, 4, 5};
float Bi1(int i, float t) {
    if (t >= tarr[i] && t <= tarr[i + 1]) return 1;
    return 0;
}
float Bi2(int i, float t) {
    return (t - tarr[i]) / (tarr[i + 1] - tarr[i]) * Bi1(i, t) +
           (tarr[i + 2] - t) / (tarr[i + 2] - tarr[i + 1]) * Bi1(i + 1, t);
}
float Bi3(int i, float t) {
    return (t - tarr[i]) / (tarr[i + 2] - tarr[i]) * Bi2(i, t) +
           (tarr[i + 3] - t) / (tarr[i + 3] - tarr[i + 1]) * Bi2(i + 1, t);
}
float Bi4(int i, float t) {
    return (t - tarr[i]) / (tarr[i + 3] - tarr[i]) * Bi3(i, t) +
           (tarr[i + 4] - t) / (tarr[i + 4] - tarr[i + 1]) * Bi3(i + 1, t);
}

float B01(float t) { return Bi1(0, t); }
float B11(float t) { return Bi1(1, t); }
float B21(float t) { return Bi1(2, t); }
float B31(float t) { return Bi1(3, t); }
float B02(float t) { return Bi2(0, t); }
float B12(float t) { return Bi2(1, t); }
float B22(float t) { return Bi2(2, t); }
float B03(float t) { return Bi3(0, t); }
float B13(float t) { return Bi3(1, t); }
float B04(float t) { return Bi4(0, t); }