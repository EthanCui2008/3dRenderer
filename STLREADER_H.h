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

#include "BITMAP_H.h"
#include "CNUMPP_H.h"
#include "STLREADER_H.h"
#include "Point3D.h"

std::string readFileIntoString(const std::string& filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "File could not be opened: " << filePath << std::endl;
        return ""; // Return an empty string if file can't be opened
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void parseSTL(const std::string& stlContent, std::vector<std::array<Vector3D, 4>>& facets) {
    std::regex facetRegex(R"(facet normal\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?)\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?)\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?))");
    std::regex vertexRegex(R"(vertex\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?)\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?)\s+([-+]?[\d.]+(?:[eE][-+]?\d+)?))");

    auto facetBegin = std::sregex_iterator(stlContent.begin(), stlContent.end(), facetRegex);
    auto facetEnd = std::sregex_iterator();

    auto vertexBegin = std::sregex_iterator(stlContent.begin(), stlContent.end(), vertexRegex);
    auto vertexEnd = std::sregex_iterator();

    for (auto it = facetBegin; it != facetEnd; ++it) {
        std::smatch match = *it;
        std::array<Vector3D, 4> facetArray;

        facetArray[0] = Vector3D(std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[3].str()));

        auto v_it = vertexBegin;
        for (int i = 0; i < 3; ++i) {
            if (v_it != vertexEnd) {
                std::smatch vertexMatch = *v_it;
                facetArray[i + 1] = Vector3D(std::stof(vertexMatch[1].str()), std::stof(vertexMatch[2].str()), std::stof(vertexMatch[3].str()));
                ++v_it;
            }
        }
        vertexBegin = v_it;
        facets.push_back(facetArray);
    }
}

#endif // STLREADER_H