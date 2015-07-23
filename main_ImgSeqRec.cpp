#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#include <sys/types.h>      // for folder cration in ubuntu
#include <sys/stat.h>       // for folder cration in ubuntu
#include <unistd.h>         // for folder cration in ubuntu

using namespace cv;
using namespace std;

enum Mode {LIVE, IMAGES};
bool flag = 0;

//############## SETTINGS ########################
const Mode mode = IMAGES;
const size_t nbimg = 0;
//################################################

bool capture(cv::Mat &frame);
bool keyhandler();
void showText(std::string text, cv::Mat &img);

int main(int argc, char *argv[]){
  stringstream filename;
  struct stat st = {0};

  if (stat("../saves", &st) == -1) {      // crating directory if not existing
      mkdir("../saves", 0700);
    }
  for(size_t i = 0; ; i++){
      Mat frame;
      capture(frame);
      Mat frameShow = frame.clone();
      if (mode == IMAGES){

          if(!flag){
              std::cout << "\tPress 's' to start/stop recording" << endl;
              while(!flag){
                  capture(frame);
                  frameShow = frame.clone();
                  showText("Press 's' to start/stop recording", frameShow);
                  imshow("Image Sequence Recorder", frameShow);
                  if(keyhandler()) return -1;
                }
            } else {
              filename << "img" << setw(5) << setfill('0') << to_string(i) << ".png";
              stringstream msg;
              msg << "File " << filename.str() << " saved!";
              filename.str("../saves/"+filename.str());
              imwrite(filename.str(), frame);
              showText(msg.str(), frameShow);
              std::stringstream temp;
              temp << "\t(info): " << msg.str();
              std::string dmsg = temp.str();
              cout << dmsg << endl;
              filename.str("");
              msg.str("");
              temp.str("");
            }
        }
      imshow("Image Sequence Recorder", frameShow);
      if (nbimg != 0){
          if(nbimg >= i){
              break;
            }
        }
      if(keyhandler()) return -1;
    }
  return 0;
}

bool capture(cv::Mat &frame){
  static bool init = 0;
  static VideoCapture cap(0);
  if(init == 0){
      if (!cap.isOpened()){
          cerr << "Cannot open the video cam" << endl;
          return -1;
        }
      init = 1;
    }
  bool bSuccess = cap.read(frame);

  if (!bSuccess){
      cerr << "Cannot read a frame from video stream" << endl;
      return -1;
    }
  return 0;
}

bool keyhandler(){
  switch(waitKey(1)){
    case 's' : flag = !flag; break;
    case 27 : cout << "\t(info) ESC key pressed by user" << endl; return -1; break;
    }
  return 0;
}

void showText(std::string text, cv::Mat &img){
  int baseline = 0;
  const int tFont = cv::FONT_HERSHEY_SIMPLEX;
  const double tScale = 1.0;
  const cv::Scalar tColour(255, 0, 60);
  const int tLineType = cv::LineTypes::LINE_AA;
  const int tThickness = 1;

  cv::Size textprop = cv::getTextSize(text, tFont, tScale, tThickness, &baseline);
  cv::Point tOrigin(static_cast<int>(img.cols/2), static_cast<int>(img.rows-textprop.height*3/2));
  cv::putText(img, text, tOrigin, tFont, tScale, tColour, tThickness, tLineType, false);
}
