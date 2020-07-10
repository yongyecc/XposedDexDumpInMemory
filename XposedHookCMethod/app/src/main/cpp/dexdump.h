//
// Created by joker on 2020-04-27.
//
#include <sys/types.h>
#include <string>


#ifndef XPOSEDHOOKCMETHOD_DEXDUMP_H
#define XPOSEDHOOKCMETHOD_DEXDUMP_H


#define Elf_Ehdr Elf32_Ehdr
#define Elf_Shdr Elf32_Shdr
#define Elf_Sym  Elf32_Sym


struct ctx {
    void *load_addr;
    void *dynstr;
    void *dynsym;
    int nsyms;
    off_t bias;
};


typedef void *(*org_artDexFileOpenMemory22)(const uint8_t *base, size_t size,
                                            const std::string &location, uint32_t location_checksum,
                                            void *mem_map, const void *oat_file, std::string *error_msg);


void *dumpdex(const uint8_t *base, size_t size,
             const std::string &location, uint32_t location_checksum,
             void *mem_map, const void *oat_file, std::string *error_msg);

void *fake_dlopen(const char *libpath, int flags);

void *fake_dlsym(void *handle, const char *name);


#endif //XPOSEDHOOKCMETHOD_DEXDUMP_H
