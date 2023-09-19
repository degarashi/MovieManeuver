#pragma once
#include "virtual_key.hpp"
#include <memory>
#include <unordered_set>

namespace dg::input {
	class KeyInput;
	struct Action;
	using Action_S = std::shared_ptr<Action>;
	struct KeyDetect;
	using KeyDetect_S = std::shared_ptr<KeyDetect>;
	using ProcessedKeys = std::unordered_set<VirtualKey>;
}
