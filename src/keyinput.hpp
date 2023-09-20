#pragma once
#include "vk_def.hpp"
#include <chrono>

namespace dg::input {
	using MilliSec = std::chrono::milliseconds;
	class KeyInput {
		private:
			bool		_pressing[Num_VirtualKey];
			struct NPress {
				int			count;
				MilliSec 	accumTime;
				int			result;

				NPress();
				void resetAccum();
				void update(MilliSec dt, bool press, int maxCount);
			} nPress[Num_VirtualKey];

		public:
			void update(const VKStateAr& state, MilliSec dt, const NDetectAr& maxLayer);

			// --- KeyDetectから呼ぶクエリ関数 ---
			bool pressed(VirtualKey key, int n) const;
			bool pressing(VirtualKey key) const;
	};

}
