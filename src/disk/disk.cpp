#include "disk.h"

//
//
//
Disk::Disk( const std::string& path, UnixFile::Mode mode )
    : m_uf( path, mode )
    , m_ds( m_uf )
{

}

//
//
//
DiskBlock Disk::Read( PageId pageId ) const
{
    return m_ds.Read( pageId );
}

//
//
//
void Disk::Write( const DiskBlock& block, PageId pageId ) const
{
    m_ds.Write( block, pageId );
}

//
//
//
PageId Disk::Allocate()
{
    return m_ds.AllocatePage();
}
