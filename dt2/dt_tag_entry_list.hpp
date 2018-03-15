#ifndef NET_CODERODDE_DT2_TAG_ENTRY_LIST_HPP
#define NET_CODERODDE_DT2_TAG_ENTRY_LIST_HPP

#include "dt_tag_entry.hpp"
#include <vector>

using net::coderodde::dt2::TagEntry;

namespace net {
namespace coderodde {
namespace dt2 {

    class TagEntryList {
    public:
        void operator<<(TagEntry const& tagEntry);
        TagEntry operator[](std::string const& tag) const;
        std::vector<TagEntry>::const_iterator begin() const;
        std::vector<TagEntry>::const_iterator end() const;
        void sortByTags();
        void sortByDirectories();

    private:
        std::vector<TagEntry> m_entries;
    };

} // End of namespace 'net::coderodde::dt2'.
} // End of namespace 'net::coderodde'.
} // End of namespace 'net'.

#endif // NET_CODERODDE_DT2_TAG_ENTRY_LIST_HPP