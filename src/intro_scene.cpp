#include "intro_scene.hpp"

IntroScene::~IntroScene() {
	for (RectList::iterator it = main_menu_.begin();
		it != main_menu_.end(); it++)
	{
		delete (*it);
	}
}

HRESULT IntroScene::Init() {
	// main menu
	HRESULT hr = S_OK;

	const int menuCnt = 4;
	for (int i = 0; i < menuCnt; i++) {
		main_menu_.push_back(new Rect());
		main_menu_[i]->setConstantBuffer(cbuffer_);
		main_menu_[i]->setTranslation(0.0f, -4.0f + (-3.0f * i));
		main_menu_[i]->setSize(5.0f, 1.0f);
		hr = main_menu_[i]->Init();
		if (FAILED(hr)) {
			wstring message = L"Fail to initialize Menu";
			MessageBox(nullptr, message.c_str(), L"Error", MB_OK);
			return hr;
		}
	}
}

HRESULT IntroScene::is_prologue() {
	static float t = 0.0f;
	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static ULONGLONG timeStart = 0;
		ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		t = (timeCur - timeStart) / 1000.0f;
	}

	if (t > time_delay_) {
		scene_number_ = 1;
		t = 0.0f;
	}

	return S_OK;
}

HRESULT IntroScene::is_main_menu() {
	for (RectList::iterator it = main_menu_.begin();
		it != main_menu_.end(); it++) {
		(*it)->Render();
	}

	return S_OK;
}

HRESULT IntroScene::is_sub_menu() {

	return S_OK;
}

HRESULT IntroScene::is_user_information() {

	return S_OK;
}

HRESULT IntroScene::is_options() {

	return S_OK;
}

HRESULT IntroScene::gps_select_map() {

	return S_OK;
}

HRESULT IntroScene::gps_select_character() {

	return S_OK;
}

void IntroScene::Render() {
	switch (scene_number_) {
	case 0:
		is_prologue();
		break;
	case 1:
		is_main_menu();
		break;
	case 2:
		is_user_information();
		break;
	case 3:
		is_options();
		break;
	case 4:
		exit(true);
		break;
	case 11:
		is_sub_menu();
		break;
	default:
		scene_number_ = 1;
	}
}

void IntroScene::HandleInput(UINT message, WPARAM wParam, LPARAM lParam) {
	GetCursorPos(&pt_);
	ScreenToClient(g_hWnd, &pt_);
	
	if (scene_number_ == 1) {
		switch (message) {
		case WM_LBUTTONDOWN:

			// maybe 1.0f = 36px
			x_ = pt_.x - (WINDOW_WIDTH / 2);
			y_ = pt_.y - (WINDOW_HEIGHT / 2);

			if (x_ > -90.0f && x_ < 90.0f) {
				click_ = true;

				if (y_ > convertCoord(4.0f, 1.0f, false) && y_ < convertCoord(4.0f, 1.0f, true))
					scene_number_ = 111;

				else if (y_ > convertCoord(7.0f, 1.0f, false) && y_ < convertCoord(7.0f, 1.0f, true))
					MessageBox(nullptr, L"occured 2", L"Debug", MB_OK);

				else if (y_ > convertCoord(10.0f, 1.0f, false) && y_ < convertCoord(10.0f, 1.0f, true))
					MessageBox(nullptr, L"occured 3", L"Debug", MB_OK);

				else if (y_ > convertCoord(13.0f, 1.0f, false) && y_ < convertCoord(13.0f, 1.0f, true))
					MessageBox(nullptr, L"occured 4", L"Debug", MB_OK);
			}

			break;

		default:

			break;
		}
	}
}