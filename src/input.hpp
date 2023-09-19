#pragma once
#include "manip_if.hpp"
#include "input_def.hpp"
#include "vk_def.hpp"
#include <functional>

namespace dg::input {
	struct InputMapAbst {
		virtual void nDetect(NDetectAr& dst) const = 0;
		virtual bool proc(const KeyInput& ki, ProcessedKeys& proced, ActionParam& ap) = 0;
	};
	using InputMap_U = std::unique_ptr<InputMapAbst>;
	using Priority = uint32_t;
	//! キー割り当てレイヤ
	class InputMapLayer : public InputMapAbst {
		private:
			struct MapEnt {
				Priority		priority;
				KeyDetect_S		detect;
				Action_S		action;

				template <class KD, class ACT>
				MapEnt(const Priority prio, KD&& kd, ACT&& act):
					  priority(prio),
					  detect(std::forward<KD>(kd)),
					  action(std::forward<ACT>(act))
				{}
				bool operator < (const MapEnt& m) const {
					return priority < m.priority;
				}
			};
			using MapEntM = std::vector<MapEnt>;
			MapEntM		_mapEnt;

		public:
			void addMap(Priority prio, const KeyDetect_S& ki, const Action_S& act) {
				_mapEnt.emplace_back(prio, ki, act);
			}
			void addOnPress(Priority prio, VirtualKey key, ManipF func);
			bool proc(const KeyInput& ki, ProcessedKeys& proced, ActionParam& ap) override;
			void nDetect(NDetectAr& dst) const override;
	};
	class InputMapSet : public InputMapAbst {
		private:
			std::vector<InputMap_U>	_inputLayer;
		public:
			template <class Layer>
			void addLayer(Layer&& l) {
				_inputLayer.emplace_back(std::forward<Layer>(l));
			}
			void removeLastLayer();
			bool proc(const KeyInput& ki, ProcessedKeys& proced, ActionParam& ap) override;
			void nDetect(NDetectAr& dst) const override;
	};
}
