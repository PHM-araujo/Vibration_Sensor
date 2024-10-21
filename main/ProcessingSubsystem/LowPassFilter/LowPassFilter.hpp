#ifndef LOWPASSFILTER_HPP
#define LOWPASSFILTER_HPP

class LowPassFilter {
public:
    LowPassFilter();
    float filt(float xn);
    void reset();
private:
    static constexpr float b[11] = {4.9945e-04, 4.9945e-03, 
                                    2.2475e-02, 5.9934e-02, 
                                    1.0489e-01, 1.2586e-01, 
                                    1.0489e-01, 5.9934e-02, 
                                    2.2475e-02, 4.9945e-03, 
                                    4.9945e-04};
    static constexpr float a[10] = { -1.9924e+00, 3.0195e+00, 
                                    -2.8185e+00, 2.0387e+00, 
                                     -1.0545e+00, 4.1445e-01, 
                                     -1.1572e-01, 2.2499e-02, 
                                     -2.6689e-03, 1.4876e-04 };

    float x[11];
    float y[11];
    int index;
};

#endif // LOWPASSFILTER_HPP