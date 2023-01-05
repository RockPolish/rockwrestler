.text
.arm
.align 2
.global run_tests_armv5_clz
.global run_tests_armv5_qadd_qsub, run_tests_armv5_qdadd_qdsub
.global run_tests_armv5_SMLAxy, run_tests_armv5_SMULxy
.global run_tests_armv5_SMLAWy, run_tests_armv5_SMULWy
.global run_tests_armv5_SMLALxy
.global run_tests_armv5_blx, run_tests_armv5_ldrpopr15
.global run_tests_armv5_ldm_stm

@ TODO. test ldrd, strd

run_tests_armv5_clz:
    mov r0, #0x000
    mov r1, #0x80000000
    clz r2, r1
    cmp r2, #0
    bne fail_test

    mov r0, #0x001
    ldr r1, =0x0075ABCD
    clz r2, r1
    cmp r2, #9
    bne fail_test

    mov r0, #0x002
    mov r1, #1
    clz r2, r1
    cmp r2, #31
    bne fail_test

    mov r0, #0x003
    mov r1, #0
    clz r2, r1
    cmp r2, #32
    bne fail_test

    mov r0, #0x004
    ldr r1, =0x00004D33
    clz r1, r1
    cmp r1, #17
    bne fail_test

    mov r0, #0x005
    mov r1, #0x50000000
    clz r1, r1
    cmp r1, #1
    bne fail_test

    bx r14

.ltorg

run_tests_armv5_qadd_qsub:
    push {r4, r5}
    mov r4, #0      @ so we can do "msr cpsr_f, r4" to reset the Q flag
    msr cpsr_f, r4   @ clear Q flag

    mov r0, #0x000
    msr cpsr_f, r4
    mov r2, #2
    mov r3, #3
    qadd r1, r2, r3  @ 5, Q = 0
    cmp r1, #5
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test    @ Q = 0

    mov r0, #0x001
    mov r2, #7
    mov r3, #-6
    qadd r1, r2, r3  @ 1, Q = 0
    cmp r1, #1
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x002
    mov r2, #-44
    mov r3, #-14
    qadd r1, r2, r3  @ -30, Q = 0
    cmp r1, #-58
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x003
    mov r2, #0x7FFFFFFF
    mov r3, #1
    qadd r1, r2, r3  @ still 7FFFFFFF, Q = 1
    cmp r1, r2
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test

    @ Q is still 1: test sticky behaviour
    mov r0, #0x004
    mov r2, #6
    mov r3, #7
    qadd r1, r2, r3  @ 13, Q is still 1 (not reset!)
    cmp r1, #13
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x005
    mov r2, #0x7FFFFFFF
    mov r3, #-1
    qadd r1, r2, r3  @ 7FFFFFFE, Q = 0
    cmp r1, #0x7FFFFFFE
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x006
    mov r2, #0x80000000
    mov r3, #-1
    qadd r1, r2, r3  @ 80000000, Q = 1
    cmp r1, #0x80000000
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x007
    mov r2, #0x80000000
    mov r3, #1
    qadd r1, r2, r3  @ 80000001, Q = 0
    cmp r1, #0x80000001
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x008
    mov r2, #0x7FFFFFFE
    mov r3, #1
    qadd r1, r2, r3  @ 7FFFFFFF, Q = 0
    cmp r1, #0x7FFFFFFF
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x009
    mov r2, #0x80000001
    mov r3, #-1
    qadd r1, r2, r3  @ 80000000, Q = 0
    cmp r1, #0x80000000
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x00A
    mov r2, #0x7FFFFFFF
    mov r3, #0x80000000
    qadd r1, r2, r3  @ FFFFFFFF, Q = 0
    cmp r1, #0xFFFFFFFF
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test


    mov r0, #0x010
    mov r2, #7
    mov r3, #11
    qsub r1, r2, r3  @ -4, Q = 0
    cmp r1, #-4
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x011
    mov r2, #0x7FFFFFFF
    mov r3, #0
    qsub r1, r2, r3  @ 0x7FFFFFFF, Q = 0
    cmp r1, r2
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x012
    mov r2, #0x7FFFFFFF
    mov r3, #-1
    qsub r1, r2, r3  @ 0x7FFFFFFF, Q = 1
    cmp r1, r2
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x013
    mov r2, #0x7FFFFFFF
    mov r3, #1
    qsub r1, r2, r3  @ 0x7FFFFFFE, Q = 0
    cmp r1, #0x7FFFFFFE
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x014
    mov r2, #0x7FFFFFFE
    mov r3, #1
    qsub r1, r2, r3  @ 0x7FFFFFFD, Q = 0
    cmp r1, #0x7FFFFFFD
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x015
    mov r2, #0x7FFFFFFE
    mov r3, #-1
    qsub r1, r2, r3  @ 0x7FFFFFFF, Q = 0
    cmp r1, #0x7FFFFFFF
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x016
    mov r2, #0x7FFFFFFE
    mov r3, #-2
    qsub r1, r2, r3  @ 0x7FFFFFFF, Q = 1
    cmp r1, #0x7FFFFFFF
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x017
    mov r2, #0x80000000
    mov r3, #0
    qsub r1, r2, r3  @ 0x80000000, Q = 0
    cmp r1, #0x80000000
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x018
    mov r2, #0x80000000
    mov r3, #-1
    qsub r1, r2, r3  @ 0x80000001, Q = 0
    cmp r1, #0x80000001
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x019
    mov r2, #0x80000000
    mov r3, #1
    qsub r1, r2, r3  @ 0x80000000, Q = 1
    cmp r1, #0x80000000
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x01A
    mov r2, #214
    mov r3, #-11
    qsub r1, r2, r3  @ 225, Q = 0
    cmp r1, #225
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    pop {r4, r5}
    bx r14


run_tests_armv5_qdadd_qdsub:
    push {r4, r5}
    mov r4, #0
    msr cpsr_f, r4   @ clear Q flag

    mov r0, #0x000
    mov r2, #106
    mov r3, #8
    qdadd r1, r2, r3  @ 122, Q = 0
    cmp r1, #122
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x001
    mov r2, #144
    mov r3, #-12
    qdadd r1, r2, r3  @ 120, Q = 0
    cmp r1, #120
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x002
    mov r2, #-11
    mov r3, #8
    qdadd r1, r2, r3  @ 5, Q = 0
    cmp r1, #5
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x003
    mov r2, #9
    mov r3, #-12
    qdadd r1, r2, r3  @ -15, Q = 0
    cmp r1, #-15
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x004
    ldr r2, =0x7FFFFFF8    @ can still add 7 without overflowing
    mov r3, #3
    qdadd r1, r2, r3  @ 7FFFFFFE, Q = 0
    ldr r5, =0x7FFFFFFE
    cmp r1, r5
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x005
    ldr r2, =0x7FFFFFF8
    mov r3, #4
    qdadd r1, r2, r3  @ 7FFFFFFF, Q = 1
    cmp r1, #0x7FFFFFFF
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x006
    ldr r2, =0x7FFFFFF9
    mov r3, #3
    qdadd r1, r2, r3  @ 7FFFFFFF, Q = 0
    cmp r1, #0x7FFFFFFF
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x007
    mov r2, #0x80000005  @ can still subtract 5 without overflowing
    mov r3, #-2
    qdadd r1, r2, r3  @ 80000001, Q = 0
    cmp r1, #0x80000001
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x008
    mov r2, #0x80000005
    mov r3, #-3
    qdadd r1, r2, r3  @ 80000000, Q = 1
    cmp r1, #0x80000000
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x009
    mov r2, #0x80000004
    mov r3, #-2
    qdadd r1, r2, r3  @ 80000000, Q = 0
    cmp r1, #0x80000000
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    @ + tests where doubling the second operand is already requiring clamping (and thus setting Q to 1)
    mov r0, #0x00A
    mov r2, #0x80000000 @ very small number
    mov r3, #0x7FFFFFFE @ very large number, only increases by 1 when doubling after clamping
    qdadd r1, r2, r3  @ -1, Q = 1
    cmp r1, #-1
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x00B
    mov r2, #0x80000000
    mov r3, #0x3FFFFFFF @ can still be doubled without clamping
    qdadd r1, r2, r3  @ -2, Q = 0
    cmp r1, #-2
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x00C
    mov r2, #0x80000000
    mov r3, #0x40000000 @ can no longer be doubled without clamping
    qdadd r1, r2, r3  @ -1, Q = 1
    cmp r1, #-1
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x00D
    mov r2, #0x7FFFFFFF @ large number
    mov r3, #0xC0000001 @ can be doubled
    qdadd r1, r2, r3  @ 1, Q = 0
    cmp r1, #1
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x00E
    mov r2, #0x7FFFFFFF @ large number
    mov r3, #0xC0000000 @ can be doubled but only barely (edge)
    qdadd r1, r2, r3  @ 1, Q = 0
    cmp r1, #-1
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x00F
    mov r2, #0x7FFFFFFF @ large number
    mov r3, #0xBFFFFFFF @ can no longer be doubled
    qdadd r1, r2, r3  @ 1, Q = 1
    cmp r1, #-1
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4


    mov r0, #0x010
    mov r2, #106
    mov r3, #8
    qdsub r1, r2, r3  @ 90, Q = 0
    cmp r1, #90
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x011
    mov r2, #144
    mov r3, #-12
    qdsub r1, r2, r3  @ 168, Q = 0
    cmp r1, #168
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x012
    mov r2, #7
    mov r3, #58
    qdsub r1, r2, r3  @ -109, Q = 0
    cmp r1, #-109
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x013
    mov r2, #-18
    mov r3, #-12
    qdsub r1, r2, r3  @ 6, Q = 0
    cmp r1, #6
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test
 
    mov r0, #0x014
    ldr r2, =0x7FFFFFF8    @ can still add 7
    mov r3, #-3
    qdsub r1, r2, r3  @ 7FFFFFFE, Q = 0
    ldr r5, =0x7FFFFFFE
    cmp r1, r5
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x015
    ldr r2, =0x7FFFFFF8
    mov r3, #-4
    qdsub r1, r2, r3  @ 7FFFFFFF, Q = 1
    cmp r1, #0x7FFFFFFF
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x016
    ldr r2, =0x7FFFFFF9
    mov r3, #-3
    qdsub r1, r2, r3  @ 7FFFFFFF, Q = 0
    cmp r1, #0x7FFFFFFF
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x017
    mov r2, #0x80000005  @ can still sub 5
    mov r3, #2
    qdsub r1, r2, r3  @ 80000001, Q = 0
    cmp r1, #0x80000001
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x018
    mov r2, #0x80000005
    mov r3, #3
    qdsub r1, r2, r3  @ 80000000, Q = 1
    cmp r1, #0x80000000
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x019
    mov r2, #0x80000004
    mov r3, #2
    qdsub r1, r2, r3  @ 80000000, Q = 0
    cmp r1, #0x80000000
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    @ + tests where doubling the second operand is already requiring clamping (and thus setting Q to 1)
    mov r0, #0x01A
    mov r2, #0x7FFFFFFF
    mov r3, #0x7FFFFFFE
    qdsub r1, r2, r3  @ 0, Q = 1
    cmp r1, #0
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x01B
    mov r2, #0x7FFFFFFF
    mov r3, #0x3FFFFFFF @ can still be doubled
    qdsub r1, r2, r3  @ -1, Q = 0
    cmp r1, #1
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x01C
    mov r2, #0x7FFFFFFF
    mov r3, #0x40000000 @ can't be doubled without clamping
    qdsub r1, r2, r3  @ 0, Q = 1
    cmp r1, #0
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x01D
    mov r2, #0x80000000
    mov r3, #0xC0000001 @ can be doubled
    qdsub r1, r2, r3  @ -2, Q = 0
    cmp r1, #-2
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x01E
    mov r2, #0x80000000
    mov r3, #0xC0000000 @ edge case
    qdsub r1, r2, r3  @ 0, Q = 0
    cmp r1, #0
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x01F
    mov r2, #0x80000000
    mov r3, #0xBFFFFFFF @ can't be doubled without clamping
    qdsub r1, r2, r3  @ 0, Q = 1
    cmp r1, #0
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    pop {r4, r5}
    bx r14


run_tests_armv5_SMLAxy:
    push {r4, r5}
    mov r4, #0   @ to clear Q
    msr cpsr_f, r4
    
    mov r0, #0x000
    mov r2, #7
    mov r3, #10
    mov r5, #2
    smlabb r1, r2, r3, r5  @ 7*10 + 2 = 72
    cmp r1, #72
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x001
    mov r2, #-3
    mov r3, #11
    mov r5, #100
    smlabb r1, r2, r3, r5  @ -3*11 + 100 = 67
    cmp r1, #67
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x002
    mov r2, #16384
    mov r3, #2
    mov r5, #0
    smlabb r1, r2, r3, r5  @ 32768
    cmp r1, #32768
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x003
    mov r2, #65536
    sub r2, r2, #1
    mov r3, r2
    mov r5, #0
    smlabb r1, r2, r3, r5  @ 1
    cmp r1, #1
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x004
    mov r2, #1
    mov r3, #1
    mov r5, #0x7FFFFFFF
    smlabb r1, r2, r3, r5  @ 0x80000000, Q = 1
    cmp r1, #0x80000000
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x005
    mov r2, #7
    mov r3, #0x40000
    mov r5, #-28
    smlabt r1, r2, r3, r5
    cmp r1, #0
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x006
    ldr r2, =0xFFFC1234  @ -4 (top halfword)
    mov r3, #57
    mov r5, #-1
    smlatb r1, r2, r3, r5
    cmp r1, #-229
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x007
    mov r2, #0x80000000 @ -32768
    mov r3, #0x7FFFFFFF @  32767
    mov r5, #0
    smlatt r1, r2, r3, r5
    sub r1, r1, #0xC0000000
    cmp r1, #0x8000
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x008
    mov r2, #0x80000000 @ -32768
    mov r3, #0x7FFFFFFF @  32767
    mov r5, #0x80000000
    smlatt r1, r2, r3, r5
    sub r1, r1, #0x40000000
    cmp r1, #0x8000
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    @ + one test with same destination & accumulate register
    mov r0, #0x009
    mov r1, #100
    mov r2, #7
    mov r3, #-3
    smlabb r1, r2, r3, r1   @ 100 + 7*(-3) = 79
    cmp r1, #79
    bne fail_test

    pop {r4, r5}
    bx r14


run_tests_armv5_SMULxy:
    @ clear Q flag
    mov r0, #0
    msr cpsr_f, r0

    mov r0, #0x000
    mov r2, #14
    mov r3, #5
    smulbb r1, r2, r3
    cmp r1, #70
    bne fail_test

    mov r0, #0x001
    mov r2, #-6
    mov r3, #104
    smulbb r1, r2, r3
    cmp r1, #-624
    bne fail_test

    mov r0, #0x002
    mov r2, #0xFFFFFFF0
    mov r3, #88
    smultb r1, r2, r3
    cmp r1, #-88
    bne fail_test

    mov r0, #0x003
    mov r2, #-14
    mov r3, #0x00050000
    add r3, r3, #0x830
    smulbt r1, r2, r3
    cmp r1, #-70
    bne fail_test

    mov r0, #0x004  @ check to make sure Q was not set: these can't cause overflow
    mrs r3, cpsr
    tst r3, #1 << 27
    bne fail_test
    
    bx r14


run_tests_armv5_SMLAWy:
    push {r4, r5, r7}
    @ clear Q flag
    mov r4, #0
    msr cpsr_f, r4

    mov r0, #0x000
    mov r2, #47
    mov r3, #5
    mov r5, #-3
    smlawb r1, r2, r3, r5    @ wide r2, bottom of r3, (r2*half(r3)) >> 16 + Rn !!
    cmp r1, #-3
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x001
    mov r2, #0x90000
    mov r3, #-7
    mov r5, #2
    smlawb r1, r2, r3, r5
    cmp r1, #-61
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x002
    mov r2, #0x400
    mov r3, #0x500
    mov r5, #-1
    smlawb r1, r2, r3, r5
    cmp r1, #19
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x003
    mov r2, #0x4
    mov r3, #0x4000
    mov r5, #0x7FFFFFFF
    smlawb r1, r2, r3, r5
    cmp r1, #0x80000000
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    mov r0, #0x004
    mov r2, #0x140000
    mov r3, #0xFFFEFFFF
    sub r3, r3, #0x0230
    mov r5, #-5
    smlawt r1, r2, r3, r5   @ -2 * 20 - 5
    cmp r1, #-45
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x005
    mov r2, #0x04D0  @ 1232
    mov r3, #0xC300  @ -15616
    mov r5, #0
    smlawb r1, r2, r3, r5
    ldr r7, =#-294
    cmp r1, r7
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    bne fail_test

    mov r0, #0x006
    mov r2, #4
    mov r3, #0x40000000
    mov r5, #0x7FFFFFFF
    smlawt r1, r2, r3, r5
    cmp r1, #0x80000000
    bne fail_test
    mrs r5, cpsr
    tst r5, #1 << 27
    beq fail_test
    msr cpsr_f, r4

    @ + one test with same destination & accumulate register
    mov r0, #0x007
    mov r1, #100
    mov r2, #7 << 16
    mov r3, #-3
    smlawb r1, r2, r3, r1   @ 100 + ((7 << 16)*(-3)) >> 16 = 79
    cmp r1, #79
    bne fail_test

    pop {r4, r5, r7}
    bx r14


run_tests_armv5_SMULWy:
    push {r7}
    @ clear Q flag
    mov r0, #0
    msr cpsr_f, r0

    mov r0, #0x000
    mov r2, #15
    mov r3, #44
    smulwb r1, r2, r3
    cmp r1, #0
    bne fail_test

    mov r0, #0x001
    smulwt r1, r2, r3
    cmp r1, #0
    bne fail_test

    mov r0, #0x002
    mov r2, #188
    mov r3, #0x7700
    smulwb r1, r2, r3
    cmp r1, #87
    bne fail_test

    mov r0, #0x003
    mov r2, #-188
    smulwb r1, r2, r3
    cmp r1, #-88
    bne fail_test

    mov r0, #0x004
    mov r2, #-241
    mov r3, #-69
    mov r2, r2, lsl #11
    smulwb r1, r2, r3
    ldr r7, =519
    cmp r1, r7
    bne fail_test

    mov r0, #0x005
    mov r2, #0x51000
    add r2, r2, #0x9A
    mov r3, #0x4000000
    smulwt r1, r2, r3
    ldr r7, =5186
    cmp r1, r7
    bne fail_test

    mov r0, #0x006 @ check to make sure Q wasn't set
    mrs r7, cpsr
    tst r7, #1 << 27
    bne fail_test

    pop {r7}
    bx r14


run_tests_armv5_SMLALxy:
    push {r4, r7}
    @ SMLALxy  (RdLo RdHi Rm Rs) (RdHiLo += HalfRm * HalfRs) (doesn't set Q even though there can be overflow!!)
    @ result in r1 (lo) r2 (hi), Rm = r3, Rs = r4
    mov r0, #0x000
    mov r1, #0
    mov r2, #0
    mov r3, #7
    mov r4, #18
    smlalbb r1, r2, r3, r4
    cmp r1, #126
    bne fail_test
    cmp r2, #0
    bne fail_test

    mov r0, #0x001
    mov r1, #0
    mov r3, #-7
    smlalbb r1, r2, r3, r4
    cmp r1, #-126
    bne fail_test
    cmp r2, #0xFFFFFFFF
    bne fail_test

    mov r0, #0x002
    ldr r1, =0x12345678
    mov r2, r1
    mov r3, #0x4700
    mov r4, #0x06D0
    smlalbb r1, r2, r3, r4
    ldr r7, =337118840
    cmp r1, r7
    bne fail_test
    ldr r7, =0x12345678
    cmp r2, r7
    bne fail_test

    mov r0, #0x003
    mov r1, #0
    mov r2, #1
    mov r3, #0xFFFFFFFF
    sub r3, r3, #0x0C10  @ ?
    mov r4, #7
    smlaltb r1, r2, r3, r4
    cmp r1, #-7
    bne fail_test
    cmp r2, #0
    bne fail_test

    mov r0, #0x004
    mov r1, #4000
    mov r2, #8
    mov r3, #-7
    mov r4, #0x150000
    smlalbt r1, r2, r3, r4
    ldr r7, =3853
    cmp r1, r7
    bne fail_test
    cmp r2, #8
    bne fail_test

    mov r0, #0x005  @ make sure Q was not set
    mrs r4, cpsr
    tst r4, #1 << 27
    bne fail_test

    pop {r4, r7}
    bx r14


run_tests_armv5_blx:
    push {r5-r7, r14}
    @ blx tests
    ldr r3, =fail_test              @ so we can do 'bx r3' instead of 'b fail_test_t' in thumb: the second doesn't work, it's too far away
    ldr r5, =test_cpu_state         @ we will branch to this address and it will return whether the branch ended in ARM or thumb (r7 = 1 or 2)
    ldr r6, =test_cpu_state_thumb   @ required for it to work

    @@ two sanity checks
    @ use bx with lowest bit set to 0, must be ARM
    mov r0, #0x000
    bic r1, r5, #1   @ make sure bottom bit is cleared
    mov r14, r15     @ set r14 to address of the instruction after the "bx r4"
    bx r1
    cmp r7, #1
    bne fail_test

    @ same but with lowest bit set to 1, must be thumb
    mov r0, #0x001
    orr r1, r5, #1
    mov r14, r15
    bx r1
    cmp r7, #2
    bne fail_test

    @@ blx reg
    mov r0, #0x002
    bic r1, r5, #1
    blx r1
    cmp r7, #1
    bne fail_test

    mov r0, #0x003
    orr r1, r5, #1
    blx r1
    cmp r7, #2
    bne fail_test

    @ blx with offset (always changes from arm to thumb)
    mov r0, #0x004
    blx test_cpu_state
    cmp r7, #2
    bne fail_test

    @@ now change to thumb mode and test those blx
    add r1, r15, #1
    bx r1

.thumb
    @ register blx
    mov r0, #0x005
    lsr r1, r5, #1 @ clear bit 0
    lsl r1, r1, #1 @
    blx r1
    cmp r7, #1
    beq skip1
    bx r3
skip1:
    mov r0, #0x006
    mov r1, #1     @ set bit 0
    orr r1, r5     @
    blx r1
    cmp r7, #2
    beq skip2
    bx r3
skip2:
    @ blx with offset (two 16-bit instructions) (always changes from thumb to arm)
    mov r0, #0x007
    blx test_cpu_state
    cmp r7, #1
    beq skip3
    bx r3
skip3:

    @ go back to ARM
.align 2
    mov r1, r15
    bx r1

.align 2
.arm

    pop {r5-r7, r15}


run_tests_armv5_ldrpopr15:
    push {r5-r7, r14}

    @ test ldr r15, [...] and pop {r15} being able to change state
    ldr r3, =fail_test              @ so we can do 'bx r3' instead of 'b fail_test_t' in thumb: the second doesn't work, it's too far away
    ldr r5, =test_cpu_state         @ we will branch to this address and it will return whether the branch ended in ARM or thumb (r7 = 1 or 2)
    ldr r6, =test_cpu_state_thumb   @ required for it to work
    str r5, buffer
    orr r2, r5, #1                  @ r2 = r5 with thumb bit set
    str r2, buffer+4

    @ test ldr r15, ...
    mov r0, #0x000
    mov r14, r15
    ldr r15, buffer
    cmp r7, #1
    bne fail_test

    mov r0, #0x001
    mov r14, r15
    ldr r15, buffer+4
    cmp r7, #2
    bne fail_test

    @ test pop {r15}: pop multiple things because the assembler turns pop {r15} into ldr r15, [r13], #4
    @ should probably just do .word (pop {r15} instruction)
    mov r0, #0x002
    push {r0, r5}
    mov r14, r15
    pop {r0, r15}
    cmp r7, #1
    bne fail_test

    mov r0, #0x003
    push {r0, r2}
    mov r14, r15
    pop {r0, r15}
    cmp r7, #2
    bne fail_test

    @ make sure that mov r15, ... does not change to thumb
    mov r0, #0x004
    mov r14, r15
    mov r15, r5
    cmp r7, #1
    bne fail_test

    mov r0, #0x005
    mov r14, r15
    mov r15, r2
    cmp r7, #1
    bne fail_test

    @@ now change to thumb mode and test pop {r15} (no ldr r15, ...)
    add r1, r15, #1
    bx r1

.thumb
    @ pop {r15}
    mov r0, #0x006
    push {r5}
    @ set r14 to address of instruction after pop {r15}, + thumb bit  (maybe change to PC relative load?)
    mov r1, r15   @ r1 = address of "mov r14, r1"
    add r1, #5    @ +5 -> address of "cmp r7, #1" + thumb bit
    mov r14, r1
    pop {r15}
    cmp r7, #1
    beq skip4
    bx r3
skip4:

    mov r0, #0x007
    push {r2}
    @ set r14 to address of instruction after pop {r15}, + thumb bit
    mov r1, r15   @ r1 = address of "mov r14, r1"
    add r1, #5    @ +5 -> address of "cmp r7, #2" + thumb bit
    mov r14, r1
    pop {r15}
    cmp r7, #2
    beq skip5
    bx r3
skip5:
    @ make sure that mov r15, ... does not change to arm
    mov r0, #0x008
    @ set r14 to address of instruction after mov r15, r5, + thumb bit
    mov r1, r15   @ r1 = address of "mov r14, r1"
    add r1, #5    @ +5 -> address of "cmp r7, #2" + thumb bit
    mov r14, r1
    mov r15, r5
    cmp r7, #2
    beq skip6
    bx r3
skip6:

    mov r0, #0x009
    @ set r14 to address of instruction after mov r15, r2, + thumb bit
    mov r1, r15   @ r1 = address of "mov r14, r1"
    add r1, #5    @ +5 -> address of "cmp r7, #2" + thumb bit
    mov r14, r1
    mov r15, r2
    cmp r7, #2
    beq skip7
    bx r3
skip7:

    @ go back to ARM
.align 2
    mov r1, r15
    bx r1

.align 2
.arm
    pop {r5-r7, r15}
@ will contain test_cpu_state and test_cpu_state|1 (thumb bit set)   (could just be initialized in the ROM itself but whatever)
buffer:
    .word 0, 0




wrong:
    .word 0x11111111
    .word 0x11111111
    .word 0x11111111
    .word 0x11111111
    .word 0x11111111
    .word 0x11111111

random_data:
    .word 0xC0FFEEAB
    .word 0xDEADBEEF
    .word 0xD00DFEED
    .word 0xC0CAC01A
    .word 0x01234567
    .word 0x89ABCDEF

old_state:
    .space 0x40

overwrite_me:
    .space 0x40

run_tests_armv5_ldm_stm:
    @ save all the registers, because we're about to
    @ overwrite them all (except r0 and r12)
    ldr r0, =old_state
    stmia r0!, {r1-r11, r13-r14}

    @ r0 = the test # we are on
    mov r0, #0

@ TEST: simple ldmia with writeback (ldmia r1!, {r2})
@ expected:
@     0. r2 = 0xC0FFEEAB
@     1. r1 = r1 + 4

    ldr r1, =random_data
    ldmia r1!, {r2}
    
    @ TEST 0
    ldr r3, =random_data
    ldr r3, [r3]
    cmp r3, r2
    bne fail_test_ldm_stm
    add r0, #1

    @ TEST 1
    ldr r3, =random_data
    add r3, #4
    cmp r3, r1
    bne fail_test_ldm_stm
    add r0, #1

@ TEST: simple stmia with writeback (stmia r1!, {r2})
@ expected:
@     2. [r1] = 0xC0FFEEAB
@     3. r1 = r1 + 4

    ldr r1, =overwrite_me
    ldr r2, =random_data
    ldr r2, [r2]
    stmia r1!, {r2}
    
    @ TEST 2
    ldr r3, =random_data
    ldr r3, [r3]
    ldr r4, =overwrite_me
    ldr r4, [r4]
    cmp r3, r4
    bne fail_test_ldm_stm
    add r0, #1

    @ TEST 3
    ldr r3, =overwrite_me
    add r3, #4
    cmp r3, r1
    bne fail_test_ldm_stm
    add r0, #1

@ those were just the sanity checks. let's get started

@ TEST: ldmia with writeback with base in rlist (ldmia r1!, {r1})
@ expected:
@     4. r1 = r1 + 4

    ldr r1, =random_data
    ldmia r1!, {r1}

    @ TEST 4
    ldr r3, =random_data
    add r3, #4
    cmp r3, r1
    bne fail_test_ldm_stm
    add r0, #1

@ TEST: ldmia with writeback with base last in rlist (ldmia r3!, {r1-r3})
@ expected:
@     5. r1 = 0xC0FFEEAB
@     6. r2 = 0xDEADBEEF
@     7. r3 = 0xD00DFEED

    ldr r3, =random_data
    ldmia r3!, {r1-r3}

    ldr r4, =random_data

    @ TEST 5
    ldr r5, [r4], #4
    cmp r5, r1
    bne fail_test_ldm_stm
    add r0, #1

    @ TEST 6
    ldr r5, [r4], #4
    cmp r5, r2
    bne fail_test_ldm_stm
    add r0, #1

    @ TEST 7
    ldr r5, [r4], #4
    cmp r5, r3
    bne fail_test_ldm_stm
    add r0, #1

@ TEST: ldmia with writeback with base not last in rlist (ldmia r2!, {r1-r3})
@ expected:
@     8. r1 = 0xC0FFEEAB
@     9. r3 = 0xD00DFEED
@     A. r2 = r2 + 12

    ldr r2, =random_data
    ldmia r2!, {r1-r3}

    ldr r4, =random_data

    @ TEST 8
    ldr r5, [r4], #8
    cmp r5, r1
    bne fail_test_ldm_stm
    add r0, #1

    @ TEST 9
    ldr r5, [r4], #4
    cmp r5, r3
    bne fail_test_ldm_stm
    add r0, #1

    @ TEST A
    ldr r1, =random_data
    add r1, #12
    cmp r1, r2
    bne fail_test_ldm_stm
    add r0, #1

@ TEST: ldmia with writeback with empty rlist (ldmia r2!, {})
@ expected:
@     B. r2 = r2 + 0x40

    ldr r2, =random_data
    .word 0xE8B20000 @ ldmia r2!, {r1-r3}

    @ TEST B
    ldr r3, =random_data
    add r3, #0x40
    cmp r3, r2
    bne fail_test_ldm_stm
    add r0, #1

@ TEST: simple ldmia with S bit with writeback (ldmia r13!, {r14}^)
@ expected:
@     C. r14_irq  = 0
@     D. r13_irq  = r1 + 4
@     E. r14_user = 0xC0FFEEAB
@     F. r13_user = &wrong

    ldr r13, =wrong @ make you read from some wrong place if you use the wrong reg bank
    mov r14, #0

	mov	r1, #0x12 @ IRQ mode
	msr	cpsr, r1

    ldr r13, =random_data
    mov r14, #0

    ldmia r13!, {r14}^
    
    @ TEST C
    mov r3, #0
    cmp r3, r14
    bne fail_test_ldm_stm
    add r0, #1

    @ TEST D
    ldr r3, =random_data
    add r3, #4
    cmp r3, r13
    bne fail_test_ldm_stm
    add r0, #1

	mov	r1, #0x1F @ User mode
	msr	cpsr, r1
    
    @ TEST E
    ldr r3, =random_data
    ldr r3, [r3]
    cmp r3, r14
    bne fail_test_ldm_stm
    add r0, #1

    @ TEST F
    ldr r3, =wrong
    cmp r3, r13
    bne fail_test_ldm_stm
    add r0, #1

@ TEST: ldmia with base in rlist with S bit (ldmia r13, {r13}^)
@ expected:
@     10. r13_irq  = unchanged
@     11. r13_user = 0xC0FFEEAB

    ldr r13, =wrong @ make you read from some wrong place if you use the wrong reg bank

	mov	r1, #0x12 @ IRQ mode
	msr	cpsr, r1

    ldr r13, =random_data
    ldmia r13, {r13}^

    @ TEST 10
    ldr r3, =random_data
    cmp r3, r13
    bne fail_test_ldm_stm
    add r0, #1

	mov	r1, #0x1F @ User mode
	msr	cpsr, r1

    @ TEST 11
    ldr r3, =random_data
    ldr r3, [r3]
    cmp r3, r13
    bne fail_test_ldm_stm
    add r0, #1

@ TEST: stmia with writeback with base in rlist (stmia r1!, {r1})
@ expected:
@     12. r1   = r1 + 4
@     13. [r1] = &overwrite_me 

    ldr r1, =overwrite_me
    stmia r1!, {r1}

    @ TEST 12
    ldr r3, =overwrite_me
    add r3, #4
    cmp r3, r1
    bne fail_test_ldm_stm
    add r0, #1

    @ TEST 13
    ldr r3, =overwrite_me
    ldr r2, [r3]
    cmp r2, r3
    bne fail_test_ldm_stm
    add r0, #1

@ TEST: stmia with writeback with base last in rlist (stmia r3!, {r1-r3})
@ expected:
@     14. [r3 + 0] = 0xDEADBEEF
@     15. [r3 + 4] = 0xC0FFEEAB
@     16. [r3 + 8] = &overwrite_me
@     17. r3 = r3 + 12

    ldr r3, =random_data
    ldr r2, [r3], #4
    ldr r1, [r3], #4
    ldr r3, =overwrite_me
    stmia r3!, {r1-r3}

    @ TEST 14
    ldr r4, =random_data
    ldr r4, [r4, #4]
    ldr r5, =overwrite_me
    ldr r5, [r5]
    cmp r4, r5
    bne fail_test_ldm_stm
    add r0, #1

    @ TEST 15
    ldr r4, =random_data
    ldr r4, [r4]
    ldr r5, =overwrite_me
    ldr r5, [r5, #4]
    cmp r4, r5
    bne fail_test_ldm_stm
    add r0, #1

    @ TEST 16
    ldr r4, =overwrite_me
    add r4, #12
    ldr r5, =overwrite_me
    ldr r5, [r5, #8]
    cmp r4, r5
    bne fail_test_ldm_stm
    add r0, #1

    @ TEST 17
    ldr r4, =overwrite_me
    add r4, #12
    cmp r4, r3
    bne fail_test_ldm_stm
    add r0, #1

end_test:
    ldr r12, =old_state
    ldmia r12!, {r1-r11, r13-r14}
    bx lr

fail_test_ldm_stm:
    ldr r12, =old_state
    ldmia r12!, {r1-r11, r13-r14}
    b fail_test

