#pragma once

namespace Radiant {
  class Float {
    public:
      Float();
      Float(float value);

      operator float() const {
        return this->value;
      }
    private:
      float value;
  };
}
