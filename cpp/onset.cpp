#include <iostream>
#include <complex>
#include <array>
#include <vector>
#include <algorithm> 
#include <iterator>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
using fft_frame_t = std::array<std::complex<float>,512>;
using spectrogram_t = std::vector<fft_frame_t>;
using pow_frame_t = std::array<float,512>;
using onset_envelope = std::vector<float>;

// pow_frame_t freq_smoothing(pow_frame_t curr_frame, int window):
// {
//   pow_frame_t out;
//   for (int i=0; i<512-window)
// }
std::vector<float> onset_function(const spectrogram_t& specgram, int length)
{
  printf("%d ", length);
  pow_frame_t past_frame, curr_frame;
  onset_envelope o_env; 
  int ind = 0; float val; int i;
  for (int j=0; j<length-1; j++) {
    //printf("%d ", j);
    fft_frame_t frame = specgram[j];
    std::transform(frame.begin(), frame.end(), curr_frame.begin(), 
                    [] (std::complex<float> z) {return (std::conj(z)*z).real();});
    if (ind == 0) { goto update;}
    val = 0; i=0;
    while (i<=511)
    {
      val += std::max(float(0), curr_frame[i]-past_frame[i]);
      //printf("%f%+fi \n", frame[i].real(), frame[i].imag());
      i++;
    } 
    o_env.push_back(val);
update:    
    past_frame = curr_frame; //lag = 1 assumed
    ind++; 
  
  }
  return o_env;
    
}

int main()
{
    std::complex<float> c;
    std::ifstream fin("stft.txt");
    std::string line;
    spectrogram_t spec; 
    int nlines = 0;
    
    while(std::getline(fin,line))
    {
      fft_frame_t frame;
      std::stringstream stream(line);
      int i = 0;
      while(stream>>c)
      {
         frame[i] = c;
         i++;
      }
      spec.push_back(frame);
      i = 0;
      nlines++; 
    }
  std::vector<float> o_env = onset_function(spec, nlines);
  ofstream outfile ("onset_env.txt");
  std::ostream_iterator<float> output_iterator(outfile, "\n");
  std::copy(o_env.begin(), o_env.end(), output_iterator); 
  outfile.close(); 
 }
