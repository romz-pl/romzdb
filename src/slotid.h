#ifndef ROMZDB_RECORDID_H
#define ROMZDB_RECORDID_H


//
// Uniquely identifies a record on the page.
//

#include <cstddef>
#include "page.h"
#include <limits>

using SlotId = std::uint16_t;

static_assert( std::numeric_limits< SlotId >::max() > Page::PageSize,
               "The size of SlotId type is not sufficiently large" );

#endif
