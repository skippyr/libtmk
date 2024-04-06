<p align="center">
  <img alt="" src="assets/ornament.webp" />
</p>
<h1 align="center">≥v≥v&ensp;libtdk&ensp;≥v≥v</h1>
<p align="center">Terminal Development Kit</p>
<p align="center">
  <img alt="" src="https://img.shields.io/github/license/skippyr/libtdk?style=plastic&label=%E2%89%A5%20license&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/v/tag/skippyr/libtdk?style=plastic&label=%E2%89%A5%20tag&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/commit-activity/t/skippyr/libtdk?style=plastic&label=%E2%89%A5%20commits&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/stars/skippyr/libtdk?style=plastic&label=%E2%89%A5%20stars&labelColor=%2324130e&color=%23b8150d" />
</p>

## ❡ About

A simple C99 terminal manipulation library for Linux and Windows that can be used to develop cross-platform terminal application using C and C++. In summary, it includes features to manipulate terminal properties, styles and key events encoded in UTF-8.

Designed to be an alternative to curses-like libraries, it offers a solid base for creating general purpose terminal based softwares, while leaving room to full-feature libraries to be built upon for more specific use cases.

## ❡ Install

### Dependencies

The following dependencies must be installed before installing it:

#### Dependencies For Windows

- **Visual Studio 2022**: it provides all the tools required to build this library.
- **git**: it will be used to clone this repository.

#### Dependencies For Linux

- **gcc**, **cmake**, **ninja**: they will be used to build this library.
- **git**: it will be used to clone this repository.

### Procedures

The recommended way of using this library is by including it as a git submodule in your project and using CMake to perform its build and linking. That way you will be able to automatically download it as a dependency when you clone your repository and also be able to decide whether or not to update it.

On Windows, using `Developer PowerShell For VS 2022`, or, on Linux, using any terminal, follow these steps:

- In the root of your project, where your `CMakeLists.txt` is, clone this repository as a submodule:

```sh
git submodule add --depth 1 "https://github.com/skippyr/libtdk" "lib/libtdk";
```

- In your `CMakeLists.txt`, add this library and link it to your executable target. In this example, it will be linked to an executable `src/main.c`, but adapt it to your case:

```cmake
cmake_minimum_required(VERSION 3.20)
project(example-project)
# Add the executable to the build.
add_executable(main "${CMAKE_SOURCE_DIR}/src/main.c" "${CMAKE_SOURCE_DIR}/src/main.h")
# Add the library to the build.
add_library(tdk "${CMAKE_SOURCE_DIR}/lib/libtdk/src/tdk.c"
                "${CMAKE_SOURCE_DIR}/lib/libtdk/src/tdk.h")
# Includes its header file to the executable build.
target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/lib/libtdk/src")
# Links the library to the executable build.
target_link_libraries(main tdk)
```

- Now, you are able to build your project and use the library in your executable:

```sh
cmake -G Ninja -B build;
cmake --build build;
```

## ❡ Documentation

After installed, you can learn how to use it by reading its [documentation](docs.md).

## ❡ Help

If you need help related to this project, open a new issue in its [issues pages](https://github.com/skippyr/libtdk/issues) or send me an [e-mail](mailto:skippyr.developer@gmail.com) describing what is going on.

## ❡ Contributing

This project is open to review and possibly accept contributions, specially fixes and suggestions. If you are interested, send your contribution to its [pull requests page](https://github.com/skippyr/libtdk/pulls) or to my [e-mail](mailto:skippyr.developer@gmail.com).

By contributing to this project, you agree to license your work under the same license that the project uses.

## ❡ License

This is free software licensed under the BSD-3-Clause License that comes WITH NO WARRANTY. Refer to the `LICENSE` file that comes in its source code for license and copyright details.

&ensp;
<p align="center"><sup><strong>≥v≥v&ensp;Here Be Dragons!&ensp;≥v≥</strong><br />Made with love by skippyr <3</sup></p>
