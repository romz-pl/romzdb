#include "diskblock.h"
#include <algorithm>

//
//
//
DiskBlock::DiskBlock()
{
    std::fill( m_data.begin(), m_data.end(), 0 );
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
