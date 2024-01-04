# libtdk

## About

A simple C99 terminal manipulation library for Linux that handles some terminal
properties, styles and trivial key readings with UTF-8 support.

## Install

### Dependencies

- `gcc`
- `make`

### Step-By-Step

- Use `make` to install it.

```sh
sudo make install
```

- Add the following environment variables to your shell startup file.

```sh
export LIBRARY_PATH=${LIBRARY_PATH}:/usr/local/lib
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib
```

- Reopen your shell.

## Uninstall

- Use `make` to uninstall it.

```sh
sudo make uninstall
```

## Documentation

Once installed, you can refer to its man pages:

```sh
man tdk.3
```

## Help

You can ask for help related to this project through its [issues page](https://github.com/skippyr/libtdk/issues).

## Contributing

This project is open to receive contributions. You can send code contributions
to be reviewed through its [pull requests page](https://github.com/skippyr/libtdk/pulls).

By contributing to this project, you agree to use the same copyright used by it.

## Copyright

BSD-3-Clause License\
Copyright (c) 2023, Sherman Rofeman \<skippyr.developer@gmail.com\>

See the `LICENSE` file that comes in its source code for more details.
