#include "scaner.h"

//
//
//
Scaner::Scaner( const HeapFile& hf )
{
    hf.get_all_rids( m_rid );
}
