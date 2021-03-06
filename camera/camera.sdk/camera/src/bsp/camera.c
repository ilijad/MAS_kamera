/*
 * camera.c
 *
 *  Created on: Jan 15, 2019
 *      Author: Ilija
 */

#include "iic.h"
#include "camera.h"
#include "sleep.h"
#include "xparameters.h"
#include "xiicps.h"
#include "xil_printf.h"
#include "xsdps.h"
#include <stdio.h>
#include "xil_cache.h"

u8 camera_read[3*640*480] __attribute__ ((aligned(32)));

int init_gpio()
{
	int status;
	status = XGpio_Initialize(&camera, CAMERA_DEVICE_ID);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	status = XGpio_Initialize(&led_btns, LED_BTNS_ID);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XGpio_SetDataDirection(&camera, 1, 0x7ff);
	XGpio_SetDataDirection(&led_btns, 2, 0x0);

	XGpio_DiscreteSet(&led_btns, 2, 0xff);

	return XST_SUCCESS;
}

int init_sd(int format)
{

	TCHAR *Path = "0:/";
	FRESULT status;
	status = f_mount(&fatfs, Path, 0);
	if (status != FR_OK) {
		return XST_FAILURE;
	}

	if(format)
	{
		status = f_mkfs(Path, 0, 0);

		if (status != FR_OK) {
			return XST_FAILURE;
		}
	}

	SD_File = (char *) file_name;

	return XST_SUCCESS;
}

int init_camera()
{
	int status;
	u8 id=0;

	status = I2C_read(0x1c, &id);
	if (status == XST_SUCCESS) {
		printf("%x ", id);
	} else {
		return XST_FAILURE;
	}
	usleep(100000);

	status = I2C_read(0x1d, &id);
	if (status == XST_SUCCESS) {
		printf("%x\n", id);
	} else {
		return XST_FAILURE;
	}
	usleep(100000);

	status = I2C_write(0x12, 0x80);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	usleep(100000);

	status = load_regs(ov7670_def_reg);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	usleep(100000);

	 status = load_regs(ov7670_fmt_rgb565);
	 if (status != XST_SUCCESS) {
		 return XST_FAILURE;
	 }
	 usleep(100000);

	//status = load_regs(ov7670_test_bar);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	usleep(100000);

	sleep(5);

	return XST_SUCCESS;
}

int load_regs(struct regval_list *regs)
{
	int status, i=0;

	while (1)
	{
		if(regs[i].reg_num == 0xff && regs[i].value == 0xff)
			break;
		status = I2C_write(regs[i].reg_num, regs[i].value);
		if (status != XST_SUCCESS)
			return status;
		i++;
		usleep(1000);
	}

	return status;
}

int take_photo()
{
	u32 InputData;
	UINT bytes_read;
	int i = 0, j = 0, line = 0, status;

	status = f_open(&file, SD_File, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if (status) {
		return XST_FAILURE;
	}

	status = f_lseek(&file, 0);
	if (status) {
		return XST_FAILURE;
	}

	while (!(XGpio_DiscreteRead(&camera, 1) & 0x400)) {}
	while ((XGpio_DiscreteRead(&camera, 1) & 0x400)) {}

	while (1) {
		InputData = XGpio_DiscreteRead(&camera, 1);
		if ((InputData & 0x400)) break;
		if (line && !(InputData & 0x100)){
			line=0;
		}
		if (line && (InputData & 0x100)) continue;
		if ((InputData & 0x200) && (InputData & 0x100)){

			if (i % 2 == 0) {
				camera_read[j] = (InputData & 0xf8) ;
				camera_read[j+1] = (InputData & 0x07) << 5;
			} else {
				camera_read[j] =  (camera_read[j] | ((InputData & 0xe0) >> 3));
				camera_read[j+1] =((InputData & 0x1f) << 3);
				j++;
			}
			i++;
			j++;
			line=1;
		}
	}

	f_write(&file, (const void *) camera_read, j, &bytes_read);


	status = f_close(&file);
	if (status) {
		return XST_FAILURE;
	}
	if(j!=640*3*480){
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int take_photo_(u8 * came)
{
	u32 InputData;
	UINT bytes_read;
	int i = 0, j = 0, line = 0, status;


	while (!(XGpio_DiscreteRead(&camera, 1) & 0x400)) {}
	while ((XGpio_DiscreteRead(&camera, 1) & 0x400)) {}

	while (1) {
		InputData = XGpio_DiscreteRead(&camera, 1);
		if ((InputData & 0x400)) break;
		if (line && !(InputData & 0x100)){
			line=0;
		}
		if (line && (InputData & 0x100)) continue;
		if ((InputData & 0x200) && (InputData & 0x100)){

			if (i % 2 == 0) {
				came[j] = (InputData & 0xf8) ;
				came[j+1] = (InputData & 0x07) << 5;
			} else {
				came[j] =  (camera_read[j] | ((InputData & 0xe0) >> 3));
				came[j+1] =((InputData & 0x1f) << 3);
				j++;
			}
			i++;
			j++;
			line=1;
		}
	}
	if(j!=640*3*480){
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

