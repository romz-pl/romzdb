#ifndef ROMZDB_DISKSBLOCK_H
#define ROMZDB_DISKSBLOCK_H

//
// 1. Represents the block on the disk
//
// 2. All disk block have the same size.
//


#include <array>

class DiskBlock
{
public:
    // The size of the disk block in the file.
    enum { Size = 4096 };

public:
    DiskBlock();
    ~DiskBlock() = default;

// private:
    const char* GetData() const;
    char* GetData();

private:
    // Data stored on the disk block
    std::array< char, Size > m_data;
};

#endif
