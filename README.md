<p align="center">
    <img alt="" src="assets/ornament.png" width=1020>
</p>
<h1 align="center">≥v≥v&ensp;libtmk&ensp;≥v≥v</h1>
<p align="center">Terminal Manipulation Kit</p>
<p align="center">
    <img alt="" src="https://img.shields.io/github/license/skippyr/libtmk?style=plastic&label=%E2%89%A5%20license&labelColor=%2324130e&color=%23b8150d">
    &nbsp;
    <img alt="" src="https://img.shields.io/github/v/tag/skippyr/libtmk?style=plastic&label=%E2%89%A5%20tag&labelColor=%2324130e&color=%23b8150d">
    &nbsp;
    <img alt="" src="https://img.shields.io/github/commit-activity/t/skippyr/libtmk?style=plastic&label=%E2%89%A5%20commits&labelColor=%2324130e&color=%23b8150d">
    &nbsp;
    <img alt="" src="https://img.shields.io/github/stars/skippyr/libtmk?style=plastic&label=%E2%89%A5%20stars&labelColor=%2324130e&color=%23b8150d">
</p>

## ❡ About

A simple C99 terminal manipulation library for C/C++ with features to manipulate terminal properties, styles and key readings targetting UTF-8 encoding. It is available for Windows, Linux and MacOS.

Designed to be an alternative to curses-like libraries, it offers a solid base for creating general purpose terminal based softwares, while leaving room to full-feature libraries to be built upon for more specific use cases.

## ❡ Install

### Dependencies

The following dependencies must be installed before it:

#### Dependencies For Windows

- **[Visual Studio 2022](https://visualstudio.microsoft.com) with the "Desktop development with C++" workload:** it provides all the tools required to build this library.
- **[Git](https://git-scm.com):** it will be used to clone this repository.

#### Dependencies For Linux

- **GCC, CMake:** they will be used to build this library.
- **Git:** it will be used to clone this repository.

> [!TIP]
> Use your distro package manager to install these packages.

#### Dependencies For MacOS

- **Apple Command Line Tools, CMake:** they will be used to build this library.
- **Git:** it will be used to clone this repository.

> [!TIP]
> Use `xcode-select --install` to install the Apple command line tools. For the rest, use [HomeBrew](https://brew.sh/).

### Procedures

On Windows, using the `Developer PowerShell for VS 2022` profile or, on any other operating systems, using any terminal, follow these instructions:


- In the root of your project, use `git` to add this library as a submodule:

```zsh
git submodule add https://github.com/skippyr/libtmk libs/libtmk
```

- In your `CMakeLists.txt` file, add the library as a subdirectory (it will be target `tmk`) and link it against your executable target, in this example, it will be `src/main.c`:

```cmake
cmake_minimum_required(VERSION 3.20)
project(tmk-example)
add_subdirectory("${CMAKE_SOURCE_DIR}/libs/libtmk" "${CMAKE_BINARY_DIR}/libtmk")
add_executable(main "${CMAKE_SOURCE_DIR}/src/main.c")
target_link_libraries(main tmk)
```

- The next time you build your project, the library will be automatically linked into your executable.

## ❡ Documentation

After installed, you can learn how to use it by reading its [documentation](DOCS.md) or by hovering your mouse pointer over any of its identifiers in your IDE.

## ❡ Help

If you need help related to this project, open a new issue in its [issues pages](https://github.com/skippyr/libtmk/issues) or send me an [e-mail](mailto:skippyr.developer@icloud.com) describing what is going on.

## ❡ Contributing

This project is open to review and possibly accept contributions, specially fixes and suggestions. If you are interested, send your contribution to its [pull requests page](https://github.com/skippyr/libtmk/pulls) or to my [e-mail](mailto:skippyr.developer@icloud.com).

In order to keep it open-source, by contributing to this project, you must agree to license your work under the same license that the project uses. For other intentions, prefer to create a fork.

## ❡ License

This is free software licensed under the BSD-3-Clause License that comes WITH NO WARRANTY. Refer to the `LICENSE` file that comes in its source code for license and copyright details.

&ensp;
<p align="center"><sup><strong>≥v≥v&ensp;Here Be Dragons!&ensp;≥v≥</strong><br>Made with love by skippyr <3</sup></p>
