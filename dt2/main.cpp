#include "dt_tag_entry.hpp"
#include "dt_tag_entry_list.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <ShlObj.h>
#include <windows.h>

const std::string FLAG_LIST_TAGS_AND_DIRECTORIES = "-L";
const std::string FLAG_LIST_TAGS_NO_DIRECTORIES = "-l";
const std::string FLAG_LIST_TAGS_AND_DIRECTORIES_SORTED = "-S";
const std::string FLAG_LIST_TAGS_NO_DIRECTORIES_SORTED = "-s";
const std::string FLAG_LIST_TAGS_AND_DIRECTORIES_SORTED_BY_DIRS = "-d";
const std::string TAG_ENTRY_LIST_FILE_LOCATION = "\\.dt\\table";
const size_t LINE_BUFFER_CAPACITY = 1024;

using net::coderodde::dt2::TagEntry;
using net::coderodde::dt2::TagEntryList;

//// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}
///////////////////////////////////////////////////////////////////////////////

static void operator>>(std::ifstream& inputFileStream, TagEntryList& tagEntryList) {
    char lineBuffer[LINE_BUFFER_CAPACITY];

    while (!inputFileStream.eof() && !inputFileStream.bad() && !inputFileStream.fail()) {
        inputFileStream.getline(lineBuffer, LINE_BUFFER_CAPACITY);
        std::stringstream ss;
        std::string tag;
        std::string directory;
        ss << lineBuffer;
        ss >> tag;
        char directoryBuffer[MAX_PATH];
        ss.getline(directoryBuffer, MAX_PATH);
        directory = directoryBuffer;
        trim(directory);
        TagEntry tagEntry(tag, directory);
        tagEntryList << tagEntry;
    }
}

static size_t getMaximumTagLength(TagEntryList const& tagEntryList) {
    auto const& maximumLengthTagIter = 
        std::max_element(tagEntryList.begin(), 
                         tagEntryList.end(), 
                         [](TagEntry const& tagEntry1,
                            TagEntry const& tagEntry2) { 
                            return tagEntry1.getTag().length() < 
                                   tagEntry2.getTag().length();
    });

    return maximumLengthTagIter->getTag().length();
}

static void listTags(TagEntryList const& tagEntryList) {
    for (const TagEntry& tagEntry : tagEntryList) {
        std::cout << tagEntry.getTag() << "\n";
    }
}

static void listTagsAndDirectories(TagEntryList const& tagEntryList) {
    size_t maximumTagLength = getMaximumTagLength(tagEntryList);

    for (TagEntry const& tagEntry : tagEntryList) {
        std::cout << std::setw(maximumTagLength) << std::left << tagEntry.getTag()
            << " " << tagEntry.getDirectory() << "\n";
    }
}

static std::string matchTag(TagEntryList const& tagEntryList, std::string const& tag) {
    try {
        TagEntry bestTagEntry = tagEntryList[tag];
        return bestTagEntry.getDirectory();
    } catch (std::runtime_error const& err) {
        std::exit(1);
    }
}

static std::string getTagEntryListFileName() {
    char fileNameBuffer[MAX_PATH];

    if (!SUCCEEDED(SHGetFolderPathA(NULL,
                                    CSIDL_PROFILE, 
                                    NULL, 
                                    0, 
                                    fileNameBuffer))) {
        throw std::runtime_error("Could not get the home directory name.");
    }

    std::string tagEntryListFileName = fileNameBuffer;
    tagEntryListFileName += TAG_ENTRY_LIST_FILE_LOCATION;
    return tagEntryListFileName;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        // Since dt requires only one argument, silently exit if any other
        // number of arguments passed.
        return 0;
    }

    std::string flag = argv[1];
    std::string tagEntryListFileName = getTagEntryListFileName();
    std::ifstream ifs(tagEntryListFileName.c_str(), std::ifstream::in);
    TagEntryList tagEntryList;
    ifs >> tagEntryList;

    if (flag == FLAG_LIST_TAGS_AND_DIRECTORIES_SORTED
        || flag == FLAG_LIST_TAGS_NO_DIRECTORIES_SORTED) {
        tagEntryList.sortByTags();
    }
    else if (flag == FLAG_LIST_TAGS_AND_DIRECTORIES_SORTED_BY_DIRS) {
        tagEntryList.sortByDirectories();
    }

    if (flag == FLAG_LIST_TAGS_AND_DIRECTORIES
        || flag == FLAG_LIST_TAGS_AND_DIRECTORIES_SORTED
        || flag == FLAG_LIST_TAGS_AND_DIRECTORIES_SORTED_BY_DIRS) {
        listTagsAndDirectories(tagEntryList);
    }
    else if (flag == FLAG_LIST_TAGS_NO_DIRECTORIES
        || flag == FLAG_LIST_TAGS_NO_DIRECTORIES_SORTED) {
        listTags(tagEntryList);
    } else {
        // Once here, we are expecting argv[1] to be a tag.
        std::cout << tagEntryList[argv[1]].getDirectory();
    }

    std::cin.get();
    return 0;
}