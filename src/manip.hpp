#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <functional>
#include <string>

class QString;
namespace dg {
	struct ManipParam;
	// ブラウザを操作するためのキーストロークやマウス操作を定義
	struct Manip {
		HWND findTarget() const;
		virtual void setFocus(HWND hw) const = 0;

		virtual void startPause(const ManipParam& param) const = 0;
		virtual void forward_few(const ManipParam& param) const = 0;
		virtual void backward_few(const ManipParam& param) const = 0;
		virtual void forward_medium(const ManipParam& param) const = 0;
		virtual void backward_medium(const ManipParam& param) const = 0;
		virtual void speedDown(const ManipParam& param) const = 0;
		virtual void speedUp(const ManipParam& param) const = 0;
		virtual void volumeUp(const ManipParam& param) const = 0;
		virtual void volumeDown(const ManipParam& param) const = 0;
		virtual void volumeMute(const ManipParam& param) const = 0;
		virtual void fullScreen(const ManipParam& param) const {}
		virtual void captionSwitch(const ManipParam& param) const {}
		virtual void seekByNumber(const ManipParam& param) const {}

		// 操作対象ウィンドウを検索する際の名前を取得
		virtual const std::wstring& getWindowTopName() const = 0;
		virtual const std::wstring& getWindowChildName() const = 0;

		virtual const QString& getName() const = 0;

		// 共通の操作(一応override可)
		virtual void mediaVolumeUp(const ManipParam& param) const;
		virtual void mediaVolumeDown(const ManipParam& param) const;
	};
}
