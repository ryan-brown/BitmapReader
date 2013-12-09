#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define PIXEL_START_INDEX 10
#define BITMAP_WIDTH_INDEX 18
#define BITMAP_HEIGHT_INDEX 22

typedef struct {
	int width;
	int height;
	int fileSize;
	unsigned char*** pixels;
}	BitmapImage;

BitmapImage readBmp(char* file) {
	BitmapImage image;

  int fd;

  fd = open(file, O_RDONLY);

  int fileSize = (int)lseek(fd, 0L, SEEK_END);
  lseek(fd, 0, SEEK_SET);

	unsigned char buff[fileSize];
	ssize_t bytes_read = read(fd, buff, fileSize);

  int pixelStart = (int)buff[PIXEL_START_INDEX];
  int bmpWidth = (int)buff[BITMAP_WIDTH_INDEX];
  int bmpHeight = (int)buff[BITMAP_HEIGHT_INDEX];

  image.fileSize = fileSize;
  image.width = bmpWidth;
  image.height = bmpHeight;

	unsigned char ***img = malloc(bmpWidth * sizeof(unsigned char **));

	int i, j;
	for(i = 0; i < bmpWidth; i++) {
		img[i] = malloc(bmpHeight * sizeof(unsigned char *));

		for(j = 0; j < bmpHeight; j++) {
			img[i][j] = calloc(3, sizeof(unsigned char));
		}
	}

  for(i = pixelStart; i < bytes_read; i+=3) {
  	int column = ((i-pixelStart)/3) % bmpWidth;
  	int row = (i-pixelStart)/(3*bmpWidth);
  	img[column][row][0] = buff[i+2];
  	img[column][row][1] = buff[i+1];
  	img[column][row][2] = buff[i];
  }

  image.pixels = img;

  return image;
}

int main(int argc, char const *argv[]) {
	BitmapImage bmp = readBmp("img3.bmp");

	printf("Bitmap Image Info\nWidth: %dpx\nHeight %dpx\nSize: %d bytes\n\n", bmp.width, bmp.height, bmp.fileSize);

	printf("Bitmap Pixels\n");
	int i, j;
	for(j = 0; j < bmp.height; j++) {
		for(i = 0; i < bmp.width; i++) {
			printf("(%d, %d): %d, %d, %d\n", i, j, (int)bmp.pixels[i][j][0], (int)bmp.pixels[i][j][1], (int)bmp.pixels[i][j][2]);
		}
	}
	
	return 0;
}
