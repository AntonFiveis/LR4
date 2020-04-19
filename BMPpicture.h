#pragma once
#include <fstream>
#include <string>
using std::ifstream;
using std::string;
class BMPpicture
{
private:
    struct BMPheader {
        int8_t id1;            // ������ �� ����� 'B' � 'M'
        int8_t id2;            // ������ �� ����� 'B' � 'M'
        int32_t filesize;        // ����� ����� � ������
        int16_t reserved1;     // 0, 0
        int16_t reserved2;     // 0, 0
        int32_t headersize;      // 54L ��� 24-����� ���������
        int32_t infoSize;        // 40L ��� 24-����� ���������
        int32_t width;           // ������ ���������� � �������
        int32_t height;           // ������ ���������� � �������
        int16_t biPlanes;        // 1 (��� 24-����� ���������)
        int16_t bits;            // 24 (��� 24-����� ���������)
        int32_t biCompression;   // 0L
        int32_t biSizeImage;     // ����� ��������� � 0L ��� ��������� ��� �������� (��� ������)
        int32_t biXPelsPerMeter; // ������������� ������� ������ �� ����, ����� 0L
        int32_t biYPelsPerMeter; // �� ����, �� �����
        int32_t biClrUsed;       // ��� ������������ ���������, ����� ��������� 0L
        int32_t biClrImportant;  // �� ����
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