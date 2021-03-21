#ifndef FILE_IO_H
#define FILE_IO_H

#include <vector>
#include <string>
class fileIO
{
private:
    /* data */
    int maxLength;
    std::vector<char> buffer;

public:
    fileIO();
    std::vector<char> getBuffer(std::string inputName);
    ~fileIO();
};

#endif