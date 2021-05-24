#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include "winsock.h"

#pragma comment(lib,"ws2_32.lib")

static const int endianTest = 1;
#define isLittleEndian (*((char *) &endianTest ) != 0)

class GetJsbsim : public QObject
{
	Q_OBJECT

public:
	GetJsbsim(QObject *parent = nullptr);
	~GetJsbsim();
	void initUDPSocket();
	void sendUDP();

	private slots:
	void readUDPPendingDatagrams();

private:
	QUdpSocket* socketUdp;

	//static uint32_t reversebytes_uint32t(uint32_t value) {
	//	return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |
	//		(value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;
	//}

	void htond(double &x);

	void htonf(float &x);

};
