#pragma once

#define DECL_SYSCALL0(fn) int syscall_##fn()
#define DECL_SYSCALL1(fn, p1) int syscall_##fn(p1)
#define DECL_SYSCALL2(fn, p1, p2) int syscall_##fn(p1, p2)
#define DECL_SYSCALL3(fn, p1, p2, p3) int syscall_##fn(p1, p2, p3)
#define DECL_SYSCALL4(fn, p1, p2, p3, p4) int syscall_##fn(p1, p2, p3, p4)
#define DECL_SYSCALL5(fn, p1, p2, p3, p4, p5) int syscall_##fn(p1, p2, p3, p4, p5)

#define DEFN_SYSCALL0(fn, num)    \
    int syscall_##fn()            \
    {                             \
        int a;                    \
        asm volatile("int $0x80"  \
                     : "=a"(a)    \
                     : "0"(num)); \
        return a;                 \
    }

#define DEFN_SYSCALL1(fn, num, P1)                                     \
    int syscall_##fn(P1 p1)                                            \
    {                                                                  \
        int __res;                                                     \
        asm volatile("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx" \
                     : "=a"(__res)                                     \
                     : "0"(num), "r"((int)(p1)));                      \
        return __res;                                                  \
    }

#define DEFN_SYSCALL2(fn, num, P1, P2)                                 \
    int syscall_##fn(P1 p1, P2 p2)                                     \
    {                                                                  \
        int __res;                                                     \
        asm volatile("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx" \
                     : "=a"(__res)                                     \
                     : "0"(num), "r"((int)(p1)), "c"((int)(p2)));      \
        return __res;                                                  \
    }

#define DEFN_SYSCALL3(fn, num, P1, P2, P3)                                        \
    int syscall_##fn(P1 p1, P2 p2, P3 p3)                                         \
    {                                                                             \
        int __res;                                                                \
        asm volatile("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx"            \
                     : "=a"(__res)                                                \
                     : "0"(num), "r"((int)(p1)), "c"((int)(p2)), "d"((int)(p3))); \
        return __res;                                                             \
    }

#define DEFN_SYSCALL4(fn, num, P1, P2, P3, P4)                                                    \
    int syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4)                                                  \
    {                                                                                             \
        int __res;                                                                                \
        asm volatile("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx"                            \
                     : "=a"(__res)                                                                \
                     : "0"(num), "r"((int)(p1)), "c"((int)(p2)), "d"((int)(p3)), "S"((int)(p4))); \
        return __res;                                                                             \
    }

#define DEFN_SYSCALL5(fn, num, P1, P2, P3, P4, P5)                                                                \
    int syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)                                                           \
    {                                                                                                             \
        int __res;                                                                                                \
        asm volatile("push %%ebx; movl %2,%%ebx; int $0x80; pop %%ebx"                                            \
                     : "=a"(__res)                                                                                \
                     : "0"(num), "r"((int)(p1)), "c"((int)(p2)), "d"((int)(p3)), "S"((int)(p4)), "D"((int)(p5))); \
        return __res;                                                                                             \
    }

DECL_SYSCALL1(exit, int);
DECL_SYSCALL1(print, const char *);