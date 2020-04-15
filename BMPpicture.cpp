#include "BMPpicture.h"
#include <iostream>
#include <cstdio>
using namespace std;
BMPpicture::BMPpicture(string ifilename) {
	iFile.open(ifilename, ios::in | ios::binary);

	readHead();
	outputHead();

}
void BMPpicture::readHead() {

	iFile.read((char*)&BMPHEAD.id1, sizeof(int8_t));
	iFile.read((char*)&BMPHEAD.id2, sizeof(int8_t));
	iFile.read((char*)&BMPHEAD.filesize, sizeof(int32_t));
	iFile.read((char*)&BMPHEAD.reserved1, sizeof(int16_t));
	iFile.read((char*)&BMPHEAD.reserved2, sizeof(int16_t));
	iFile.read((char*)&BMPHEAD.headersize, sizeof(int32_t));
	iFile.read((char*)&BMPHEAD.infoSize, sizeof(int32_t));
	iFile.read((char*)&BMPHEAD.width, sizeof(int32_t));
	iFile.read((char*)&BMPHEAD.height, sizeof(int32_t));
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
	cout << BMPHEAD.height << endl;
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
void BMPpicture::BiggerWidth(int n) {
	ofstream oFile(tempfilename, ios::binary);
	int8_t pixel[3];
	int32_t newwidth = BMPHEAD.width *n;
	int32_t newsize = BMPHEAD.filesize * n - (n-1)*BMPHEAD.headersize;
	char MyByte;
	iFile.seekg(0, ios::beg);
	for (int i = 0; i < BMPHEAD.headersize; i++) {
		iFile.read(&MyByte, sizeof(int8_t));
		oFile.write(&MyByte, sizeof(int8_t));
	}
	MyByte = 0;
	while (!iFile.eof()) {
	
		for (int j = 0; j < BMPHEAD.width; j++) {
			iFile.read((char*)pixel, 3 * sizeof(int8_t));

			for (int i = 0; i < n; i++) {
				oFile.write((char*)pixel, 3 * sizeof(int8_t));
			}
		}
		for (int i = 0; i < 4 - (BMPHEAD.width * 3) % 4; i++) {
			if ((BMPHEAD.width * 3) % 4 == 0)
				break;
			iFile.read(&MyByte, sizeof(int8_t));
		}
		for (int i = 0; i < 4 - (newwidth * 3) % 4; i++) {
			if ((newwidth * 3) % 4 == 0)
				break;
			oFile.write(&MyByte, sizeof(int8_t));
		}
	}
	
	oFile.seekp(18);
	
	oFile.write((char*)&newwidth, sizeof(int32_t));
	oFile.seekp(2);
	oFile.write((char*)&newsize, sizeof(int32_t));
	oFile.close();
}
void BMPpicture::BiggerHeight( int n) {
	ifstream tmpfile(tempfilename, ios::binary);
	ofstream oFile(oFileName, ios::binary);
	BMPheader tmpheader;
	
	
	//reading filesize, width and height
	tmpfile.seekg(2, ios::beg);
	tmpfile.read((char*)&tmpheader.filesize, sizeof(int32_t));
	tmpfile.seekg(18, ios::beg);
	tmpfile.read((char*)&tmpheader.width, sizeof(int32_t));
	tmpfile.read((char*)&tmpheader.height, sizeof(int32_t));
	tmpfile.seekg(0, ios::beg);
	//declaring important variables for final file
	int8_t head;
	tmpheader.height = n * (tmpheader.height);
	tmpheader.filesize = n * tmpheader.filesize - (n - 1) * BMPHEAD.headersize;
	
	//byte width of row
	if ((tmpheader.width * 3) % 4 != 0) {
		tmpheader.width =3* tmpheader.width  + 4 - (3*tmpheader.width ) % 4;
	}
	else {
		tmpheader.width = 3*int(tmpheader.width);
	}
	//copy header from tmpfile
	for (int i = 0; i < BMPHEAD.headersize; i++) {
		tmpfile.read((char*)&head, sizeof(int8_t));
		oFile.write((char*)&head, sizeof(int8_t));
	}

	
	//this array for copying one row of image(with ending 00)
	
	int8_t *row1 = new int8_t[tmpheader.width];
	while (!tmpfile.eof()) {
		tmpfile.read((char*)row1, tmpheader.width*sizeof(int8_t));
		for (int i = 0; i < n; i++) {
			oFile.write((char*)row1, tmpheader.width* sizeof(int8_t));
		}
		
	}
	
	
	//changing filesize and height
	oFile.seekp(2, ios::beg);
	oFile.write((char*)&tmpheader.filesize, sizeof(int32_t));
	oFile.seekp(22, ios::beg);
	oFile.write((char*)&tmpheader.height, sizeof(int32_t));
	tmpfile.close();
	oFile.close();
}
void BMPpicture::copyBigger(string ofname, int n) {
	oFileName = ofname;
	tempfilename = "temporaryfile.bmp";
	BiggerWidth(n);

	BiggerHeight(n);
	remove(tempfilename.c_str());
}