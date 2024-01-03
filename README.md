# libtdk - Terminal Development Kit

## About

A simple C/C++ terminal manipulation library for Linux development. Designed to
be an alternative for curses-like libraries, it is capable of handling some
terminal properties, styles and trivial key press events with support for UTF-8
encoding.

It is easy to learn, to debug and to be less likely to give you memory leak
issues. However, it lacks features: what opens room for interested people to
create forks and use its functions as building blocks for more extensive
libraries.

## Install

### Dependencies

The following dependencies are required to install it:

- The version control software `git` will be used to download this repository.
- The GNU core utilities and C build utilites, such as `gcc` and `make`, will be
  used to compile and install this software in your system. They may also
  include the standard C headers.

Generally, you are able to install most these dependencies through the use of
your Linux distro package manager. Some dependencies may already be installed in
your operating system

### Step-By-Step Procedures

After its dependencies have been met, to install it, follow this step-by-step
using a command-line utility:

- Clone this repository using `git`.

```sh
git clone --depth 1 https://github.com/skippyr/libtdk
```

- Access the repository directory using `cd`.

```sh
cd libtdk
```

- Use `make` to install it.

```sh
sudo make install clean
```

- Add the following environment variables to your shell startup file, in order
  to make compilers/linkers find the installed files under the `/usr/local`
  directories upon its next restart:

```sh
# It adds /usr/local/lib to compilers look-up directories.
export LIBRARY_PATH=${LIBRARY_PATH}:/usr/local/lib

# It adds /usr/local/lib to linkers look-up directories.
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib
```

Some compilers may also get in conflict with each other, so it is a good idea to
use the same compiler that you used to install this library on your source files
either.

If you want a custom install, you can configure the build process by editing the
`config.mk` file.

## Uninstall

To uninstall it, follow this step-by-step using a command-line utility:

- Go back to the repository directory.
- Use `make` to uninstall it.

```sh
sudo make uninstall clean
```

## Documentation

Once installed, you can refer to its man pages:

```sh
man tdk.3
```

## Help

You can ask for help related to this project through its [issues page](https://github.com/skippyr/libtdk/issues).

## Contributing

This project is open to receive contributions from people of any skill level.
You can send code contributions to be reviewed through its [pull requests page](https://github.com/skippyr/libtdk/pulls).

By contributing to this project, you agree to use the same copyright used by it.

## Copyright

BSD-3-Clause License\
Copyright (c) 2023, Sherman Rofeman \<skippyr.developer@gmail.com\>

See the `LICENSE` file that comes in its source code for more details.
