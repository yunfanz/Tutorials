#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <complex>
#include <algorithm>
#include <typeinfo>
#include <array>
int main ()
{
  FILE *fp;
  fp=fopen("real.txt","r");
  
  float temp, real, imag; std::complex<float> c; int i=0;
  std::array<float,2> arr = {1,2};
  while(fscanf(fp,"%f %f",&real, &imag)!=EOF) //Works as good as line below
  {
    c = {real,imag};
    float dif = 0;
    std::cout <<c<< std::max(dif,arr[1]);
  }
  fclose(fp);

}
