#include <cassert>
#include "space.h"

//
//
//
Space::Space( )
{

}


//
//
//
Space::Space( DbFile &db_file )
{
    Add( db_file );
}

//
//
//
DbFileId Space::Add( DbFile& db_file )
{
    DbFileId db_file_id( m_db_file_map.size() );
    m_db_file_map[ db_file_id ] = &db_file;
    return db_file_id;
}

//
//
//
void Space::Remove( DbFileId id )
{
    auto it = m_db_file_map.find( id );
    if( it == m_db_file_map.end() )
    {
        throw std::runtime_error( "Space::Remove: Db File does not exist." );
    }

    m_db_file_map.erase( it );
}

//
//
//
DiskBlock Space::Read( PageId pageId ) const
{
    const DbFile *dbFile = get_db_file( pageId );
    const BlockId blockId = pageId.get_block_id();

    return dbFile->Read( blockId );
}

//
//
//
void Space::Write( const DiskBlock &block, PageId pageId ) const
{
    const DbFile *dbFile = get_db_file( pageId );
    const BlockId blockId = pageId.get_block_id();

    dbFile->Write( block, blockId );
}

//
//
//
PageId Space::Alloc()
{
    for( auto it : m_db_file_map )
    {
        DbFile* f = it.second;
        if( !f->full() )
        {
            const BlockId blockId = f->Alloc( );
            return PageId( blockId, it.first );
        }
    }

    throw std::runtime_error( "Space::Alloc: Space is full" );
}

//
//
//
void Space::Dealloc( PageId pageId )
{
    DbFile *dbFile = get_db_file( pageId );
    const BlockId blockId = pageId.get_block_id();

    dbFile->Dealloc( blockId );
}

//
//
//
DbFile* Space::get_db_file( PageId pageId )
{
    auto it = m_db_file_map.find( pageId.get_db_file_id() );
    if( it == m_db_file_map.end() )
    {
        throw std::runtime_error( "Space::get_db_file: Db File does not exist." );
    }

    return it->second;
}

//
//
//
const DbFile* Space::get_db_file( PageId pageId ) const
{
    auto it = m_db_file_map.find( pageId.get_db_file_id() );
    if( it == m_db_file_map.end() )
    {
        throw std::runtime_error( "Space::get_db_file const: Db File does not exist." );
    }

    return it->second;
}

//
//
//
bool Space::full() const
{
    for( auto it : m_db_file_map )
    {
        DbFile* f = it.second;
        if( !f->full() )
        {
            return false;
        }
    }
    return true;
}
