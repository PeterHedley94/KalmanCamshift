// thread example
#include <iostream>
#include <thread>         // std::thread
#include <chrono>
#include "opencv2/opencv.hpp"
#include <mutex>
using namespace cv;


template <class T>
class Buffer{
  T* start;
  T* end;
  bool written_to = true;
  int read_ = -1;
  int write_ = 0;
  std::mutex* buffer_mutex_ptr;
  std::mutex read_mutex;
  int length;

public:
  Buffer(int length_){
    this->length = length_;
    start = new T[length];
    end = start+length;
    buffer_mutex_ptr = new std::mutex[length];
  }

  //read element from buffer
  T get(){
    if(written_to){
      std::lock_guard<std::mutex> guard_0(buffer_mutex_ptr[read_]);
      return start[read_];
    }else{
      std::cerr << "hasn't been written to yet \n";
      return T();
    }
  }

  //write element to buffer
  void write(T& elem){
    std::lock_guard<std::mutex> guard_0(buffer_mutex_ptr[write_]);
    std::lock_guard<std::mutex> guard_1(read_mutex);
    start[write_++] = elem;
    read_++;

    if(write_ >= length){
      write_ = 0;
      read_ = length-1;
    }
    if(read_ >= length) read_=0;
  }
  template <typename ST>
  friend std::ostream& operator<<(std::ostream& os, const Buffer<ST>&);

};

template <typename ST>
std::ostream& operator<<(std::ostream& os, const Buffer<ST>& b){
    for(int i=0;i<b.length;i++){
      if(i==b.read_){
	std::cout << "r";
      }else if(i == b.write_){
	std::cout << "w";
      }else{
	std::cout << " ";
      }
    }
    std::cout << std::endl;
    for(int i=0;i<b.length;i++){
      std::cout << b.start[i];
    }
    std::cout << std::endl;
    return os;
}

int main(){

  Buffer<Mat> b(5);
  Mat E = Mat::eye(1, 1, CV_64F);

  for(int i=0; i<10;i++){
      b.write(E);
      std::cout << b;
  }
  return 0;
}
