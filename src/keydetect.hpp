#pragma once
#include "input_def.hpp"
#include "vk_def.hpp"

namespace dg::input {
	//! キー入力判定
	struct KeyDetect {
		virtual void nDetect(NDetectAr& dst) const {}
		virtual bool check(const KeyInput& input, ProcessedKeys& proced) const = 0;
	};
	struct KD_Press : KeyDetect {
		VirtualKey key;
		KD_Press(VirtualKey key);

		bool check(const KeyInput& input, ProcessedKeys& proced) const override;
	};
	// 複数回押し判定
	struct KD_Double : KeyDetect {
		VirtualKey	key;
		int			count;
		KD_Double(VirtualKey key, int count);
		void nDetect(NDetectAr& dst) const override;
		bool check(const KeyInput& input, ProcessedKeys& proced) const override;
	};
	// key[0]を押しながらkey[1]を押す判定
	struct KD_Step : KeyDetect {
		VirtualKey key[2];
		KD_Step(VirtualKey key0, VirtualKey key1);
		bool check(const KeyInput& input, ProcessedKeys& proced) const override;
	};
}
