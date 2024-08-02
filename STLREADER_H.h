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

void parseSTL(const std::string& stlContent, std::vector<std::array<Vector3D, 4>>& facets){

    std::regex facetRegex(R"(facet normal\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?)\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?)\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?))");
    std::regex vertexRegex(R"(vertex\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?)\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?)\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?))");

    auto facetBegin = std::sregex_iterator(stlContent.begin(), stlContent.end(), facetRegex);
    auto facetEnd = std::sregex_iterator();

    auto vertexBegin = std::sregex_iterator(stlContent.begin(), stlContent.end(), vertexRegex);
    auto vertexEnd = std::sregex_iterator();

    //std::cout << "solid " << std::endl;
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
    //std::cout << "endsolid  " << std::endl;
}

#endif STLREADER_H