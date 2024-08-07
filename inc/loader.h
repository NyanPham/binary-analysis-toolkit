#ifndef LOADER_H
#define LOADER_H

#include <stdint.h>
#include <string>
#include <vector>

// In 2.34+ versions of ubuntu, the bfd_get_section_flags marco is deleted.
// Declare our own
#define bfd_get_section_flags(bfd, ptr) ((void) bfd, (ptr)->flags)

class Binary;
class Section;
class Symbol;

class Symbol
{
public:
    enum SymbolType
    {
        SYM_TYPE_UKN = 0,
        SYM_TYPE_FUNC = 1,  
    };

    Symbol() 
        : type { SYM_TYPE_UKN }
        , name {}
        , addr { 0 }
    {}

    SymbolType type;
    std::string name;
    uint64_t addr;
};

class Section
{
public:
    enum SectionType
    {
        SEC_TYPE_NONE = 0,
        SEC_TYPE_CODE = 1,
        SEC_TYPE_DATA = 2,
    };

    Section()
        : binary { NULL }
        , type { SEC_TYPE_NONE }
        , vma { 0 }
        , size { 0 }
        , bytes { NULL }
    {}
    
    Binary *binary;
    std::string name;
    SectionType type;
    uint64_t vma;
    uint64_t size;
    uint8_t *bytes;
};

class Binary
{
public:
    enum BinaryType
    {
        BIN_TYPE_AUTO = 0,
        BIN_TYPE_ELF = 1,
        BIN_TYPE_PE = 2,
    };

    enum BinaryArch
    {
        ARCH_NONE = 0,
        ARCH_X86 = 1,
    };

    Binary()
        : type { BIN_TYPE_AUTO }
        , arch { ARCH_NONE }
        , bits { 0 }
        , entry { 0 }
    {}

    Section *get_text_section() 
    {
        for (auto& section: sections)
        {
            if (section.name == ".text")
                return &section;
        }

        return NULL;
    }

    std::string filename;
    BinaryType type;
    std::string type_str;
    BinaryArch arch;
    std::string arch_str;
    unsigned int bits;
    uint64_t entry;
    std::vector<Section> sections;
    std::vector<Symbol> symbols;
};

int load_binary(std::string &fname, Binary *bin, Binary::BinaryType type);
void unload_binary(Binary *bin);

#endif 