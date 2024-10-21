# Tmk – Terminal Manipulation Kit

## About
A cross-platform terminal manipulation library for C++20 with features to manipulate terminal properties, styles and key readings using UTF-8 encoding.

With a design inspired by the C# Console API and intended to be used in CMake projects, it offers a solid base for creating general purpose terminal software, while leaving room for full-feature libraries to be built upon for more specific use cases.

What about introducing you to its syntax by using a quick demo application:

```cpp
#include <Tmk/CommandLineArguments.h>
#include <Tmk/Key.h>
#include <Tmk/Terminal.h>

int main(int totalMainArguments, const char** mainArguments)
{
    Tmk::CommandLineArguments commandLineArguments = Tmk::CommandLineArguments(totalMainArguments, mainArguments);
    Tmk::Terminal::Window::OpenAlternateWindow();
    Tmk::Terminal::Cursor::SetVisible(false);
    Tmk::Terminal::Output::WriteLine("Here Be Dragons! 🔥🐉");
    Tmk::Terminal::Output::WriteLine("Welcome to the family");
    Tmk::Terminal::Output::WriteLine();
    Tmk::Terminal::Output::WriteLine("Press the [{}] key to exit", Tmk::Key::Enter.GetName());
    while (true)
    {
        Tmk::KeyEvent keyEvent = Tmk::Terminal::Input::ReadKeyEvent();
        if (keyEvent.GetKey() == Tmk::Key::Enter)
        {
            break;
        }
    }
    Tmk::Terminal::Cursor::SetVisible(true);
    Tmk::Terminal::Window::CloseAlternateWindow();
    return 0;
}
```

## Help
If you need help related to this project, open a new issue in its [issues pages](https://github.com/skippyr/Tmk/issues) or send me an [e-mail](mailto:skippyr.developer@icloud.com) describing what is going on.

## Contributing
This project is open to review and possibly accept contributions, specially fixes and suggestions. If you are interested, send your contribution to its [pull requests](https://github.com/skippyr/Tmk/pulls) page or to my [e-mail](mailto:skippyr.developer@icloud.com).

In order to keep it open-source, by contributing to this project, you must agree to license your work under the same license that the project uses. For other intentions, prefer to create a fork.

## Copyright
This is free software licensed under the BSD-3-Clause License that comes WITH NO WARRANTY. Refer to the `LICENSE` file that comes in its source code for license and copyright details.
