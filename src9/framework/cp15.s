.text
.arm
.global cp15_get_controlreg, cp15_set_controlreg, cp15_get_DTCMcontrol, cp15_set_DTCMcontrol, cp15_get_ITCMcontrol, cp15_set_ITCMcontrol
.align 2

cp15_get_controlreg:
    mrc p15, #0, r0, c1, c0, #0   @ C1,C0,0
    bx r14

cp15_set_controlreg:
    mcr p15, #0, r0, c1, c0, #0   @ C1,C0,0
    bx r14

cp15_get_DTCMcontrol:
    mrc p15, #0, r0, c9, c1, #0   @ C9,C1,0
    bx r14

cp15_set_DTCMcontrol:
    mcr p15, #0, r0, c9, c1, #0   @ C9,C1,0
    bx r14

cp15_get_ITCMcontrol:
    mrc p15, #0, r0, c9, c1, #1   @ C9,C1,1
    bx r14

cp15_set_ITCMcontrol:
    mcr p15, #0, r0, c9, c1, #1   @ C9,C1,1
    bx r14
