/*
 * Construct a 512-bit vector from 8 64-bit doubles. Add it to itself and print the
 * result.
 */

#include <stdio.h>
#include <immintrin.h>

int main()
{
    //__m512i hello;
    // Construction from scalars or literals. Note these are set in reverse order
    __m512d a = _mm512_set_pd(4.0, 3.0, 2.0, 1.0, -1.1, -2.2, -3.3, -4.4);

    // Does GCC generate the correct mov, or (better yet) elide the copy and pass two of
    // the same register into the add? Let's look at the assembly.
    __m512d b = a;

    // Add the two vectors, interpreting the bits as 4 double-precision floats.
    __m512d c = _mm512_add_pd(a, b);

    // Do we ever touch DRAM or will these four be registers?
    __attribute__ ((aligned (64))) double output[8];
    _mm512_store_pd(output, c);

    printf("%f %f %f %f %f %f %f %f\n", output[0], output[1], output[2], output[3],
           output[4], output[5], output[6], output[7]);
    return 0;
}
