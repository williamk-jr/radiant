#include "radiant/core/engine/font/FontManager.h"
#include "radiant/core/engine/font/Font.h"
#include "radiant/util/logger/Logger.h"
#include <freetype/freetype.h>
#include <freetype/fttypes.h>
#include <string>

namespace Radiant {
  FontManager::FontManager() {
    FT_Error error = FT_Init_FreeType(&this->freetype);
    if (error) {
      Logger::error("Failed to load Font Manager, error code: " + std::to_string(error), {
        {"FONT", MessageStyle::WHITE},
      });
    } else {
      Logger::info("Loaded Font Manager.", {
        {"FONT", MessageStyle::WHITE},
      }, 1);
    }
  }
  FontManager::FontManager(FontManager&& other) noexcept :
    freetype(other.freetype) {
    other.freetype = nullptr;
  }

  FontManager::~FontManager() {
    FT_Done_FreeType(this->freetype);
  }

  Font FontManager::loadFont(std::filesystem::path path) {
    const char* rawPath = path.c_str();

    FT_Face fontFace;
    FT_Error error = FT_New_Face(freetype, rawPath, 0, &fontFace);

    Logger::info(fontFace->family_name);

    return {this->freetype, path};
  }
}
