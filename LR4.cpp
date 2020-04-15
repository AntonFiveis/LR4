// LR4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "BMPpicture.h"
#include <iostream>
using namespace std;
int main()
{
    BMPpicture test("test.bmp");
    test.copyBigger("test7.bmp",2);
}
