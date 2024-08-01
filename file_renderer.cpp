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
    Vector3D light_vector = Vector3D(0.1,0.2,1);

    //Import an stl file, process it
    std::string stlFilePath = "Dazzling Borwo-converted-ASCII.stl";
    std::string stl_string = readFileIntoString(stlFilePath);
    std::cout << stl_string << std::endl;
    std::vector<std::array<Vector3D, 4>> stl_data = parseSTL(stl_string);
    int j = 0;
    std::cout << "solid  " << std::endl;
    for(auto i:stl_data){
        j+=1;
        float vertexarray[3][4] = 
           {{i[1].x,i[1].y,i[1].z,1},
            {i[2].x,i[2].y,i[2].z,1},
            {i[3].x,i[3].y,i[3].z,1}};
        Vector3D facet_vector = Vector3D(i[0].x,i[0].y,i[0].z);

        //there is a better way to do this, maybe @ilcheese2 will find it for me, or more likely he will make fun of me
        //move(*vertexarray, 3, -35, -35, -35);
        scale(*vertexarray, 3, 0.05, 0.05, 0.05);
        
        //xrotate(*vertexarray, 3, 60, facet_vector);
        //yrotate(*vertexarray, 3, 60);
        //perstrans(*vertexarray, 3, 1, 2);
        //when turning object recalculate light vector
        //xrotate();

        //Shade calculated by dot product to light vector
        float shade = dotproduct(facet_vector,light_vector)/3.14195;
        shade = 1-shade;
        //std::cout << shade << " ";

        if(shade > 0.5){
            continue;
        }

        int d = 1;

        float x1 = i[1].x/d;
        float x2 = i[2].x/d;
        float x3 = i[3].x/d;

        float y1 = i[1].y/d;
        float y2 = i[2].y/d;
        float y3 = i[3].y/d;

        float z1 = i[1].z/d;
        float z2 = i[2].z/d;
        float z3 = i[3].z/d;

        /*float x1 = vertexarray[0][0];
        float x2 = vertexarray[1][0];
        float x3 = vertexarray[2][0];

        float y1 = vertexarray[0][1];
        float y2 = vertexarray[1][1];
        float y3 = vertexarray[2][1];

        float z1 = vertexarray[0][2];
        float z2 = vertexarray[1][2];
        float z3 = vertexarray[2][2];*/

        /*bitmap.drawFacet(x1, y1, x2, y2, x3, y3, 256*shade,256*shade,256*shade, j);
        bitmap.drawFacet(x2, y2, x1, y1, x3, y3, 256*shade,256*shade,256*shade, j);

        bitmap.drawFacet(x1, y1, x3, y3, x2, y2, 256*shade,256*shade,256*shade, j);
        bitmap.drawFacet(x3, y3, x1, y1, x2, y2, 256*shade,256*shade,256*shade, j);

        bitmap.drawFacet(x3, y3, x2, y2, x1, y1, 256*shade,256*shade,256*shade, j);
        bitmap.drawFacet(x2, y2, x3, y3, x1, y1, 256*shade,256*shade,256*shade, j);*/

        //std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << std::endl;



        //bitmap.drawFacetedge(x1, y1, x2, y2, x3, y3, 0,255,255);       

        
        std::cout << "  facet normal " << facet_vector.x << " " << facet_vector.y << " " << facet_vector.z << std::endl;
        std::cout << "    outer loop" << std::endl;
        std::cout << "      vertex " << x1 << " " << y1 << " " << z1 << std::endl;
        std::cout << "      vertex " << x2 << " " << y2 << " " << z2 << std::endl;
        std::cout << "      vertex " << x3 << " " << y3 << " " << z3 << std::endl;
        std::cout << "    endloop" << std::endl;
        std::cout << "  endfacet" << std::endl;
        
    }
    std::cout << "endsolid" << std::endl;
    bitmap.save("output.bmp");
    return 0;
}