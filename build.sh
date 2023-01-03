# create time.s file
python3 time.py

echo doing arm9...
arm-none-eabi-g++ -march=armv5te -O2 -marm -o arm9.o src9/entry.s src9/*.cpp src9/framework/*.cpp src9/framework/*.s src9/tests/*.cpp src9/tests/*.s common/*.cpp common/*.s -nostartfiles -T linker9.ld -fno-exceptions -fno-rtti -Wno-narrowing -fno-delete-null-pointer-checks # --save-temps
arm-none-eabi-objcopy -O binary arm9.o arm9.bin --image-base=0x2000100
arm-none-eabi-objdump arm9.o -D -M reg-names-raw > 9.txt

echo doing arm7...
arm-none-eabi-g++ -mcpu=arm7tdmi -O2 -marm -o arm7.o src7/entry.s src7/*.cpp src7/framework/*.cpp src7/framework/*.s src7/tests/*.cpp common/*.cpp common/*.s -nostartfiles -T linker7.ld -fno-exceptions -fno-rtti -Wno-narrowing -fno-delete-null-pointer-checks
arm-none-eabi-objcopy -O binary arm7.o arm7.bin --image-base=0x3800100
arm-none-eabi-objdump arm7.o -D -M reg-names-raw > 7.txt

ndstool -c rockwrestler.nds -r9 0x2000100 -e9 0x2000100 -r7 0x3800100 -e7 0x3800100 -9 arm9.bin -7 arm7.bin # -d data/

# remove files
rm arm7.bin arm9.bin arm7.o arm9.o
