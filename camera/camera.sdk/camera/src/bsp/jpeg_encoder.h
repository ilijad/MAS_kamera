/*
 * jpeg_encoder.h
 *
 *  Created on: Jan 15, 2019
 *      Author: Ilija
 */

#ifndef SRC_JPEG_ENCODER_H_
#define SRC_JPEG_ENCODER_H_

#include "ff.h"

static FIL fil;
static char *SD_Pic;

static char PicName[32] = "photo.yuv";

int TakeJPG();
int TakeJPG_(u8 *came);

#endif /* SRC_JPEG_ENCODER_H_ */
