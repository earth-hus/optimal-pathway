#include "image_ppm.hpp"

// implement the rest of ImagePPM's functions here

// given functions below, DO NOT MODIFY
using namespace std;
ImagePPM::ImagePPM(const string& path) {
  ifstream input_file(path);
  input_file >> *this;
  input_file.close();
}

ImagePPM::ImagePPM(const ImagePPM& source) { *this = source; }

ImagePPM& ImagePPM::operator=(const ImagePPM& source) {
  if (this == &source) {
    return *this;
  }

  Clear();

  height_ = source.height_;
  width_ = source.width_;
  max_color_value_ = source.max_color_value_;

  pixels_ = new Pixel*[height_];
  for (int row = 0; row < height_; row++) {
    pixels_[row] = new Pixel[width_];

    for (int col = 0; col < width_; col++) {
      pixels_[row][col] = source.pixels_[row][col];
    }
  }

  return *this;
}

ImagePPM::~ImagePPM() { Clear(); }

void ImagePPM::Clear() {
  for (int i = 0; i < height_; i++) {
    delete[] pixels_[i];
  }

  delete[] pixels_;

  height_ = 0;
  width_ = 0;
  pixels_ = nullptr;
}

istream& operator>>(istream& is, ImagePPM& image) {
  image.Clear();
  string line;
  // ignore magic number line
  getline(is, line);
  // check to see if there's a comment line
  getline(is, line);
  if (line[0] == '#') {
    getline(is, line);
  }
  // parse width and height
  int space = line.find_first_of(' ');
  image.width_ = stoi(line.substr(0, space));
  image.height_ = stoi(line.substr(space + 1));
  // get max color value
  getline(is, line);
  image.max_color_value_ = stoi(line);
  // init and fill in Pixels array
  image.pixels_ = new Pixel*[image.height_];
  for (int i = 0; i < image.height_; i++) {
    image.pixels_[i] = new Pixel[image.width_];
  }
  for (int row = 0; row < image.height_; row++) {
    for (int col = 0; col < image.width_; col++) {
      getline(is, line);
      int red = stoi(line);
      getline(is, line);
      int green = stoi(line);
      getline(is, line);
      int blue = stoi(line);

      Pixel p(red, green, blue);
      image.pixels_[row][col] = p;
    }
  }
  return is;
}

Pixel ImagePPM::GetPixel(int row, int col) const {
  return ImagePPM::pixels_[row][col];
}

int ImagePPM::GetMaxColorValue() const {
  return max_color_value_;  // returns the max color value of the image
}

// PPM format print out the file
ostream& operator<<(ostream& os, const ImagePPM& image) {
  os << "P3"
     << "\n";
  os << image.GetWidth() << " " << image.GetHeight() << '\n';
  os << image.GetMaxColorValue() << '\n';
  for (int row = 0; row < image.GetHeight(); row++) {   // GetHeight --> row
    for (int col = 0; col < image.GetWidth(); col++) {  // GetWidth --> column

      os << image.pixels_[row][col].GetRed() << '\n';
      os << image.pixels_[row][col].GetGreen() << '\n';
      os << image.pixels_[row][col].GetBlue() << '\n';
      // if (row == image.GetHeight() - 1 || col == image.GetWidth() - 1) {
      //   os << '\n';  // print next line
      // }
    }
  }
  return os;
}
Pixel** ImagePPM::InitiliazeMatriz() const {
  Pixel** arr = new Pixel*[GetHeight()];
  for (int i = 0; i < GetHeight(); i++) {
    arr[i] = new Pixel[GetWidth()];
  }
  return arr;
}

//----------------------------------------------------------------
// void ImagePPM::PrintMatrix() const {
//   for (int row = 0; row < GetHeight(); row++) {
//     for (int col = 0; col < GetWidth(); col++) {
//       cout << pixels_[row][col] << " ";
//     }
//     cout << endl;
//   }
// }

void ImagePPM::ShiftVerticalMatrix(const int* best_optimal_indexes) {
  // left, middle, right
  // bestOptimalIndexes --> up to down
  // remove pixel and shift to the left b/c its avertical seam
  width_ = GetWidth() - 1;

  // cout << "byeeee" << endl;
  for (int row = 0; row < GetHeight(); row++) {
    // cout << "!!!!!!" << best_optimal_indexes[row] << endl;
    for (int col = best_optimal_indexes[row]; col < GetWidth(); col++) {
      pixels_[row][col] = pixels_[row][col + 1];
    }
  }
  // PrintMatrix(pixels_);

  Pixel** new_arr = new Pixel*[GetHeight()];
  for (int i = 0; i < GetHeight(); i++) {
    new_arr[i] = new Pixel[width_];
  }

  for (int row = 0; row < GetHeight(); row++) {
    for (int col = 0; col < width_; col++) {
      new_arr[row][col] = pixels_[row][col];
    }
  }

  // PrintMatrix(new_arr);

  ClearMatrix(pixels_);
  pixels_ = new_arr;
}

void ImagePPM::ShiftHorizontalMatrix(const int* best_optimal_indexes) {
  // top, middle, bottom
  // bestOptimalIndexes --> left to right
  // remove pixel and shift to the up b/c its horizontal seam
  for (int col = 0; col < GetWidth(); col++) {
    for (int row = best_optimal_indexes[col]; row < GetHeight() - 1; row++) {
      pixels_[row][col] = pixels_[row + 1][col];  // shift it up
    }
  }

  Pixel** new_arr = new Pixel*[GetHeight() - 1];
  for (int i = 0; i < GetHeight() - 1; i++) {
    new_arr[i] = new Pixel[GetWidth()];
  }

  for (int row = 0; row < GetHeight() - 1; row++) {
    for (int col = 0; col < width_; col++) {
      new_arr[row][col] = pixels_[row][col];
    }
  }
  ClearMatrix(pixels_);
  height_ = GetHeight() - 1;
  pixels_ = new_arr;
}

void ImagePPM::ClearMatrix(Pixel**& arr) const {
  for (int i = 0; i < GetHeight(); i++) {
    delete[] arr[i];
  }
  delete[] arr;
  arr = nullptr;
}

void ImagePPM::ClearOneDArray(Pixel*& arr) {
  delete[] arr;
  arr = nullptr;
}