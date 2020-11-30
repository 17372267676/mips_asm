rm -f sl
#mips64el-unknown-linux-gnu-gcc -march=mips64r2 -mabi=64 -static -g test.c -otest
mips64el-unknown-linux-gnu-gcc -march=mips64r2 -mabi=64 -static -g spinlock.c -osl
qemu-mips64el sl
