.text
.arm
.align 2
.global arm9_main, main_post_init, val_CPSR

val_CPSR: .word 0

.global val_cp15 @ just use the start as an array
val_cp15:
val_cp15_100: .word 0
val_cp15_200: .word 0
val_cp15_201: .word 0
val_cp15_300: .word 0
val_cp15_500: .word 0
val_cp15_501: .word 0
val_cp15_502: .word 0
val_cp15_503: .word 0

val_cp15_600: .word 0
val_cp15_610: .word 0
val_cp15_620: .word 0
val_cp15_630: .word 0
val_cp15_640: .word 0
val_cp15_650: .word 0
val_cp15_660: .word 0
val_cp15_670: .word 0

val_cp15_910: .word 0
val_cp15_911: .word 0

@ entry point
arm9_main:
    mrs r0, cpsr
    str r0, val_CPSR

    mrc p15, #0, r0, c1, c0, #0
    str r0, val_cp15_100
    mrc p15, #0, r0, c2, c0, #0
    str r0, val_cp15_200
    mrc p15, #0, r0, c2, c0, #1
    str r0, val_cp15_201
    mrc p15, #0, r0, c3, c0, #0
    str r0, val_cp15_300
    mrc p15, #0, r0, c5, c0, #0
    str r0, val_cp15_500
    mrc p15, #0, r0, c5, c0, #1
    str r0, val_cp15_501
    mrc p15, #0, r0, c5, c0, #2
    str r0, val_cp15_502
    mrc p15, #0, r0, c5, c0, #3
    str r0, val_cp15_503

    mrc p15, #0, r0, c6, c0, #0
    str r0, val_cp15_600
    mrc p15, #0, r0, c6, c1, #0
    str r0, val_cp15_610
    mrc p15, #0, r0, c6, c2, #0
    str r0, val_cp15_620
    mrc p15, #0, r0, c6, c3, #0
    str r0, val_cp15_630
    mrc p15, #0, r0, c6, c4, #0
    str r0, val_cp15_640
    mrc p15, #0, r0, c6, c5, #0
    str r0, val_cp15_650
    mrc p15, #0, r0, c6, c6, #0
    str r0, val_cp15_660
    mrc p15, #0, r0, c6, c7, #0
    str r0, val_cp15_670

    mrc p15, #0, r0, c9, c1, #0
    str r0, val_cp15_910
    mrc p15, #0, r0, c9, c1, #1
    str r0, val_cp15_911

    bl init_stackpointers
    bl test_initialstate
    bl init_iostate
    bl l_resetIPCSYNC

@ main loop
main_post_init:
    bl init_stackpointers    @ there's some jumps that aren't really function calls.. if we return to this from a test we just reset the stack
    bl cpp_menu
    @ cpp menu never returns, but just to be sure: infinite loop
forever:
    b forever



@ initialize the current r13 + IRQ r13
init_stackpointers:
    mrs r3, cpsr   @ save old CPSR
    mov r4, r3
    @ IRQ
    bic r3, r3, #0x1F
    orr r3, r3, #0x12
    msr cpsr_c, r3
    ldr r13, =stack_irq+4096
    @ change back to old mode
    msr cpsr_c, r4
    ldr r13, =stack_normal+4096
    bx r14


.pool

@ stacks are just put in RAM to avoid DTCM dependency
stack_normal:
    .space 4096
stack_irq:
    .space 4096

