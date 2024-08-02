#include <iostream>
#include <vector>

#include "BITMAP_H.h"
#include "CNUMPP_H.h"
#include "STLREADER_H.h"
#include "Point3D.h"

int main() {
    //Creates a Bitmap image
    Bitmap bitmap(1024, 1024);

    //Scales down image such that fits [I think it is currently redundant]
    float scalar = 0.7;
    
    //Light Vector, used to calculate amount of shade
    Vector3D light_vector = Vector3D(0.1,0.2,1);
    Vector3D camera_vector = Vector3D(0,0,-1);
 
    //Import an stl file, process it
    std::string stlFilePath = "Dazzling Borwo-converted-ASCII.stl";
    std::string stl_string = readFileIntoString(stlFilePath);

    std::vector<std::array<Vector3D, 4>> stl_data;

    parseSTL(stl_string, stl_data);

    int left = -20;
    int bottom = -20;
    int near = -20;

    int right = 20;
    int top = 20;
    int far = 20;
    for(std::array<Vector3D, 4> i : stl_data){
        float vertexarray[3][4] = 
           {{i[1].x,i[1].y,i[1].z,1},
            {i[2].x,i[2].y,i[2].z,1},
            {i[3].x,i[3].y,i[3].z,1}};
        Vector3D facet_vector = Vector3D(i[0].x,i[0].y,i[0].z);
        //there is a better way to do this, maybe @ilcheese2 will find it for me, or more likely he will make fun of me
        //xrotate(*vertexarray, 3, 45, facet_vector);
        //yrotate(*vertexarray, 3, 45, facet_vector);
        //zrotate(*vertexarray, 3, 45, facet_vector);

        perstrans(*vertexarray, 3, near, far);
        viewvol(*vertexarray, 3, right, left, bottom, top, far, near);

        //yrotate(*vertexarray, 3, 30, facet_vector);
        float w1 = vertexarray[0][2]; float w2 = vertexarray[1][2]; float w3 = vertexarray[2][2];

        //Shade calculated by dot product to light vector
        float shade = dotproduct(facet_vector,light_vector)/3.14195;

        float hiddenvals = dotproduct(facet_vector,camera_vector)/3.14195;
        shade = 1-shade; 
        shade = 0.24 + shade*0.75;

        Vector3D point1 = Vector3D(vertexarray[0][0]/w1,vertexarray[0][1]/w1,vertexarray[0][3]);
        Vector3D point2 = Vector3D(vertexarray[1][0]/w2,vertexarray[1][1]/w2,vertexarray[1][3]);
        Vector3D point3 = Vector3D(vertexarray[2][0]/w3,vertexarray[2][1]/w3,vertexarray[2][3]);
        shade = 0.3*shade;
        bitmap.drawallFacet(point1, point2, point3, 256*shade,256*shade,256*shade);
        
        //std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << std::endl
        std::cout << "h";
        //bitmap.drawFacetedge(point1, point2, point3, 0,0,0);
    }
    bitmap.save("output.bmp");
    return 0;
}