#pragma once

#include "AnsiColor.h"
#include "CursorShape.h"
#include "Dimensions.h"
#include "FontEffect.h"
#include "FontWeight.h"
#include "Layer.h"
#include "RgbColor.h"
#include "StreamRedirectionException.h"
#include "System.h"

#include <iostream>

namespace Tmk
{
    /// <summary>Represents the terminal emulator.</summary>
    class Terminal final
    {
    private:
        /// <summary>A boolean that states the terminal features have been enabled.</summary>
        static bool s_hasEnabledFeatures;
        /// <summary>A boolean that states the input stream is redirected.</summary>
        static bool s_isInputRedirected;
        /// <summary>A boolean that states the output stream is redirected.</summary>
        static bool s_isOutputRedirected;
        /// <summary>A boolean that states the error stream is redirected.</summary>
        static bool s_isErrorRedirected;

        Terminal() = delete;

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
        /// <summary>Sets UTF-8 as the output encoding on Windows.</summary>
        static void SetUtf8Encoding();
        /// <summary>Enables the parse of ANSI escape sequences on Windows.</summary>
        static void EnableVirtualTerminalProcessing();
#endif
        /// <summary>Initiates the stream redirection cache.</summary>
        static void InitStreamRedirectionCache();
        /// <summary>Enables the required terminal features.</summary>
        static void EnableFeatures();

        /// <summary>Writes an ANSI escape sequence to the standard output or error streams.
        /// <tparam name="Args">A parameter pack containing the arguments to be formatted.</param>
        /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
        /// <param name="...">The arguments to be formatted.</param>
        template <typename... Args>
        static void WriteAnsiEscapeSequence(std::string_view format, Args... arguments)
        {
            if (!IsOutputRedirected())
            {
                Write(format, arguments...);
            }
            else if (!IsErrorRedirected())
            {
                WriteError(format, arguments...);
            }
        }

    public:
        /// <summary>Checks if the standard input stream is redirected.</summary>
        /// <return>A boolean that states the stream is redirected.</summary>
        static bool IsInputRedirected();
        /// <summary>Checks if the standard output stream is redirected.</summary>
        /// <return>A boolean that states the stream is redirected.</summary>
        static bool IsOutputRedirected();
        /// <summary>Checks if the standard error stream is redirected.</summary>
        /// <return>A boolean that states the stream is redirected.</summary>
        static bool IsErrorRedirected();

        /// <summary>Flushes the output buffer.</summary>
        static void FlushOutputBuffer();

        /// <summary>Formats and writes a string to the standard output stream.</summary>
        /// <tparam name="Args">A parameter pack containing the arguments to be formatted.</tparam>
        /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
        /// <param name="...">The arguments to be formatted.</param>
        template <typename... Args>
        static void Write(std::string_view format, Args... arguments)
        {
            EnableFeatures();
            std::cout << std::vformat(format, std::make_format_args(arguments...));
        }

        /// <summary>Writes a newline to the standard output stream.</summary>
        static void WriteLine();

        /// <summary>Formats and writes a string to the standard output stream with a newline appended to its end.</summary>
        /// <tparam name="Args">A parameter pack containing the arguments to be formatted.</tparam>
        /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
        /// <param name="...">The arguments to be formatted.</param>
        template <typename... Args>
        static void WriteLine(std::string_view format, Args... arguments)
        {
            Write(format, arguments...);
            std::cout << std::endl;
        }

        /// <summary>Formats and writes a string to the standard error stream.</summary>
        /// <tparam name="Args">A parameter pack containing the arguments to be formatted.</tparam>
        /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
        /// <param name="...">The arguments to be formatted.</param>
        template <typename... Args>
        static void WriteError(std::string_view format, Args... arguments)
        {
            EnableFeatures();
            FlushOutputBuffer();
            std::cerr << std::vformat(format, std::make_format_args(arguments...));
        }

        /// <summary>Writes a newline to the standard error stream.</summary>
        static void WriteErrorLine();

        /// <summary>Formats and writes a string to the standard error stream with a newline appended to its end.</summary>
        /// <tparam name="Args">A parameter pack containing the arguments to be formatted.</tparam>
        /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
        /// <param name="...">The arguments to be formatted.</param>
        template <typename... Args>
        static void WriteErrorLine(std::string_view format, Args... arguments)
        {
            WriteError(format, arguments...);
            std::cerr << std::endl;
        }

        /// <summary>Sets an ANSI color into a layer.</summary>
        /// <param name="color">The color to be applied.</param>
        /// <param name="layer">The layer to be affected.</param>
        static void SetFontColor(AnsiColor color, Layer layer);
        /// <summary>Sets an RGB color into a layer.</summary>
        /// <param name="color">The color to be applied.</param>
        /// <param name="layer">The layer to be applied.</param>
        static void SetFontColor(const RgbColor& color, Layer layer);
        /// <summary>Resets the font colors.</summary>
        static void ResetFontColors();
        /// <summary>Sets a font weight.</summary>
        /// <param name="weight">The weight to be set.</param>
        static void SetFontWeight(FontWeight weight);
        /// <summary>Resets the font weight.</summary>
        static void ResetFontWeight();
        /// <summary>Sets a font effect.</summary>
        /// <param name="effect">The effect to be set.</param>
        static void SetFontEffects(FontEffect effect);
        /// <summary>Sets the font effect flagged in a bitmask.</summary>
        /// <param name="effects">The effects to be set.</param>
        static void SetFontEffects(int effects);
        /// <summary>Resets the font effects.</summary>
        static void ResetFontEffects();
        /// <summary>Sets the visibility of the cursor.</summary>
        /// <param name="isVisible">A boolean that states the cursor should be visible.</param>
        static void SetCursorVisible(bool isVisible);
        /// <summary>Sets the cursor shape.</summary>
        /// <param name="shape">The shape to be set.</param>
        /// <param name="shouldBlink">A boolean that states the cursor should blink.</param>
        static void SetCursorShape(CursorShape shape, bool shouldBlink);
        /// <summary>Resets the cursor shape.</summary>
        static void ResetCursorShape();
        /// <summary>Gets the window dimensions.</summary>
        /// <exception cref="StreamRedirectionException">Thrown when the standard streams are redirected.</exception>
        /// <return>The dimensions.</return>
        static Dimensions GetWindowDimensions();
        /// <summary>Opens the alternate window.</summary>
        static void OpenAlternateWindow();
        /// <summary>Closes the alternate window.</summary>
        static void CloseAlternateWindow();
    };

    /// <summary>Concatenates the two terminal font effects into a bitmask.</summary>
    /// <param name="effectI">The first effect.</param>
    /// <param name="effectII">The second effect.</param>
    /// <return>A bitmask containing the effects.</return>
    int operator|(FontEffect effectI, FontEffect effectII);
}
