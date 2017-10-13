#ifndef ROMZDB_PAGEID_H
#define ROMZDB_PAGEID_H

//
// Uniquely identifies a page on the disk.
//

#include <cstddef>
#include <limits>

using PageId = std::size_t;

const PageId InvalidPageId = std::numeric_limits< PageId >::max();


#endif
