#include <iostream>
#include <Eigen/Dense>
#include <morph/Random.h>
#include <chrono>

using namespace std::chrono;
using std::chrono::steady_clock;

typedef Eigen::Array<float, Eigen::Dynamic, 1> EigenVec;

int main()
{
    morph::RandUniform<float> rng;
    EigenVec v(8192);
    for (auto& vv : v) { vv = rng.get(); }
    EigenVec v1(8192);
    float i = 0.0f;
    for (auto& vv : v1) { vv = i; i += 1.0f; }

    EigenVec v2(8192);

    steady_clock::time_point start = steady_clock::now();
    v2 = v * 2.0f;
    steady_clock::duration sincestart = steady_clock::now() - start;

    std::cout << "Operation took " << duration_cast<nanoseconds>(sincestart).count() << " ns" << std::endl;

    return 0;
}
