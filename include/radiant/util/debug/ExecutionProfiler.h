#pragma once
#include <chrono>
#include <string>

namespace Radiant::Debug {
	class ExecutionProfiler {
		public:
			ExecutionProfiler(std::string name, bool manual);
			void begin();
			void end();
			~ExecutionProfiler();

		private:
			bool                                                        started = false;
			std::chrono::time_point<std::chrono::high_resolution_clock> start;
			std::string                                                 name;
	};
} // namespace Radiant::Debug
