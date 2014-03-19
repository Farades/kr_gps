#ifndef WHITE_NOISE_H
#define WHITE_NOISE_H

#include "RandG.h"
using namespace std;

class White_noise
{
    private:
    float math_expectation;
    float sigma;

    public:
    White_noise()
    {}
    White_noise(float m, float s): math_expectation(m), sigma(s)
    {}
    double get();
    double get(float m, float s);
};

double White_noise::get()
{
    return randn_trig(math_expectation, sigma);
}

double White_noise::get(float m, float s)
{
    return randn_trig(m, s);
}


#endif // WHITE_NOISE_H
