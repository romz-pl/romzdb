#include "diskblock.h"
#include <cstring>
//
//
//
DiskBlock::DiskBlock()
{
    m_data.fill( 0 );
}

//
//
//
DiskBlock::DiskBlock( const std::string &data )
{
    const std::size_t s = std::min( data.size(), static_cast< std::size_t >( DiskBlock::Size ) );

    for( std::size_t i = 0; i < s; i++ )
    {
        m_data[ i ] = data[ i ];
    }
}

//
//
//
const char* DiskBlock::GetData() const
{
    return m_data.data();
}

//
//
//
char* DiskBlock::GetData()
{
    return m_data.data();
}

//
//
//
bool DiskBlock::operator== ( const DiskBlock& block ) const
{
    const int ret = std::memcmp( GetData(), block.GetData(), DiskBlock::Size );
    return ( ret == 0 );
}

//
//
//
off_t DiskBlock::PageIdToOffset( BlockId blockId ) const
{
    return blockId.GetValue() * static_cast< off_t >( DiskBlock::Size );
}

//
//
//
void DiskBlock::Read( const UnixFile& uf, BlockId blockId )
{
    const off_t offset = PageIdToOffset( blockId );
    uf.Read( GetData(), DiskBlock::Size, offset );
}
//
//
//
void DiskBlock::Write( const UnixFile& uf, BlockId blockId ) const
{
    const off_t offset = PageIdToOffset( blockId );
    uf.Write( GetData(), DiskBlock::Size, offset );
}
