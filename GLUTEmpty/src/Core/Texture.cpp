
#include "Core/Texture.h"
#include <iostream>
#include <glut.h>

namespace Core
{
#define VERBOSE		false
#define BMP_MAGIC_NUMBER	0x4d42
#ifndef BI_RGB
#define BI_RGB			0
#define BI_RLE8			1
#define BI_RLE4			2
#endif
	struct bmfh
	{
		short bfType;		// BMP_MAGIC_NUMBER = "BM"
		int bfSize;			// size of this file in bytes
		short bfReserved1;
		short bfReserved2;
		int bfOffBytes;		// # bytes to get to the start of the per-pixel data
	} FileHeader;
	struct bmih
	{
		int biSize;		// info header size, should be 40
		int biWidth;		// image width
		int biHeight;		// image height
		short biPlanes;		// #color planes, should be 1
		short biBitCount;	// #bits/pixel, should be 1, 4, 8, 16, 24, 32
		int biCompression;	// BI_RGB, BI_RLE4, BI_RLE8
		int biSizeImage;
		int biXPixelsPerMeter;
		int biYPixelsPerMeter;
		int biClrUsed;		// # colors in the palette
		int biClrImportant;
	} InfoHeader;


	static int ReadInt(FILE* fp)
	{
		const unsigned char b0 = fgetc(fp);
		const unsigned char b1 = fgetc(fp);
		const unsigned char b2 = fgetc(fp);
		const unsigned char b3 = fgetc(fp);
		return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
	}
	static short ReadShort(FILE* fp)
	{
		const unsigned char b0 = fgetc(fp);
		const unsigned char b1 = fgetc(fp);
		return (b1 << 8) | b0;
	}
	static unsigned char* BmpToTexture(const char* filename, int* width, int* height)
	{
		FILE* fp;
#ifdef _WIN32
		errno_t err = fopen_s(&fp, filename, "rb");
		if (err != 0)
		{
			fprintf(stderr, "Cannot open Bmp file '%s'\n", filename);
			return NULL;
		}
#else
		FILE* fp = fopen(filename, "rb");
		if (fp == NULL)
		{
			fprintf(stderr, "Cannot open Bmp file '%s'\n", filename);
			return NULL;
		}
#endif

		FileHeader.bfType = ReadShort(fp);

		// if bfType is not BMP_MAGIC_NUMBER, the file is not a bmp:

		if (VERBOSE) fprintf(stderr, "FileHeader.bfType = 0x%0x = \"%c%c\"\n",
			FileHeader.bfType, FileHeader.bfType & 0xff, (FileHeader.bfType >> 8) & 0xff);
		if (FileHeader.bfType != BMP_MAGIC_NUMBER)
		{
			fprintf(stderr, "Wrong type of file: 0x%0x\n", FileHeader.bfType);
			fclose(fp);
			return NULL;
		}

		FileHeader.bfSize = ReadInt(fp);
		if (VERBOSE)	fprintf(stderr, "FileHeader.bfSize = %d\n", FileHeader.bfSize);

		FileHeader.bfReserved1 = ReadShort(fp);
		FileHeader.bfReserved2 = ReadShort(fp);

		FileHeader.bfOffBytes = ReadInt(fp);
		if (VERBOSE)	fprintf(stderr, "FileHeader.bfOffBytes = %d\n", FileHeader.bfOffBytes);

		InfoHeader.biSize = ReadInt(fp);
		if (VERBOSE)	fprintf(stderr, "InfoHeader.biSize = %d\n", InfoHeader.biSize);
		InfoHeader.biWidth = ReadInt(fp);
		if (VERBOSE)	fprintf(stderr, "InfoHeader.biWidth = %d\n", InfoHeader.biWidth);
		InfoHeader.biHeight = ReadInt(fp);
		if (VERBOSE)	fprintf(stderr, "InfoHeader.biHeight = %d\n", InfoHeader.biHeight);

		const int nums = InfoHeader.biWidth;
		const int numt = InfoHeader.biHeight;

		InfoHeader.biPlanes = ReadShort(fp);
		if (VERBOSE)	fprintf(stderr, "InfoHeader.biPlanes = %d\n", InfoHeader.biPlanes);

		InfoHeader.biBitCount = ReadShort(fp);
		if (VERBOSE)	fprintf(stderr, "InfoHeader.biBitCount = %d\n", InfoHeader.biBitCount);

		InfoHeader.biCompression = ReadInt(fp);
		if (VERBOSE)	fprintf(stderr, "InfoHeader.biCompression = %d\n", InfoHeader.biCompression);

		InfoHeader.biSizeImage = ReadInt(fp);
		if (VERBOSE)	fprintf(stderr, "InfoHeader.biSizeImage = %d\n", InfoHeader.biSizeImage);

		InfoHeader.biXPixelsPerMeter = ReadInt(fp);
		InfoHeader.biYPixelsPerMeter = ReadInt(fp);

		InfoHeader.biClrUsed = ReadInt(fp);
		if (VERBOSE)	fprintf(stderr, "InfoHeader.biClrUsed = %d\n", InfoHeader.biClrUsed);

		InfoHeader.biClrImportant = ReadInt(fp);

		// fprintf( stderr, "Image size found: %d x %d\n", ImageWidth, ImageHeight );

		// pixels will be stored bottom-to-top, left-to-right:
		unsigned char* texture = new unsigned char[3 * nums * numt];
		if (texture == NULL)
		{
			fprintf(stderr, "Cannot allocate the texture array!\n");
			return NULL;
		}

		// extra padding bytes:

		int requiredRowSizeInBytes = 4 * ((InfoHeader.biBitCount * InfoHeader.biWidth + 31) / 32);
		if (VERBOSE)	fprintf(stderr, "requiredRowSizeInBytes = %d\n", requiredRowSizeInBytes);

		int myRowSizeInBytes = (InfoHeader.biBitCount * InfoHeader.biWidth + 7) / 8;
		if (VERBOSE)	fprintf(stderr, "myRowSizeInBytes = %d\n", myRowSizeInBytes);

		int oldNumExtra = 4 * (((3 * InfoHeader.biWidth) + 3) / 4) - 3 * InfoHeader.biWidth;
		if (VERBOSE)	fprintf(stderr, "Old NumExtra padding = %d\n", oldNumExtra);

		int numExtra = requiredRowSizeInBytes - myRowSizeInBytes;
		if (VERBOSE)	fprintf(stderr, "New NumExtra padding = %d\n", numExtra);

		// this function does not support compression:

		if (InfoHeader.biCompression != 0)
		{
			fprintf(stderr, "Wrong type of image compression: %d\n", InfoHeader.biCompression);
			fclose(fp);
			return NULL;
		}

		// we can handle 24 bits of direct color:
		if (InfoHeader.biBitCount == 24)
		{
			rewind(fp);
			fseek(fp, FileHeader.bfOffBytes, SEEK_SET);
			int t;
			unsigned char* tp;
			for (t = 0, tp = texture; t < numt; t++)
			{
				for (int s = 0; s < nums; s++, tp += 3)
				{
					*(tp + 2) = fgetc(fp);		// b
					*(tp + 1) = fgetc(fp);		// g
					*(tp + 0) = fgetc(fp);		// r
				}

				for (int e = 0; e < numExtra; e++)
				{
					fgetc(fp);
				}
			}
		}

		// we can also handle 8 bits of indirect color:
		if (InfoHeader.biBitCount == 8 && InfoHeader.biClrUsed == 256)
		{
			struct rgba32
			{
				unsigned char r, g, b, a;
			};
			struct rgba32* colorTable = new struct rgba32[InfoHeader.biClrUsed];

			rewind(fp);
			fseek(fp, sizeof(struct bmfh) + InfoHeader.biSize - 2, SEEK_SET);
			for (int c = 0; c < InfoHeader.biClrUsed; c++)
			{
				colorTable[c].r = fgetc(fp);
				colorTable[c].g = fgetc(fp);
				colorTable[c].b = fgetc(fp);
				colorTable[c].a = fgetc(fp);
				if (VERBOSE)	fprintf(stderr, "%4d:\t0x%02x\t0x%02x\t0x%02x\t0x%02x\n",
					c, colorTable[c].r, colorTable[c].g, colorTable[c].b, colorTable[c].a);
			}

			rewind(fp);
			fseek(fp, FileHeader.bfOffBytes, SEEK_SET);
			int t;
			unsigned char* tp;
			for (t = 0, tp = texture; t < numt; t++)
			{
				for (int s = 0; s < nums; s++, tp += 3)
				{
					int index = fgetc(fp);
					*(tp + 0) = colorTable[index].r;	// r
					*(tp + 1) = colorTable[index].g;	// g
					*(tp + 2) = colorTable[index].b;	// b
				}

				for (int e = 0; e < numExtra; e++)
				{
					fgetc(fp);
				}
			}

			delete[] colorTable;
		}

		fclose(fp);

		*width = nums;
		*height = numt;
		return texture;
	}


	Texture::Texture(const char* filePath)
	{
		int width, height;
		unsigned char* texture = BmpToTexture(filePath, &width, &height);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	}
}