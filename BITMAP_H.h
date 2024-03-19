#ifndef BITMAP_H
#define BITMAP_H

#include <fstream>
#include <cstdint>
#include <iostream>

class Bitmap {
private:
    int width;
    int height;
    int bytesPerPixel;
    int padding;
    unsigned char* imageData;

    void createHeader(std::ostream& outputFile) {
        const int imageSize = (width * bytesPerPixel + padding) * height;
        const int fileSize = 54 + imageSize;

        unsigned char bmpHeader[54] = {
            'B', 'M',
            static_cast<unsigned char>(fileSize), static_cast<unsigned char>(fileSize >> 8), static_cast<unsigned char>(fileSize >> 16), static_cast<unsigned char>(fileSize >> 24),
            0, 0, 0, 0,
            54, 0, 0, 0,
            40, 0, 0, 0,
            static_cast<unsigned char>(width), static_cast<unsigned char>(width >> 8), static_cast<unsigned char>(width >> 16), static_cast<unsigned char>(width >> 24),
            static_cast<unsigned char>(height), static_cast<unsigned char>(height >> 8), static_cast<unsigned char>(height >> 16), static_cast<unsigned char>(height >> 24),
            1, 0,
            24, 0,
            0, 0, 0, 0,
            static_cast<unsigned char>(imageSize), static_cast<unsigned char>(imageSize >> 8), static_cast<unsigned char>(imageSize >> 16), static_cast<unsigned char>(imageSize >> 24),
            0, 0, 0, 0,
            0, 0, 0, 0
        };

        // Write BMP header
        outputFile.write(reinterpret_cast<char*>(bmpHeader), sizeof(bmpHeader));
    }

public:
    Bitmap(int w, int h) : width(w), height(h), bytesPerPixel(3) {
        padding = (4 - (width * bytesPerPixel) % 4) % 4;
        imageData = new unsigned char[(width * bytesPerPixel + padding) * height]();
    }

    ~Bitmap() {
        delete[] imageData;
    }

    void drawLine(float fx1, float fy1,float fx2, float fy2) {
        int x1 = fx1*width/2 + width/2;
        int x2 = fx2*width/2 + width/2;
        int y1 = fy1*height/2 + height/2;
        int y2 = fy2*height/2 + height/2;
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;

        while (true) {
            int index = (y1 * width + x1) * bytesPerPixel;
            // Set pixel to white
            imageData[index] = 255;   // Red
            imageData[index + 1] = 255; // Green
            imageData[index + 2] = 255; // Blue

            if (x1 == x2 && y1 == y2) break;
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }

    void save(const std::string& filename) {
        std::ofstream outputFile(filename, std::ios::binary);
        if (!outputFile.is_open()) {
            std::cerr << "Error: Failed to create image file." << std::endl;
            return;
        }
        // Write BMP header
        createHeader(outputFile);

        // Write BMP pixel data
        outputFile.write(reinterpret_cast<char*>(imageData), (width * bytesPerPixel + padding) * height);

        // Close the image file
        outputFile.close();
    }
};

#endif // BITMAP_H