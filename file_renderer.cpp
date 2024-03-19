#include <iostream>
#include <vector>

#include "BITMAP_H.h"
#include "CNUMPP_H.h"

int main() {
    Bitmap bitmap(256, 256);
    float scalar = 0.7;
    float cube[8][4] = {
                {1, 1, 1,1},
                {1, 1, -1,1},
                {1, -1, 1,1},
                {1, -1, -1,1}, 
                {-1, 1, 1,1},
                {-1, 1, -1,1}, 
                {-1, -1, 1,1}, 
                {-1, -1, -1,1}};
    std::vector<std::pair<int, int>> lines = {
                                            {0,1},{0,2},{0,4},
                                            {6,7},{6,4},{6,2},
                                            {5,7},{5,1},{5,4},
                                            {3,1},{3,7},{3,2}
                                            };
    
    float result[8][4];
    move(*cube, 8, 0, 0, 5, *result);
    for(auto i:lines){
        float x1 = (result[i.first][0]*scalar);
        float y1 = (result[i.first][1]*scalar);
        float x2 = (result[i.second][0]*scalar);
        float y2 = (result[i.second][1]*scalar);
        bitmap.drawLine(x1,y1,x2,y2);
    }
    bitmap.save("output.bmp");
    return 0;
}