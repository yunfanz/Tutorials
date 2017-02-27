#include <iostream>
#include <complex>
#include <array>
#include <vector>
#include <algorithm> 
#include <iterator>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

/*
 * This is a simple function that returns an onset_envelop given the STFT of an
 * audio series input. It's only meant to be of illustrative purposes and does
 * not boasts advanced features of an actual onset function. 
 * From input stft, a spectrogram S is formed by computing the power, and onset is computed
 * as onset(time_t) = average_over_f(max(0, S(t, f) -  S(t-1, f))). 
 * The function main performs test by taking in input from file stft.txt and outputs
 * to file  onset_env.txt. 
 */ 

using fft_frame_t = std::array<std::complex<float>,512>;
using spectrogram_t = std::vector<fft_frame_t>;
using pow_frame_t = std::array<float,512>;
using onset_envelope = std::vector<float>;
std::vector<float> onset_function(const spectrogram_t& specgram, int length)
{
  pow_frame_t past_frame, curr_frame;
  onset_envelope o_env; 
  int ind = 0; float val; int i;
  for (int j=0; j<length-1; j++) {
    //Convert STFT to power spectrogram
    fft_frame_t frame = specgram[j];
    std::transform(frame.begin(), frame.end(), curr_frame.begin(), 
                    [] (std::complex<float> z) {return (std::conj(z)*z).real();});
    //Compute Onset
    if (ind == 0) { goto update;}
    val = 0; i=0;
    while (i<=511)
    {
      val += std::max(float(0), curr_frame[i]-past_frame[i]);
      i++;
    } 
    o_env.push_back(val/512);
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
  std::ofstream outfile ("onset_env.txt");
  std::ostream_iterator<float> output_iterator(outfile, "\n");
  std::copy(o_env.begin(), o_env.end(), output_iterator); 
  outfile.close(); 
 }
