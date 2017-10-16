#include "scaner.h"

//
//
//
Scaner::Scaner( const HeapFile& hf )
{
    hf.GetRid( m_rid );
}
