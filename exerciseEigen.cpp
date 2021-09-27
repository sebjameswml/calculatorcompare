#include <iostream>
#include <Eigen/Dense>
#include <morph/Random.h>
#include <chrono>

using namespace std::chrono;
using std::chrono::steady_clock;

int main()
{
    morph::RandUniform<float> rng;
    Eigen::VectorXf v(1000000);
    for (auto& vv : v) { vv = rng.get(); }

    steady_clock::time_point start = steady_clock::now();

    Eigen::VectorXf v2(1000000);
    for (float i = 0.0f; i < 500.0f; i += 1.0f) {
        v2 = v * i;
    }

    steady_clock::duration sincestart = steady_clock::now() - start;

    std::cout << "Operation took " << duration_cast<milliseconds>(sincestart).count() << " ms" << std::endl;

    return 0;
}
