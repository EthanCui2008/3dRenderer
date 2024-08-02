#ifndef BITMAP_H
#define BITMAP_H

#include <fstream>
#include <cstdint>
#include <iostream>
#include <algorithm>

#include "Point3D.h"


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
            (unsigned char) (fileSize), (unsigned char) (fileSize >> 8), (unsigned char) (fileSize >> 16), (unsigned char) (fileSize >> 24),
            0, 0, 0, 0,
            54, 0, 0, 0,
            40, 0, 0, 0,
            (unsigned char) (width), (unsigned char) (width >> 8), (unsigned char) (width >> 16), (unsigned char) (width >> 24),
            (unsigned char) (height), (unsigned char) (height >> 8), (unsigned char) (height >> 16), (unsigned char) (height >> 24),
            1, 0,
            24, 0,
            0, 0, 0, 0,
            (unsigned char) (imageSize), (unsigned char) (imageSize >> 8), (unsigned char) (imageSize >> 16), (unsigned char) (imageSize >> 24),
            0, 0, 0, 0, 
            0, 0, 0, 0
        };

        // Write BMP header
        outputFile.write(reinterpret_cast<char*>(bmpHeader), sizeof(bmpHeader));
    }

    

    void transPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, float a) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            int index = (y * width + x) * bytesPerPixel;
            imageData[index] += r*a;
            imageData[index + 1] += g*a;
            imageData[index + 2] += b*a;
        }
    }

    struct Point {
        float x;
        float y;
    };

    int edgeFunction(Point a, Point b, Point c) {
        return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
    }

public:
    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            int index = (y * width + x) * bytesPerPixel;
            imageData[index] = r;
            imageData[index + 1] = g;
            imageData[index + 2] = b;
        }
    }
    Bitmap(int w, int h) : width(w), height(h), bytesPerPixel(3) {
        padding = (4 - (width * bytesPerPixel) % 4) % 4;
        imageData = new unsigned char[(width * bytesPerPixel + padding) * height]();
    }

    ~Bitmap() {
        delete[] imageData;
    }
    void drawallFacet(Vector3D p1, Vector3D p2, Vector3D p3, uint8_t r, uint8_t g, uint8_t b){
        float fx1 = p1.x;
        float fy1 = p1.y; 
        float fx2 = p2.x;
        float fy2 = p2.y;
        float fx3 = p3.x;
        float fy3 = p3.y;
        drawFacet(fx1, fy1, fx2, fy2, fx3, fy3, r, g, b);
        drawFacet(fx1, fy1, fx3, fy3, fx2, fy2, r, g, b);

        drawFacet(fx2, fy2, fx1, fy1, fx3, fy3, r, g, b);
        drawFacet(fx2, fy2, fx3, fy3, fx1, fy1, r, g, b);

        drawFacet(fx3, fy3, fx2, fy2, fx1, fy1, r, g, b);
        drawFacet(fx3, fy3, fx1, fy1, fx2, fy2, r, g, b);
    }
    void drawFacet(float fx1, float fy1, float fx2, float fy2, float fx3, float fy3, uint8_t r, uint8_t g, uint8_t b) {
        Point p1 = { fx1 * width / 2 + width / 2, fy1 * height / 2 + height / 2 };
        Point p2 = { fx2 * width / 2 + width / 2, fy2 * height / 2 + height / 2 };
        Point p3 = { fx3 * width / 2 + width / 2, fy3 * height / 2 + height / 2 };

        Point min = { std::min({ p1.x, p2.x, p3.x }), std::min({ p1.y, p2.y, p3.y }) };
        Point max = { std::max({ p1.x, p2.x, p3.x }), std::max({ p1.y, p2.y, p3.y }) };

        min.x = std::max(0.0f, min.x);
        min.y = std::max(0.0f, min.y);
        max.x = std::min((float)width - 1, max.x);
        max.y = std::min((float)height - 1, max.y);

        for (int y = static_cast<int>(min.y); y <= static_cast<int>(max.y); ++y) {
            for (int x = static_cast<int>(min.x); x <= static_cast<int>(max.x); ++x) {
                Point p = { static_cast<float>(x), static_cast<float>(y) };
                
                int w0 = edgeFunction(p2, p3, p);
                int w1 = edgeFunction(p3, p1, p);
                int w2 = edgeFunction(p1, p2, p);

                if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                    setPixel(x, y, r, g, b);
                }
            }
        }
    }
    void drawFacetedge(Vector3D p1, Vector3D p2, Vector3D p3, uint8_t r, uint8_t g, uint8_t b) {
        float fx1 = p1.x;
        float fy1 = p1.y; 
        float fx2 = p2.x;
        float fy2 = p2.y;
        float fx3 = p3.x;
        float fy3 = p3.y;

        drawLine(fx1, fy1, fx2, fy2, r, g, b);
        drawLine(fx2, fy2, fx3, fy3, r, g, b);
        drawLine(fx3, fy3, fx1, fy1, r, g, b);
    }
    void drawLine(float fx1, float fy1, float fx2, float fy2, uint8_t r, uint8_t g, uint8_t b) {
        int x1 = fx1 * width / 2 + width / 2;
        int x2 = fx2 * width / 2 + width / 2;
        int y1 = fy1 * height / 2 + height / 2;
        int y2 = fy2 * height / 2 + height / 2;
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;

        while (true) {
            int index = (y1 * width + x1) * bytesPerPixel;
            if (!(y1 < 0 || x1 < 0 || y1 >= height - 1 || x1 >= width - 1)) {
                imageData[index] = r;   // Red
                imageData[index + 1] = g; // Green
                imageData[index + 2] = b; // Blue
            }
            if (x1 == x2 && y1 == y2) {
                break;
            }

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
        std::cout << "Image Created";
        // Close the image file
        outputFile.close();
    }
};

#endif // BITMAP_H

