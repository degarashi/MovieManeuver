#pragma once
#include <windows.h>
#include <functional>
#include <string>

class QString;
namespace dg {
	struct Manip {
		using Proc_t = std::function<void ()>;
		HWND findTarget() const;

		virtual void startPause(HWND hw) const = 0;
		virtual void forward_5sec(HWND hw) const = 0;
		virtual void backward_5sec(HWND hw) const = 0;
		virtual void forward_10sec(HWND hw) const = 0;
		virtual void backward_10sec(HWND hw) const = 0;
		virtual void speedDown(HWND hw) const = 0;
		virtual void speedUp(HWND hw) const = 0;
		virtual void volumeUp(HWND hw) const = 0;
		virtual void volumeDown(HWND hw) const = 0;
		virtual void volumeMute(HWND hw) const = 0;
		virtual void fullScreen(HWND hw) const {}

		virtual const std::wstring& getWindowTopName() const = 0;
		virtual const std::wstring& getWindowChildName() const = 0;

		virtual const QString& getName() const = 0;
	};
}
