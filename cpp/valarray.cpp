#include <valarray>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <complex>
using namespace std;
int main()
{
    auto conjop = [](complex<float> v) { return std::conj(v); };
    std::valarray<complex<float>> a{ {{0,1}, {2,-0.4}, {0,3}, {4,1}, {5,0}}};
    std::valarray<complex<float>> b = a.apply(conjop);
    std::valarray<complex<float>> c = a * b;
    for (complex<float> C:c) { cout << C.real() << endl;};
    std::copy(begin(c), end(c),
        std::ostream_iterator<complex<float>>(std::cout, " "));
}
