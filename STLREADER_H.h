#ifndef STLREADER_H 
#define STLREADER_H 

#include <cstdint>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <regex>


#include "Point3D.h"

std::string readFileIntoString(const std::string& filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cout << "File is not here" << std::endl;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<std::array<Vector3D, 4>> parseSTL(const std::string& stlContent){
    std::vector<std::array<Vector3D, 4>> facets;

    std::regex facetRegex(R"(facet normal\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?)\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?)\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?))");
    std::regex vertexRegex(R"(vertex\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?)\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?)\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?))");

    auto facetBegin = std::sregex_iterator(stlContent.begin(), stlContent.end(), facetRegex);
    auto facetEnd = std::sregex_iterator();

    auto vertexBegin = std::sregex_iterator(stlContent.begin(), stlContent.end(), vertexRegex);
    auto vertexEnd = std::sregex_iterator();

    

    for (auto it = facetBegin; it != facetEnd; ++it) {
        std::smatch match = *it;
        std::array<Vector3D, 4> facetArray;

        facetArray[0] = Vector3D(stof(match[1]),stof(match[2]),stof(match[3]));

        //std::cout << "  facet normal " << facetArray[0].x << " " << facetArray[0].y << " " << facetArray[0].z << std::endl;
        //std::cout << "    outer loop" << std::endl;


        auto v_it = vertexBegin;
        for (int i = 0; i < 3; ++i) {
            if (v_it != vertexEnd) {
                std::smatch vertexMatch = *v_it;

                facetArray[i+1] = Vector3D(stof(vertexMatch[1]),stof(vertexMatch[2]),stof(vertexMatch[3]));

                //std::cout << "      vertex " << facetArray[i+1].x << " " << facetArray[i+1].y << " " << facetArray[i+1].z << std::endl;
                ++v_it;
            }
        }
        vertexBegin = v_it;
        //std::cout << "    endloop" << std::endl;
        facets.push_back(facetArray);
    }
    //std::cout << "solid  " << std::endl;
    int j = 0;
    for(auto i:facets){
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
        //float shade = dotproduct(facet_vector,light_vector)/3.14195;
        //shade = 1-shade;
        //std::cout << shade << " ";

        //if(shade > 0.5){
        //    continue;
        //}

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

        
        //std::cout << "  facet normal " << facet_vector.x << " " << facet_vector.y << " " << facet_vector.z << std::endl;
        //std::cout << "    outer loop" << std::endl;
        //std::cout << "      vertex " << x1 << " " << y1 << " " << z1 << std::endl;
        //std::cout << "      vertex " << x2 << " " << y2 << " " << z2 << std::endl;
        //std::cout << "      vertex " << x3 << " " << y3 << " " << z3 << std::endl;
        //std::cout << "    endloop" << std::endl;
        //std::cout << "  endfacet" << std::endl;
        
    }
    //std::cout << "endsolid" << std::endl;

    return facets;
}

#endif STLREADER_H