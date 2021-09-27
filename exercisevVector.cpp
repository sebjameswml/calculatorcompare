#include <iostream>
#include <morph/vVector.h>
#include <chrono>

using namespace std::chrono;
using std::chrono::steady_clock;

// How much numerical precision in the test numbers?
typedef float F;

int main()
{
    morph::vVector<F> v(1000000);
    v.randomize();

    steady_clock::time_point start = steady_clock::now();

    morph::vVector<F> v2(1000000);
    for (F i = F{0}; i < F{500}; i += F{1}) {
        //v *= i; // super fast. 500 million mults in 8 ms = 62.5 gflops (with omp parallel)
        //v2 = v * i; // slower than Eigen because memory is allocated for the return
                      // vector for each v * i operator* function. vVector needs some
                      // temporary storage!
        v.mult (i, v2); // With OpenMP parallel on an i9, this whips Eigen (20ms to 66 ms).
    }

    steady_clock::duration sincestart = steady_clock::now() - start;

    std::cout << "Operation took " << duration_cast<milliseconds>(sincestart).count() << " ms" << std::endl;

    return 0;
}
