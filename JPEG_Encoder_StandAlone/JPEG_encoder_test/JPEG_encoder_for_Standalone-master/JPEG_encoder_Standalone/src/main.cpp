/*
 *  Created on: 18 de may. de 2017
 *      Author: Raul
 *
 * Project that contains JPEG encoder for an embedded system whitout OS ( Standalone )
 * This project is for Xilinx FPGA's
 */

#include "jpeg_encoder.h"
#include <iostream>

using namespace std;

int main(){

	cout<< "Taking picture\n\r";
	TakePicture();
	cout<<"Picture taken \n\r";

}

