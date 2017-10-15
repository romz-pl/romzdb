#include "db.h"
#include "dirpage.h"


//
//
//
Db::Db( const std::string &path, std::size_t frameNo )
{
    //
    // If C++14, then instead using "reset" function, use std::make_unique< T >
    //

    m_unixFile.reset( new UnixFile ( path, UnixFile::Mode::Create ) );
    m_ds.reset( new DiskSpaceMgr( *m_unixFile ) );
    m_bufferMgr.reset( new BufferMgr( *m_ds, frameNo ) );
}

//
//
//
HeapFile Db::CreteHeapFile()
{
    const PageId headerPage = m_bufferMgr->GetNew().first;
    m_bufferMgr->Unpin( headerPage );

    {
        DirPage dp( *m_bufferMgr, headerPage );
        dp.ToPage();
    }

    HeapFile hf( *m_bufferMgr, headerPage );

    return hf;
}
