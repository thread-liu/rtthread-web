# rtthread-web

使用 WebAssembly 技术将 RT-Thread 运行在浏览器上。

## [开发环境准备](https://emscripten.org/docs/getting_started/downloads.html)

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


### 工具

https://github.com/WebAssembly/binaryen

https://github.com/emscripten-core/emscripten

### Demo

- [JSC](https://mbbill.github.io/JSC.js/demo/index.html).js** is a JavaScript engine which can run on top of you browser's JavaScript engine. It's based on the JavaScript engine (JavaScriptCore) of WebKit and compiled to wasm with emscripten. Therefore, if you're using Safari, you are literally running its JavaScript engine on top of itself.

  The size of JSC.wasm is around 4MB (compressed js and mem file).

### 参考链接

emcc 优化代码：https://cunzaizhuyi.github.io/emcscripten-Optimizing-Code/

emcc 命令行参数： https://blog.csdn.net/weixin_34235371/article/details/88962382?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.channel_param&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.channel_param

JS 调用 C 代码：https://developer.mozilla.org/zh-CN/docs/WebAssembly/C_to_wasm

## Reference

- [WebAssembly](https://en.wikipedia.org/wiki/WebAssembly)
- [Dynamic Machine Code Generation. Google.](https://v8.dev/#mach_code)

