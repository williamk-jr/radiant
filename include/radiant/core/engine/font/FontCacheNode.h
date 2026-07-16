#pragma once

#include <freetype/ftcache.h>
namespace Radiant {

  template<typename T>
  class FontCacheNode {
    public:
      FontCacheNode(FTC_Manager manager, T value, FTC_Node node) :
        manager(manager), value(value), node(node) {}

      ~FontCacheNode() {
        FTC_Node_Unref(this->node, this->manager);
      }

      T getValue() {
        return this->value;
      }
    private:
      FTC_Manager manager;
      FTC_Node node;
      T value;
  };
}
