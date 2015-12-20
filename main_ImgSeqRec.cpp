#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>            // for setw(), setfill(), etc.

#include <sys/types.h>      // for folder creation in ubuntu
#include <sys/stat.h>         // for folder creation in ubuntu
#include <unistd.h>            // for folder creation in ubuntu

//############## VARIABLES #######################
enum Mode{IMGCAP, VIDEXT, VIEW, EXIT};
//TODO: remove global variables & use classes
cv::Mat frameShow;
cv::Mat frame;

//############## SETTINGS ########################
Mode mode = VIEW;
//const size_t nbimg = 0;

//############## FUNCTION PROTOTYPES ###############
// camera live view
void liveView();
// saves images from camera live view
void imgcap();
// extracts an image sequence from a video
bool vidext();
// program control
bool keyhandler();
// text overlay in live view
void showText(std::string text, cv::Mat &img, int pos);
// folder creation
void createSavesFolder(std::string filename);
// nomal capture
void capture(cv::Mat &img);
// capture for creating image sequences from videos >> esp. for vidext()
void capture(size_t &count, cv::Mat &img);


int main(int argc, char *argv[]){
  createSavesFolder("../saves");
  createSavesFolder("../vid_extr2");
  while(true){
      switch(mode){
        case IMGCAP : imgcap(); break;
        case VIDEXT : vidext(); break;
        case VIEW : liveView(); break;
        case EXIT : return(0);
        }
      keyhandler();
      cv::imshow("Imeo Tool", frameShow);
      cv::waitKey(1);
    }
}

inline void liveView(){
  capture(frameShow);
  showText("Press 's' to start/stop recording", frameShow, 3);
  showText("Press 'v' to extract an img seq of a .mp4 video file", frameShow, 2);
}

inline void imgcap(){
  static size_t i = 0;
  capture(frameShow);
  frame = frameShow.clone();
  std::stringstream filename;
  filename << "img" << std::setw(5) << std::setfill('0') << std::to_string(i) << ".png";
  std::stringstream msg;
  msg << "File " << filename.str() << " saved!";
  filename.str("../saves/"+filename.str());
  cv::imwrite(filename.str(), frame);
  showText(msg.str(), frameShow, 2);
  i++;
}

inline bool vidext(){
  static size_t i = 0;
  static size_t count = 0;
  static cv::VideoCapture video("../dataset2.mp4");
  static const size_t count_max = (video.get(CV_CAP_PROP_FRAME_COUNT)-1);
  if(count >= count_max){
      showText("Video extracting finished", frameShow, 2);
      cv::waitKey(1000);
      showText("Video extracting finished.", frameShow, 2);
      cv::waitKey(1000);
      showText("Video extracting finished..", frameShow, 2);
      cv::waitKey(1000);
      showText("Video extracting finished...", frameShow, 2);
      cv::waitKey(1000);
      showText("\n>> Starting livestream in 2 seconds...", frameShow, 2);
      cv::waitKey(2000);
      mode = VIEW;
      return 0;
    } else{
      capture(count, frame);
      frameShow = frame.clone();

      std::stringstream filename;
      filename << "vid" << std::setw(5) << std::setfill('0') << std::to_string(count) << ".png";
      std::stringstream msg;
      msg << "File " << filename.str() << " saved!";
      filename.str("../vid_extr2/"+filename.str());
      count++;

      cv::imwrite(filename.str(), frame);
      showText(msg.str(), frameShow, 2);
    }
}

inline bool keyhandler(){
  switch(cv::waitKey(10)){
    case 's' : mode = mode == IMGCAP ? VIEW : IMGCAP; break;
    case 'v' : mode = mode == VIDEXT ? VIEW : VIDEXT; break;
    case 27 : std::cout << "\t(info) ESC key pressed by user" << std::endl; mode = EXIT; return(false); break;
    }
  return true;
}

inline void showText(std::string text, cv::Mat &img, int pos){
  int baseline = 0;
  const int tFont = cv::FONT_HERSHEY_SIMPLEX;
  const double tScale = 0.7;
  const cv::Scalar tColour(255, 0, 60);
  const int tLineType = cv::LineTypes::LINE_AA;
  const int tThickness = 1;
  cv::Size textprop = cv::getTextSize(text, tFont, tScale, tThickness, &baseline);
  cv::Point tOrigin(static_cast<int>(img.cols/2), static_cast<int>(img.rows-textprop.height*pos*1.2));
  cv::putText(img, text, tOrigin, tFont, tScale, tColour, tThickness, tLineType, false);
}

void createSavesFolder(std::string filename){
  struct stat st = {0};
  if (stat(filename.c_str(), &st) == -1) {      // creating directory if not existing
      mkdir(filename.c_str(), 0700);
    }
}

void capture(cv::Mat &img){
  switch(mode){
    case VIEW :
    case IMGCAP :
      static cv::VideoCapture cap(0);
      cap >> img;
      break;
    default : // do nothing
      break;
    }
}

void capture(size_t &count, cv::Mat &img){
  static cv::VideoCapture video("../dataset2.mp4");
  video.set(CV_CAP_PROP_POS_FRAMES, count);
  video.read(img);
}
