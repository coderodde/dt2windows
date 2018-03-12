#include "dt_tag_entry_list.hpp"

void net::coderodde::dt2::TagEntryList::operator<<(TagEntry const& tagEntry) {
    m_entries.push_back(tagEntry);
}