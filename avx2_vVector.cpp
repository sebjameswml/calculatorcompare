/*
 * Construct a 256-bit vector from 4 64-bit doubles. Add it to itself and print the
 * result.
 */

#include <stdio.h>
#include <immintrin.h>
#define USE_ALIGNED_ALLOCATOR 1
#include <morph/vVector.h>
#include <iostream>
#include <chrono>
using namespace std::chrono;
using std::chrono::steady_clock;

#define INTR_BLOCKS 1024

int main()
{
    //std::cout << "sizeof(__m256)=" << sizeof(__m256) << std::endl; // 32 bytes
    morph::vVector<float> float_vec(8*INTR_BLOCKS);
    float_vec.randomize();

    __m256 fv = _mm256_set1_ps (1.0f);
    __m256 two = _mm256_set1_ps (2.0f);
    __m256 c;

    // Load first
    __m256 float_intrinsics[INTR_BLOCKS];
    for (size_t i = 0; i < INTR_BLOCKS; ++i) {
        float_intrinsics[i] = _mm256_load_ps (&float_vec[i*8]);
    }

    steady_clock::time_point start = steady_clock::now();
    for (size_t i = 0; i < INTR_BLOCKS; i++) {
        c = _mm256_mul_ps(float_intrinsics[i], two);
    }
    steady_clock::duration sincestart = steady_clock::now() - start;
    std::cout << "Scalar mult took " << duration_cast<nanoseconds>(sincestart).count() << " ns with intrinsics" << std::endl;

    start = steady_clock::now();
    float cf = 0.0f;

    for (size_t i = 0; i < 8*INTR_BLOCKS; ++i) {
        cf = float_vec[i] * 2.0f;
    }
    sincestart = steady_clock::now() - start;
    std::cout << "Scalar mult took " << duration_cast<nanoseconds>(sincestart).count() << " ns as plain loop; last cf was" << cf << std::endl;

    __attribute__ ((aligned (sizeof(__m256)))) float output[8];
    _mm256_store_ps(output, c);
    std::cout << "output:\n";
    for (auto o : output) { std::cout << o << std::endl; }
    return 0;
}
