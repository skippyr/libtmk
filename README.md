<p align="center">
	<img alt="" src="ornament.webp" />
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

A simple C99 terminal manipulation library for GNU/Linux that is capable of handling some terminal properties, styles and key readings with UTF-8 encoding.

Designed to be an alternative to curses-like libraries, it offers a solid base for creating general purpose terminal based softwares, while leaving room to full-feature libraries to be built upon for more specific use cases.

## ❡ Install

### Dependencies

The following dependencies are required to install it:

- `git`: it will be used to clone this repository.
- `gcc`, `make`: they will be used to build and install this library.

### Procedures

Follow these steps:

- Clone this repository using `git`:

```sh
git clone --depth 1 https://github.com/skippyr/libtdk;
```

- Access the repository using `cd`:

```sh
cd libtdk;
```

- Use `make` to build and install it:

```sh
sudo make install clean;
```

- Export the following environment variables in your shell configuration file:

```sh
export LIBRARY_PATH=${LIBRARY_PATH}:/usr/local/lib;
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib;
```

- Reopen your shell.

## ❡ Uninstall

Follow these steps:

- Go back to the repository directory.
- Use `make` to uninstall it.

```sh
sudo make uninstall;
```

## ❡ Documentation

Once installed, you can read its documentation using `man`:

```sh
man tdk.3;
```

## ❡ Help

If you need help related to this project, open a new issue in its [issues pages](https://github.com/skippyr/libtdk/issues) or send me an [e-mail](mailto:skippyr.developer@gmail.com) describing what is going on.

## ❡ Contributing

This project is open to review and possibly accept contributions, specially fixes and suggestions. If you are interested, send your contribution to its [pull requests page](https://github.com/skippyr/libtdk/pulls) or to my [e-mail](mailto:skippyr.developer@gmail.com).

By contributing to this project, you agree to license your work under the same license that the project uses.

## ❡ License

This project is licensed under the BSD-3-Clause License. Refer to the `LICENSE` file that comes in its source code for license and copyright details.

&ensp;
<p align="center"><sup><strong>≥v≥v&ensp;Here Are Dragons!&ensp;≥v≥</strong><br />Made with love by skippyr <3</sup></p>