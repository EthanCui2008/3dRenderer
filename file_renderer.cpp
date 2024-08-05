#include <iostream>
#include <vector>

#include "BITMAP_H.h"
#include "CNUMPP_H.h"
#include "STLREADER_H.h"
#include "Point3D.h"

int main() {
    //creates a Bitmap image
    Bitmap bitmap(1024, 1024);
    
    //light Vector, used to calculate amount of shade
    Vector3D light_vector = Vector3D(-0.1,-0.2,-1);

    //camera Vector [currently deprecated], was used to 
    Vector3D camera_vector = Vector3D(0,0,-1);
 
    //import an stl file, process it
    std::string stlFilePath = "Dodecahedron-converted-ASCII.stl";
    std::string stl_string = readFileIntoString(stlFilePath);

    //Datastructure used to store coordinates
    std::vector<std::array<Vector3D, 4>> stl_data;

    //parses raw STL string, appends data to STL_data
    parseSTL(stl_string, stl_data);

    //coordinates that help define the rerndering box
    int left = -20;
    int bottom = -20;
    int near = -20;

    //Coordinates that help define the rendering box
    int right = 20;
    int top = 20;
    int far = 20;

    for(std::array<Vector3D, 4> face : stl_data){

        //converts Raw coordinates into an array, to be processed by functions
        float vertex_array[3][4] = 
           {{face[1].x,face[1].y,face[1].z,1},
            {face[2].x,face[2].y,face[2].z,1},
            {face[3].x,face[3].y,face[3].z,1}};

        //vector of the tesselated Facet, used to calculate light
        Vector3D facet_vector = Vector3D(face[0].x,face[0].y,face[0].z);

        //modifies the vertex array coordinates [in the case of rotation, changes the facet vector too]
        //may change such that the camera moves instead
        xrotate(*vertex_array, 3, 45, facet_vector);
        yrotate(*vertex_array, 3, 45, facet_vector);
        zrotate(*vertex_array, 3, 45, facet_vector);

        //fit object, and add depth 
        perstrans(*vertex_array, 3, near, far);
        viewvol(*vertex_array, 3, right, left, bottom, top, far, near);

        //weights, used to divide the x.y coordinates for depth
        float w1 = vertex_array[0][2]; float w2 = vertex_array[1][2]; float w3 = vertex_array[2][2];

        //shade calculated by dot product to light vector
        float shade = dotproduct(facet_vector,light_vector)/3.14195;
        shade = 1-shade;

        //defines points that are drawn on the canvas
        Vector3D point1 = Vector3D(vertex_array[0][0]/w1,vertex_array[0][1]/w1,vertex_array[0][3]);
        Vector3D point2 = Vector3D(vertex_array[1][0]/w2,vertex_array[1][1]/w2,vertex_array[1][3]);
        Vector3D point3 = Vector3D(vertex_array[2][0]/w3,vertex_array[2][1]/w3,vertex_array[2][3]);

        //draws Facets
        bitmap.drawallFacet(point1, point2, point3, 256*shade,0,0);
        //bitmap.drawFacetedge(point1, point2, point3, 255,255,255);
    }
    //saves bitmap
    bitmap.save("output.bmp");
    return 0;
}