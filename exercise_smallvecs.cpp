/*
 * Like exercise.cpp but on small vVectors which might make vVector's OpenMP slow due to
 * start up costs.
 */

#include <iostream>
#include <Eigen/Dense>
#include <morph/Random.h>
#include <morph/vVector.h>
#include <chrono>

using namespace std::chrono;
using std::chrono::steady_clock;

// How much numerical precision in the test numbers?
typedef float F;
typedef Eigen::Array<F, Eigen::Dynamic, 1> EigenVec;

// How long are the vectors?
const size_t veclen = 4;

int main()
{
    morph::RandUniform<float> rng;
    steady_clock::time_point start = steady_clock::now();
    steady_clock::duration sincestart;

    // Multiplication. Eigen
    EigenVec ev(veclen);
    for (auto& vv : ev) { vv = rng.get(); }
    EigenVec ev2(veclen);

    start = steady_clock::now();
    for (F i = F{0}; i < F{500}; i += F{1}) {
        ev2 = ev * i;
    }
    sincestart = steady_clock::now() - start;

    std::cout << "Scalar mult took " << duration_cast<nanoseconds>(sincestart).count() << " ns with Eigen" << std::endl;

    // Multiplication. vVector
    morph::vVector<F> v(veclen);
    v.randomize();
    morph::vVector<F> v2(veclen);

    start = steady_clock::now();
    for (F i = F{0}; i < F{500}; i += F{1}) {
        //v *= i; // super fast. 500 million mults in 8 ns = 62.5 gflops (with omp parallel)
        //v2 = v * i; // Same speed as Eigen (but with omp parallel). 65 ns.
        v.mult (i, v2); // With OpenMP parallel on an i9, this whips Eigen (20ns to 66 ns).
    }
    sincestart = steady_clock::now() - start;

    std::cout << "Scalar mult took " << duration_cast<nanoseconds>(sincestart).count() << " ns with vVector" << std::endl;

    // Vec Multiplication. Eigen
    EigenVec ev3(veclen);
    for (auto& vv : ev3) { vv = rng.get(); }

    start = steady_clock::now();
    for (F i = F{0}; i < F{500}; i += F{1}) {
        ev2 = ev * ev3;
    }
    sincestart = steady_clock::now() - start;

    std::cout << "Vector mult took " << duration_cast<nanoseconds>(sincestart).count() << " ns with Eigen" << std::endl;

    // Vec Multiplication. vVector
    morph::vVector<F> v3(veclen);
    v3.randomize();

    start = steady_clock::now();
    for (F i = F{0}; i < F{500}; i += F{1}) {
        v2 = v * v3;
    }
    sincestart = steady_clock::now() - start;

    std::cout << "Vector mult took " << duration_cast<nanoseconds>(sincestart).count() << " ns with vVector" << std::endl;

    // Vec Division. Eigen
    start = steady_clock::now();
    for (F i = F{0}; i < F{500}; i += F{1}) {
        ev2 = ev / ev3;
    }
    sincestart = steady_clock::now() - start;

    std::cout << "Vector div took " << duration_cast<nanoseconds>(sincestart).count() << " ns with Eigen" << std::endl;

    // Vec Division. vVector
    start = steady_clock::now();
    for (F i = F{0}; i < F{500}; i += F{1}) {
        v2 = v / v3;
    }
    sincestart = steady_clock::now() - start;

    std::cout << "Vector div took " << duration_cast<nanoseconds>(sincestart).count() << " ns with vVector" << std::endl;

    // Pow
    start = steady_clock::now();
    for (F i = F{0}; i < F{500}; i += F{1}) {
        ev = ev2.pow(F{1}/i); // 2640 ns
    }
    sincestart = steady_clock::now() - start;
    std::cout << "Raise to scalar power took " << duration_cast<nanoseconds>(sincestart).count() << " ns with Eigen" << std::endl;

    start = steady_clock::now();
    for (F i = F{0}; i < F{500}; i += F{1}) {
        //v.pow_inplace (F{1}/i); // 949 ns no OMP, 120 ns with.
        v2 = v.pow (F{1}/i); // 2617 ns. 320 ns with.
    }
    sincestart = steady_clock::now() - start;
    std::cout << "Raise to scalar power took " << duration_cast<nanoseconds>(sincestart).count() << " ns with vVector" << std::endl;

    return 0;
}
