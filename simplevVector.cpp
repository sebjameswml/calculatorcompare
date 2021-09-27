#include <morph/vVector.h>
int main()
{
    morph::vVector<float> v = { 1.0f, 2.0f, 3.0f, 4.0f };
    morph::vVector<float> v2 = v * 2;
    return v2[0] > 1.0f ? 1 : 0;
}
