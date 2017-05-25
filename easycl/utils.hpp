#pragma once

#include <fstream>
#include <sstream>


static std::string ReadFile(std::string file_name) {
    std::ifstream file;
    file.open(file_name);
    std::stringstream str_stream;
    str_stream << file.rdbuf();
    file.close();

    return str_stream.str();
}
