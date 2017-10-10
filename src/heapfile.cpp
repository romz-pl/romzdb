#include "heapfile.h"
#include <cassert>

//
//
//
HeapFile::HeapFile( PageId headerPage, Mode mode )
    : m_headerPage( headerPage )
{
    if( mode == Mode::Open )
    {
        Open( );
        return;
    }

    if( mode == Mode::Create )
    {
        Create( );
        return;
    }

    assert( 0 );
}

//
//
//
void HeapFile::Open( )
{

}

//
//
//
void HeapFile::Create( )
{

}

//
//
//
Record HeapFile::Get( RecordId rid )
{

}

//
//
//
void HeapFile::Insert( RecordId rid, const Record& rec )
{

}

//
//
//
void HeapFile::Delete( RecordId rid )
{

}

//
//
//
void HeapFile::Update( RecordId rid, const Record& rec )
{
    //
    // This can be done more efficiently for records of the same length.
    // But deleting "old" record, and inserting "new" record is straithforward.
    //

    Delete( rid );
    Insert( rid, rec );
}
