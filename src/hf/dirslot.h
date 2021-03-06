#ifndef ROMZDB_DIRSLOT_H
#define ROMZDB_DIRSLOT_H

#include <optional>
#include "disk/pageid.h"
#include "record.h"
#include "buffer/buffermgr.h"
#include "recordid.h"

class DirSlot
{
public:
    // DirSlot();
    // ~DirSlot() = default;

    std::optional< RecordId > insert_record( BufferMgr &buffer, const Record &rec );
    bool remove_record( BufferMgr& buffer, RecordId record_id );

    bool alloc_page( BufferMgr& buffer);

    std::uint32_t get_record_no( BufferMgr& buffer ) const;

    void make_invalid();

    void get_all_records( BufferMgr& buffer, std::vector< Record>& all ) const;
    void get_all_rids( BufferMgr& buffer, std::vector< RecordId >& all ) const;

private:
    bool is_empty() const;


private:
    bool m_valid;

    // Page into the "DirSlot" points
    PageId m_page_id;

    // Free space on the "m_pageId" page
    std::uint32_t m_free_space;

    static const std::uint32_t m_max_free_space;
};

#endif
