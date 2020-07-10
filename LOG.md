# Xposed脱内存加载壳

## 思路

1. xposed插件为每一个安装的apk注入一个包含脱壳功能的so模块
2. so模块主要inline hook加固样本的原DEX加载函数，在加载的时候将DEX dump到指定目录下


# 问题
A/libc: Fatal signal 11 (SIGSEGV), code 128, fault addr 0x0 in tid 1985 (com.h)
InlineHook 中执行桩函数时出现的崩溃，准备IDA调试跟踪

上传inline hook模块 dexdump.so
adb push "d:\Android Project\XposedHookCMethod\app\build\outputs\apk\debug\libdexdump.so" /data/local/tmp/test.so

IDA调试：
adb shell  /data/local/tmp/android_x86_server
adb forward tcp:23946 tcp:23946
adb shell  am start -D com.h/.M
attach->Linux Debugger

adb forward tcp:8700 jdwp:$PID
jdb -connect com.sun.jdi.SocketAttach:hostname=localhost,port=8700


根据日志：Hook前 openMemory22处指令值：ec835356 找到对应API并下断点

libart->openMemory ： 0xf3875150 [ec 83 53 56]
hook函数 dexdump(F0232680) -> [53 e5 89 55]

但是从原函数执行到桩函数dexdump时：libart->openMemory：0xf3875150 的指令却不是 e51ff004
【？】有可能是ida执行的字节码在inlinehook后没有被刷新  -> IDA调试时查看 0xf3875150 处字节码是不是 e51ff004
HOOK后，openmemory函数地址字节码是 e51ff004 但是没有被识别为跳转指令
调试发现没有PC寄存器，结合attach liunx选项，可以知道执行该 so文件是按照 x86 指令集来运行，也就是要让inline 生效需要arm架构so文件

【？】怎么让跳转指令生效？
构造x86跳转指令： 
E9 addr(dexdump) - addr(openMemory) - 5  //5是跳转指令的长度，从本条指令之后跳N字节偏移
= FC9B D530‬


修改inlinehook.cpp代码添加x86对应跳转指令，实现hook


从桩函数跳转回原流程出错，开源代码是针对ARM架构的，自己实现x86架构对应跳回功能

被覆盖的原始指令： 56 53 83 EC 34

了解x86 inlinehook 流程
#http://rk700.github.io/2017/05/18/x86-inline-hook-mistake/


#https://www.cnblogs.com/luoyesiqiu/p/12306336.html
1.构造裸函数来调用hook函数，然后执行原函数、jmp回原函数
怎么构造裸函数

#https://github.com/liuyx/inline-hook
linux平台hook代码
2. 分析hook代码查看
WSL编译项目
```
sudo apt-get update
sudo apt-get install build-essential gdb
sudo apt-get install cmake
```

# 目的：实现x86 架构 Inline Hook

Ubuntu + IDA 动态调试ELF
```
//启动IDA服务
chmod +x ./linux_server64 
./linux_server64

//IDA远程连接
Debugger -> Run -> Remote Linux debugger 
Application:    /home/ubuntu/inline-hook/bin/hooker-test
Directory:  /home/ubuntu/inline-hook/bin
Hostname:   192.168.235.131
```

调试发现由于使用 JMP指令来执行 hook函数，所以 hook函数执行到最后 RET命令就会返回原程序

继续分析代码看怎么运行被覆盖的那些指令
思路：将被覆盖的指令在 hook函数执行完毕后还原，如果要执行原函数的话就在 call完hook函数后再 call还原后的原函数


被覆盖的函数已经保存在内存中，为什么会segment fault，原因？
对比正常执行strcmp，查看异常原因？
```
[heap]:0000556EE3EE9F40 89 F1                   mov     ecx, esi
[heap]:0000556EE3EE9F42 89 F8                   mov     eax, edi
[heap]:0000556EE3EE9F44 48 83 E1 3F             and     rcx, 3Fh
[heap]:0000556EE3EE9F48 48 83 E0 3F             and     rax, 3Fh
[heap]:0000556EE3EE9F4C 83 F9 30                cmp     ecx, 30h ; '0'
[heap]:0000556EE3EE9F4F 77 3F                   ja      short loc_556EE3EE9F90
[heap]:0000556EE3EE9F51 83 F8 30                cmp     eax, 30h ; '0'
[heap]:0000556EE3EE9F54 77 3A                   ja      short loc_556EE3EE9F90
[heap]:0000556EE3EE9F56 66 0F 12 0F             movlpd  xmm1, qword ptr [rdi]
[heap]:0000556EE3EE9F5A 66 0F 12 16             movlpd  xmm2, qword ptr [rsi]
[heap]:0000556EE3EE9F5E 66 0F 16 4F 08          movhpd  xmm1, qword ptr [rdi+8]
[heap]:0000556EE3EE9F63 66 0F 16 56 08          movhpd  xmm2, qword ptr [rsi+8]
[heap]:0000556EE3EE9F68 66 0F EF C0             pxor    xmm0, xmm0
[heap]:0000556EE3EE9F6C 66 0F 74 C1             pcmpeqb xmm0, xmm1
[heap]:0000556EE3EE9F70 66 0F 74 CA             pcmpeqb xmm1, xmm2
[heap]:0000556EE3EE9F74 66 0F F8 C8             psubb   xmm1, xmm0
[heap]:0000556EE3EE9F78 66 0F D7 D1             pmovmskb edx, xmm1
[heap]:0000556EE3EE9F7C 81 EA FF FF 00 00       sub     edx, 0FFFFh
[heap]:0000556EE3EE9F82 0F 85 E8 11 00 00       jnz     near ptr unk_556EE3EEB170		这一步跑丢的
[heap]:0000556EE3EE9F88 48 83 C6 10             add     rsi, 10h
[heap]:0000556EE3EE9F8C 48 83 C7 10             add     rdi, 10h
```


unk_556EE3EEB170 这个offset改变了，who引起的呢？
从opcode发现，opcode相同而offset位置不同了，因为这块堆内存只是申请存放被覆盖函数的opcode，当出现跨函数的call啊，jmp等指令就会出现找不到对应的代码

思路1. 可以解析执行指令代码，出现call啊，jmp的，把函数调用到的所有指令都复制过来

思路2. 再hook函数头部先恢复原函数，结束执行完后再覆盖
析构hooker HookerFactory->default_delete 出现segment fault

原指令存放再第一个对象hooker对象内，而恢复使用的是一个没有保存被覆盖函数指令的hooker对象


# 脱壳so模块

回到脱壳so模块的功能上来

1. 构造`JMP`指令覆盖加固样本加载原DEX的API，跳转到桩函数处
2. 桩函数
	1) 执行dump原DEX的功能
	2) 还原被hook API头部的指令
	3) 执行被hook API

提取inline hook项目里的x86hook部分到xposedhookCMethod的dump模块中，替换掉c代码

完成hooker.cpp里的类初始化操作

单取 HookerX86 类出来使用，需要解决未实现虚函数而导致【error: undefined reference to 'vtable for HookerX86'】得问题
#https://stackoverflow.com/questions/7720205/linking-error-undefined-reference-to-vtable-for-xxx
为所有虚函数提供一个实现

同样得代码在Visual Studio上可以运行，注释所有函数，每次放一个函数来编译，发现问题出现在纯虚函数`virtual void doHook(void *func, void *newAddr, void **origFunc) const = 0;`上，需要在头文件里实现才可以编译成功，这谁顶得住
原因是cmake配置没有写对，让编译器没有识别得代码cpp

cmake target_link_libraries自定义库依赖的时候不要用`${}`包裹，否则会出现`【 error: undefined reference to 'vtable for 】`错误，因为使用`${}`包裹后就`inlinehook`被识别为`VAR`而不是一个`NAME`，所以找不到编译好的库`inlinehook`，加上dexdump只导入了`HookerX86.h`文件，没有包含需要的依赖`hooker`这个父类
```
add_library(inlinehook SHARED ${LIB_SRC})
add_library(dexdump SHARED dexdump.cpp)
target_link_libraries(dexdump ${inlinehook})
```

