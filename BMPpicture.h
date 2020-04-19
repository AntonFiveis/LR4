#pragma once
#include <fstream>
#include <string>
using std::ifstream;
using std::string;
class BMPpicture
{
private:
    struct BMPheader {
        int8_t id1;            // Завжди дві літери 'B' і 'M'
        int8_t id2;            // Завжди дві літери 'B' і 'M'
        int32_t filesize;        // Розмір файла в байтах
        int16_t reserved1;     // 0, 0
        int16_t reserved2;     // 0, 0
        int32_t headersize;      // 54L для 24-бітних зображень
        int32_t infoSize;        // 40L для 24-бітних зображень
        int32_t width;           // ширина зображення в пікселях
        int32_t height;           // висота зображення в пікселях
        int16_t biPlanes;        // 1 (для 24-бітних зображень)
        int16_t bits;            // 24 (для 24-бітних зображень)
        int32_t biCompression;   // 0L
        int32_t biSizeImage;     // Можна поставити в 0L для зображень без компрессії (наш варіант)
        int32_t biXPelsPerMeter; // Рекомендована кількість пікселів на метр, можна 0L
        int32_t biYPelsPerMeter; // Те саме, по висоті
        int32_t biClrUsed;       // Для індексованих зображень, можна поставити 0L
        int32_t biClrImportant;  // Те саме
    };
    struct pixelData {
        int8_t redComponent;
        int8_t greenComponent;
        int8_t blueComponent;
    };
    void BiggerWidth(string, string, int);
    void BiggerHeight(string, string, int);
    void SmallerWidth(string, string, int);
    void SmallerHeight(string, string, int);
    BMPheader readHead(string);
public:
    void copyTo(string, string);
    void copyBigger(string, string, int);
    void copyBigger(string, string, double);
};