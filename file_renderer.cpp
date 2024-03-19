#include <iostream>
#include <vector>

#include "BITMAP_H.h"
#include "CNUMPP_H.h"

int main() {
    float scalar = 0.75;
    float cube[8][4] = 
   {{ 1, 1, 1,1},
    { 1, 1,-1,1},
    { 1,-1, 1,1},
    { 1,-1,-1,1}, 
    {-1, 1, 1,1},
    {-1, 1,-1,1}, 
    {-1,-1, 1,1}, 
    {-1,-1,-1,1}};
    std::vector<std::pair<int, int>> lines = 
   {{0,1},{0,2},{0,4},
    {6,7},{6,4},{6,2},
    {5,7},{5,1},{5,4},
    {3,1},{3,7},{3,2}};

    float result[8][4];
    float result2[8][4];
    
    scale(*cube, 8, 1, 2, 1, *result);
    copy(*result, *cube, 4, 8);

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 4; j++){
            std::cout << cube[i][j] << " ";
        }
        std::cout << std::endl;
    }

    for(int deg = 0; deg < 180; deg+=5){
        Bitmap bitmap(256, 256);

        copy(*cube, *result, 8, 4);

        xrotate(*result, 8, deg, *result2);
        copy(*result2, *result, 8, 4);

        move(*result, 8, 0, 0, 5, *result2);
        copy(*result2, *result, 8, 4);

        perstrans(*result, 8,2,10,*result2);
        copy(*result2, *result, 8, 4);

        for(auto i:lines){

            float w1 = (result[i.first][3]);
            float w2 = (result[i.second][3]);

            float x1 = (result[i.first][0]*scalar)/w1;
            float y1 = (result[i.first][1]*scalar)/w1;
            float x2 = (result[i.second][0]*scalar)/w2;
            float y2 = (result[i.second][1]*scalar)/w2;

            bitmap.drawLine(x1,y1,x2,y2);
        }
        bitmap.save("output" + std::to_string(deg) + ".bmp");
    }
    return 0;
}