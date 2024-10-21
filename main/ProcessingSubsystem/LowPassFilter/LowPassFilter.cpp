#include "LowPassFilter.hpp"

LowPassFilter::LowPassFilter()
    : x{0}, y{0}, index{0} {}


float LowPassFilter::filt(float xn) {
    // Atualizar o buffer circular
    index = (index + 1) % 11;
    x[index] = xn;
    y[index] = 0;

    // Calcular o valor filtrado
    for (int i = 0; i < 10; i++) {
        int idx = (index - i + 11) % 11;
        y[index] += b[i] * x[idx] - a[i] * y[(index - i - 1 + 11) % 11];
    }
    y[index] += b[10] * x[(index - 10 + 11) % 11];

    return y[index];
}

void LowPassFilter::reset() {
    for (int i = 0; i < 11; i++) {
        x[i] = 0;
        y[i] = 0;
    }
    index = 0;
}