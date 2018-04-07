#ifndef ROMZDB_DIRPAGE_H
#define ROMZDB_DIRPAGE_H

#include <optional>
#include <vector>
#include "buffer/buffermgr.h"
#include "dirslot.h"
#include "page.h"
#include "recordid.h"


class DirPage : public Page
{
public:
    explicit DirPage( BufferMgr& buffer );
    DirPage( BufferMgr& buffer, PageId page_id );
    ~DirPage() = default;

    PageId get_next_page() const;
    void set_next_page( PageId id );

    std::optional< RecordId > insert_record( const Record &rec );
    bool remove_record( RecordId record_id );

    bool alloc_page();
    bool dispose_page( PageId page_id );


    bool is_next_page() const;

    static std::uint32_t max_slot_no();

    std::uint32_t get_record_no() const;

private:
    void init( );
    DirSlot* get_slot();
    const DirSlot* get_slot() const;

private:
    enum Offset
    {
        Next_page = 0,
        Slot = Next_page + sizeof( PageId )
    };

private:

    static const PageId m_invalid_page_id;
};

#endif
