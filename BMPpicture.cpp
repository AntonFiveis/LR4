#include "BMPpicture.h"
#include <iostream>
#include <cstdio>
using namespace std;

BMPpicture::BMPheader BMPpicture::readHead(string iFileName) {
	BMPheader BMPHEAD;
	ifstream iFile(iFileName, ios::binary);
	iFile.read((char*)&BMPHEAD.id1, sizeof(uint8_t));
	iFile.read((char*)&BMPHEAD.id2, sizeof(uint8_t));
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
	iFile.close();
	return BMPHEAD;
}
void BMPpicture::copyTo(string ifilename,string ofilename) {
	ofstream oFile(ofilename, ios::binary);
	ifstream iFile(ifilename, ios::binary);
	char temp;
	iFile.seekg(0, ios::beg);
	while (!iFile.eof()) {
		iFile.read(&temp, sizeof(uint8_t));
		oFile.write(&temp,sizeof(uint8_t));
	}

}

void BMPpicture::BiggerWidth(string iFileName, string oFileName, int n) {
	BMPheader head = readHead(iFileName);
	//files
	ofstream oFile(oFileName, ios::binary);
	ifstream iFile(iFileName, ios::binary);

	//variable for pixels, oFile width and filesize, temporary byte for copying single bytes
	uint8_t pixel[3];
	int32_t newwidth = head.width *n;
	int32_t newsize = head.filesize * n - (n-1)*head.headersize;
	char MyByte;

	// copy header
	for (int i = 0; i < head.headersize; i++) {
		iFile.read(&MyByte, sizeof(uint8_t));
		oFile.write(&MyByte, sizeof(uint8_t));
	}
	MyByte = 0;

	//double width
	while (!iFile.eof()) {
		//copy row
		for (int j = 0; j < head.width; j++) {
			iFile.read((char*)pixel, 3 * sizeof(uint8_t));
			for (int i = 0; i < n; i++) {
				oFile.write((char*)pixel, 3 * sizeof(uint8_t));
			}
		}


		//skiping end zero bytes in iFile
		for (int i = 0; i < 4 - (head.width * 3) % 4; i++) {
			//4 - (head.width * 3) % 4 must be in range 1 - 3, not 4
			if ((head.width * 3) % 4 == 0)
				break;
			iFile.read(&MyByte, sizeof(uint8_t));
		}


		//writing end zero bytes in oFile
		for (int i = 0; i < 4 - (newwidth * 3) % 4; i++) {
			if ((newwidth * 3) % 4 == 0)
				break;
			oFile.write(&MyByte, sizeof(uint8_t));
		}
	}
	
	//rewriting oFile header
	oFile.seekp(18);
	oFile.write((char*)&newwidth, sizeof(int32_t));
	oFile.seekp(2);
	oFile.write((char*)&newsize, sizeof(int32_t));
	oFile.close();
	iFile.close();
}
void BMPpicture::BiggerHeight(string iFileName,string oFileName, int n) {
	
	//reading header
	BMPheader head = readHead(iFileName),
		tmpheader;
	
	//files
	ifstream iFile(iFileName, ios::binary);
	ofstream oFile(oFileName, ios::binary);


	//declaring important variables for final file
	uint8_t MyByte;
	tmpheader.height = n * (head.height);
	tmpheader.filesize = n * head.filesize - (n - 1) * head.headersize;
	int32_t widthInBytes;
	//byte width in row
	if ((head.width * 3) % 4 != 0) {
		widthInBytes =3* head.width  + 4 - (3*head.width ) % 4;
	}
	else {
		widthInBytes = 3*(head.width);
	}


	//copy header from iFile to oFile
	for (int i = 0; i < head.headersize; i++) {
		iFile.read((char*)&MyByte, sizeof(uint8_t));
		oFile.write((char*)&MyByte, sizeof(uint8_t));
	}

	
	//this array for copying one row of image(with ending 00)
	uint8_t *row1 = new uint8_t[widthInBytes];
	while (!iFile.eof()) {
		iFile.read((char*)row1, widthInBytes*sizeof(uint8_t));
		for (int i = 0; i < n; i++) {
			oFile.write((char*)row1, widthInBytes* sizeof(uint8_t));
		}
		
	}
	
	
	//changing filesize and height
	oFile.seekp(2, ios::beg);
	oFile.write((char*)&tmpheader.filesize, sizeof(int32_t));
	oFile.seekp(22, ios::beg);
	oFile.write((char*)&tmpheader.height, sizeof(int32_t));
	iFile.close();
	oFile.close();
}
void BMPpicture::SmallerHeight(string iFileName,string oFileName,int n) {
	
	//reading header
	BMPheader head = readHead(iFileName);
	int32_t oFileSize  = head.headersize;
	int32_t oFileHeight =0;

	//files
	ifstream iFile(iFileName, ios::binary);
	ofstream oFile(oFileName, ios::binary);
	
	//copy header
	char tmp;
	for (int i = 0; i < head.headersize; i++) {
		iFile.read(&tmp, sizeof(uint8_t));
		oFile.write(&tmp, sizeof(uint8_t));
	}

	//byte width in row
	int32_t widthInBytes;
	if ((head.width * 3) % 4 != 0) {
		widthInBytes = 3 * head.width + 4 - (3 * head.width) % 4;
	}
	else {
		widthInBytes = 3 * head.width;
	}

	//for final and temporary pixels
	uint8_t *temp = new uint8_t[2*widthInBytes];

	//main action
	for (int k = 0; k < head.height / n;k++) {
		
		for (int i = 0; i < 2*widthInBytes; i++) {
			temp[i] = 0;
		}
		for (int j = 0; j < n && !iFile.eof(); j++) {
			iFile.read((char*)(temp + widthInBytes), widthInBytes * sizeof(uint8_t));
			for (int i = 0; i < widthInBytes; i++) {

				temp[i] += ((uint8_t)temp[i + widthInBytes])/n;
			}

		}
		
		oFile.write((char*)temp, widthInBytes * sizeof(uint8_t));
		oFileSize += widthInBytes;
		oFileHeight++;
	}

	delete[] temp;

	//changing filesize and height in oFile header
	oFile.seekp(2, ios::beg);
	oFile.write((char*)&oFileSize, sizeof(int32_t));
	oFile.seekp(22, ios::beg);
	oFile.write((char*)&oFileHeight, sizeof(int32_t));
	iFile.close();
	oFile.close();
}
void BMPpicture::SmallerWidth(string iFileName, string oFileName, int n) {
	BMPheader head = readHead(iFileName),
		ohead;
	//files
	ifstream iFile(iFileName, ios::binary);
	ofstream oFile(oFileName, ios::binary);

	//copy header
	char tmp;
	for (int i = 0; i < head.headersize; i++) {
		iFile.read(&tmp, sizeof(uint8_t));
		oFile.write(&tmp, sizeof(uint8_t));
	}

	//byte width in row IwidthInBytes - bytes in row in Input file, OwidthInBytes - in output file
	int32_t IwidthInBytes;
	if ((head.width * 3) % 4 != 0) {
		IwidthInBytes = 3 * head.width + 4 - (3 * head.width) % 4;
	}
	else {
		IwidthInBytes = 3 * head.width;
	}

	uint32_t OwidthInBytes = head.width/n*3;
	int End0 = 0;
	if (OwidthInBytes % 4 != 0) {
		End0 = 4 - OwidthInBytes % 4;
	}
	ohead.width = head.width / n;
	ohead.filesize = head.filesize / n;

	//for two pixels: finaly pixel - first 3 bytes and for temporary pixel - 3 bytes
	uint8_t* temp = new uint8_t[3*2];
	for (int i = 0; i < head.height; i++) {
		//variable for saving count of readed bits
		int BytesReadedInRow = 0;

		//generic new pixels
		for (int j = 0; j < head.width / n; j++) {
			
			for (int k = 0; k < 3; k++) {
				temp[k] = 0;
			}
			for (int k = 0; k < n; k++) {
				iFile.read((char*)(temp + 3), 3 * sizeof(uint8_t));
				temp[0] += temp[3] / n;
				temp[1] += temp[4] / n;
				temp[2] += temp[5] / n;
				BytesReadedInRow += 3;
			}
			
			oFile.write((char*)temp, 3 * sizeof(uint8_t));
			
		}
		//reading end in row in input file
		for (int k = 0; k < IwidthInBytes - BytesReadedInRow; k++) {
			iFile.read((char*)(temp + 3), sizeof(uint8_t));
		}
		//writing 0 in end of  row in output file
		for (int k = 0; k < End0; k++) {
			int8_t a = 0;
			oFile.write((char*)&a, sizeof(uint8_t));
		}
	}

	delete[] temp;

	oFile.seekp(2, ios::beg);
	oFile.write((char*)&ohead.filesize, sizeof(int32_t));
	oFile.seekp(18, ios::beg);
	oFile.write((char*)&ohead.width, sizeof(int32_t));
	iFile.close();
	oFile.close();
}

//public functions copyBigger
void BMPpicture::copyBigger(string ifname,string ofname, int n) {
	cout << "Begin working with image...";
	//temp filename
	string tempfilename = "temporaryfile.bmp";
	
	//main action
	BiggerWidth(ifname,tempfilename, n);
	BiggerHeight(tempfilename, ofname, n);

	//removing temp file
	remove(tempfilename.c_str());
	cout << "Done!";
}
void BMPpicture::copyBigger(string ifname,string ofname, double n) {
	//temporary file names
	string tfn1="tnf1.bmp",
		tfn2="tnf2.bmp",
		tfn3="tnf3.bmp",
		tfn4="tnf4.bmp";


	//find double n like up/down 
	int up, down;
	for (int i = 2;; i++) {
		
		if (abs(round(n * i) - n * i) < 0.1) {
			up = round(n * i);
			down = i;
			cout << "Begin working with image...";
			break;
		}
		if (i > 40) {
			cout << "Invalid value" << endl;
			return;
		}
	}

	//raising image up times
	BiggerWidth(ifname, tfn1,up);
	BiggerHeight(tfn1,tfn2,up);


	//doing image smaller down times
	SmallerHeight(tfn2,tfn3,down);
	SmallerWidth(tfn3,ofname,down);


	//removing temporaryfiles
	remove(tfn1.c_str());
	remove(tfn2.c_str());
	remove(tfn3.c_str());
	remove(tfn4.c_str());
	cout << "Done!";
}