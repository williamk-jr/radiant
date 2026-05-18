#pragma once

namespace Radiant {
  namespace MessageStyle {
    typedef const char* MessageColor;

    const MessageColor BLACK = "\x1b[30m";
    const MessageColor RED = "\x1b[31m";
    const MessageColor GREEN = "\x1b[32m";
    const MessageColor YELLOW = "\x1b[33m";
    const MessageColor BLUE = "\x1b[34m";
    const MessageColor MAGENTA = "\x1b[35m";
    const MessageColor CYAN = "\x1b[36m";
    const MessageColor WHITE = "\x1b[37m";
    const MessageColor DEFAULT = "\x1b[39m";
  }
}
