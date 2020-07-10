//
// Created by Root on 2020/6/22.
//

#ifndef XPOSEDHOOKCMETHOD_HOOKERX86_H
#define XPOSEDHOOKCMETHOD_HOOKERX86_H


#include "hooker.h"


class HookerX86 : public hooker {

public:
    void hook(void *func, void *newAddr, bool saveOrig = true) const;

private:
    void overFuncByJMPOpcode(void *func, void *newAddr) const;

    size_t getHookHeadSize() const{
        return 6;
    }

    ~HookerX86(){};
};



#endif //XPOSEDHOOKCMETHOD_HOOKERX86_H
