.text
.arm
.align 2
.global fail_test, timeout_test, timeout_rw

@ after failing a test: wait until B is pressed and then released
wait_b_released:
    ldr r0, =0x4000130
    ldrh r1, [r0]
    tst r1, #1 << 1   @ B
    bne wait_b_released
wait_b_released2:
    ldrh r1, [r0]
    tst r1, #1 << 1
    beq wait_b_released2

    b main_post_init

@ these are called as functions but do not return (the test failed anyway)
@ -> show that the test failed, then reset back to the menu
@ r0 contains the number of the test that failed (for fail_test, timeout_test)
fail_test:
    push {r0}
    bl disable_trace
    ldr r0, =txt_fail
    mov r1, #5
    pop {r2}
    bl cpp_fail_test
    b wait_b_released

timeout_test:
    push {r0}
    bl disable_trace
    ldr r0, =txt_timeout
    mov r1, #8
    pop {r2}
    bl cpp_fail_test
    b wait_b_released

timeout_rw:
    bl disable_trace
    ldr r0, =txt_timeout_rw
    mov r1, #12
    mov r2, #-1
    bl cpp_fail_test
    b wait_b_released


txt_fail:       .asciz "FAIL "
txt_timeout:    .asciz "TIMEOUT "
txt_timeout_rw: .asciz "TIMEOUT R/W "
