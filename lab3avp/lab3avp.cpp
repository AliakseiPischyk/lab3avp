#include "image.h"
#include <future>

void testCreate2Async() 
{
    const size_t data_sz = 31266873;//3127*999999
    uint8_t* buffer1 = (uint8_t*)malloc(data_sz * sizeof(uint8_t));
    for (int i = 0; i < data_sz; i++)
    {
        buffer1[i] = (rand() % 256);
    }

    std::future<shared_ptr<Image>> futImg = std::async(std::launch::async, [buffer1]()
        {
            std::shared_ptr<Image> img = std::make_shared<Image>(3127, 9999, buffer1);
            return img;
        });


    uint8_t* buffer2 = (uint8_t*)malloc(data_sz * sizeof(uint8_t));
    for (int i = 0; i < data_sz; i++)
    {
        buffer2[i] = (rand() % 256);
    }
    Image img2(3127, 9999, buffer2);

    std::cout << img2.data[31266873];
    std::cout << futImg.get().operator*().data[31266873];

    //_ASSERT(memcmp(buffer1, futImg.get().data, data_sz) == 0);
    std::free(buffer1);
    std::free(buffer2);
}

void testCreate2NotAsync()
{
    const size_t data_sz = 31266873;//3127*9999
    uint8_t* buffer1 = (uint8_t*)malloc(data_sz * sizeof(uint8_t));
    for (int i = 0; i < data_sz; i++)
    {
        buffer1[i] = (rand() % 256);
    }


            Image img(3127, 9999, buffer1);



    uint8_t* buffer2 = (uint8_t*)malloc(data_sz * sizeof(uint8_t));
    for (int i = 0; i < data_sz; i++)
    {
        buffer2[i] = (rand() % 256);
    }
    Image img2(3127, 9999, buffer2);

  
    std::cout << img.data[31266873];
    std::cout << img2.data[31266873];

    
    std::free(buffer1);
    std::free(buffer2);
}

void testInitSizeMultipleTo32()
{
    const size_t data_sz = 32000000;
    uint8_t* buffer1 = new uint8_t[data_sz];
    for (int i = 0; i < data_sz; i++)
    {
        buffer1[i] = (rand() % 256);
    }
    Image img(32, 1000000, buffer1);
    _ASSERT(memcmp(buffer1, img.data, data_sz)==0);
    delete[](buffer1);
}

void testInitSizeNotMultipleTo32()
{
    const size_t data_sz = 31266873;//3127*9999
    uint8_t* buffer1 = (uint8_t*)malloc(data_sz * sizeof(uint8_t));
    for (int i = 0; i < data_sz; i++)
    {
        buffer1[i] = (rand() % 256);
    }
    Image img(3127, 9999, buffer1);
    _ASSERT(memcmp(buffer1, img.data, data_sz)==0);
    std::free(buffer1);
}

void testSum()
{

        const size_t data_sz = 31266873;//3127*9999
        uint8_t* buffer1 = (uint8_t*)malloc(data_sz * sizeof(uint8_t));
        for (int i = 0; i < data_sz; i++)
        {
            buffer1[i] = (rand() % 256);
        }

        auto futImg = std::async(std::launch::async, [buffer1]()
            {  
                std::shared_ptr<Image > img=std::make_shared<Image>(3127, 9999, buffer1);
                return img; 
            });

       
        uint8_t* buffer2 = (uint8_t*)malloc(data_sz * sizeof(uint8_t));
        for (int i = 0; i < data_sz; i++)
        {
            buffer2[i] = (rand() % 256);
        }
        Image img2(3127, 9999, buffer2);

        auto res = img2 + futImg.get().operator*();
        std::cout << res.data[31266873];

        
        std::free(buffer1);
        std::free(buffer2);
}

void testSumOld()
{
    const size_t data_sz = 31266873;//3127*9999
    uint8_t* buffer1 = (uint8_t*)malloc(data_sz * sizeof(uint8_t));
    for (int i = 0; i < data_sz; i++)
    {
        buffer1[i] = (rand() % 256);
    }
    auto futImg = std::async(std::launch::async, [buffer1]()
        {
            std::shared_ptr<Image > img = std::make_shared<Image>(3127, 9999, buffer1);
            return img;
        });


    uint8_t* buffer2 = (uint8_t*)malloc(data_sz * sizeof(uint8_t));
    for (int i = 0; i < data_sz; i++)
    {
        buffer2[i] = (rand() % 256);
    }
    Image img2(3127, 9999, buffer2);

    auto res = img2.sumOld( futImg.get().operator*());
    std::cout << res.data[0];

//    _ASSERT(memcmp(buffer1, futImg.get().operator*().data, data_sz) == 0);
    std::free(buffer1);
    std::free(buffer2);
}

void testSlowDivide()
{
    const size_t data_sz = 31266873;//3127*9999
    uint8_t* buffer1 = (uint8_t*)malloc(data_sz * sizeof(uint8_t));
    for (int i = 0; i < data_sz; i++)
    {
        buffer1[i] = (rand() % 256);
    }
    auto futImg = std::async(std::launch::async, [buffer1]()
        {
            std::shared_ptr<Image > img = std::make_shared<Image>(3127, 9999, buffer1);
            return img;
        });


    uint8_t* buffer2 = (uint8_t*)malloc(data_sz * sizeof(uint8_t));
    for (int i = 0; i < data_sz; i++)
    {
        buffer2[i] = (rand() % 256);
    }
    Image img2(3127, 9999, buffer2);

    auto res = img2.slowDivide(futImg.get().operator*());
    std::cout << res.data[0];

  //  _ASSERT(memcmp(buffer1, futImg.get().operator*().data, data_sz) == 0);
    std::free(buffer1);
    std::free(buffer2);
}
void testDivide()
{
    const size_t data_sz = 31266873;//3127*9999
    uint8_t* buffer1 = (uint8_t*)malloc(data_sz * sizeof(uint8_t));
    for (int i = 0; i < data_sz; i++)
    {
        buffer1[i] = (rand() % 256);
    }
    auto futImg = std::async(std::launch::async, [buffer1]()
        {
            std::shared_ptr<Image > img = std::make_shared<Image>(3127, 9999, buffer1);
            return img;
        });


    uint8_t* buffer2 = (uint8_t*)malloc(data_sz * sizeof(uint8_t));
    for (int i = 0; i < data_sz; i++)
    {
        buffer2[i] = (rand() % 256);
    }
    Image img2(3127, 9999, buffer2);

    auto res = img2/futImg.get().operator*();
    std::cout << res.data[31266873];

//    _ASSERT(memcmp(buffer1, futImg.get().operator*().data, data_sz) == 0);
    std::free(buffer1);
    std::free(buffer2);
}

void testHash()
{

    const size_t data_sz = 31266873;//3127*9999
    uint8_t* buffer1 = (uint8_t*)malloc(data_sz * sizeof(uint8_t));
    for (int i = 0; i < data_sz; i++)
    {
        buffer1[i] = (rand() % 256);
    }
    Image img(3127, 9999, buffer1);
    img.Hash();
    std::cout << img.data[31266873];
}

int main(int argc, char* argv[])
{
std::async (testCreate2Async);
std::async  (testCreate2NotAsync);

std::async (testInitSizeMultipleTo32);
std::async (testInitSizeNotMultipleTo32);

std::async     (testSum);
std::async     (testSumOld);

std::async (testHash);

std::async (testSlowDivide);
std::async (testDivide);

    return 0;
}