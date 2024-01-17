<p align="center">
	<img src="banner.webp" alt="" />
</p>
<h1 align="center">⊼⊵⊼⊵&ensp;libtdk&ensp;⊼⊵⊼⊵</h1>
<p align="center">Terminal Development Kit</p>
<p align="center">
	<img src="https://img.shields.io/github/license/skippyr/libtdk?style=social" alt="" />
	&nbsp;
	<img src="https://img.shields.io/github/v/tag/skippyr/libtdk?style=social" alt="" />
	&nbsp;
	<img src="https://img.shields.io/github/commit-activity/t/skippyr/libtdk?style=social" alt="" />
	&nbsp;
	<img src="https://img.shields.io/github/stars/skippyr/libtdk?style=social" alt="" />
</p>

## ❡ About

A simple C/C++ terminal manipulation library for Linux that is capable of handling some terminal properties, styles, effects and trivial key readings with UTF-8 encoding.

Designed to be an alternative to curses-like libraries, it offers a solid base for creating general purpose terminal based softwares, while leaving room to full-feature libraries to be built upon for more specific use cases.

## ❡ Install

### Dependencies

The following dependencies are required to install it:

&emsp;**1.**&ensp;**git**: it will be used to clone this repository.\
&emsp;**2.**&ensp;**gcc**, **make**: they will be used to compile and install it.

### Procedures

Using a terminal, follow these steps:

&emsp;**1.**&ensp;Clone this repository using git:

```sh
git clone https://github.com/skippyr/libtdk;
```

&emsp;**2.**&ensp;Access the repository directory using cd:

```sh
cd libtdk;
```

&emsp;**3.**&ensp;Use make to compile and install it:

```sh
sudo make install clean;
```

&emsp;**4.**&ensp;Export the following environment variables in your shell startup file:

```sh
export LIBRARY_PATH=${LIBRARY_PATH}:/usr/local/lib;
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib;
```

&emsp;**5.**&ensp;Reopen your shell.

## ❡ Uninstall

Using a terminal, follow these steps:

&emsp;**1.**&ensp;Go back to the repository directory.\
&emsp;**2.**&ensp;Use make to uninstall it:

```sh
sudo make uninstall;
```

## ❡ Documentation

After installed, you can refer to its manual pages by using man:

```sh
man tdk.3;
```

## ❡ Help

If you need help about this project, open a new issue in its [issues page](https://github.com/skippyr/libtdk/issues) or send me an [e-mail](mailto:skippyr.developer@gmail.com) describing what is going on.

## ❡ Contributing

This project is open to review and possibly accept contributions, specially fixes and suggestions. If you are interested, send your contribution to its [pull requests page](https://github.com/skippyr/libtdk/pulls) or to my [e-mail](mailto:skippyr.developer@gmail.com).

By contributing to this project, you must agree to license your work under the same license that the project uses.

## ❡ License

This project is licensed under the BSD-3-Clause License. Refer to the LICENSE file that comes in its source code for license and copyright details.

&nbsp;

<p align="center"><sup><strong>⊼⊵⊼⊵&ensp;Here Are Dragons!&ensp;⊼⊵⊼⊵</strong><br />Made with love by skippyr <3</sup></p>
