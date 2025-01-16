/* kapoorvm.h */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <birchutils.h>
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#pragma GCC diagnostic ignored "-Wformat-truncation="
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"

#define NoErr      0x00 /* 00 00 */
#define SysHlt     0x01 /* 00 01 */
#define ErrMem     0x02 /* 00 10 */
#define ErrSegv    0x04 /* 01 00 */
#define ErrInstr   0x08 /* 10 00 */ 

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;

#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32)
#define $8 (int64)
#define $c (char *)
#define $i (int)

#define $ax ->c.r.ax
#define $bx ->c.r.bx
#define $cx ->c.r.cx
#define $dx ->c.r.dx
#define $sp ->c.r.sp
#define $ip ->c.r.ip
        
#define $flags ->c.r.flags
#define equal(x)    (!!((x $flags & 0x08) >>3))
#define gt(x)       (!!((x $flags & 0x04) >>2))
#define higher(x)   (!!((x $flags & 0x02) >>1))
#define lower(x)    (!!(x $flags & 0x01))

#define segfault(x)     error((x), ErrSegv)

/*

    16 bit
    REGISTERS
        AX 16bit
            - AL 8bit
            - AH 8bit
        BX
        CX
        DX
        SP
        IP
        FLAGS

        1010
        ^____
        Zero flag
        Carry flag

        CMP AX,0x05

    65 KB memory
    (Serial COM Port)
    (Floppy Drive)

*/

typedef unsigned short int Reg;  //16 bit Registers
struct s_registers {
    Reg ax;
    Reg bx;
    Reg cx;
    Reg dx;
    Reg sp;
    Reg ip;
    Reg flags;
    /*  P1 E - Equal flag
        P2 G - Greater-than flag
        P3 H - Higher part of reg
        P4 L - Lower part of reg    */
};
typedef struct s_registers Registers;

struct s_cpu{
    Registers r;
};
typedef struct s_cpu CPU;

/*

    mov ax,0xabcd // (0x01 OR 0x02)
                // 0000 0008 = mov to cx
                // 0000 0000
                // 0000 abdc = abcd

*/

enum e_opcode {
    nop = 0x01,
    hlt = 0x02,
    mov = 0x08, /* 0x08 - 0x0f */
    ste = 0x10,
    cle = 0x11,
    stg = 0x12,
    clg = 0x13,
    sth = 0x14,
    clh = 0x15,
    stl = 0x16,
    cll = 0x17
    /*
        reserved = 0x18,
        reserved = 0x19
    */
};
typedef enum e_opcode Opcode;

struct s_instrmap {
    Opcode o;
    int8 s;
};
typedef struct s_instrmap IM;

typedef int16 Args;
struct s_instruction{
    Opcode o;
    Args a[]; /* 0-2 bytes*/
};
typedef struct s_instruction Instruction;

typedef unsigned char Errorcode;
typedef int8 Memory[((int16)(-1))];   //65 KB Memory
typedef int8 Program;

struct s_vm{
    CPU c;
    Memory m;
    int16 b; /* break line */
    // Program *p;
};
typedef struct s_vm VM;
typedef Memory *Stack;

// static Opcode opc;
static IM instrmap[] = {
    { nop, 0x01 },
    { hlt, 0x01 },
    { mov, 0x03 },
        {0x09,0x03},{0x0a,0x03},{0x0b,0x03},{0x0c,0x03},
        {0x0d,0x03},{0x0e,0x03},{0x0f,0x03}
};
#define IMs (sizeof(instrmap) / sizeof(struct s_instrmap))


void __mov(VM*,Opcode,Args,Args);

void error(VM*,Errorcode);
void executeinstr(VM*,Program*);
void execute(VM*);
 Program *exampleprogram(VM*);
int8 map(Opcode);
VM *virtualmachine(void);
int main(int,char**);

/*
Section .text (code) +read +exec -write
 #########
 #######
 ________________________

 ***
 *****
 *******
Section .data +read -exec +write
*/