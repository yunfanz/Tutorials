#include <fstream>
#include <iostream>
#include <iomanip>
#include <complex>
using std::cout;
using std::endl;


int main() {
  const char* filename = "stft.txt";
  std::ifstream inFile(filename);

  // Make sure the file stream is good
  if(!inFile) {
    cout << endl << "Failed to open file " << filename;
    return 1;
  }
  using namespace std::complex_literals;
  std::complex<float> n;
  while(!inFile.eof()) {
    inFile >> n;
    cout << std::setw(2) << n;
  }
  cout << endl;
  return 0;
}
