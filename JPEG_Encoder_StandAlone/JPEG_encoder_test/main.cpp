#include <iostream>
#include <stdlib.h>
#include "jpeg_encoder.h"

using namespace std;


#define width 640
#define height 480
#define elems 3

//polje mora biti slozeno: [visina=480][sirina=640][elems=3=RGB]
//unsigned char picture_data_test[height][width][elems]; //koristeno za isprobavanje
unsigned char picture_data[height][width][elems];

//const char *filename = "test_izl.jpg";

void init_bijela() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			picture_data[j][i][0] = picture_data[j][i][1] = picture_data[j][i][2] = unsigned char(255);
		}
	}
}

void init_crna() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			picture_data[j][i][0] = picture_data[j][i][1] = picture_data[j][i][2] = unsigned char(0);
		}
	}
}

void init_siva(unsigned char razina) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			picture_data[j][i][0] = picture_data[j][i][1] = picture_data[j][i][2] = razina;
		}
	}
}

void test_slika() {
	
	//cetvrtina gore lijevo
	for(int i=0; i<(width/2); i++){
		for (int j = 0; j < (height / 2); j++) {
			picture_data[j][i][0] = picture_data[j][i][1] = picture_data[j][i][2] = 255;
		}
	}

	//cetvrtina gore desno
	for (int i = (width / 2); i < width; i++) {
		for (int j = 0; j < (height / 2); j++) {
			picture_data[j][i][0] = 255;
			picture_data[j][i][1] = picture_data[j][i][2] = 0;
		}
	}

	//cetvrtina dolje lijevo
	for (int i = 0; i < (width / 2); i++) {
		for (int j = (height / 2); j < height; j++) {
			picture_data[j][i][0] = picture_data[j][i][1] = 0;
			picture_data[j][i][2] = 255;
		}
	}

	//cetvrtina dolje desno
	for (int i = (width/2); i < width; i++) {
		for (int j = (height / 2); j < height; j++) {
			picture_data[j][i][0] = picture_data[j][i][2] = 0;
			picture_data[j][i][1] =  255;
		}
	}
}

void init_boja(unsigned char R, unsigned char G, unsigned char B) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			picture_data[j][i][0] = R;
			picture_data[j][i][1] = G;
			picture_data[j][i][2] = B;
		}
	}
}

int main(void) {
	
	//popuni slikovne podatke za obradu 
	test_slika();
	//init_siva(128);
	//init_crna();
	//init_boja(128,0,128);

	//jo_write_jpg("foo.jpg", foo, 128, 128, 4, 90); // comp can be 1, 3, or 4. Lum, RGB, or RGBX respectively.

	if (jo_write_jpg("test_izl.jpg", picture_data, width, height, 3, 90)==true) {
		cout << "Encoding successful!" << endl;
		return 0;
	} else {
		cout << "Encoding failed!" << endl;
		return 1;
	}

}