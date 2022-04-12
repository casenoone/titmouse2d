//#include <iostream>
//#include <fstream>
//#include <sstream>
//
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb/stb_image.h>
//
//#define STBI_MSC_SECURE_CRT
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include <stb/stb_image_write.h>
//
//int main()
//{
//	const int width = 400;
//	const int height = 400;
//#define CHANNEL_NUM 4
//
//	/*** NOTICE!! You have to use uint8_t array to pass in stb function  ***/
//	// Because the size of color is normally 255, 8bit.
//	// If you don't use this one, you will get a weird imge.
//	uint8_t* pixels = new uint8_t[width * height * CHANNEL_NUM];
//
//	int index = 0;
//	for (int j = height - 1; j >= 0; --j)
//	{
//		for (int i = 0; i < width; ++i)
//		{
//			float r = (float)i / (float)width;
//			float g = (float)j / (float)height;
//			float b = 0.2f;
//			int ir = int(255.99 * r);
//			int ig = int(255.99 * g);
//			int ib = int(255.99 * b);
//			int ia = int(255.99 * 1);
//
//			pixels[index++] = ir;
//			pixels[index++] = ig;
//			pixels[index++] = ib;
//			pixels[index++] = ia;
//		}
//	}
//
//	// if CHANNEL_NUM is 4, you can use alpha channel in png
//	stbi_write_png("stbpng.png", width, height, CHANNEL_NUM, pixels, width * CHANNEL_NUM);
//
//	//// You have to use 3 comp for complete jpg file. If not, the image will be grayscale or nothing.
//	//stbi_write_jpg("stbjpg3.jpg", width, height, 3, pixels, 100);
//	delete[] pixels;
//
//	return 0;
//}