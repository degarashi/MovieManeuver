#pragma once
#include <cstddef>
#include <tuple>
#include "vector.hpp"
#include "axisstate.hpp"

namespace dg {
	enum class Dir2D {
		Horizontal,
		Vertical
	};
	template <class T, size_t N, class... Cur>
	struct MakeTup {
			using result = typename MakeTup<T,N-1, T,Cur...>::result;
	};
	template <class T, class... Cur>
	struct MakeTup<T, 0, Cur...> {
		using result = std::tuple<Cur...>;
	};

	template <size_t N>
	class AxisState_N {
		private:
			constexpr static auto Dim = N;
			using Vec = VecBase<float, N>;
			constexpr static auto ToInt = [](const auto val){ return static_cast<int>(val); };

			std::array<AxisState, Dim>	_axisState;
			//! 軸入力を最大長さ1のベクトルにした値
			Vec			_inputVec;

		private:
			struct Ctor {};
			template <class Obj, class Arg, size_t... Idx>
			static auto MakeWithArgs(const Arg& arg, std::index_sequence<Idx...>) {
				return Obj(Ctor{}, (Idx, arg)...);
			}
			// (AxisState...)を指定するctor
			template <class... Ts>
			AxisState_N(Ctor, Ts&&... ts):
				  _axisState{std::forward<Ts>(ts)...}
			{
				init();
			}
			template <size_t... Idx, class Tup>
			void _update(std::index_sequence<Idx...>, Tup&& tup) {
				auto dummy = [](auto...){};
				dummy((_update(Idx, std::get<Idx>(tup)), 0)...);
			}
			void _update(const int index, const int value) {
				auto& a = _axisState.at(index);
				a.update(value);
				_inputVec.ar.at(index) = a.axis();
			}

		public:
			AxisState_N(const int range, const int deadZone):
				// move-ctor
				AxisState_N(
					MakeWithArgs<AxisState_N>(AxisState(range, deadZone), std::make_index_sequence<N>{})
				)
			{}
			//! 明示的な初期化(コンストラクタでも呼ばれる)
			void init() {
				for(auto& a : _axisState)
					a.init();
				_inputVec = {};
			}
			//! デバッグ用(内部の値が正常かをチェック)
			[[nodiscard]] bool check() const {
				for(auto& a : _axisState)
					if(!a.check())
						return false;
				return true;
			}

			// --- 毎フレーム呼ぶ関数 ---
			//! 前回と入力の変化が無いときに呼ぶ
			void update() {
				for(auto& a : _axisState)
					a.update();
			}
			//! 内部ステートの更新
			template <class... Values>
			void update(const Values... values) {
				static_assert(sizeof...(Values) == Dim, "Invalid input values Dimension");
				_update(std::make_index_sequence<Dim>{},
						std::make_tuple(values...));
			}
			// ----------------------------

			[[nodiscard]] const AxisState& axis(const int n) const {
				return _axisState.at(n);
			}
			[[nodiscard]] AxisState& refAxis(const int n) {
				return _axisState.at(n);
			}
			[[nodiscard]] const Vec& vec() const {
				return _inputVec;
			}

			void setDeadZone(const int dz) {
				for(auto& a : _axisState)
					a.setDeadZone(dz);
			}
	};
}
