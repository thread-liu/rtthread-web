C_SOURCES =  \
main.c \
src/clock.c \
src/components.c \
src/idle.c \
src/ipc.c \
src/irq.c \
src/kservice.c \
src/mem.c \
src/object.c \
src/scheduler.c \
src/thread.c \
src/timer.c \
components/finsh/shell.c \
components/finsh/cmd.c \
components/finsh/msh.c \
libcpu/posix/cpu_port.c 

C_INCLUDES =  \
-I. \
-Iinclude \
-Iinclude/libc \
-Icomponents/finsh 

rtthread: rtthread.o
	emcc $(C_SOURCES) $(C_INCLUDES) -pthread -s WASM=1 -o rtthread.html
rtthread.o: $(C_SOURCES)

clean:
	rm -rf rtthread rtthread.o

