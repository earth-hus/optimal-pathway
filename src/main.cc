#include <fstream>
#include <iostream>

#include "image_ppm.hpp"
#include "seam_carver.hpp"
using namespace std;
void PrintUsage(const string& command) {
  cout << "Usage: " << command << " INPUT_FILE [options] -o OUTPUT_FILE\n"
       << "Options:\n"
       << "\t-h times\tcarve times horizontal seams\n"
       << "\t-v times\tcarve times vertical seams\n";
}

int main() {
  // ImagePPM image = ImagePPM("images/oceancopy.ppm");
  ImagePPM image = ImagePPM("images/image1.ppm");
  // image.operator=(image);
  SeamCarver tester = SeamCarver();
  tester.SetImage(image);
  cout << "height " << tester.GetHeight() << endl;
  cout << "width " << tester.GetWidth() << endl;

  cout << endl;

  cout << "Enegery matrix:" << endl;
  int** energy_matrix = tester.FillingEnergyMatrix();
  tester.PrintMatrix(energy_matrix);

  cout << endl;

  cout << "Populating Horizontal Matrix:" << endl;
  int** horizontalMatrix = tester.PopulatingHorizontalMatrix();
  tester.PrintMatrix(horizontalMatrix);

  // cout << "Populating Vertical Matrix:" << endl;
  // int** verticalMatrix = tester.PopulatingVerticalMatrix();
  // tester.PrintMatrix(verticalMatrix);

  cout << endl;

  cout << "Horizontal Seam One D-Array:" << endl;
  int* arrayy = tester.GetHorizontalSeam();
  for (int i = 0; i < tester.GetHeight(); i++) {
    cout << arrayy[i] << " ";
  }
  cout << endl;
  // cout << "Vertical Seam One D-Array:" << endl;
  // int* arrayy = tester.GetVerticalSeam();
  // for (int i = 0; i < tester.GetHeight(); i++) {
  //   cout << arrayy[i] << " ";
  // }
  cout << endl;

  cout << "Remove Horizontal Seam:" << endl;
  tester.RemoveHorizontalSeam();

  // cout << "Remove Vertical Seam:" << endl;
  // tester.RemoveVerticalSeam();

  // tester.PrintMatrix(verticalMatrix);
  //  tester.PrintMatrix(verticalMatrix);
}

/*
int main(int argc, char* argv[]) {
  std::string out_option = "-o";
  if (argc < 4 || out_option != argv[argc - 2]) {
    PrintUsage(argv[0]);
    return 1;
  }

  std::ifstream infile(argv[1]);
  if (!infile.good()) {
    std::cout << "file '" << argv[1] << "' not found" << std::endl;
    return 1;
  }

  ImagePPM image;
  infile >> image;
  infile.close();

  SeamCarver carver( image );
  for ( int i = 2; i < argc - 2; i += 2 ) {
    std::string times_string = argv[i + 1];
    int times = 0;
    try {
      times = std::stoi( times_string );
    } catch ( const std::exception& e ) {
      std::cout << "Malformed option" << std::endl;
      PrintUsage( argv[0] );
      return 1;
    }

    std::string option = argv[i];
    if ( option == "-h" ) {
      for ( int t = 0; t < times; t++ ) {
        carver.RemoveHorizontalSeam();
      }
    } else if ( option == "-v" ) {
      for ( int t = 0; t < times; t++ ) {
        carver.RemoveVerticalSeam();
      }
    } else {
      std::cout << argv[i] << " not an option" << std::endl;
      PrintUsage( argv[0] );
      return 1;
    }
  }

  std::ofstream outfile( argv[argc - 1] );
  outfile << carver.GetImage();
  outfile.close();
}
*/