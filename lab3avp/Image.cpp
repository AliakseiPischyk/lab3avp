#include "Image.h"
#pragma once
 #include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <immintrin.h>
#include <cstdint>
#include <memory>
#include "ThreadPool.h"
#include <algorithm>



using namespace std;


// dst and src must be 32-byte aligned
// size must be multiple of 32*2 = 64 bytes
static void CopyWithAVXNoCache(uint8_t* dst, uint8_t* src, size_t size)
{
    size_t stride = 2 * sizeof(__m256i);
    while (size)
    {
        __m256i a = _mm256_load_si256((__m256i*)src + 0);
        __m256i b = _mm256_load_si256((__m256i*)src + 1);
        _mm256_stream_si256((__m256i*)dst + 0, a);
        _mm256_stream_si256((__m256i*)dst + 1, b);

        size -= stride;
        src += stride;
        dst += stride;
    }
}

Image::Image(size_t w, size_t h, uint8_t* buffer) :
    width(w),
    height(h),
    size(w*h)
{
    data = (uint8_t*)malloc(size * sizeof(uint8_t));
    const size_t pos_for_fast_memcpy = size - size % (64* ThreadPool::threadCount);
    bool flagsEnd[ThreadPool::threadCount] = {0};
    size_t block_sz = pos_for_fast_memcpy / ThreadPool::threadCount;
    for (int currOffset = 0,i=0; i < ThreadPool::threadCount; currOffset += block_sz,i++)
    {
        std::async(std::launch::async, [this, buffer, block_sz, currOffset, &flagsEnd, i]()
            {
                std::memcpy(data + currOffset, buffer + currOffset, block_sz);
                flagsEnd[i] = true;
            });
       
    }
    std::memcpy(data + pos_for_fast_memcpy, buffer + pos_for_fast_memcpy, size - pos_for_fast_memcpy);

    while (!std::all_of(std::begin(flagsEnd), std::end(flagsEnd), [](bool val) {return val == true; }))
    {    };
}


Image::~Image() { // destructor
   // free(data);
    data = nullptr;
}


Image Image::operator+(const Image& N)
{      
    auto var = (uint8_t*)calloc(size, sizeof(uint8_t));
    Image ret(width, height, var);
    free(var);

    const size_t pos_for_fast_memcpy = size - size % (64 * ThreadPool::threadCount);
    bool flagsEnd[ThreadPool::threadCount] = { 0 };
    size_t block_sz = pos_for_fast_memcpy / ThreadPool::threadCount;
    for (int currOffset = 0, i = 0; i < ThreadPool::threadCount; currOffset += block_sz, i++)
    {
        std::async(std::launch::async, [this, &N, &flagsEnd, i, &ret, currOffset, block_sz]()
            {
                std::transform(data + currOffset, data + currOffset + block_sz, N.data + currOffset, ret.data + currOffset, std::plus<>());
                flagsEnd[i] = true;
            });
        //threadPool.enqueue([this,&N,&flagsEnd,i,&ret,currOffset,block_sz]()
        //    {
        //        std::transform(data + currOffset, data + currOffset + block_sz, N.data + currOffset, ret.data + currOffset, std::plus<>());
        //        flagsEnd[i] = true;
        //    });
    }
    std::transform(data+ pos_for_fast_memcpy, data + size, N.data+ pos_for_fast_memcpy, ret.data+ pos_for_fast_memcpy, std::plus<>());

    while (!std::all_of(std::begin(flagsEnd), std::end(flagsEnd), [](bool val) {return val == true; }))
    {    };
    return ret;
}


Image Image::sumOld(const Image& N)
{
    auto var = (uint8_t*)calloc(size, sizeof(uint8_t));
    Image ret(width, height, var);
    free(var);

    auto otherDataPtr = N.data;
    int i = 0;
    int sz = size;
    while (size--) {
        i++;
        int check = (*data + *otherDataPtr);
        if (check > 255) {
            check = 255;
        }

        *ret.data = check;

        ++data;
        ++otherDataPtr;
        ++ret.data;
    }

    return ret;
}
//Image Image::operator-(const Image& N) {
//
//
//}
Image Image::operator/(const Image& N) 
{
    auto var = (uint8_t*)calloc(size, sizeof(uint8_t));
    Image ret(width, height, var);
    free(var);

    const size_t pos_for_fast_memcpy = size - size % (64 * ThreadPool::threadCount);
    bool flagsEnd[ThreadPool::threadCount] = { 0 };
    size_t block_sz = pos_for_fast_memcpy / ThreadPool::threadCount;
    for (int currOffset = 0, i = 0; i < ThreadPool::threadCount; currOffset += block_sz, i++)
    {
       std::async(std::launch::async ,[this, &N, &flagsEnd, i, &ret, currOffset, block_sz]()
            {
                std::transform(data + currOffset, data + currOffset + block_sz, N.data + currOffset, ret.data + currOffset, 
                    [](uint8_t val1, uint8_t val2) {return val1/(val2+1);});
                flagsEnd[i] = true;
            });
    }
    std::transform(data + pos_for_fast_memcpy, data + size, N.data + pos_for_fast_memcpy, ret.data + pos_for_fast_memcpy, std::plus<>());

    while (!std::all_of(std::begin(flagsEnd), std::end(flagsEnd), [](bool val) {return val == true; }));
    
    return ret;

}

Image Image::slowDivide(const Image& N)
{
    auto var = (uint8_t*)calloc(size, sizeof(uint8_t));
    Image ret(width, height, var);
    free(var);

    auto otherDataPtr = N.data;
    int sz = size;
    while (sz--) 
    {
        *ret.data = *data/(*otherDataPtr+1);

        ++data;
        ++otherDataPtr;
        ++ret.data;
    }

    return ret;
}
//Image Image::operator!() {
//
//
//
//}
////threshold
void Image::Hash()
{
    const size_t pos_for_fast_memcpy = size - size % (64 * ThreadPool::threadCount);
    bool flagsEnd[ThreadPool::threadCount] = { 0 };
    size_t block_sz = pos_for_fast_memcpy / ThreadPool::threadCount;
    for (int currOffset = 0, i = 0; i < ThreadPool::threadCount; currOffset += block_sz, i++)
    {
    std::async (std::launch::async,[this, &flagsEnd, i, currOffset, block_sz]()
            {
                std::transform(data + currOffset, data + currOffset + block_sz,data+currOffset, std::hash<uint8_t>());
                flagsEnd[i] = true;
            });
    }
    std::transform(data + pos_for_fast_memcpy, data + size, data + pos_for_fast_memcpy, std::hash<uint8_t>());

    while (!std::all_of(std::begin(flagsEnd), std::end(flagsEnd), [](bool val) {return val == true; }))
    {
    };
}