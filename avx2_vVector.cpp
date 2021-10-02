/*
 * Experimenting with operations using AVX2 and vectors of numbers.
 *
 * Here, the contents of a vVector are copied into an array of __m256 objects. The time
 * to actually do the multiplications is 1808 ns on the Alienware. That's 8*1024 FLOPS
 * in 1808 ns or 4.53097 operations per ns or 4.53e9 operations per second or 4.5 GFlops.
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
    morph::vVector<float> float_vec2(8*INTR_BLOCKS);
    float_vec.randomize();

    __m256 two = _mm256_set1_ps (2.237f);

    // Load first
    __m256 float_intrinsics[INTR_BLOCKS];
    __m256 float_intrinsics_result[INTR_BLOCKS];

    steady_clock::time_point start = steady_clock::now();
    for (size_t i = 0; i < INTR_BLOCKS; ++i) {
        float_intrinsics[i] = _mm256_load_ps (&float_vec[i*8]);
    }
    steady_clock::duration sincestart = steady_clock::now() - start;
    std::cout << "Copy to float_intrinsics array took " << duration_cast<nanoseconds>(sincestart).count() << " ns" << std::endl;

    start = steady_clock::now();
    for (size_t i = 0; i < INTR_BLOCKS; i++) {
        float_intrinsics_result[i] = _mm256_mul_ps(float_intrinsics[i], two);
    }
    sincestart = steady_clock::now() - start;
    std::cout << "Scalar mult took " << duration_cast<nanoseconds>(sincestart).count() << " ns with intrinsics" << std::endl;

    start = steady_clock::now();
    for (size_t i = 0; i < 8*INTR_BLOCKS; ++i) {
        float_vec2[i] = float_vec[i] * 2.0f;
    }
    sincestart = steady_clock::now() - start;
    std::cout << "Scalar mult took " << duration_cast<nanoseconds>(sincestart).count() << " ns as plain loop;" << std::endl;
#if 0
    __attribute__ ((aligned (sizeof(__m256)))) float output[8];
    _mm256_store_ps(output, float_intrinsics_result[0]);
    std::cout << "output:\n";
    for (auto o : output) { std::cout << o << std::endl; }
#endif
    return 0;
}
