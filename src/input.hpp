#pragma once
#include <memory>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <functional>
#include "vk_def.hpp"
#include <chrono>

namespace dg {
	struct Manip;
	using ManipF = void (Manip::*)(HWND) const;
	class InputMapSet;
	using PreProc = std::function<void ()>;

	//! 動作を定義
	struct Action {
		virtual void proc(const Manip* manip, HWND hw, const PreProc& pre) const = 0;
	};
	using Action_S = std::shared_ptr<Action>;
	//! Manipのメソッドを呼び出す
	struct Act_Manip : Action {
		ManipF	_ptr;

		Act_Manip(void (Manip::*ptr)(HWND) const);
		void proc(const Manip* manip, HWND hw, const PreProc& pre) const override;
	};
	//! 事前にセットした任意の関数を呼ぶ
	class Act_Func : public Action {
		private:
			using Proc = std::function<void ()>;
			Proc _proc;
		public:
			Act_Func(const Proc& proc);
			void proc(const Manip* manip, HWND hw, const PreProc& pre) const override;
	};

	using Priority = uint32_t;
	//! キー入力判定
	struct KeyInput {
		Priority	priority;

		KeyInput(Priority prio);
		virtual bool check(const VKStateAr& state) = 0;
		virtual bool needReset() const;
		virtual void reset();
	};
	using KeyInput_U = std::unique_ptr<KeyInput>;
	struct KI_Press : KeyInput {
		VirtualKey key;

		KI_Press(Priority prio, VirtualKey key);
		bool check(const VKStateAr& state) override;
	};
	struct KI_Release : KeyInput {
		VirtualKey key;

		KI_Release(Priority prio, VirtualKey key);
		bool check(const VKStateAr& state) override;
	};
	// key[0]を押しながらkey[1]を押す判定
	struct KI_Step : KeyInput {
		VirtualKey	key[2];

		KI_Step(Priority prio, VirtualKey first, VirtualKey second);
		bool check(const VKStateAr& state) override;
	};

	struct InputMapAbst {
		enum class Result {
			NotProceeded,
			Proceeded,
			ProceededWithReset,
		};
		virtual Result proc(const VKStateAr& state, const Manip* m, HWND hw, const PreProc& pre) = 0;
		virtual void reset() = 0;
	};
	using InputMap_U = std::unique_ptr<InputMapAbst>;

	//! キー割り当てレイヤ
	class InputMapLayer : public InputMapAbst {
		private:
			// キー入力条件 -> キーアクション
			// キー入力は固有の変数を持つのでUnique
			// アクションは固有の変数を持たないのでShared
			using ActionEntry = std::pair<KeyInput_U, Action_S>;
			using ActionMap = std::vector<ActionEntry>;
			ActionMap		_actionMap;
			void _resort();

		public:
			template <class KI, class ACT>
			void addMap(KI&& ki, ACT&& act) {
				_actionMap.emplace_back(std::forward<KI>(ki), std::forward<ACT>(act));
				_resort();
			}
			void addOnPress(Priority prio, VirtualKey key, ManipF func);
			Result proc(const VKStateAr& state, const Manip* m, HWND hw, const PreProc& pre) override;
			void reset() override;
	};
	class InputMapSet : public InputMapAbst {
		private:
			std::vector<InputMap_U>	_inputLayer;

		public:
			template <class Layer>
			void addLayer(Layer&& l) {
				_inputLayer.emplace_back(std::forward<Layer>(l));
			}
			void removeSet();
			Result proc(const VKStateAr& state, const Manip* m, HWND hw, const PreProc& pre) override;
			void reset() override;
	};
}
