#include <iostream>
#include <vector>

#include "BITMAP_H.h"
#include "CNUMPP_H.h"

void draw(const std::vector<std::pair<int, int>>& lines, float* result, int rows, Bitmap bitmap, int scalar){
    for(auto i:lines){
        std::cout << i.first << std::endl;
        float w1 = (result[i.first * rows + 3]);
        float w2 = (result[i.second * rows + 3]);

        float x1 = (result[i.first * rows + 0]*scalar)/w1;
        float y1 = (result[i.first * rows + 1]*scalar)/w1;
        float x2 = (result[i.second * rows + 0]*scalar)/w2;
        float y2 = (result[i.second * rows + 1]*scalar)/w2;

        bitmap.drawLine(x1,y1,x2,y2);
        //std::cout << x1 << " " << y1 << " / " << x2 << " " << y2 << std::endl;
    }
}

int main() {
    float scalar = 0.75;
    int obj_size = 8;
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

    float result[obj_size][4];
    float result2[obj_size][4];
    
    scale(*cube, obj_size, 0.5, 0.5, 0.5, *result);
    copy(*result, *cube, 4, obj_size);
    move(*cube, obj_size, 0, 0, 5, *result);
    copy(*result, *cube, 4, obj_size);

    perstrans(*cube, obj_size, 2, 10, *result2);
    copy(*result2, *result, 4, obj_size);

    for(int i = 0; i < obj_size; i++){
        for(int j = 0; j < 4; j++){
            std::cout << result[i][j] << " ";
        }
        std::cout << std::endl;
    }

    

    Bitmap bitmap(256, 256);
    //std::cout << "hi";
    //draw(lines, *result, obj_size, bitmap, scalar);
    bitmap.save("output.bmp");
    return 0;
}