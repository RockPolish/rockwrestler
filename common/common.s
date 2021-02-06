.arm
.align 2
.text
.global wait_busy, l_EIC, l_DIC

wait_busy:
    subs r0, #1
    bne wait_busy
    bx r14

l_EIC:
    @ clear "interrupt disable" flag in CPSR
    mrs r3, cpsr
    bic r3, #1 << 7
    msr cpsr_c, r3
    bx r14

l_DIC:
    @ set "interrupt disable" flag in CPSR
    mrs r3, cpsr
    orr r3, #1 << 7
    msr cpsr_c, r3
    bx r14




