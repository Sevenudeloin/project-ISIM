#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "color.hh"

class PPMParser
{
public:
    std::string filename_;

    PPMParser(const std::string &filename);

    bool parse(std::vector<Color> &pixels, int &width, int &height);
};