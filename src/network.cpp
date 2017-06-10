#include "network.hpp"

Network::Network() {}

void Network::Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		cerr << "Socket Initialization: Error with WSAStartup\n";
		system("pause");
		WSACleanup();
		exit(10);
	}

	mySocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (mySocket == INVALID_SOCKET)
	{
		cerr << "Socket Initialization: Error creating socket" << endl;
		system("pause");
		WSACleanup();
		exit(11);
	}

	myBackup = mySocket;

	//bind
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	myAddress.sin_port = htons(port);

	if (bind(mySocket, (SOCKADDR*)&myAddress, sizeof(myAddress)) == SOCKET_ERROR)
	{
		cerr << "ServerSocket: Failed to connect\n";
		system("pause");
		WSACleanup();
		exit(14);
	}
}

void Network::Communication(UINT message, WPARAM wParam, LPARAM lParam)
{
	server_length = sizeof(struct sockaddr_in);

	Buffer buff = { message, wParam, lParam };
	memcpy(sendMessage, &buff, sizeof(Buffer));
	sendto(mySocket, sendMessage, sizeof(Buffer), 0, (SOCKADDR*)&myAddress, server_length);

	recvfrom(mySocket, recMessage, sizeof(Buffer), 0, (SOCKADDR*)&myAddress, &server_length);
	Buffer get;
	memcpy(&get, recMessage, sizeof(Buffer));
	HandleMessage(get.message, get.wParam, get.lParam);
}

void Network::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_KEYDOWN:
	case WM_KEYUP:
	{
		bool state;
		if (message == WM_KEYDOWN)		state = true;
		else if (message == WM_KEYUP)	state = false;

		if (wParam <= 0x5A && wParam >= 0x41) {
			char key = wParam - 0x41 + 'A';
			switch (key) {
			case 'W': vk_.w = state;	break;
			case 'A': vk_.a = state;	break;
			case 'S': vk_.s = state;	break;
			case 'D': vk_.d = state;	break;
			}
		}
		else if (wParam == VK_SPACE) {
			vk_.space = state;
			break;
		}
		break;
	}
	case WM_LBUTTONDOWN:
		vm_.lb = true;
		break;
	case WM_LBUTTONUP:
		vm_.lb = false;
		this->enemy_->Vault(charge_);
		break;
	case WM_MOUSEMOVE:
		if (this->vm_.init)
		{
			this->vm_.x = LOWORD(lParam);
			this->vm_.y = HIWORD(lParam);
			this->vm_.init = false;
		}
		else
	{
			float curr_x = LOWORD(lParam);
			float curr_y = HIWORD(lParam);

			// Update unit rotation
			float amount = (this->rotation_sensitivity_) / 10.0f;
			float delta_y = (curr_y - this->vm_.y) * amount * 0.3;
			float delta_x = (curr_x - this->vm_.x) * amount * 0.7;
			XMFLOAT2 rotation = this->enemy_->get_transform().rotation_;
			rotation.x += delta_y;
			if (rotation.x > 90.0f) rotation.x = 90.0f;
			if (rotation.x < -90.0f) rotation.x = -90.0f;
			rotation.y -= delta_x;
			if (rotation.y > 360.0f) rotation.y -= 360.0f;
			if (rotation.y < 360.0f) rotation.y += 360.0f;
			this->enemy_->set_transform_rotation(rotation);

			// Update previous values
			this->vm_.x = LOWORD(lParam);
			this->vm_.y = HIWORD(lParam);
		}
		break;
	}
}

void Network::Update()
{
	if (this->enemy_ == NULL)
	{
		MessageBox(nullptr,
			L"No assigned unit to the player.", L"Error", MB_OK);
	}
	if (vk_.w)
	{
		if (vk_.a)
			this->enemy_->Move(-45.0f);
		else if (vk_.d)
			this->enemy_->Move(45.0f);
		else
			this->enemy_->Move(0.0f);
	}
	else if (vk_.s)
	{
		if (vk_.a)
			this->enemy_->Move(-135.0f);
		else if (vk_.d)
			this->enemy_->Move(135.0f);
		else
			this->enemy_->Move(180.0f);
	}
	else if (vk_.a) this->enemy_->Move(Unit::Direction::kLeft);
	else if (vk_.d) this->enemy_->Move(Unit::Direction::kRight);
	if (vk_.space)
	{
		this->enemy_->Jump();
		vk_.space = false;
	}
	if (vm_.lb) set_charge(charge_ + charge_speed_);
}