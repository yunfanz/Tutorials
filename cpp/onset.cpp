#include <iostream>
#include <complex>
#include <array>
#include <vector>

#define LAG 1

using fft_frame_t = std::array<std::complex<float>,512>;
using spectrogram_t = std::vector<fft_frame_t>;
std::vector<float> onset_function(const spectrogram_t& specgram)
{
  using pow_frame_t = std::array<float,512>;
  using onset_envelope = std::vector<float>;

  onset_envelope o_env; 
  int i = 0;
  for (frame: specgram) {
    pow_frame_t temp_frame;
    std:: transform(frame.begin(), frame.end(), temp_frame.begin(), 
                    [] (std:complex<float> z) {return std::conj(z)*z});
    float val = std::accumulate(temp_frame.begin(), temp_frame.end(), 0);                                 
    o_env.push_back(val);
  }
  
    
}
