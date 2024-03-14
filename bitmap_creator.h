// bitmap_creator.h
#ifndef BITMAP_CREATOR_H
#define BITMAP_CREATOR_H

#include <iostream>
#include <fstream>
#include <cstdint>
#include <cmath>

using namespace std;

#pragma pack(push, 1)
struct BitmapFileHeader {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct BitmapInfoHeader {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

class BitmapCreator {
public:
    static void createBitmap(const char* filename, int width, int height) {
        BitmapFileHeader fileHeader;
        BitmapInfoHeader infoHeader;

        fileHeader.bfType = 0x4D42;
        fileHeader.bfSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + width * height * 3;
        fileHeader.bfReserved1 = 0;
        fileHeader.bfReserved2 = 0;
        fileHeader.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

        infoHeader.biSize = sizeof(BitmapInfoHeader);
        infoHeader.biWidth = width;
        infoHeader.biHeight = height;
        infoHeader.biPlanes = 1;
        infoHeader.biBitCount = 24;
        infoHeader.biCompression = 0;
        infoHeader.biSizeImage = 0;
        infoHeader.biXPelsPerMeter = 0;
        infoHeader.biYPelsPerMeter = 0;
        infoHeader.biClrUsed = 0;
        infoHeader.biClrImportant = 0;

        ofstream file(filename, ios::out | ios::binary);
        if (!file) {
            cout << "Error: Could not create file." << endl;
            return;
        }

        file.write(reinterpret_cast<char*>(&fileHeader), sizeof(BitmapFileHeader));
        file.write(reinterpret_cast<char*>(&infoHeader), sizeof(BitmapInfoHeader));

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                file.put(0); file.put(0);  file.put(0);
            }
        }

        file.close();
    }

    static void drawLine(const char* filename, float x1, float y1, float x2, float y2) {
        ifstream file(filename, ios::in | ios::binary);

        BitmapFileHeader fileHeader;
        BitmapInfoHeader infoHeader;

        file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BitmapFileHeader));
        file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BitmapInfoHeader));

        int width = abs(infoHeader.biWidth);
        int height = abs(infoHeader.biHeight);

        int x1_int = max(0, min(width - 1, static_cast<int>(round(x1 * width))));
        int y1_int = max(0, min(height - 1, static_cast<int>(round(y1 * height))));
        int x2_int = max(0, min(width - 1, static_cast<int>(round(x2 * width))));
        int y2_int = max(0, min(height - 1, static_cast<int>(round(y2 * height))));

        const int pixelOffset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

        float slope = (y2_int - y1_int) / static_cast<float>(x2_int - x1_int);
        float intercept = y1_int - slope * x1_int;

        ofstream outFile(filename, ios::out | ios::in | ios::binary);

        outFile.seekp(pixelOffset + (y1_int * width + x1_int) * 3);

        for (int x = x1_int; x <= x2_int; ++x) {
            int y = slope * x + intercept;
            if (x >= 0 && x < width && y >= 0 && y < height) {
                outFile.seekp(pixelOffset + (y * width + x) * 3);
                outFile.put(255);
                outFile.put(255); 
                outFile.put(255); 
            }
        }

        outFile.close();
    }
};

#endif 