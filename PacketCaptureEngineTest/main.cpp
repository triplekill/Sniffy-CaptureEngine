// PacketCaptureEngineTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>

#include "capture.h"
#include "comm.h"
#include "optparse.h"

#define PAUSE system("PAUSE")

unsigned int packetCount = 0;

int main(int argc, char** argv)
{
	Parameters p;
	if (!ParseParams(argc, argv, &p)) 
	{
		LOG("Invalid arguments, usage: PacketCaptureEngine.exe -a <ip_address> -p <port>.");
		return 1;
	}

	LOG("Address: " << p.IpAddress);
	LOG("Port: " << p.Port);

	std::vector<CaptureDevice> devices = ListAllDevices();
	for (auto &device : devices)
	{
		device.PrintDevice();
	}

	bool connectionSuccessful = CreateSocket(5005, (char*) "127.0.0.1");

	if (!connectionSuccessful) 
	{
		std::cerr << "Could not connect to display component!" << std::endl;
		PAUSE;
		return 1;
	}

	RegisterCaptureCallback(1, PacketReady);
	CloseConnection();

	PAUSE;
	return 0;
}

void PacketReady(u_char* s, const struct pcap_pkthdr* header, const u_char* data)
{
	LOG("Captured a packet of length: " << header->caplen << std::endl);
	LOG("Packet index: " << packetCount++);
	SendData((u_char*) data, header->caplen);
}