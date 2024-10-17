#include "Tmk.hpp"
#include <iostream>

namespace Tmk
{
    Terminal::StreamRedirectionCache::StreamRedirectionCache(bool isInputRedirected, bool isOutputRedirected, bool isErrorRedirected)
        : m_isInputRedirected(isInputRedirected), m_isOutputRedirected(isOutputRedirected), m_isErrorRedirected(isErrorRedirected)
    {
    }

    bool Terminal::StreamRedirectionCache::IsRedirected(int fileNo) const
    {
        return !fileNo ? m_isInputRedirected : fileNo == 1 ? m_isOutputRedirected : m_isErrorRedirected;
    }
}
