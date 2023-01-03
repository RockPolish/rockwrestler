.text
.arm
.align 2
.global run_tests_armv4_conditioncodes

@ load all 16 combinations of sign/zero/carry/overflow flags and test the 15 different condition codes
@ fail test 0x0XY : condition code Y of flag combination X gave the wrong result (and is the first wrong result, only first is shown)

cc_gt:             @ S Z C V   al le gt  lt ge ls hi  vc vs pl mi  cc cs ne eq
    .hword 0x56AA  @ 0 0 0 0    1  0  1   0  1  1  0   1  0  1  0   1  0  1  0
    .hword 0x6A6A  @ 0 0 0 1    1  1  0   1  0  1  0   0  1  1  0   1  0  1  0
    .hword 0x55A6  @ 0 0 1 0    ...
    .hword 0x6966
    .hword 0x66A9
    .hword 0x6A69
    .hword 0x66A5
    .hword 0x6A65
    .hword 0x6A9A
    .hword 0x565A
    .hword 0x6996
    .hword 0x5556
    .hword 0x6A99
    .hword 0x6659
    .hword 0x6A95
    .hword 0x6655


.align 2

run_tests_armv4_conditioncodes:
    adr r0, cc_gt

    @ combination (sign/zero/carry/overflow and then 28 zero bits)
    mov r1, #0

cc_loop:
    mov r2, #0
    msr cpsr_f, r1
    
    orreq r2, #0x1
    orrne r2, #0x2
    orrcs r2, #0x4
    orrcc r2, #0x8
    orrmi r2, #0x10
    orrpl r2, #0x20
    orrvs r2, #0x40
    orrvc r2, #0x80
    orrhi r2, #0x100
    orrls r2, #0x200
    orrge r2, #0x400
    orrlt r2, #0x800
    orrgt r2, #0x1000
    orrle r2, #0x2000
    orral r2, #0x4000

    ldrh r3, [r0], #2  @ load the expected value
    cmp r2, r3
    bne cc_find_error
    adds r1, #1 << 28
    bcc cc_loop
    bx r14


cc_find_error:
    lsr r0, r1, #24   @ shift the 4 bits to be bit 4-7 of the test number
    @ r2 contains our value, r3 the ground truth: find the lowest bit in which r2 and r3
    @ are different, not gonna use clz to find the highest bit as it might not yet be implemented
    eor r2, r3
    mov r1, #0
    @ this loop won't be infinite because r2 ^ r3 != 0
cc_errloop:
    lsrs r2, #1
    addcc r1, #1
    bcc cc_errloop

    @ r1 contains the bit number so which condition failed, add it to r0 and print it
    add r0, r1
    b fail_test
