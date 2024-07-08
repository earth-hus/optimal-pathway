#include "seam_carver.hpp"

// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY
using namespace std;
//----------------------------------------------------------------
SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}
//----------------------------------------------------------------
void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

//----------------------------------------------------------------
// returns the instance's image_
const ImagePPM& SeamCarver::GetImage() const { return image_; }

//----------------------------------------------------------------
// returns the image's height
int SeamCarver::GetHeight() const { return height_; }

//----------------------------------------------------------------
// returns the image's width
int SeamCarver::GetWidth() const { return width_; }

//----------------------------------------------------------------
// returns the energy of the pixel at row col in image_
// find a seam with the least amount of energy
// calculation
int** SeamCarver::FillingEnergyMatrix() const {
  int** arr = new int*[GetHeight()];
  for (int i = 0; i < GetHeight(); i++) {
    arr[i] = new int[GetWidth()];
  }
  for (int row = 0; row < GetHeight(); row++) {
    for (int col = 0; col < GetWidth(); col++) {
      arr[row][col] = GetEnergy(row, col);
    }
  }
  return arr;
}
//----------------------------------------------------------------
int SeamCarver::GetEnergy(int row, int col) const {
  int left_col = 0;
  int right_col = 0;
  int delta_row = 0;
  int e = 0;  // edge cases if your on a row; side to side
  if (col ==
      0) {  // calling column calculktion for when calculating horizontally
    left_col = GetWidth() - 1;
    right_col = col + 1;
    delta_row = ColCalc(row, left_col, right_col);
  } else if (col == GetWidth() - 1) {
    left_col = col - 1;
    right_col = 0;
    delta_row = ColCalc(row, left_col, right_col);

  } else {  // in middle row
    left_col = col - 1;
    right_col = col + 1;
    delta_row = ColCalc(row, left_col, right_col);
  }

  int row_top = 0;  // edge cases if your on a col ; up and down
  int row_bottom =
      0;  // calling row calculktion for when calculating vertically
  int delta_col = 0;
  if (row == 0) {
    row_top = GetHeight() - 1;
    row_bottom = row + 1;
    delta_col = RowCalc(row_top, row_bottom, col);
  } else if (row == GetHeight() - 1) {
    row_top = row - 1;
    row_bottom = 0;
    delta_col = RowCalc(row_top, row_bottom, col);
  } else {  // in middle row
    row_top = row - 1;
    row_bottom = row + 1;
    delta_col = RowCalc(row_top, row_bottom, col);
  }
  e = delta_row + delta_col;
  return e;
}
//----------------------------------------------------------------
int SeamCarver::ColCalc(int row, int leftCol, int rightCol) const {
  int col_red = image_.GetPixel(row, leftCol).GetRed() -
                image_.GetPixel(row, rightCol).GetRed();
  int col_green = image_.GetPixel(row, leftCol).GetBlue() -
                  image_.GetPixel(row, rightCol).GetBlue();
  int col_blue = image_.GetPixel(row, leftCol).GetGreen() -
                 image_.GetPixel(row, rightCol).GetGreen();
  int delta_col =
      (col_red * col_red) + (col_green * col_green) + (col_blue * col_blue);

  return delta_col;
}
//----------------------------------------------------------------
int SeamCarver::RowCalc(int rowTop, int rowBottom, int col) const {
  int row_red = image_.GetPixel(rowTop, col).GetRed() -
                image_.GetPixel(rowBottom, col).GetRed();
  int row_green = image_.GetPixel(rowTop, col).GetBlue() -
                  image_.GetPixel(rowBottom, col).GetBlue();
  int row_blue = image_.GetPixel(rowTop, col).GetGreen() -
                 image_.GetPixel(rowBottom, col).GetGreen();
  int delta_row =
      (row_red * row_red) + (row_green * row_green) + (row_blue * row_blue);
  return delta_row;
}
//----------------------------------------------------------------
// It is indeed satisfactory to only calculate it once
// finding the best path from our current cell to the end
// depends on the three cells below, and adjacent, to the current cell
// also notice that the cells on the last row have no dependencies
// so we can calculate them first without knowledge of any other cells

int** SeamCarver::InitiliazeMatriz() const {
  int** arr = new int*[GetHeight()];
  for (int i = 0; i < GetHeight(); i++) {
    arr[i] = new int[GetWidth()];
  }
  return arr;
}

//----------------------------------------------------------------
int** SeamCarver::PopulatingVerticalMatrix() const {
  int** energy_matrix = FillingEnergyMatrix();
  int** populate_vertical_seam_matrix = InitiliazeMatriz();

  for (int col = 0; col < GetWidth();
       col++) {  // the last column should be the same
    populate_vertical_seam_matrix[GetHeight() - 1][col] =
        energy_matrix[GetHeight() - 1][col];
  }
  for (int row = GetHeight() - 2; row >= 0; row--) {
    for (int col = 0; col < GetWidth(); col++) {
      if (col == 0) {  // left
        populate_vertical_seam_matrix[row][col] =
            min(populate_vertical_seam_matrix[row + 1][col],
                populate_vertical_seam_matrix[row + 1][col + 1]);
      } else if (col == GetWidth() - 1) {  // right
        populate_vertical_seam_matrix[row][col] =
            min(populate_vertical_seam_matrix[row + 1][col - 1],
                populate_vertical_seam_matrix[row + 1][col]);
      } else {  // middle - list
        populate_vertical_seam_matrix[row][col] =
            min({populate_vertical_seam_matrix[row + 1][col - 1],
                 populate_vertical_seam_matrix[row + 1][col],
                 populate_vertical_seam_matrix[row + 1][col + 1]});
      }
      populate_vertical_seam_matrix[row][col] += energy_matrix[row][col];
    }
  }
  ClearMatrix(energy_matrix);
  return populate_vertical_seam_matrix;
}
//----------------------------------------------------------------
int** SeamCarver::PopulatingHorizontalMatrix() const {
  int** energy_matrix = FillingEnergyMatrix();
  int** populate_horizontal_seam_matrix = InitiliazeMatriz();

  // prefer the middle > left/top > right/bottom
  // last column is the same as energy matrix
  for (int row = 0; row < GetHeight();
       row++) {  // the last column should be the same
    populate_horizontal_seam_matrix[row][GetWidth() - 1] =
        energy_matrix[row][GetWidth() - 1];
  }
  for (int col = GetWidth() - 2; col >= 0; col--) {
    for (int row = 0; row < GetHeight(); row++) {
      if (row == 0) {  // top
        populate_horizontal_seam_matrix[row][col] =
            min(populate_horizontal_seam_matrix[row][col + 1],
                populate_horizontal_seam_matrix[row + 1][col + 1]);
      } else if (row == GetHeight() - 1) {  // bottom
        populate_horizontal_seam_matrix[row][col] =
            min(populate_horizontal_seam_matrix[row][col + 1],
                populate_horizontal_seam_matrix[row - 1][col + 1]);
      } else {  // middle - list
        populate_horizontal_seam_matrix[row][col] =
            min({populate_horizontal_seam_matrix[row + 1][col + 1],
                 populate_horizontal_seam_matrix[row][col + 1],
                 populate_horizontal_seam_matrix[row - 1][col + 1]});
      }
      populate_horizontal_seam_matrix[row][col] += energy_matrix[row][col];
    }
  }
  ClearMatrix(energy_matrix);
  return populate_horizontal_seam_matrix;
}
//----------------------------------------------------------------

void SeamCarver::HelperForHorizontalSeam(int**& horizontal_seam_matrix,
                                         int& starting_index,
                                         int*& best_optimal_indexes) const {
  // prefer the middle > left/top > right/bottom
  for (int col = 0; col < GetWidth() - 1; col++) {
    int row = starting_index;  // starts at the same row index as the previous
    if (row == 0) {            // if top does not exist
      // if middle <= bottom  --> middle
      if (horizontal_seam_matrix[row][col + 1] <=
          horizontal_seam_matrix[row + 1][col + 1]) {
        starting_index = row;  // prioritize the middle
      } else {
        starting_index = row + 1;  // then prioritze the bottom
      }
    } else if (row == GetHeight() - 1) {  // bottom does not exist
      // here over bottom if top < middle --> top
      if (horizontal_seam_matrix[row - 1][col + 1] <
          horizontal_seam_matrix[row][col + 1]) {
        starting_index = row - 1;  // top
      } else {  // if they are equal,we prirotize middle or if middle < left =
        starting_index = row;  // return the minimum in the middle
      }
    } else {  // middle - list
      // top < middle && top <= bottom --> top
      if (horizontal_seam_matrix[row - 1][col + 1] <
              horizontal_seam_matrix[row][col + 1] &&
          horizontal_seam_matrix[row - 1][col + 1] <=
              horizontal_seam_matrix[row + 1][col + 1]) {  // top
        starting_index = row - 1;                          // return the top
        // if middle <= bottom && middle <= top--> middle
      } else if (horizontal_seam_matrix[row][col + 1] <=
                     horizontal_seam_matrix[row - 1][col + 1] &&
                 horizontal_seam_matrix[row][col + 1] <=
                     horizontal_seam_matrix[row + 1][col + 1]) {  // middle
        starting_index = row;      // return the middle
      } else {                     // the bottom
        starting_index = row + 1;  // choose right as last option
      }
    }  // insert the index into the array
    best_optimal_indexes[col + 1] = starting_index;
  }
}
//----------------------------------------------------------------
// the end most
// the column would remain the same
int* SeamCarver::GetHorizontalSeam() const {
  int** horizontal_seam_matrix = PopulatingHorizontalMatrix();
  int* best_optimal_indexes = new int[GetWidth()];
  // finding the best optimal pathway
  int starting_index = 0;
  for (int row = 0; row < GetHeight(); row++) {  // the min first column
    if (horizontal_seam_matrix[row][0] <
        horizontal_seam_matrix[starting_index][0]) {
      starting_index = row;  // if it is the sanme, it will not be upated
    }
  }
  best_optimal_indexes[0] = starting_index;  // insert the first minimum value
  HelperForHorizontalSeam(
      horizontal_seam_matrix, starting_index, best_optimal_indexes);
  // prefer the middle > left/top > right/bottom
  ClearMatrix(horizontal_seam_matrix);
  return best_optimal_indexes;
}

//----------------------------------------------------------------

/*
  bottom row is goign to the same
  eveyrhting above the bottom row will be depend the
  minimum energy in the row below
  the end most
*/
void SeamCarver::HelperForVerticalSeam(int**& vertical_seam_matrix,
                                       int& starting_index,
                                       int*& best_optimal_indexes) const {
  for (int row = 0; row < GetHeight() - 1; row++) {
    int col = starting_index;
    if (col == 0) {  // left side does not exist
      if (vertical_seam_matrix[row + 1][col] <=
          vertical_seam_matrix[row + 1][col + 1]) {
        starting_index = col;  // reutrn middle
      } else {
        starting_index = col + 1;
      }
    } else if (col == GetWidth() - 1) {  // right does not exist
      // here over right left < middle
      if (vertical_seam_matrix[row + 1][col - 1] <
          vertical_seam_matrix[row + 1][col]) {
        starting_index = col - 1;  // return left
      } else {  // if they are equal,we prirotize middle or if middle < left =
        starting_index = col;
      }
    } else {  // middle - list
      // if left < middle && left <= right
      if (vertical_seam_matrix[row + 1][col - 1] <
              vertical_seam_matrix[row + 1][col] &&
          vertical_seam_matrix[row + 1][col - 1] <=
              vertical_seam_matrix[row + 1][col + 1]) {  // left
        starting_index = col - 1;                        // retunr left column
        // because middle is priority anyways
      } else if (vertical_seam_matrix[row + 1][col] <=
                     vertical_seam_matrix[row + 1][col + 1] &&
                 vertical_seam_matrix[row + 1][col] <=
                     vertical_seam_matrix[row + 1][col - 1]) {  // middle
        starting_index = col;                                   // middle
      } else {                                                  // the right
        starting_index = col + 1;  // choose right as last option
      }
    }
    best_optimal_indexes[row + 1] =
        starting_index;  // insert the index into the array
  }
}

int* SeamCarver::GetVerticalSeam() const {
  // done populating the seam matirx
  int** vertical_seam_matrix = PopulatingVerticalMatrix();
  int* best_optimal_indexes =
      new int[GetHeight()];  // What I'm returning/In the heap, b/c its what I'm

  // finding the best optimal pathway
  // the first row
  int starting_index = 0;
  for (int col = 0; col < GetWidth(); col++) {
    if (vertical_seam_matrix[0][col] <
        vertical_seam_matrix[0][starting_index]) {
      starting_index = col;  // if it is the sanme, it will not be upated
                             // gets the minimum value in the first row
    }
  }
  best_optimal_indexes[0] = starting_index;  // insert the first minimum value
  HelperForVerticalSeam(
      vertical_seam_matrix, starting_index, best_optimal_indexes);
  ClearMatrix(vertical_seam_matrix);
  return best_optimal_indexes;
}
//----------------------------------------------------------------
void SeamCarver::RemoveHorizontalSeam() {
  int* best_optimal_indexes = GetHorizontalSeam();
  image_.ShiftHorizontalMatrix(best_optimal_indexes);
  height_ = GetHeight() - 1;
  ClearOneDArray(best_optimal_indexes);
}
//----------------------------------------------------------------
void SeamCarver::RemoveVerticalSeam() {
  int* best_optimal_indexes = GetVerticalSeam();
  image_.ShiftVerticalMatrix(best_optimal_indexes);
  width_ = GetWidth() - 1;
  ClearOneDArray(best_optimal_indexes);
}

//----------------------------------------------------------------
void SeamCarver::ClearMatrix(int**& arr) const {
  for (int i = 0; i < GetHeight(); i++) {
    delete[] arr[i];
  }
  delete[] arr;
  arr = nullptr;
}

void SeamCarver::ClearOneDArray(int*& arr) {
  delete[] arr;
  arr = nullptr;
}
//----------------------------------------------------------------
void SeamCarver::PrintMatrix(int** matrix) const {
  for (int row = 0; row < GetHeight(); row++) {
    for (int col = 0; col < GetWidth(); col++) {
      cout << matrix[row][col] << " ";
    }
    cout << endl;
  }
}