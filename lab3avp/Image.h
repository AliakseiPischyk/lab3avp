#ifndef IMAGE_H
#define IMAGE_H
#include <memory>
#include <string>
#include <iostream>
using namespace std;



class Image {
public:
    size_t width;
    size_t height;
    size_t size;
    uint8_t* data;


public:
    Image(size_t w, size_t h, uint8_t* buffer); // default constructor - define in .cpp
   
    ~Image(); // destructor - define in .cpp file

 
    //method to read input files
    //Image operator!();

    Image operator+(const Image& N);
	Image sumOld(const Image& N);
    //Image operator-(const Image& N);
    Image operator/(const Image& N);
    Image slowDivide(const Image& N);
    void Hash();

};


#endif