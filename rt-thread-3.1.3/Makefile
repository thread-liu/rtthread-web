C_SOURCES =  \
main.c \
drivers/*.c \
src/*.c \
libcpu/posix/*.c

C_INCLUDES =  \
-I. \
-Iinclude \
-Iinclude/libc \
-Icomponents/finsh 

rtthread: rtthread.o
	emcc $(C_SOURCES) $(C_INCLUDES) -pthread -D_GNU_SOURCE -O0 -v --memoryprofiler -s WASM=1 -o rtthread.html
rtthread.o: $(C_SOURCES)

clean:
	rm -rf rtthread rtthread.o rtthread.wasm rtthread.js rtthread.html rtthread.worker.js
