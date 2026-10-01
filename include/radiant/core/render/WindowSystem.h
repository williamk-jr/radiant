#pragma once

#include "radiant/core/render/Window.h"

#include <memory>

namespace Radiant {
	class WindowSystem {
		public:
			WindowSystem();

			WindowSystem(const WindowSystem&)            = delete;
			WindowSystem& operator=(const WindowSystem&) = delete;

			WindowSystem(WindowSystem&&) noexcept;
			WindowSystem& operator=(WindowSystem&&) noexcept = delete;

			std::unique_ptr<Window> createWindow(const std::string& title, int width, int height);

			~WindowSystem();

		private:
	};
} // namespace Radiant
