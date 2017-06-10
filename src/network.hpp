#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include "unit.hpp"

using namespace std;


class Network
{
public:
	struct Buffer
	{
		UINT message;
		WPARAM wParam;
		LPARAM lParam;
	};
	struct VirtualKeyboard {
		bool					w;
		bool					a;
		bool					s;
		bool					d;
		bool					space;
	};
	struct VirtualMouse {
		bool					lb;
		float					x;
		float					y;
		bool					init;
	};
	Network();
	void Init();
	void Communication(UINT message, WPARAM wParam, LPARAM lParam);
	void HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void Update();

	void SetUnits(Unit* unit, Unit* enemy)
	{
		this->unit_ = unit;
		this->enemy_ = enemy;
	}
	void set_charge(float charge) {
		if (charge > this->max_charge_) charge = this->max_charge_;
		this->charge_ = charge;
	}

private:
	Unit*						unit_;
	Unit*						enemy_;
	VirtualKeyboard				vk_;
	VirtualMouse				vm_;
	float						rotation_sensitivity_ = 15.0f;
	float						charge_;
	float						charge_speed_;
	float						max_charge_;

	int server_length;
	int port = 123;
	char recMessage[sizeof(Buffer)];
	char sendMessage[sizeof(Buffer)];
	char *sendMes = "SERVER READY";
	WSADATA wsaData;
	SOCKET mySocket;
	SOCKET myBackup;
	SOCKET acceptSocket;
	sockaddr_in myAddress;
};