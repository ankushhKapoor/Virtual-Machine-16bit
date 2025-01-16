/* kapoorvm.c */
#include "../include/kapoorvm.h"

void __mov(VM *vm, Opcode opcode, Args a1, Args a2) {
    int16 dst, dstl, dsth;

    //1011
    //0011
    /* (xx) */
    dst = $2 a1;
    dstl = (($2 a1) & 0x03);
    dsth = ((($2 a1) & 0x0c) >>2);

    switch (opcode) {
        /* mov ax       ; 1000 */
        case 0x08:
            if (higher(vm))
                vm $ax = (Reg)dsth;
            else if (lower(vm))
                vm $ax = (Reg)dstl;
            else
                vm $ax = (Reg)dst;
            break;
        
        /* mov bx       ; 1001 */
        case 0x09:
            if (higher(vm))
                vm $bx = (Reg)dsth;
            else if (lower(vm))
                vm $bx = (Reg)dstl;
            else
                vm $bx = (Reg)dst;
            break;
        
        /* mov cx       ; 1010 */
        case 0x0a:
            if (higher(vm))
                vm $cx = (Reg)dsth;
            else if (lower(vm))
                vm $cx = (Reg)dstl;
            else
                vm $cx = (Reg)dst;
            break;
        
        /* mov dx       ; 1011 */
        case 0x0b:
            if (higher(vm))
                vm $dx = (Reg)dsth;
            else if (lower(vm))
                vm $dx = (Reg)dstl;
            else
                vm $dx = (Reg)dst;
            break;
        
        /* mov sp       ; 1100 */
        case 0x0c:
            if (higher(vm))
                vm $sp = (Reg)dsth;
            else if (lower(vm))
                vm $sp = (Reg)dstl;
            else
                vm $sp = (Reg)dst;
            break;        
        
        /* 1101 
        case 0x0d:
            vm $ip = (Reg)dst;
            break;
        
        /* 1110 
        case 0x0e:
            vm $ax = (Reg)dst;
            break; */
        
        /* mov [addr]       ; 1111 */
        case 0x0f:
            /* (xx) */
            break;

        default:
            error(vm, ErrInstr);
            break;
        
    }
    // 1 0000
    // 15 = 1111 mov [0xaabb], 0x05
    // 1 - 15

    return;
}

void execinstr(VM *vm, Program *p) {
    Args a1, a2;
    int16 size;

    a1=a2 = 0;
    size = map(*p);
    switch (size) {
        case 1:
            break;

        case 2:
            a1 = *(p+1);
            break;

        case 3:
            a1 = (
                (((int16)*(p+2) & 0xff) << 8) 
                    | ((int16)*(p+1) & 0xff)
            );
            break;

        case 5:
            a1 = (
                (((int16)*(p+2) & 0xff) << 8) 
                    | ((int16)*(p+1) & 0xff)
            );

            a2 = (
                (((int16)*(p+4) & 0xff) << 8) 
                    | ((int16)*(p+3) & 0xff)
            );

            break;

        default:
            segfault(vm);
            break;
    }

    switch (*p) {
        case mov:
        case 0x09 ... 0x0f:
            __mov(vm, *p, a1, a2);
            break;

        case nop:
            break;

        case hlt:
            error(vm, SysHlt);
            break;
    }

    return;
}

void execute(VM *vm) {
    int32 brkaddr;
    Program *pp;
    int16 size;
    Instruction ip; 

    assert(vm && *vm->m);
    size = 0;
    brkaddr = ((int32)vm->m + vm->b);
    pp = (Program *)&vm->m;

    /* instr1 arg1 instr2 instr3 */
    /* mov cx,0xabcd; nop; hlt; */
    /* 0x0a 0xabcd */
    /* 0x02 */
    /* 0x03 */

 do {
        vm $ip += size;
        pp += size;

        ip.o = *pp;
        if ((int32)pp > brkaddr)
            segfault(vm);
            size = map(*pp);
        execinstr(vm,pp);
    } while (*pp != (Opcode)hlt);

    return;
}

void error(VM *vm, Errorcode e) {
    int8 exitcode;

    exitcode = -1;
    switch (e) {
        case ErrSegv:
            fprintf(stderr, "%s\n", "VM Segmentation Fault");
            break;
            
        case ErrInstr:
            fprintf(stderr, "%s\n", "VM Illegal Instruction");
            break;
            
        /* (xx) */
        case SysHlt:
            fprintf(stderr, "%s\n", "System halted");
            exitcode = 0;
            printf("cx = %.04hx\n", $i vm $cx);
            vm $flags = 0x04;
            if (equal(vm))
                printf("E flag set\n");
            if (gt(vm))
                printf("GT flag set\n");

            break;

        default:
            break;
    }

    if(vm)
        free(vm);

    exit($i exitcode);
}

int8 map(Opcode o) {
    int8 n, ret;
    IM *p;

    ret = 0;
    for (n=IMs, p=instrmap; n; n--, p++)
        if(p->o == o) {
            ret = p->s;
            break;
        }

    return ret;
}

VM *virtualmachine() {
    VM *p;
    // Program *pp;
    int16 size;

    size = $2 sizeof(struct s_vm);
    p = (VM *)malloc($i size);
    if (!p) {
        errno = ErrMem;
        return (VM *)0;
    } 
    zero($1 p, size);

    return p;
}

Program *exampleprogram(VM *vm) {
    Program *p;
    Instruction *i1, *i2, *i3;
    Args a1;
    int16 s1, s2, sa1;

    a1 = 0;
    s1 = map(mov);
    s2 = map(nop);

    i1 = (Instruction *)malloc($i s1);
    i2 = (Instruction *)malloc( s2);
    i3 = (Instruction *)malloc( s2);
    assert(i1 && i2);
    zero($1 i1, s1);
    zero($1 i2, s2);
    zero($1 i3, s2);

    i1->o = 0x0a; //mov
    sa1 = (s1-1);
        a1     = 0xabcd;
        // 0000 1010 mov cx
        // 0000 0000
        // 0000 abcd mov cx,0xabcd

    p = vm->m;
    copy($1 p, $1 i1, 1);
    p++;

    if (a1) {
        copy($1 p, $1 &a1, sa1);
        p += sa1;
    }

    i2->o = nop;
    copy($1 p, $1 i2, 1);
    p++;

    i3->o = hlt;
    copy($1 p, $1 i3, 1);

    vm->b = (s1+sa1+s2+s2);
    vm $ip = (Reg)vm->m;
    vm $sp = (Reg)-1;

    free(i1);
    free(i2);

    return (Program *)&vm->m;
}

int main(int argc, char *argv[]) {
    Program *prog;
    VM *vm;
 
    vm = virtualmachine();
    printf("vm   = %p (sz: %ld)\n", vm, sizeof(struct s_vm));

    prog = exampleprogram(vm);
    printf("prog = %p\n", prog);

    execute(vm);

    printhex($1 prog, (map(mov)+map(nop)+map(hlt)), ' ' );

    return 0;
}
