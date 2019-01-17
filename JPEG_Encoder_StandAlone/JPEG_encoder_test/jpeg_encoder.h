
#ifndef SRC_JPEG_ENCODER_H_
#define SRC_JPEG_ENCODER_H_

/*
 * Functions
 */

extern bool jo_write_jpg(const char *filename, const void *data, int width, int height, int comp, int quality);

#endif /* SRC_JPEG_ENCODER_H_ */
