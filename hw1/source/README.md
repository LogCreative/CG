# 源代码编译

本源代码需要使用 [CMake](https://cmake.org) 编译，推荐使用 CLion 集成环境。

编译方式：
```bash
mkdir cmake-build
cd cmake-build
cmake ..
```
然后打开相关的工程文件进行编译。

## Windows

必须使用 MSVC 编译（生成 Visual Studio 文件的方法），暂不支持使用 MinGW GCC 编译（会导致运行时动态依赖丢失）。如果使用的是 CLion，需要调整设置：`File` -> `Settings` -> `Build, Execution and Deployment` -> `CMake` -> `Toolchain` 选为 `Visual Studio`。

## macOS

没有特别需要安装的依赖。

## Linux

对于 Ubuntu 需要安装以下依赖：

```bash
sudo apt-get install build-essential
sudo apt-get install libx11-dev
sudo apt-get install xorg-dev libglu1-mesa-dev
```