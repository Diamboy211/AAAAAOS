export CC=i686-elf-gcc
export LD=i686-elf-ld
export CFLAGS="-Wall -Wextra -O2 -msse2 -ffreestanding -m32"

nasm -f bin src/hello.asm -o build/hello.bin
nasm -f elf src/kernel_init.asm -o build/kernel_init.o
${CC} ${CFLAGS} -c src/kernel.c -o build/kernel.o
${CC} ${CFLAGS} -c src/io.c -o build/io.o
${CC} ${CFLAGS} -c src/rand.c -o build/rand.o
${CC} ${CFLAGS} -c src/exception_handler.c -o build/exception_handler.o
${CC} ${CFLAGS} -c src/idt.c -o build/idt.o
${CC} ${CFLAGS} -c src/draw.c -o build/draw.o
${CC} ${CFLAGS} -c src/math.c -o build/math.o
${LD} -Ofast -Ttext 0x1000\
	build/kernel_init.o\
	build/kernel.o\
	build/io.o\
	build/rand.o\
	build/exception_handler.o\
	build/idt.o\
	build/draw.o\
	build/math.o\
	-o build/kernel.bin --oformat binary
dd if=/dev/zero of=final.img conv=notrunc bs=512 seek=0 count=16
dd if=build/hello.bin of=final.img conv=notrunc bs=512 seek=0 count=1
dd if=build/kernel.bin of=final.img conv=notrunc bs=512 seek=1
# dd if=sound.bin of=final.img conv=notrunc bs=512 seek=1
