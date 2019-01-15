/*
 * iic.c
 *
 *  Created on: Jan 15, 2019
 *      Author: Ilija
 */

#include "iic.h"
#include "xiicps.h"

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
