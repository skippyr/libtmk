<h1 align="center">libtdk</h1>
<p align="center">Terminal Development Kit</p>

## About

It is a simple C/C++ terminal manipulation library for UNIX-like operating
systems that is capable of handling some terminal properties, styles, effects
and trivial key readings with UTF-8 encoding.

Designed to be an alternative to curses-like libraries, it offers a solid base
for creating general purpose terminal based softwares, while leaving room to
full-feature libraries to be built upon for more specific use cases.

## Install

### Dependencies

The following dependencies are required to install it:

1. **git**: it will be used to clone this repository.
2. **gcc**, **make**: they will be used to compile and install it.

### Procedures

Using a terminal, follow these steps:

1. Clone this repository using git:

```sh
git clone --depth 1 https://github.com/skippyr/libtdk;
```

2. Access the repository directory using cd:

```sh
cd libtdk;
```

3. Use make to compile and install it:

```sh
sudo make install clean;
```

4. Export the following environment variables in your shell startup file:

```sh
export LIBRARY_PATH=${LIBRARY_PATH}:/usr/local/lib;
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib;
```

5. Reopen your shell.

## Uninstall

Using a terminal, follow these steps:

1. Go back to the repository directory.
2. Use make to uninstall it:

```sh
sudo make uninstall;
```

## Documentation

After installed, you can refer to its manual pages by using man:

```sh
man tdk.3;
```

## Help

If you need help about this project, open a new issue in its
[issues page](https://github.com/skippyr/libtdk/issues) or send me an
[e-mail](mailto:skippyr.developer@gmail.com) describing what is going on.

## Contributing

This project is open to review and possibly accept contributions, specially
fixes and suggestions. If you are interested, send your contribution to its
[pull requests page](https://github.com/skippyr/libtdk/pulls) or to my
[e-mail](mailto:skippyr.developer@gmail.com).

By contributing to this project, you must agree to license your work under the
same license that the project uses.

## License

This project is licensed under the BSD-3-Clause License. Refer to the LICENSE
file that comes in its source code for license and copyright details.
