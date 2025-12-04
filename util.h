#include <iostream>
#include <random>

template <typename T>
int sign(const T& val) {
    if (val > 0) return 1;
    if (val < 0) return -1;
    return 0; // val == 0
}

// float randomFloat(float state) {
//     float theta = 2 * 3.1415926 * rand();
// }