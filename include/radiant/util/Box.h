#pragma once

namespace Radiant {
	class Box {
		public:
			Box();
			Box(float minX, float minY, float maxX, float maxY);

			float getMinX();
			float getMinY();
			float getMaxX();
			float getMaxY();

			bool containsX(Box box);
			bool containsY(Box box);
			bool contains(Box box);

		private:
			float minX, minY;
			float maxX, maxY;
	};
} // namespace Radiant
