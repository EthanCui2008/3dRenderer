#include <iostream>
#include <vector>

#include "BITMAP_H.h"
#include "CNUMPP_H.h"
#include "STLREADER_H.h"

int main() {
    //Creates a Bitmap image
    Bitmap bitmap(1024, 1024);

    //Scales down image such that fits [I think it is currently redundant]
    float scalar = 0.7;
    
    //Light Vector, used to calculate amount of shade
    Vector3D light_vector = Vector3D(0.5,0.2,1);

    //Import an stl file, process it
    std::string stlFilePath = "dodecahedron.stl";
    std::string stl_string = readFileIntoString(stlFilePath);
    std::vector<std::array<Vector3D, 4>> stl_data = parseSTL(stl_string);
    int j = 0;
    for(auto i:stl_data){
        j+=1;
        //there is a better way to do this, maybe @ilcheese2 will find it for me, or more likely he will make fun of me
        float vertexarray[3][4] = 
           {{i[1].x,i[1].y,i[1].z,1},
            {i[2].x,i[2].y,i[2].z,1},
            {i[3].x,i[3].y,i[3].z,1}};
        //move(*vertexarray, 3, -35,-35,-35);
        //scale(*vertexarray, 3, 0.02, 0.02, 0.02);
        //xrotate(*vertexarray, 3, 45);
        //yrotate(*vertexarray, 3, 60);
        //yrotate(*vertexarray, 3, 60);
        //perstrans(*vertexarray, 3, 1, 2);
        //when turning object recalculate light vector
        //xrotate();

        //Shade calculated by dot product to light vector
        float shade = dotproduct(i[0],light_vector)/3.14195;
        
        //std::cout << shade << " ";

        float x1 = vertexarray[0][0];
        float x2 = vertexarray[1][0];
        float x3 = vertexarray[2][0];

        float y1 = vertexarray[0][1];
        float y2 = vertexarray[1][1];
        float y3 = vertexarray[2][1];

        float z1 = vertexarray[0][2];
        float z2 = vertexarray[1][2];
        float z3 = vertexarray[2][2];
        
        std::cout << x1 << " " << y1<< " " << x2<< " " <<  y2<< " " << x3<< " " << y3<< std::endl;
        if (j<60){
            bitmap.drawFacet(x1, y1, x2, y2, x3, y3, shade*255,0,0 );
        }
        else{
            bitmap.drawFacet(x1, y1, x2, y2, x3, y3, 0,0,shade*255 );
        }
    }

    bitmap.save("output.bmp");
    return 0;
}