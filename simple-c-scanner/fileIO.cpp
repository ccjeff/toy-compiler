#include <iostream>
#include <fstream>
#include <vector>
#include "fileIO.h"


FileIO::FileIO() = default;
FileIO::~FileIO() = default;

int FileIO::openFile(std::string inputName) {
    std::ifstream rawFile(inputName);
    std::copy(std::istream_iterator<std::string>(rawFile),
              std::istream_iterator<std::string>(),
              std::back_inserter(this->buffer));
    /* DEBUG */

    std::cout << "lines read in:\n";
    std::copy(this->buffer.begin(), this->buffer.end(),
              std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << std::endl;

    return 0;
}

std::vector<std::string> FileIO::getBuffer() {
    return this->buffer;
}