# libtdk
A stupidly simple terminal manipulation library for Linux that handles
properties, styles and blocking key press with UTF-8 support.

## Requirements
In order to build it, you will only need a C99 compiler, like `gcc`, and `make`.

## Install
Edit the file `config.mk` to match your needs. Then, run the following command
to install it (use `sudo` if necessary):

```sh
make install
```

By default, this library gets installed under `/usr/local` directories. Some
compilers and operating systems may have settings that causes linkers to fail
locating the installed files.

If that is your case, you can add the following environment variables to your
shell sessions:

```sh
export LIBRARY_PATH=${LIBRARY_PATH}:/usr/local/lib
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib
```

Some compilers may also get in conflict with each other, so it is a good idea
to use the same compiler that you used to install this library on your source
files either.

## Uninstall
If you want to uninstall it, run the following command (use `sudo` if
necessary):

```sh
make uninstall
```

## Documentation
Once installed, you can refer to its man pages:

```sh
man tdk.3
```

## Copyright
See `LICENSE` for copyright and license details.
