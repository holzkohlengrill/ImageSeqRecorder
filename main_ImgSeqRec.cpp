#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio.hpp"
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
bool flag=0;

//############## SETTINGS ########################
const Mode mode = IMAGES;
const size_t nbimg = 0;
//################################################

bool capture(cv::Mat &frame);
bool keyhandler();

int main(int argc, char *argv[]){
  stringstream filename;
  struct stat st = {0};

  if (stat("../saves", &st) == -1) {      // crating directory if not existing
      mkdir("../saves", 0700);
    }
  for(size_t i = 0; ; i++){
      Mat frame;
      capture(frame);
      imshow("Image sequence recorder", frame);
      if (mode == IMAGES){
          if(!flag) cout << "Press 's' to start/stop recording" << endl;
          while(!flag){
              capture(frame);
              imshow("Live view", frame);
              if(keyhandler()) return -1;
            }
          filename << "img" << setw(5) << setfill('0') << to_string(i) << ".png";
          cout << "(info): " << "file " << filename.str() << " saved!" << endl;
          filename.str("../saves/"+filename.str());
          imwrite(filename.str(), frame);
          filename.str("");

        }

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
    case 27 : cout << "ESC key pressed by user" << endl; return -1; break;
    }
  return 0;
}
