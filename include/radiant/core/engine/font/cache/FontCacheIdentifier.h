#pragma once

#include "radiant/util/hash_util.h"
#include <cstddef>
#include <filesystem>
#include <functional>
namespace Radiant {
  struct FontCacheIdentifier {
    std::filesystem::path path;
    uint32_t faceId;

    bool operator==(const FontCacheIdentifier& other) const {
      return this->path == other.path && 
              this->faceId == other.faceId;
    }
  };

  struct FontCacheIdentifierHasher {
    std::size_t operator()(const FontCacheIdentifier& id) const noexcept {
      std::size_t hash = std::hash<std::filesystem::path>{}(id.path);
      hash_util::combineHash<uint32_t>(hash, id.faceId);
      return hash;
    }
  };
}
