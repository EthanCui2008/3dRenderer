#ifndef BITMAP_H
#define BITMAP_H

#include <fstream>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <limits>

#include "Point3D.h"

class Bitmap {
private:
    int width;
    int height;
    int bytesPerPixel;
    // Internal buffer: no padding, just width*height*3 bytes (BGR)
    std::vector<unsigned char> imageData;
    std::vector<std::vector<float>> zBuffer;

    struct Point {
        float x;
        float y;
    };

    int edgeFunction(Point a, Point b, Point c) {
        return (int)((c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x));
    }

public:
    Bitmap(int w, int h) : width(w), height(h), bytesPerPixel(3) {
        imageData.resize(width * height * bytesPerPixel, 0);
        zBuffer = std::vector<std::vector<float>>(height, std::vector<float>(width, std::numeric_limits<float>::max()));
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Returns pointer to raw pixel data (BGR, no padding, top-to-bottom)
    const unsigned char* getData() const { return imageData.data(); }

    void clear() {
        std::fill(imageData.begin(), imageData.end(), 0);
        for (auto& row : zBuffer)
            std::fill(row.begin(), row.end(), std::numeric_limits<float>::max());
    }

    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, float z) {
        if (x >= 0 && x < width && y >= 0 && y < height && z < zBuffer[y][x]) {
            int index = (y * width + x) * bytesPerPixel;
            imageData[index] = b;      // BMP is BGR
            imageData[index + 1] = g;
            imageData[index + 2] = r;
            zBuffer[y][x] = z;
        }
    }

    void drawallFacet(Vector3D p1, Vector3D p2, Vector3D p3, uint8_t r, uint8_t g, uint8_t b) {
        float fx1 = p1.x, fy1 = p1.y;
        float fx2 = p2.x, fy2 = p2.y;
        float fx3 = p3.x, fy3 = p3.y;
        float z = (p1.z + p2.z + p3.z) / 3.0f;

        drawFacet(fx1, fy1, fx2, fy2, fx3, fy3, z, r, g, b);
        drawFacet(fx1, fy1, fx3, fy3, fx2, fy2, z, r, g, b);
        drawFacet(fx2, fy2, fx1, fy1, fx3, fy3, z, r, g, b);
        drawFacet(fx2, fy2, fx3, fy3, fx1, fy1, z, r, g, b);
        drawFacet(fx3, fy3, fx2, fy2, fx1, fy1, z, r, g, b);
        drawFacet(fx3, fy3, fx1, fy1, fx2, fy2, z, r, g, b);
    }

    void drawFacet(float fx1, float fy1, float fx2, float fy2, float fx3, float fy3, float z, uint8_t r, uint8_t g, uint8_t b) {
        Point p1 = { fx1 * width / 2 + width / 2.0f, fy1 * height / 2 + height / 2.0f };
        Point p2 = { fx2 * width / 2 + width / 2.0f, fy2 * height / 2 + height / 2.0f };
        Point p3 = { fx3 * width / 2 + width / 2.0f, fy3 * height / 2 + height / 2.0f };

        Point min = { std::min({ p1.x, p2.x, p3.x }), std::min({ p1.y, p2.y, p3.y }) };
        Point max = { std::max({ p1.x, p2.x, p3.x }), std::max({ p1.y, p2.y, p3.y }) };

        min.x = std::max(0.0f, min.x);
        min.y = std::max(0.0f, min.y);
        max.x = std::min((float)(width - 1), max.x);
        max.y = std::min((float)(height - 1), max.y);

        for (int y = (int)min.y; y <= (int)max.y; ++y) {
            for (int x = (int)min.x; x <= (int)max.x; ++x) {
                Point p = { (float)x, (float)y };

                int w0 = edgeFunction(p2, p3, p);
                int w1 = edgeFunction(p3, p1, p);
                int w2 = edgeFunction(p1, p2, p);

                if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                    setPixel(x, y, r, g, b, z);
                }
            }
        }
    }

    void drawFacetedge(Vector3D p1, Vector3D p2, Vector3D p3, uint8_t r, uint8_t g, uint8_t b) {
        drawLine(p1.x, p1.y, p2.x, p2.y, r, g, b);
        drawLine(p2.x, p2.y, p3.x, p3.y, r, g, b);
        drawLine(p3.x, p3.y, p1.x, p1.y, r, g, b);
    }

    void drawLine(float fx1, float fy1, float fx2, float fy2, uint8_t r, uint8_t g, uint8_t b) {
        int x1 = (int)(fx1 * width / 2 + width / 2.0f);
        int x2 = (int)(fx2 * width / 2 + width / 2.0f);
        int y1 = (int)(fy1 * height / 2 + height / 2.0f);
        int y2 = (int)(fy2 * height / 2 + height / 2.0f);
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;

        while (true) {
            if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
                int index = (y1 * width + x1) * bytesPerPixel;
                imageData[index] = b;      // BGR
                imageData[index + 1] = g;
                imageData[index + 2] = r;
            }
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

        int padding = (4 - (width * bytesPerPixel) % 4) % 4;
        int imageSize = (width * bytesPerPixel + padding) * height;
        int fileSize = 54 + imageSize;

        unsigned char bmpHeader[54] = {};
        bmpHeader[0] = 'B'; bmpHeader[1] = 'M';
        bmpHeader[2] = fileSize; bmpHeader[3] = fileSize >> 8; bmpHeader[4] = fileSize >> 16; bmpHeader[5] = fileSize >> 24;
        bmpHeader[10] = 54;
        bmpHeader[14] = 40;
        bmpHeader[18] = width; bmpHeader[19] = width >> 8; bmpHeader[20] = width >> 16; bmpHeader[21] = width >> 24;
        bmpHeader[22] = height; bmpHeader[23] = height >> 8; bmpHeader[24] = height >> 16; bmpHeader[25] = height >> 24;
        bmpHeader[26] = 1;
        bmpHeader[28] = 24;
        bmpHeader[34] = imageSize; bmpHeader[35] = imageSize >> 8; bmpHeader[36] = imageSize >> 16; bmpHeader[37] = imageSize >> 24;

        outputFile.write(reinterpret_cast<char*>(bmpHeader), 54);

        // Write rows bottom-to-top (BMP format) with padding
        unsigned char pad[3] = {0, 0, 0};
        for (int y = height - 1; y >= 0; --y) {
            outputFile.write(reinterpret_cast<const char*>(&imageData[y * width * bytesPerPixel]), width * bytesPerPixel);
            if (padding > 0)
                outputFile.write(reinterpret_cast<char*>(pad), padding);
        }

        std::cout << "Image Created" << std::endl;
        outputFile.close();
    }
};

#endif
