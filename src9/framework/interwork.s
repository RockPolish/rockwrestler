.text
.arm
.align 2
.global test_cpu_state, test_cpu_state_thumb

@ call this function (from thumb or ARM, set r14 properly) and this function will return
@ whether it was called in ARM or thumb state (r7 = 1 for ARM, r7 = 2 for thumb)
@ note: load the address of test_cpu_state_thumb (ARM) in r6 before jumping here
@ value of r4 may be changed
@ useful for testing if things like blx, ldr r15, [...], pop {r15} ... are properly changing state in ARMv5

test_cpu_state:
    .hword 0x4730  @ bx r6 in thumb: jump to test_cpu_state_thumb
    .hword 0xE3A0  @ these two halfwords: mov r4, #0xC00000 in ARM
    @ if we reach this part: we are in ARM mode
    @ set r7 to 1 and return
    mov r7, #1
    bx r14    

test_cpu_state_thumb:
    @ set r7 to 2 and return
    mov r7, #2
    bx r14


@ 0100 01Op H1H2 Rs/Hs Rd/Hd - branch to Rs is possible (opcode 3)
@ 0100 0111  0 0  110   000    branches to r6
