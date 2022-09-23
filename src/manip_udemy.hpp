#pragma once
#include "manip.hpp"

namespace dg {
	struct Manip_Udemy : Manip {
		static void UProc(HWND hw, const Proc_t& p);

		void startPause(HWND hw) const override;
		void forward_5sec(HWND hw) const override;
		void backward_5sec(HWND hw) const override;
		void forward_10sec(HWND hw) const override;
		void backward_10sec(HWND hw) const override;
		void speedDown(HWND hw) const override;
		void speedUp(HWND hw) const override;
		void volumeDown(HWND hw) const override;
		void volumeUp(HWND hw) const override;
		void volumeMute(HWND hw) const override;
		void fullScreen(HWND hw) const override;

		const std::wstring& getWindowTopName() const override;
		const std::wstring& getWindowChildName() const override;

		const QString& getName() const override;
	};
}