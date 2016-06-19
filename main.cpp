#include "bmp2/EasyBMP.h"

int main() {
	BMP out;
	out.SetSize(1000, 1000);
	out.SetBitDepth(32);
	
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 1000; j++) {
			out(i, j)->Red = i * 255.0 / 1000;
			out(i, j)->Green = j * 255.0 / 1000;
			out(i, j)->Blue = j * 255.0 / 1000;
			out(i, j)->Alpha = 255;
		}
	}

	out.WriteToFile("hw5.bmp");
}
