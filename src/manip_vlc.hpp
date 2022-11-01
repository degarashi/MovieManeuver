#pragma once
#include "manip.hpp"

namespace dg {
	class Manip_VLC: public Manip {
		private:
			static void _Focus(HWND hw, const Proc_t& p);
		public:
			void startPause(HWND hw) const override;
			void forward_few(HWND hw) const override;
			void backward_few(HWND hw) const override;
			void forward_medium(HWND hw) const override;
			void backward_medium(HWND hw) const override;
			void speedDown(HWND hw) const override;
			void speedUp(HWND hw) const override;
			void volumeDown(HWND hw) const override;
			void volumeUp(HWND hw) const override;
			void volumeMute(HWND hw) const override;
			void captionSwitch(HWND hw) const override;

			const std::wstring& getWindowTopName() const override;
			const std::wstring& getWindowChildName() const override;

			const QString& getName() const override;
	};
}
