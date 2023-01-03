#pragma once

// ARMv4
extern "C" void run_tests_armv4_conditioncodes();

// ARMv5
extern "C" void run_tests_armv5_clz();
extern "C" void run_tests_armv5_qadd_qsub();
extern "C" void run_tests_armv5_qdadd_qdsub();
extern "C" void run_tests_armv5_SMLAxy();
extern "C" void run_tests_armv5_SMULxy();
extern "C" void run_tests_armv5_SMLAWy();
extern "C" void run_tests_armv5_SMULWy();
extern "C" void run_tests_armv5_SMLALxy();
extern "C" void run_tests_armv5_blx();
extern "C" void run_tests_armv5_ldrpopr15();
extern "C" void run_tests_armv5_ldm_stm();

// IPC
void run_tests_ipcsync();
void run_tests_ipcfifo();
void run_tests_ipcfifo_irq();

// DS maths
void run_tests_ds_maths_sqrt_32();
void run_tests_ds_maths_sqrt_64();
void run_tests_ds_maths_div_32_32();
void run_tests_ds_maths_div_64_32();
void run_tests_ds_maths_div_64_64();

// memory control
void run_tests_vramcnt();
void run_tests_wramcnt();
void run_tests_tcm();

// initial state
void run_tests_initialstate_ipc_irq_cpsr();
void run_tests_initialstate_cp15();
