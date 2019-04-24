#include <functions.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /* Проверка версии SSL
    qDebug()<<"SSL version use for build: "<<QSslSocket::sslLibraryBuildVersionString();
    qDebug()<<"SSL version use for run-time: "<<QSslSocket::sslLibraryVersionNumber();
    qDebug()<<QCoreApplication::libraryPaths();*/

    return a.exec();
}
