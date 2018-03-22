#include <cassert>
#include <stdexcept>
#include "dbfile.h"


//
//
//
DbFile::DbFile( const std::string& path, UnixFile::Mode mode )
    : m_uf( path, mode )
{
    //
    // class Util
    // {
    // public:
    //  static int ceil( int numerator, int denominator )
    //  {
    //    assert( denominator != 0 );
    //    const std::div_t res = std::div( numerator, denominator );
    //    return res.quat + !!res.num;
    //  }
    // };
    //
    // class SpaceMap
    //    {
    //        SpaceMap( std::uint32_t blockNo );
    //        std::uint32_t BitsOnBlock() const { return 8 * DiskBlock::Size; }
    //        void Read( const UnixFile& uf );
    //        void Write( const UnixFile& uf ) const;
    //        bool Is( BlockId blockId ) const;
    //        BlockId Alloc();
    //        void Dealloc( BlockId blockId );
    //    };
    //    
    //    
    // maximal size of file in bytes
    // std::uint32_t maxSize; 
    //
    // Number of bits per block used in space map
    // const std::uint32_t bitsOnBlock = 8 * DiskBlock::Size;
    //
    // maximal number of bloks in file
    // const std::uint32_t maxBlockNo = maxSize / DiskBlock::Size; 
    //
    // Number of blocks with data in file
    // const std::uint32_t dataBlockNo = maxBlockNo * bitsOnBlock / (bitsOnBlock + 1); 
    //
    // Number of blocks with map space in file
    // const std::uint32_t mapBlockNo = Utils::ceil( dataPageNo, bitsOnBlock );
    //
    // assert( dataBlockNo + mapBlockNo <= maxBlockNo );
        
    assert( m_spaceMap.byte_no() == DiskBlock::Size );

    if( mode == UnixFile::Mode::Open )
    {
        m_uf.Read( m_spaceMap.data(), m_spaceMap.byte_no(), 0 );
    }
    else if( mode == UnixFile::Mode::Create )
    {
        m_spaceMap.set( 0 );
        m_uf.Write( m_spaceMap.data(), m_spaceMap.byte_no(), 0 );
    }
}

//
//
//
DbFile::~DbFile()
{
    m_uf.Write( m_spaceMap.data(), m_spaceMap.byte_no(), 0 );
}

//
//
//
DiskBlock DbFile::Read( BlockId blockId ) const
{
    assert( blockId.GetValue() != 0 ); // Block 0 is reserved for space map
    if( !m_spaceMap.test( blockId.GetValue() ) )
    {
        throw std::runtime_error( "DbFile::Read: Block not in file" );
    }

    DiskBlock block;
    block.Read( m_uf, blockId );
    return block;
}

//
//
//
void DbFile::Write( const DiskBlock& block, BlockId blockId ) const
{
    assert( blockId.GetValue() != 0 ); // Block 0 is reserved for space map
    if( !m_spaceMap.test( blockId.GetValue() ) )
    {
        throw std::runtime_error( "DbFile::Write: Block not in file" );
    }

    block.Write( m_uf, blockId );
}

//
//
//
BlockId DbFile::Alloc()
{
    for( std::uint32_t i = 0; i < m_spaceMap.byte_no(); i++ )
    {
        if( !m_spaceMap.test( i ) )
        {
            m_spaceMap.set( i );
            const BlockId blockId( i );
            DiskBlock().Write( m_uf, blockId );
            return blockId;
        }
    }
    throw std::runtime_error( "DbFile::Alloc: File full" );
}

//
//
//
void DbFile::Dealloc( BlockId blockId )
{
    if( !m_spaceMap.test( blockId.GetValue() ) )
    {
        throw std::runtime_error( "DbFile::Dealloc: Page was not allocated" );
    }

    m_spaceMap.reset( blockId.GetValue() );
}

