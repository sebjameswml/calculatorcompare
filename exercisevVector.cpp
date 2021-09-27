#include <iostream>
#include <morph/vVector.h>
#include <chrono>

using namespace std::chrono;
using std::chrono::steady_clock;

int main()
{
    morph::vVector<float> v(1000000);
    v.randomize();

    steady_clock::time_point start = steady_clock::now();

    morph::vVector<float> v2(1000000);
    for (float i = 0.0f; i < 500.0f; i += 1.0f) {
        v2 = v * i;
    }

    steady_clock::duration sincestart = steady_clock::now() - start;

    std::cout << "Operation took " << duration_cast<milliseconds>(sincestart).count() << " ms" << std::endl;

    return 0;
}
