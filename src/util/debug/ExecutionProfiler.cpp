#include "radiant/util/debug/ExecutionProfiler.h"

#include <chrono>
#include <iostream>

namespace Radiant::Debug {
	ExecutionProfiler::ExecutionProfiler(std::string name, bool manualStart) : name(name) {
		if (!manualStart) {
			this->begin();
		}
	}

	void ExecutionProfiler::begin() {
		if (!this->started) {
			this->started = true;
			this->start   = std::chrono::high_resolution_clock::now();
		}
	}

	void ExecutionProfiler::end() {
		if (this->started) {
			this->started = false;
			auto end      = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - this->start);
			std::cout << "Completion time for " + this->name + ": " << duration.count() << "\n";
		}
	}

	ExecutionProfiler::~ExecutionProfiler() {
		this->end();
	}
} // namespace Radiant::Debug
