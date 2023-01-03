.text
.arm
.align 2
.global arm7_main

.global val_CPSR
val_CPSR: .word 0

arm7_main:
    @ some initial values..
    mrs r0, cpsr
    str r0, val_CPSR


    @ init stack pointers
    bl init_stackpointers
    
    @ call "initial state" test which stores some results in main RAM where ARM9 can also access it
    bl test_initialstate


arm7_main_post_inittest:
    @ just reset stack
    bl init_stackpointers

    @ call C++ main loop function
    bl arm7_waitloop

    @ disable interrupts
    ldr r0, =0x4000208
    mov r1, #0
    str r1, [r0]

    bl disable_trace  @ useful to automatically disable trace when a test fails (enable it before ofc)
    b arm7_main_post_inittest



init_stackpointers:
    @ set irq and user mode stack pointer
    mrs r3, cpsr   @ old CPSR
    mov r4, r3
    bic r3, r3, #0x1F
    orr r3, r3, #0x12  @ IRQ
    msr cpsr_c, r3
    ldr r13, =0x380FF00  @ ARM7-WRAM +64 KB, minus a little bit so we don't overwrite the magic word at the end of WRAM

    msr cpsr_c, r4
    ldr r13, =0x380F000
    bx r14
