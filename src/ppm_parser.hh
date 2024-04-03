#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "color.hh"
#include "image2d.hh"

class PPMParser
{
public:
    std::string filename_;

    PPMParser(const std::string &filename);

    bool parse(Image2D &pixels);
};