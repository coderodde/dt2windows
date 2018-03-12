#include "dt_tag_entry.hpp"
#include <string>

net::coderodde::dt2::TagEntry::TagEntry(std::string const& tag, 
                                        std::string const& directory)
:
m_tag(tag),
m_directory(directory) {

}