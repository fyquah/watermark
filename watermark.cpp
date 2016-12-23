#include <iostream>
#include <string>
#include <vector>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


const double MAX_ROW_SIZE = 300.0;

string replace_extension(string s, const string &extension)
{
  for (int i = s.size() - 1 ; i >= 0 ; i--) {
    if (s[i] == '.') {
      s = s.substr(0, i);
      break;
    }
  }
  return s + "." + extension;
}


void print_usage()
{
  cout << "Usage:" << endl;
  cout << "    watermark -i image.png -s 'For someone Only' [-o 'output.png']" << endl;
}


class InputParser{
public:
  InputParser (int &argc, char **argv){
    for (int i=1; i < argc; ++i)
      this->tokens.push_back(std::string(argv[i]));
  }
  // @author iain
  const std::string& getCmdOption(const std::string &option) const{
    std::vector<std::string>::const_iterator itr;
    itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
    if (itr != this->tokens.end() && ++itr != this->tokens.end()){
        return *itr;
    }
    return EMPTY_STRING;
  }
  // referred from http://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c
  // @author iain
  bool cmdOptionExists(const std::string &option) const{
    return std::find(this->tokens.begin(), this->tokens.end(), option)
      != this->tokens.end();
  }
private:
  std::vector<std::string> tokens;
  std::string EMPTY_STRING = "";
};

int main(int argc, char* argv[])
{
  InputParser input(argc, argv);
  string input_file_name;
  string output_file_name;
  string stamp;

  if (!input.cmdOptionExists("-s") ||
      !input.cmdOptionExists("-i")) {
    print_usage();
    return 1;
  }
  input_file_name = input.getCmdOption("-i");
  stamp = input.getCmdOption("-s");

  if (input.cmdOptionExists("-o")) {
    output_file_name = input.getCmdOption("-o");
  } else {
    output_file_name = replace_extension(input_file_name, "output.jpeg");
  }

  // 1. Load the image form the file.
  Mat image = imread(input_file_name, CV_LOAD_IMAGE_COLOR);

  // 2. Resize the image if it's too big.
  if (image.rows >= MAX_ROW_SIZE) {
    double ratio = image.rows / MAX_ROW_SIZE;
    Size size(image.cols / ratio, MAX_ROW_SIZE);
    resize(image, image, size);
  }

  // Draw onto the image.
  Mat text_image = Mat::zeros(image.rows, image.cols, image.type());
  Mat new_image;

  int fontFace = FONT_HERSHEY_SIMPLEX;
  double fontScale = 1;
  int thickness = 2;
  double angle = -23.0;

  // should do some maths here.
  Point textOrg(text_image.cols / 2.0 - stamp.length() * 10.0,
                text_image.rows / 2.0 - stamp.length() * 0.5);
  putText(text_image, stamp, textOrg, fontFace, fontScale, Scalar::all(255), thickness, 8);
  rotate(text_image, text_image, -45);
  cv::Point2f center(text_image.cols/2.0, text_image.rows/2.0);
  cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
  cv::warpAffine(text_image, text_image, rot, text_image.size());
  text_image = cv::Scalar::all(255) - text_image;



  cv::min(text_image, image, new_image);
  imwrite(output_file_name, new_image);
  return 0;
}
