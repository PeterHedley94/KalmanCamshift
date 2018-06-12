// thread example
#include <iostream>
#include <thread>         // std::thread
#include <chrono>
#include "opencv2/opencv.hpp"
using namespace cv;


/// Global Variables
const int alpha_slider_max = 100;
const int beta_slider_max = 100;
int alpha_slider;
int beta_slider;
double alpha;
double beta;

///Image Arrays
Mat input;
Mat window;




void get_image()
{
  //Camera
  VideoCapture cap;
  // do stuff...
  std::cout << "Getting Image" << std::endl;

  if(!cap.open(0)){
      std::cout << "Failed to get camera \n";
      return;
  }else{
    std::cout << "Located camera \n";
  }
  /// Create Windows
  namedWindow("Original Image", 1);

  cap >> input;
  std::cout << "Placed image into input \n";

  if( input.empty() ){
      std::cout << "Empty input" << std::endl;
     return; // end of video stream
  }
  //imshow("Original Image", input);
  //waitKey(10);
  for(;;){
    std::cout << "Sleeping" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }
  /*
  for(;;)
  {
        cap >> input;
        if( input.empty() ) break; // end of video stream
        imshow("Original Image", input);
        if( waitKey(10) == 27 ) break; // stop capturing by pressing ESC
  }*/
}

void on_trackbar(int, void* )
{
  alpha = (double) alpha_slider/alpha_slider_max ;
  beta = (double) beta_slider;
  input.convertTo(window, -1, alpha, beta);
  //addWeighted( src1, alpha, src2, beta, 0.0, dst);
  imshow( "Processed Image", window );
}

void process_image(int x)
{
  // do stuff...
  std::cout << "Processing Image" << std::endl;
  /// Create Windows
  namedWindow("Processed Image", 1);

  for(;;)
  {
      //cap >> input;
      if(!input.empty() ){

          input.convertTo(window, -1, 0.2, 50);
          imshow( "Processed Image", window );

          if( waitKey(10) == 27 ){
            std::cout << "ESC pressed \n";
             break; // stop capturing by pressing ESC
          }
          std::cout << "Running \n";
      }else{
        std::cout << "Sleeping Processing Thread \n";
        std::this_thread::sleep_for(std::chrono::milliseconds(30000));
      }
  }
}

int main()
{

  std::thread first (get_image);     // spawn new thread that calls get_image()
  std::thread second (process_image,0);  // spawn new thread that calls process_image(0)

  std::cout << "now execute concurrently...\n";

  // synchronize threads:
  first.join();                // pauses until first finishes
  std::cout << "Thread one finshed...\n";
  second.join();               // pauses until second finishes

  std::cout << "Threads completed.\n";

  return 0;
}
