// complex::real example
#include <iostream>     // std::cout
#include <complex>      // std::complex
#include <array>
#include <algorithm>
#include <iterator>
int main ()
{
  std::complex<double> mycomplex (10.0,1.0);
  float myreal = mycomplex.real();
  std::cout << "Real part: " << myreal << '\n';
  std::array<std::complex<float>,2> frame {{{2.0,1},{3,4}}};
  std::array<float,2> temp_frame;
  std::transform(frame.begin(), frame.end(), temp_frame.begin(),
                    [] (std::complex<float> z) {return (std::conj(z)*z).real();});
  std::copy(begin(temp_frame), end(temp_frame),
        std::ostream_iterator<float>(std::cout, " "));


  return 0;
}
