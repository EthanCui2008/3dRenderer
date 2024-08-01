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

        auto v_it = vertexBegin;
        for (int i = 0; i < 3; ++i) {
            if (v_it != vertexEnd) {
                std::smatch vertexMatch = *v_it;
                facetArray[i+1] = Vector3D(stof(vertexMatch[1]),stof(vertexMatch[2]),stof(vertexMatch[3]));
                ++v_it;
            }
        }
        vertexBegin = v_it;
        facets.push_back(facetArray);
    }

    return facets;
}

#endif STLREADER_H