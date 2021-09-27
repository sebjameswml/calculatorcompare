#include <Eigen/Dense>
int main()
{
    Eigen::VectorXf v(4);
    v[0] = 1.0f;
    v[1] = 2.0f;
    v[2] = 3.0f;
    v[3] = 4.0f;
    Eigen::VectorXf v2(4);
    v2 = v * 2;
    return v2[0] > 1.0f ? 1 : 0;
}
