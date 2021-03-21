#ifndef FILE_IO_H
#define FILE_IO_H

#include <vector>
#include <string>
class FileIO
{
private:
    /* data */
    std::vector<std::string> buffer;

public:
    int lexemeBegin = 0;
    int forward = 0;

    FileIO();
    int openFile(std::string inputName);
    std::vector<std::string> getBuffer();
    ~FileIO();
};

#endif