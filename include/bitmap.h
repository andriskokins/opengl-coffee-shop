#ifndef BITMAP_H
#define BITMAP_H

#include <stdio.h>
#include <windows.h>
#include <wingdi.h>


GLuint savebitmap(const char* filename, 
	unsigned char* pixelBuffer, 
	BITMAPINFOHEADER* infoHeader, BITMAPFILEHEADER* fileHeader)
{
	FILE* bitmapFile;

	errno_t err = fopen_s(&bitmapFile, filename, "wb");
	if (err != 0 || bitmapFile == NULL)
	{
		printf("savebitmap - open failed for %s\n", filename);
		return NULL;
	}

	fwrite(fileHeader, sizeof(BITMAPFILEHEADER), 1, bitmapFile);

	fwrite(infoHeader, sizeof(BITMAPINFOHEADER), 1, bitmapFile);

	fseek(bitmapFile, fileHeader->bfOffBits, SEEK_SET);

	int nBytes = infoHeader->biWidth * infoHeader->biHeight * 3;
	fwrite(pixelBuffer, sizeof(unsigned char), nBytes, bitmapFile);

	fclose(bitmapFile);

	printf("savebitmap - saved %s w=%d h=%d bits=%d\n", filename, infoHeader->biWidth, infoHeader->biHeight, infoHeader->biBitCount);
}

#endif //BITMAP_H
