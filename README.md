# rtthread-web

使用 WebAssembly 技术将 RT-Thread 运行在浏览器上。

## 开发环境准备

### 安装 Emscripten

- basic

```
git clone https://github.com/emscripten-core/emsdk.git
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

- option

```
echo 'source "/Users/mac/work/emsdk/emsdk_env.sh"' >> $HOME/.bash_profile
```

### simple demo

- 创建一个 helloword 文件夹

```
mkdir hello
cd hello
```

- 创建 hello.c:

```
#include <unistd.h>
#include <stdio.h>

int main(int argc, char ** argv) {
    for (size_t i = 0; i < 1000; i++)
    {
        printf("Hello, world!\n");
        sleep(1)
    }
}
```

- 编译 hello.c

```
emcc hello.c -s WASM=1 -o hello.html
```

- 创建一个 web server 来展示编译后的文件：

```
emrun --no_browser --port 8080 .
```

- 在浏览器中打开页面

```
http://localhost:8080/hello.html
```

通过这个例子我发现目前这个 demo 是等程序运行完才会在网页端显示，而不是能看到没延时 1s，打印一次 hello world!