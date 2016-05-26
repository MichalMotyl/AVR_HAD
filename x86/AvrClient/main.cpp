#include <QCoreApplication>

#include "avrclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    AvrClient client;
    QMetaObject::invokeMethod( &client, "Run", Qt::QueuedConnection );
    return a.exec();
}
