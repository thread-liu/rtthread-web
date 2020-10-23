
C_SOURCES =  \
main.c \
drivers/*.c \
src/*.c \
libcpu/posix/*.c \

C_INCLUDES =  \
-I. \
-Iinclude \
-Iinclude/libc \
-Icomponents/finsh \
-Idrivers  \
-I../../../include \

# -s LLD_REPORT_UNDEFINED -s INVOKE_RUN=0 --memoryprofiler -Wwarn-absolute-paths -Werror -g -O2
EMCC_SETTING = -pthread  -v

rtthread: rtthread.o
	emcc $(C_SOURCES) $(C_INCLUDES) $(EMCC_SETTING) -o rtthread.html
rtthread.o: $(C_SOURCES)

clean:
	rm -rf rtthread rtthread.o rtthread.wasm rtthread.js rtthread.html rtthread.worker.js
