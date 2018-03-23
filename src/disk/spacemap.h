#ifndef ROMZDB_DISK_SPACEMAP_H
#define ROMZDB_DISK_SPACEMAP_H

#include "unixfile.h"
#include "blockid.h"
#include "bitarray.h"


class SpaceMap
{
public:
    SpaceMap( UnixFile& uf );
    SpaceMap( UnixFile& uf, std::uint32_t max_size );
    ~SpaceMap();

    bool is_allocated( BlockId blockId ) const;
    bool is_valid( BlockId blockId ) const;

    BlockId allocate();
    void free( BlockId blockId );

    std::uint32_t max_data_block_no( ) const;
    std::uint32_t curr_data_block_no( ) const;

private:
    void open( );
    void create( std::uint32_t max_size );
    void write();

    std::uint32_t bits_in_header() const;
    std::uint32_t bits_on_block() const;

private:
    std::uint32_t m_version = 1;

    std::uint32_t m_map_block_no;

    std::uint32_t m_data_block_no;

    UnixFile& m_uf;

    BitArray *m_bitMap;


};


#endif
