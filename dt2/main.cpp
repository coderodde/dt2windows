#include "dt_tag_entry.hpp"
#include "dt_tag_entry_list.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <ShlObj.h>
#include <windows.h>

const std::string FLAG_LIST_TAGS_AND_DIRECTORIES = "-l";
const std::string FLAG_LIST_TAGS_NO_DIRECTORIES = "-n";
const std::string TAG_ENTRY_LIST_FILE_LOCATION = "\\.dt\\table";
const size_t MAXIMUM_TAG_LENGTH = 12;

using net::coderodde::dt2::TagEntryList;

static void operator>>(std::ifstream& inputFileStream, TagEntryList& tagEntryList) {
    char lineBuffer[MAXIMUM_TAG_LENGTH + MAX_PATH];

    while (!inputFileStream.eof() && !inputFileStream.bad() && !inputFileStream.fail()) {
        inputFileStream.getline(lineBuffer, MAXIMUM_TAG_LENGTH + MAX_PATH);
        std::stringstream ss;
        std::string tag;
        std::string directory;
        ss << lineBuffer;
        ss >> tag >> directory;
        TagEntry tagEntry(tag, directory);
        tagEntryList << tagEntry;
    }
}

static void listTagsAndDirectories() {

}

static void listTagsOnly() {

}

static void matchTag(std::string const& tag) {

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
    /*if (argc == 1 || argc >= 3) {
        // Since dt requires only one argument, silently exit if any other
        // number of arguments passed.
        return 0;
    }*/

    std::string flag = "yeah"; // argv[1];
    std::string tagEntryListFileName = getTagEntryListFileName();
    std::ifstream ifs(tagEntryListFileName.c_str(), std::ifstream::in);
    TagEntryList tagEntryList;
    ifs >> tagEntryList;

    if (flag == FLAG_LIST_TAGS_AND_DIRECTORIES) {
        listTagsAndDirectories();
    }
    else if (flag == FLAG_LIST_TAGS_NO_DIRECTORIES) {
        listTagsOnly();
    }
    else {
        //std::string targetTag = argv[1];
        //matchTag(targetTag);
    }

    std::cin.get();
    return 0;
}