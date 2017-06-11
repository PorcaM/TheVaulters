#include "user_interface.hpp"

UserInterface::~UserInterface() {
	for (RectList::iterator it = stuffs_.begin();
		it != stuffs_.end(); it++)
	{
		delete (*it);
	}
}

HRESULT UserInterface::Init() {
	HRESULT hr = S_OK;
	
	const int stuffCnt	= btnCnt_ + stamina_;		// Forward : Aim & Button // Backward : Stamina Bar

	position_list_	= new XMFLOAT2[stuffCnt];
	size_list_		= new XMFLOAT2[stuffCnt];

	// Set size & position
	// Aim
	position_list_[0].x = 0.0f;
	position_list_[0].y = 0.0f;
	size_list_[0].x = 0.1f;
	size_list_[0].y = 0.8f;

	position_list_[1].x = 0.0f;
	position_list_[1].y = 0.0f;
	size_list_[1].x = 0.8f;
	size_list_[1].y = 0.1f;

	// Skill
	position_list_[2].x = -32.0f;
	position_list_[2].y = -15.0f;
	size_list_[2].x = 3.0f;
	size_list_[2].y = 3.0f;

	// Stamina Bar
	for (int i = btnCnt_; i < stuffCnt; i++) {
		position_list_[i].x = -28.0f + (0.08f * i - btnCnt_);
		position_list_[i].y = -17.0f;
		size_list_[i].x = 0.02f;
		size_list_[i].y = 0.4f;
	}

	// push
	for (int i = 0; i < stuffCnt; i++) {
		stuffs_.push_back(new Rect());
		stuffs_[i]->setConstantBuffer(cbuffer_);
		stuffs_[i]->setTranslation(position_list_[i].x, position_list_[i].y);
		stuffs_[i]->setSize(size_list_[i].x, size_list_[i].y);
		hr = stuffs_[i]->Init();
		if (FAILED(hr)) {
			wstring message = L"Fail to initialize UI";
			MessageBox(nullptr, message.c_str(), L"Error", MB_OK);
			return hr;
		}
	}
}

void UserInterface::Render(int status) {
	ShowCursor(false);

	for (RectList::iterator it =stuffs_.begin();
		it != stuffs_.end() - (stamina_ - status); it++) {
		(*it)->setPosition(unit_pos_);
		(*it)->setRotationX(5.0f);
		(*it)->rotationApply(true);
		(*it)->Render();
	}
}