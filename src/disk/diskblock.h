#ifndef ROMZDB_DISK_DISKSBLOCK_H
#define ROMZDB_DISK_DISKSBLOCK_H

//
// 1. Represents the block on the disk
//
// 2. All disk block have the same size.
//


#include <array>
#include <string>
#include "unixfile.h"
#include "pageid.h"
#include "diskblock.h"

class DiskBlock
{
    friend class Page;
public:
    // The size of the disk block in the file.
    enum { Size = 4096 };

public:
    DiskBlock();
    DiskBlock( const std::string& data );
    ~DiskBlock() = default;

    bool operator== ( const DiskBlock& block ) const;

    void Read( const UnixFile& uf, PageId pageId );
    void Write( const UnixFile& uf, PageId pageId ) const;

private:
    const char* GetData() const;
    char* GetData();

    off_t PageIdToOffset( PageId pageId ) const;

private:
    // Data stored on the disk block
    std::array< char, Size > m_data;
};

#endif
