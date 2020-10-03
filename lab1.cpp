#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>


using namespace std;

float function(float x) {
  return ( -x*x - 0.5);
}

int main() {
  freopen("graphika.txt", "w", stdout); //creation of the file
  int width, height;
  vector<float> array_Y;
  float Ymin = 0, Ymax = 0, Xmin, Xmax;
  cin >> width >> height >> Xmin >> Xmax;
  height++;
  float cell_width = (Xmax - Xmin) / width;
  for (int a = 0; a < width; a++) {
    float now_X = Xmin + cell_width * a;
    float now_Y = function(now_X);
    array_Y.push_back(now_Y);
    Ymin = min(now_Y, Ymin);
    Ymax = max(now_Y, Ymax);
  }
  float cell_height = (Ymax - Ymin) / (height - 1);
  int line0 = ceil(Ymax / cell_height);
  for (int a = 0; a < line0; ++a) {
    for (int b = 0; b < width; ++b) {
      if ((line0 - a) * cell_height < array_Y[b]) { cout << '#'; }
      else { cout << ' '; }
    }
    cout << endl;
  }
  for (int b = 0; b < width; ++b) {
    cout << '-';
  }
  cout << endl;
  for (int a = line0 + 1; a <= height+1; ++a) {
    for (int b = 0; b < width; ++b) {
      if ((a - line0) * cell_height < (-array_Y[b])) { cout << '#'; }
      else { cout << ' '; }
    }
    cout << endl;
  }
}