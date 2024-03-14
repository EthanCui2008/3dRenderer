#include <bits/stdc++.h>
#include "bitmap_creator.h"
using namespace std;

int main() {
    const char* filename = "output.bmp";
    int width = 512;
    int height = 256;
    BitmapCreator::createBitmap(filename, width, height);
	BitmapCreator::drawLine(filename, 0.0,0.0,0.531, 0.152);
    return 0;
}