#pragma once
#include "manip.hpp"

namespace dg {
	class Manip_Box : public Manip {
		public:
            // 動画ウィンドウにフォーカスする処理
			void setFocus(HWND hw) const override;

			void startPause(const ManipParam& param) const override;
			void forward_few(const ManipParam& param) const override;
			void backward_few(const ManipParam& param) const override;
			void forward_medium(const ManipParam& param) const override;
			void backward_medium(const ManipParam& param) const override;
			void speedDown(const ManipParam& param) const override;
			void speedUp(const ManipParam& param) const override;
			void volumeDown(const ManipParam& param) const override;
			void volumeUp(const ManipParam& param) const override;
			void volumeMute(const ManipParam& param) const override;
			void captionSwitch(const ManipParam& param) const override;

			const std::wstring& getWindowTopName() const override;
			const std::wstring& getWindowChildName() const override;

			const QString& getName() const override;
	};
}
