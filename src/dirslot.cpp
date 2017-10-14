#include "dirslot.h"

DirSlot::DirSlot( PageId pageId, PageOffset freeSpace )
    : m_pageId( pageId )
    , m_freeSpace( freeSpace )
{

}
