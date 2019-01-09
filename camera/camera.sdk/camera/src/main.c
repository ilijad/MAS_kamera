#include "xparameters.h"
#include "xiicps.h"
#include "xil_printf.h"
#include <stdio.h>
#include "sleep.h"
#include "xgpio.h"
#include "camera_reg.h"
#include "xsdps.h"
#include "ff.h"
#include "xil_cache.h"

#define IIC_ADDRESS            0x21

#define CAMERA_DEVICE_ID  	XPAR_GPIO_1_DEVICE_ID
#define LED_BTNS_ID			XPAR_GPIO_0_DEVICE_ID


XIicPs i2c;

int I2C_write(u8 register_offset, u8 write_value);
int I2C_read(u8 register_offset, u8 *read_value);
int load_regs(struct regval_list *regs);

static FIL fil;
static FATFS fatfs;

u8 camera_read[5120*480] __attribute__ ((aligned(32)));

static char FileName[32] = "photo.yuv";
static char *SD_File;
u32 Platform;

XGpio camera;
XGpio led_btns;


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

int init_i2c()
{
	int status;
	XIicPs_Config *config;

	config = XIicPs_LookupConfig(XPAR_XIICPS_0_DEVICE_ID);
	if (NULL == config) {
		return XST_FAILURE;
	}

	status = XIicPs_CfgInitialize(&i2c, config, config->BaseAddress);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	status = XIicPs_SelfTest(&i2c);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XIicPs_SetSClk(&i2c, 100000);

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

	SD_File = (char *) FileName;


	status = f_open(&fil, SD_File, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
	if (status) {
		return XST_FAILURE;
	}

	status = f_lseek(&fil, 0);
	if (status) {
		return XST_FAILURE;
	}

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

	status = load_regs(a);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	usleep(100000);

	 status = load_regs(ov7670_test_bar);
	 if (status != XST_SUCCESS) {
		 return XST_FAILURE;
	 }
	 usleep(100000);

	return XST_SUCCESS;
}

void take_photo()
{
	u32 InputData;
	UINT bytes_read;
	int i = 0;
	while (!(XGpio_DiscreteRead(&camera, 1) & 0x400)) {}
	while ((XGpio_DiscreteRead(&camera, 1) & 0x400)) {}

	while (1) {
		while (!((InputData = XGpio_DiscreteRead(&camera, 1)) & 0x600)) {}
		if ((InputData & 0x400)) break;

		while (1) {
			InputData = XGpio_DiscreteRead(&camera, 1);
			if (!(InputData & 0x200))
				break;
			if (!(InputData & 0x100))
				continue;
			camera_read[i] = InputData & 0xff;
			i++;

			while ((XGpio_DiscreteRead(&camera, 1) & 0x100)) {
			}
		}
	}
	f_write(&fil, (const void *) camera_read, i, &bytes_read);
	if(i != 2*640*480) printf("Failed!\n");
	else printf("Image taken!\n");

	return;
}

int main()
{
	int status;

	status = init_gpio();
	if (status != XST_SUCCESS) {
		printf("GPIO initialization failed.\n");
		return 0;
	}
	usleep(10000);

	status = init_i2c();
	if (status != XST_SUCCESS) {
		printf("I2C initialization failed.\n");
		return 0;
	}
	usleep(10000);

	status = init_sd(0);
	if (status != XST_SUCCESS) {
		printf("SD card initialization failed.\n");
		return 0;
	}
	usleep(10000);

	status = init_camera();
	if (status != XST_SUCCESS) {
		printf("Camera card initialization failed.\n");
		return 0;
	}
	usleep(10000);

	take_photo();

	status = f_close(&fil);
	if (status) {
		return XST_FAILURE;
	}

	printf("Complete\n\r");

	XGpio_DiscreteSet(&led_btns, 2, 0x00);

	return 0;
}

int I2C_write(u8 register_offset, u8 write_value)

{
	int Status = XST_SUCCESS;
	u8 TxBuffer[2];

	TxBuffer[0] = register_offset;
	TxBuffer[1] = write_value;

	Status = XIicPs_MasterSendPolled(&i2c, TxBuffer, 2, IIC_ADDRESS);
	if (Status != XST_SUCCESS) return XST_FAILURE;

	while (XIicPs_BusIsBusy(&i2c)){}

	return(Status);
}

int I2C_read(u8 register_offset, u8 *read_value)
{
	int Status = XST_SUCCESS;
	u8 TxBuffer[1];
	u8 RxBuffer[1];

	TxBuffer[0] = register_offset;

	Status = XIicPs_MasterSendPolled(&i2c, TxBuffer, 1, IIC_ADDRESS);
	if (Status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	while (XIicPs_BusIsBusy(&i2c)) {}


	Status = XIicPs_MasterRecvPolled(&i2c, RxBuffer, 1, IIC_ADDRESS);
	if (Status != XST_SUCCESS)
		return XST_FAILURE;

	if(Status==XST_SUCCESS)
		*read_value = RxBuffer[0];

	while (XIicPs_BusIsBusy(&i2c)){}
	return(Status);
}

int load_regs(struct regval_list *regs) {
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
