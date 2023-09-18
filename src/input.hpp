#pragma once
#include "virtual_key.hpp"
#include <memory>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <functional>
#include "keydiff.hpp"

namespace dg {
	struct KeyDiff;
	struct Manip;
	class InputMapSet;

	using PreProc = std::function<void ()>;
	//! 動作を定義。Manipのメソッドを呼び出してアクションを起こす
	struct Action {
		virtual void proc(InputMapSet& ims, const Manip* manip, HWND hw, const PreProc& pre) const = 0;
	};
	using Action_S = std::shared_ptr<Action>;
	struct Act_Inst : Action {
		void (Manip::*_ptr)(HWND) const;

		Act_Inst(void (Manip::*ptr)(HWND) const);
		void proc(InputMapSet& ims, const Manip* manip, HWND hw, const PreProc& pre) const override;
	};
	class Act_Func : public Action {
		private:
			using Proc = std::function<void (InputMapSet&)>;
			Proc _proc;
		public:
			Act_Func(const Proc& proc);
			void proc(InputMapSet& ims, const Manip* manip, HWND hw, const PreProc& pre) const override;
	};

	//! キー入力判定
	struct KeyInput {
		virtual bool check(const KeyDiff& k) = 0;
		virtual void reset() {}
	};
	using KeyInput_U = std::unique_ptr<KeyInput>;
	struct KI_Press : KeyInput {
		VirtualKey key;

		KI_Press(VirtualKey key);
		bool check(const KeyDiff& k) override;
	};
	struct KI_Release : KeyInput {
		VirtualKey key;

		KI_Release(VirtualKey key);
		bool check(const KeyDiff& k) override;
	};
	struct KI_Double : KeyInput {
		VirtualKey	key[2];
		bool		pressed[2];

		KI_Double(VirtualKey first, VirtualKey second);
		bool check(const KeyDiff& k) override;
	};

	struct InputMapAbst {
		virtual bool proc(InputMapSet& ims, const KeyDiff& k, const Manip* m, HWND hw, const PreProc& pre) = 0;
		virtual void reset() = 0;
	};
	using InputMap_U = std::unique_ptr<InputMapAbst>;

	using ManipF = void (Manip::*)(HWND) const;
	//! キー割り当てレイヤ
	class InputMapLayer : public InputMapAbst {
		private:
			// キー入力条件 -> キーアクション
			// キー入力は固有の変数を持つのでUnique
			// アクションは固有の変数を持たないのでShared
			using ActionMap = std::vector<std::pair<KeyInput_U, Action_S>>;
			ActionMap		_actionMap;

		public:
			template <class KI, class ACT>
			void addMap(KI&& ki, ACT&& act) {
				_actionMap.emplace_back(std::forward<KI>(ki), std::forward<ACT>(act));
			}
			void addOnPress(VirtualKey key, ManipF func);
			bool proc(InputMapSet& ims, const KeyDiff& k, const Manip* m, HWND hw, const PreProc& pre) override;
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
			bool proc(InputMapSet& ims, const KeyDiff& k, const Manip* m, HWND hw, const PreProc& pre) override;
			void reset() override;
	};
}
