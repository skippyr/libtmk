<p align="center">
  <img alt="" src="assets/ornament.png" width=1020 />
</p>
<p align="center">
<img alt="" src="assets/dragon.png" width=200 />
</p>
<h1 align="center">&ensp;libtmk&ensp;</h1>
<p align="center">— Terminal Manipulation Kit – README – Release 17.0.x —<br />C Programming Language</p>
<p align="center">
  <img alt="" src="https://img.shields.io/github/license/skippyr/libtmk?style=plastic&label=%E2%89%A5%20license&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/v/tag/skippyr/libtmk?style=plastic&label=%E2%89%A5%20tag&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/commit-activity/t/skippyr/libtmk?style=plastic&label=%E2%89%A5%20commits&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/stars/skippyr/libtmk?style=plastic&label=%E2%89%A5%20stars&labelColor=%2324130e&color=%23b8150d" />
</p>

## About
Libtmk (_aka_ "Terminal Manipulation Kit") is a simple C99 terminal manipulation library that contains a modest set of features to manipulate terminal properties, styles, arguments and key readings primarily using UTF-8 encoding. It is available for Windows, macOS and Linux.

With its minimalistic and opinionated design, it encourages the development of software with great system compatibility, while leaving room to full-feature libraries to be built upon for more specific use cases.
## Install
### Dependencies
Due to its cross-platform nature, it is highly recommended to use this library with a setup that combines CMake and Git. For that, the following dependencies are required, varying depending of the operating system you are using:
- **Windows:**
  -  **Visual Studio 2022** or later **with the "Desktop Development with C++ Workload":** it contains most of the tools required to develop and build C projects, including a compiler and CMake.
  - **Git:** it will be used to clone and maintain a version of the library's source code within your project.
> [!TIP]
> You can install these packages using Winget, the official Windows package manager, or Chocolatey, an alternative package manager.
- **macOS:**
  - **Apple Command Line Tools:** it contains most of the tools required to build C projects, including a compiler and Git.
  - **CMake:** it will be used to integrate and build the library within your project.
> [!TIP]
> You can install the Apple Command Line Tools using the command `xcode select --install`. For the rest, use [HomeBrew](https://brew.sh).

- **Linux:**
  - **GCC** or **Clang:** it will be used to compile the library.
  - **CMake** and **Make:** it will be used to integrate and build the library within your project.
  - **Git:** it will be used to clone and maintain a version of the library's source code within your project.

> [!TIP]
> You can install these packages using your distro's package manager. If it does not have one, use [HomeBrew](https://brew.sh).
### Procedures
On Windows, using the `Developer PowerShell for VS 2022` profile or, on any other operating systems, using any terminal, follow these instructions:
- In the root directory of your project, use Git to clone the library's repository as a submodule. This will download its latest version and allow you to update it easily by pulling new changes in the future:
```sh
git submodule add https://github.com/skippyr/libtmk libs/libtmk
```
- In your `CMakeLists.txt` configuration file, add the library's directory you have cloned as a subdirectory. That will make the library target `tmk` available for you to link it to your executable targets. Use the following example as a reference, adapting it to your project. It links the library to the executable `src/main.c`, named as target `main` in the project `tmk-example`:
```cmake
cmake_minimum_required(VERSION 3.25)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
add_subdirectory("${CMAKE_SOURCE_DIR}/libs/libtmk"
                 "${CMAKE_BINARY_DIR}/libtmk")
project(tmk-example)
add_executable(main "${CMAKE_SOURCE_DIR}/src/main.c")
target_link_libraries(main tmk)
```
- Delete and recreate the CMake auto-generated build files and, at the next time you build your project, the library will be automatically linked to your executable.
## Documentation
After installed, you can learn how to use it by reading its [documentation](DOCS.pdf).
## Help
If you need any help related to this project, create a new issue in its [issues page](https://github.com/skippyr/libtmk/issues) or send an [e-mail](mailto:skippyr.developer@icloud.com) describing what is going on.
## Contributing
This project is open to review and possibly accept contributions in the form of bug reports and suggestions. If you are interested, send your contribution to its pull [requests page](https://github.com/skippyr/libtmk/pulls) or via [e-mail](mailto:skippyr.developer@icloud.com).
## License
This is free software licensed under the BSD-3-Clause License that comes WITH NO WARRANTY. Refer to the `LICENSE` file that comes in its source code for license and copyright details.

&ensp;
<p align="right"><i>For the memory of all fellow dragons seeing this,<br />in the help to keep your flames alive.</i></p>
&ensp;
<p align="right"><sup><strong>Here Be Dragons!</strong><br />Made with love.</sup></p>
