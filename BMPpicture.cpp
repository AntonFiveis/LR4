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
	cout << BMPHEAD.filesize << endl;
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
void BMPpicture::copyBigger(string ofilename,int n) {
	ofstream oFile(ofilename, ios::binary);
	int8_t temp[3];
	int32_t newwidth = (BMPHEAD.width << 1);
	int32_t newsize = BMPHEAD.filesize * 2 - BMPHEAD.headersize;
	char temp2;
	iFile.seekg(0, ios::beg);
	for (int i = 0; i < BMPHEAD.headersize; i++) {
		iFile.read(&temp2, sizeof(int8_t));
		oFile.write(&temp2, sizeof(int8_t));
	}
	cout << (BMPHEAD.width / 4)*4 << ' ' << BMPHEAD.width % 4;
	temp2 = 0;
	while (!iFile.eof()) {
		for (int j = 0; j < BMPHEAD.width; j++) {
			iFile.read((char*)temp, 3 * sizeof(int8_t));

			for (int i = 0; i < n; i++) {
				oFile.write((char*)temp, 3 * sizeof(int8_t));
			}
		}
		for (int i = 0; i < (BMPHEAD.width * 3) % 4; i++) {
			iFile.read(&temp2, sizeof(int8_t));
		}
		for (int i = 0; i < (newwidth * 3) % 4; i++) {
			oFile.write(&temp2, sizeof(int8_t));
		}
	}
	
	oFile.seekp(18);
	
	oFile.write((char*)&newwidth, sizeof(int32_t));
	oFile.seekp(2);
	oFile.write((char*)&newsize, sizeof(int32_t));
}
BMPpicture::BMPpicture(string ifilename) {
	iFile.open(ifilename, ios::binary);
	
	readHead();
	outputHead();

}