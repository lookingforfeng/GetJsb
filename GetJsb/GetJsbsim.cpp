#include "GetJsbsim.h"
#include "net_fdm.hxx"

GetJsbsim::GetJsbsim(QObject *parent)
	: QObject(parent)
{
}

GetJsbsim::~GetJsbsim()
{
}

void GetJsbsim::initUDPSocket()
{
	socketUdp = new QUdpSocket(this);
	socketUdp->bind(QHostAddress::LocalHost, 5550);
	connect(socketUdp, SIGNAL(readyRead()), this, SLOT(readUDPPendingDatagrams()));
}

void GetJsbsim::htond(double &x)
{
	if (isLittleEndian) {
		int    *Double_Overlay = nullptr;
		int     Holding_Buffer;

		Double_Overlay = (int *)&x;
		Holding_Buffer = Double_Overlay[0];

		Double_Overlay[0] = htonl(Double_Overlay[1]);
		Double_Overlay[1] = htonl(Holding_Buffer);
	}
	else {
		return;
	}
}

void GetJsbsim::htonf(float &x)
{
	if (isLittleEndian) {
		int    *Float_Overlay = nullptr;
		int     Holding_Buffer = 0;

		Float_Overlay = (int *)&x;
		Holding_Buffer = *Float_Overlay;

		*Float_Overlay = htonl(Holding_Buffer);
	}
	else {
		return;
	}
}

void GetJsbsim::sendUDP()
{
	QString msg = "UDP test- in Berger King";
	QByteArray bytes = msg.toUtf8();
	QHostAddress *remoteIp = new QHostAddress("127.0.0.1");
	socketUdp->connectToHost(*remoteIp, 1000);
	socketUdp->write(bytes);
}

void GetJsbsim::readUDPPendingDatagrams()
{
	while (socketUdp->hasPendingDatagrams())
	{
		QNetworkDatagram datagram = socketUdp->receiveDatagram();
		FGNetFDM1_3 *net13 = new FGNetFDM1_3();
		FGNetFDM3 *net3 = &(net13->data3);
		FGNetFDM1 *net1 = &(net13->data1);

		memcpy(net1, datagram.data().data(), sizeof(FGNetFDM1) + sizeof(FGNetFDM3));

		if (isLittleEndian) {
			htond(net1->longitude);
			htond(net1->latitude);
			htond(net1->altitude);
			htonf(net1->agl);
			htonf(net1->phi);
			htonf(net1->theta);
			htonf(net1->psi);
			htonf(net1->alpha);
			htonf(net1->beta);

			htonf(net1->phidot);
			htonf(net1->thetadot);
			htonf(net1->psidot);
			htonf(net1->vcas);
			htonf(net1->climb_rate);
			htonf(net1->v_north);
			htonf(net1->v_east);
			htonf(net1->v_down);
			htonf(net1->v_body_u);
			htonf(net1->v_body_v);
			htonf(net1->v_body_w);

			htonf(net1->A_X_pilot);
			htonf(net1->A_Y_pilot);
			htonf(net1->A_Z_pilot);

			htonf(net1->stall_warning);
			htonf(net1->slip_deg);

			net1->num_engines = htonl(net1->num_engines);
			for (int i = 0; i < net1->num_engines; ++i)
			{
				net1->eng_state[i] = htonl(net1->eng_state[i]);
				htonf(net1->rpm[i]);
				htonf(net1->fuel_flow[i]);
				htonf(net1->fuel_px[i]);
				htonf(net1->egt[i]);
				htonf(net1->cht[i]);
				htonf(net1->mp_osi[i]);
				htonf(net1->tit[i]);
				htonf(net1->oil_temp[i]);
				htonf(net1->oil_px[i]);
			}

			net1->num_tanks = htonl(net1->num_tanks);
			for (int i = 0; i < net1->num_tanks; ++i)
			{
				htonf(net1->fuel_quantity[i]);
			}

			net1->version = htonl(net1->version);

			net3->num_wheels = htonl(net3->num_wheels);
			for (int i = 0; i < net3->num_wheels; ++i)
			{
				net3->wow[i] = htonl(net3->wow[i]);
				htonf(net3->gear_pos[i]);
				htonf(net3->gear_steer[i]);
				htonf(net3->gear_compression[i]);
			}

			net3->cur_time = htonl(net3->cur_time);
			net3->warp = htonl(net3->warp);
			htonf(net3->visibility);

			htonf(net3->elevator);
			htonf(net3->elevator_trim_tab);
			htonf(net3->left_flap);
			htonf(net3->right_flap);
			htonf(net3->left_aileron);
			htonf(net3->right_aileron);
			htonf(net3->rudder);
			htonf(net3->nose_wheel);
			htonf(net3->speedbrake);
			htonf(net3->spoilers);
		}
	}
}