#ifndef NET_CODERODDE_DT2_TAG_ENTRY_HPP
#define NET_CODERODDE_DT2_TAG_ENTRY_HPP

#include <string>

namespace net {
namespace coderodde {
namespace dt2 {

    class TagEntry {
    public:
        TagEntry(std::string const& tag, std::string const& directory);
        TagEntry();
        TagEntry(TagEntry const& other);
        TagEntry& operator=(TagEntry&& other);
        TagEntry& operator=(TagEntry const& other);
        std::string const& getTag() const;
        std::string const& getDirectory() const;

    private:
        std::string m_tag;
        std::string m_directory;
    };
        
} // End of namespace 'net::coderodde::dt2'.
} // End of namespace 'net::coderodde'.
} // End of namespace 'net'.

#endif // NET_CODERODDE_DT2_TAG_ENTRY_HPP