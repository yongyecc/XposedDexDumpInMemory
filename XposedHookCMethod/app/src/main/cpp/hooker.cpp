//
// Created by Root on 2020/6/22.
//

#include "hooker.h"


hooker::~hooker() {}

/**
 * 修改内存属性
 * @param func
 * @param attr
 */
void hooker::changeCodeAttrs(void *func, int attr) const {
    int pagesize = getpagesize();
    long startAddr = PAGE_START((long)func, pagesize);

    if(mprotect((void *)startAddr, pagesize, attr) < 0){
        __android_log_print(ANDROID_LOG_ERROR, TAG, "mprotect failed in hooker::changeCodeAttrs.");
        exit(0);
    }
}

/**
 * 保存函数opcode到 成员属性gHookedMap内存储
 * @param func
 */
void hooker::saveOriginFuncBytes(void *func) const {
    size_t originFunctionSize = getOrigFunctionSize();
    void * savedOpcodeBlock = malloc(originFunctionSize);
    memcpy(savedOpcodeBlock, func, originFunctionSize);
    gHookedMap[(long)func] = (long)savedOpcodeBlock;
}

/**
 *  将原函数opcode从map中取出并还原到原函数。
 * @param func
 */
void hooker::unhook(void *func) const {
    changeCodeAttrs(func, CODE_RWX);
    long srcFuncOpcodeAddr = gHookedMap[(long)func];

    if(srcFuncOpcodeAddr == 0){
        __android_log_print(ANDROID_LOG_ERROR, TAG, "src function opcode address obtain failed.");
    }

    memcpy(func, (void*)srcFuncOpcodeAddr, getOrigFunctionSize());
    free(reinterpret_cast<void *>(srcFuncOpcodeAddr));
    gHookedMap.erase((long)func);
}


