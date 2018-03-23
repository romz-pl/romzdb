#include "db.h"
#include "dirpage.h"


//
//
//
Db::Db( const std::string &path, std::size_t frameNo )
    : m_space( path, 10 * 1024 * 1024 )
    , m_bufferMgr( m_space, frameNo )
{


}

//
//
//
HeapFile Db::CreteHeapFile()
{
    const PageId headerPage = m_bufferMgr.GetNew();

    CreateHeaderPage( headerPage );
    HeapFile hf( m_bufferMgr, headerPage );

    return hf;
}

//
//
//
void Db::CreateHeaderPage( PageId headerPage )
{
    DirPage dp( m_bufferMgr, headerPage );
    dp.ToPage();
}
