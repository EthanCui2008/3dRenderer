#include <iostream>
#include <vector>

#include "BITMAP_H.h"
#include "CNUMPP_H.h"

int main() {
    Bitmap bitmap(256, 256);
    float scalar = 0.5;
    float cube[8][4] = 
   {{ 1, 1, 1,1},
    { 1, 1,-1,1},
    { 1,-1, 1,1},
    { 1,-1, -1,1}, 
    {-1, 1, 1,1},
    {-1, 1, -1,1}, 
    {-1,-1, 1,1}, 
    {-1,-1, -1,1}};
    std::vector<std::pair<int, int>> lines = 
   {{0,1},{0,2},{0,4},
    {6,7},{6,4},{6,2},
    {5,7},{5,1},{5,4},
    {3,1},{3,7},{3,2}};
    
    float result[8][4];

    copy(*cube, *result, 4, 8);
    xrotate(*cube, 8, 45, *result);
    copy(*result, *cube, 8, 4);
    yrotate(*cube, 8, 45, *result);
    copy(*result, *cube, 8, 4);
    move(*cube, 8, 0, 3, 5, *result); 
    //shitty code my beloved
    //use in place functions
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 4; j++){
            std::cout << cube[i][j] << " ";
        }
        std::cout << std::endl;
    }
    for(auto i:lines){
        float w1 = (result[i.first][2]);
        float w2 = (result[i.second][2]);
        float x1 = (result[i.first][0]*scalar)/w1;
        float y1 = (result[i.first][1]*scalar)/w1;
        float x2 = (result[i.second][0]*scalar)/w2;
        float y2 = (result[i.second][1]*scalar)/w2;
        bitmap.drawLine(x1,y1,x2,y2);
    }
    bitmap.save("output.bmp");
    return 0;
}