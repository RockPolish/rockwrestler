# Rockwrestler test ROM for NDS emulators
Work in progress - aims to test all NDS features such as the IPC, memory control, interrupts, and so forth. You should first aim to boot and pass armwrestler to ensure that the ARM CPU is running relatively bug free. As armwrestler does not very thoroughly check some new ARMv5 features, tests for this are available too.
Rockwrestler uses the same LCDC mode as armwrestler to draw on the screen.

Almost all code except CPU tests is written in C++. You can use the `objdump` files `7.txt` and `9.txt` to translate between addresses and C++ code (mind the name mangling). An advantage of writing it in C++ is that you can compile the ROM without optimizations, which might generate simpler code that is easier to run for a buggy ARM core. It is meant to be compiled to ARM. No "advanced" C++ features like exceptions and polymorphism are used.

## The tests
On the top right, there's a green or red light next to ARM7. This indicates if the ARM7 is currently in its main loop where it awaits tests. If this light is red due to a failed test, the ARM7 may be stuck somewhere and so future tests involving it will certainly fail. If this light is red when booting the ROM, either `IPCSYNC` is not working which is used for this, or the ARM7 is stuck somewhere else. Use the source code to look up a failed test number to find out what test it is and why it failed.

### ARMv5
These tests written in assembly provide some testing of new instructions, as well as testing the improved ARM-thumb interworking. `clz`, `qadd`, `qsub`, `qdadd`, `qdsub`, `blx`, and the five new multiplies `SMULxy`, `SMLAxy`, `SMULWy`, `SMLAWy`, and `SMLALxy` are the new instructions it currently tests. It additionally tests to ensure that `ldr`, `ldm` (ARM) and `pop` (thumb) can switch the state bit when loading into `r15`.

### IPC
- The `IPCSYNC` test tries out this feature, including interrupts on both CPUs.
- `IPCFIFO` sends some words back and forth between the two CPUs. This includes checking cases like reading from an empty FIFO and acknowledging the error bit.
- `IPCFIFO IRQ` tests the IPCFIFO's "send FIFO empty" and "receive FIFO not empty" IRQs.

### DS MATH
These tests try out the NDS IO registers for calculating divisions and square roots. Only results are checked, the timings aren't checked. `SQRT 32` and `SQRT 64` test the square root on 32 and 64-bit numbers respectively, while `DIV 32/32`, `DIV 64/32`, and `DIV 64/64` test division where the first number represents the size of the numerator, and the second number the size of the denominator.
### Memory
- `WRAMCNT` tests sharing (parts of) the 32 KB of shared WRAM with the ARM7. This test requires IPCSYNC and IPCFIFO to be implemented as this is used to communicate between the CPUs.
- `VRAMCNT` tests allocating the different VRAM banks to different addresses, including allocating banks to the ARM7 to be used as WRAM . This test requires IPCSYNC and IPCFIFO to be implemented as this is used to communicate between the CPUs.
- `TCM` tests the DTCM and ITCM, including load mode, and overlapping TCM with another memory region.
### Initial state
Tests in this submenu are not actually doing a test, but rather printing values of certain IO registers, CP15 registers, and other values at startup. There are currently no correct/wrong values here.

## Contributing
While the name may be a funny combination of RockPolish and armwrestler, this is in no way *my* test ROM. I welcome anyone to contribute and to improve existing tests or add new tests so that we can turn this into a very complete ROM that thoroughly tests all NDS features. Please contact me if you wish to contribute.

## Special thanks to
- http://problemkaputt.de/gbatek.htm
- https://github.com/StrikerX3 for helping me test on hardware and giving good feedback
- Many people that populate the NDS channel in the Emudev Discord server and discussed with me these tests, giving me feedback on how to improve them and which tests might be useful.

