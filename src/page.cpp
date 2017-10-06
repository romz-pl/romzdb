#include "page.h"
#include <cstring>


Page::Page()
{
    Zero();
}

//
// Zeroing the data
//
void Page::Zero( )
{
    std::memset( m_data, 0, m_size );
}

int Page::GetSize()
{
    return m_size;
}
