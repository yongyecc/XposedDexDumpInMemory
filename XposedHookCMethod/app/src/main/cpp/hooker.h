//
// Created by Root on 2020/6/22.
//

#ifndef XPOSEDHOOKCMETHOD_hooker_H
#define XPOSEDHOOKCMETHOD_hooker_H

#include <cstdint>
#include <unordered_map>
#include <unistd.h>
#include <sys/mman.h>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <assert.h>
#include <android/log.h>

#define TAG "yongyecc"

#define PAGE_START(x,pagesize)	((x) &~ ((pagesize) - 1))
#define CODE_RWX		PROT_READ | PROT_WRITE | PROT_EXEC
#define CODE_RX			PROT_READ | PROT_EXEC

class hooker {

public:
    virtual void hook(void *func, void *newAddr, bool saveOrig = true) const = 0;
    void unhook(void *func) const;

protected:
    mutable std::unordered_map<long, long> gHookedMap;

    void changeCodeAttrs(void *func, int attr) const;
    void saveOriginFuncBytes(void *func) const;

    virtual size_t getHookHeadSize() const = 0;
    virtual void overFuncByJMPOpcode(void *func, void *newAddr) const = 0;
    virtual size_t getOrigFunctionSize() const {
        return getHookHeadSize();
    }

    virtual ~hooker() ;
};


#endif //XPOSEDHOOKCMETHOD_hooker_H
