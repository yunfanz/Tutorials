#include <iostream>
#include <complex>
#include <array>
#include <vector>
#include <algorithm> 
#include <iterator>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <exception>
using namespace std;
using fft_frame_t = std::array<std::complex<float>,512>;
using spectrogram_t = std::vector<fft_frame_t>;
using pow_frame_t = std::array<float,512>;
using onset_envelope = std::vector<float>;
std::vector<float> onset_function(const spectrogram_t& specgram, int length)
{
  printf("%d ", length);
  pow_frame_t past_frame, curr_frame;
  onset_envelope o_env; 
  int ind = 0; float val; int i;
  for (int j=0; j<length; j++) {
    //printf("%d ", j);
    fft_frame_t frame = specgram[j];
    std::transform(frame.begin(), frame.end(), curr_frame.begin(), 
                    [] (std::complex<float> z) {return (std::conj(z)*z).real();});
    if (ind == 0) { goto update;}
    val = 0; i=0;
    while (i<511)
    {
      val += std::max(float(0), curr_frame[i]-past_frame[i]);
      //printf("%f%+fi \n", frame[i].real(), frame[i].imag());
      i++;
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
  /*ifstream infile ("stft.txt");*/
  // FILE *fp;
  // fp=fopen("stft.txt","r");
  // if (fp==NULL) 
  //   { 
  //     std::cout<<"Spectrogram file could not be read, check stft.txt exists"<<std::endl;
  //     return 0;
  //     //throw std::exception();
  //   }
  // std::ifstream input( "stft.txt" );
  // std::vector<std::complex<float>> buffer{
  //   std::istream_iterator<std::complex<float>>(input), 
  //   std::istream_iterator<std::complex<float>>() };
  // spectrogram_t spec;
  // fft_frame_t temp;
  // int nlines = 0;
  // while (int i != std::end(buffer))
  // {
  //   printf("%f%+fi", buffer[i].real(), buffer[i].imag());
  //   i++;
  // }
    std::complex<float> c;
    std::ifstream fin("stft.txt");
    std::string line;
    spectrogram_t spec; 
    int nlines = 0;
    
    while(std::getline(fin,line))
    {
      //std::array<std::complex<float>> vec;
      //vec.reserve(512);
      fft_frame_t frame;
      std::stringstream stream(line);
      int i = 0;
      while(stream>>c)
      {
         frame[i] = c;
         i++;
         //printf("%f%+fi \n", c.real(), c.imag());
      }
      spec.push_back(frame);
      i = 0;
      nlines++; 
    }

  // float real, imag;
  // int i=0;
  // int nlines = 0;
  // while(fscanf(fp,"%f%+fi",&real, &imag)!=EOF)
  // {
  //   temp[i] = {real,imag};
  //   //printf("%f%+fi \n ", real, imag);
  //   //printf("%c ", temp[i]);
  //   if (i==511)
  //   {
  //     spec.push_back(temp);
  //     i = 0;
  //     nlines++; 
  //   }
  // }
  /*while ( getline (infile,line) )
  {
    fft_frame_t temp;
    strtk::parse(line, " ", temp);
    spec.push_back(temp);
    cout << line<<temp[0] << '\n';
  }
  infile.close();
  }*/




  //if (i!=0) { std::cout<<"Incorrect number of entries, i="<<i<<std::endl;}  
  std::vector<float> o_env = onset_function(spec, nlines);
  //std::cout << o_env;
  ofstream outfile ("onset_env.txt");
  std::ostream_iterator<float> output_iterator(outfile, "\n");
  std::copy(o_env.begin(), o_env.end(), output_iterator); 
  outfile.close(); 
 }
