#include <iostream>
#include <Eigen/Dense>
#include <morph/Random.h>
#include <morph/vVector.h>
#include <chrono>

using namespace std::chrono;
using std::chrono::steady_clock;

// How much numerical precision in the test numbers?
typedef float F;
typedef Eigen::Matrix<F, Eigen::Dynamic, 1> EigenVec;

int main()
{
    morph::RandUniform<float> rng;
    steady_clock::time_point start = steady_clock::now();
    steady_clock::duration sincestart;

    // Multiplication. Eigen
    EigenVec ev(1000000);
    for (auto& vv : ev) { vv = rng.get(); }
    EigenVec ev2(1000000);

    start = steady_clock::now();
    for (F i = F{0}; i < F{500}; i += F{1}) { ev2 = ev * i; }
    sincestart = steady_clock::now() - start;

    std::cout << "Operation took " << duration_cast<milliseconds>(sincestart).count() << " ms with Eigen" << std::endl;

    // Multiplication. vVector
    morph::vVector<F> v(1000000);
    v.randomize();
    morph::vVector<F> v2(1000000);

    start = steady_clock::now();
    for (F i = F{0}; i < F{500}; i += F{1}) {
        //v *= i; // super fast. 500 million mults in 8 ms = 62.5 gflops (with omp parallel)
        //v2 = v * i; // slower than Eigen because memory is allocated for the return
                      // vector for each v * i operator* function. vVector needs some
                      // temporary storage!
        v.mult (i, v2); // With OpenMP parallel on an i9, this whips Eigen (20ms to 66 ms).
    }
    sincestart = steady_clock::now() - start;

    std::cout << "Operation took " << duration_cast<milliseconds>(sincestart).count() << " ms with vVector" << std::endl;

    // Exp
    start = steady_clock::now();
    for (F i = F{0}; i < F{500}; i += F{1}) { ev2 = ev * i; }
    sincestart = steady_clock::now() - start;

    std::cout << "Operation took " << duration_cast<milliseconds>(sincestart).count() << " ms with Eigen" << std::endl;

    return 0;
}
