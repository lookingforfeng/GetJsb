#include <QtCore/QCoreApplication>
#include "GetJsbsim.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	GetJsbsim* test = new GetJsbsim();
	test->initUDPSocket();

	return a.exec();
}
