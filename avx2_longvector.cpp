/*
 * Construct a 256-bit vector from 4 64-bit doubles. Add it to itself and print the
 * result.
 */

#include <stdio.h>
#include <immintrin.h>
#include <morph/aligned_allocator.h>
#include <vector>
#include <iostream>

int main()
{
    //std::cout << "sizeof(__m256)=" << sizeof(__m256) << std::endl; // 32 bytes
    std::vector<float, morph::aligned_allocator<float, sizeof(__m256)>> float_vec = {100, 200, 300, 400, 500, 600, 700, 800, 101, 201, 301, 401, 501, 601, 701, 801};

    __m256 fv = _mm256_load_ps (&float_vec[0]);
    __m256 fv2 = _mm256_load_ps (&float_vec[8]);
    __m256 two = _mm256_set1_ps (2.0f);

    __m256 c = _mm256_mul_ps(fv, two);
    __m256 c2 = _mm256_mul_ps(fv2, two);

    __attribute__ ((aligned (32))) float output[16];
    _mm256_store_ps(output, c);
    _mm256_store_ps(output+8, c2);

    for (int i = 0; i < 16; ++i) { printf("%f\n", output[i]); }
    return 0;
}
