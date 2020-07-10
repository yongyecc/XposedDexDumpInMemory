//
// Created by Root on 2020/6/22.
//

#include "HookerX86.h"


/**
 * hook函数，修改原函数首部opcode为jmp指令，跳转到目标函数
 * @param func      原函数，即被hook函数
 * @param newAddr   目标函数
 * @param origFunc  原函数的opcode
 * @param saveOrig  标志位：是否保存原函数opcode
 */

void HookerX86::hook(void *func, void *newAddr, bool saveOrig) const {
    hooker::changeCodeAttrs(func, CODE_RWX);

    if(saveOrig){
        hooker::saveOriginFuncBytes(func);
    }

    overFuncByJMPOpcode(func, newAddr);
    hooker::changeCodeAttrs(func, CODE_RX);
}

/**
 * 构造JMP指令覆盖原函数头部实现函数跳转
 * @param func      原函数地址
 * @param newAddr   目标函数地址
 * @param origFunc
 */
void HookerX86::overFuncByJMPOpcode(void *func, void *newAddr) const{
    char *f = (char *)func;
    f[0] = 0x68;
    *(long *)&f[1] = (long)newAddr;
    f[5] = 0xc3;
}