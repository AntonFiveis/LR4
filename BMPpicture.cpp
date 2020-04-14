#include "BMPpicture.h"
#include <iostream>
using namespace std;
void BMPpicture::readHead() {

	iFile.read((char*)&BMPHEAD.id1, sizeof(int8_t));
	iFile.read((char*)&BMPHEAD.id2, sizeof(int8_t));
	iFile.read((char*)&BMPHEAD.filesize, sizeof(int32_t));
	iFile.read((char*)&BMPHEAD.reserved1, sizeof(int16_t));
	iFile.read((char*)&BMPHEAD.reserved2, sizeof(int16_t));
	iFile.read((char*)&BMPHEAD.headersize, sizeof(int32_t));
	iFile.read((char*)&BMPHEAD.infoSize, sizeof(int32_t));
	iFile.read((char*)&BMPHEAD.width, sizeof(int32_t));
	iFile.read((char*)&BMPHEAD.depth, sizeof(int32_t));
	iFile.read((char*)&BMPHEAD.biPlanes, sizeof(int16_t));
	iFile.read((char*)&BMPHEAD.bits, sizeof(int16_t));
	iFile.read((char*)&BMPHEAD.biCompression, sizeof(int32_t));
	iFile.read((char*)&BMPHEAD.biSizeImage, sizeof(int32_t));
	iFile.read((char*)&BMPHEAD.biXPelsPerMeter, sizeof(int32_t));
	iFile.read((char*)&BMPHEAD.biYPelsPerMeter, sizeof(int32_t));
	iFile.read((char*)&BMPHEAD.biClrUsed, sizeof(int32_t));
	iFile.read((char*)&BMPHEAD.biClrImportant, sizeof(int32_t));
	
}
void BMPpicture::outputHead() {
	cout << BMPHEAD.id1 << endl;
	cout << BMPHEAD.width << endl;
	cout << BMPHEAD.depth << endl;
}
void BMPpicture::copyTo(string ofilename) {
	ofstream oFile(ofilename, ios::binary);
	char temp;
	iFile.seekg(0, ios::beg);
	while (!iFile.eof()) {
		iFile.read(&temp, sizeof(int8_t));
		oFile.write(&temp,sizeof(int8_t));
	}

}
BMPpicture::BMPpicture(string ifilename) {
	iFile.open(ifilename, ios::binary);
	readHead();
	outputHead();
	copyTo("test1.bmp");
}