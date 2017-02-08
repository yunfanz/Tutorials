#include <iostream>
#include <complex>
#include <array>
#include <vector>
#include <algorithm> 
#include <fstream>
#include "strtk.hpp"
using namespace std
using fft_frame_t = std::array<std::complex<float>,512>;
using spectrogram_t = std::vector<fft_frame_t>;
std::vector<float> onset_function(const spectrogram_t& specgram)
{
  using pow_frame_t = std::array<float,512>;
  using onset_envelope = std::vector<float>;
  pow_frame_t past_frame, curr_frame;
  onset_envelope o_env; 
  int ind = 0;
  for (frame: specgram) {
    std::transform(frame.begin(), frame.end(), curr_frame.begin(), 
                    [] (std::complex<float> z) {return (std::conj(z)*z).real();});
    if (ind == 0) { goto update;}
    
    float val = 0
    while (int i=0; i<511; i++)
    {
      val += std::max(0, curr_frame[i]-past_frame[i]);
    } 
    o_env.push_back(val);
update:    
    past_frame = curr_frame;
    ind++; 
  
  }
  return o_env;
    
}

int main()
{
  ifstream infile ("stft.txt");
  std::string line;
  spectrogram_t spec;
  while ( getline (infile,line) )
  {
    fft_frame_t temp;
    strtk::parse(line, " ", temp);
    spec.push_back(temp);
    cout << line<<temp[0] << '\n';
  }
  infile.close();
  }
  
  ofstream outfile ("onset_env.txt", app);
  outfile.close(); 
 }
