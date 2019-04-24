#include <functions.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /* Проверка версии SSL
    qDebug()<<"SSL version use for build: "<<QSslSocket::sslLibraryBuildVersionString();
    qDebug()<<"SSL version use for run-time: "<<QSslSocket::sslLibraryVersionNumber();
    qDebug()<<QCoreApplication::libraryPaths();*/

    QDomDocument tree;

    //bool tmp = downloadHTML("file:///C:/Users/%D0%94%D0%BC%D0%B8%D1%82%D1%80%D0%B8%D0%B9/Desktop/html_example.html", "test.html");
    //bool tmp_1 = htmlToXml("test.html", "test.xml");
    bool tmp_2 = parsingXml("test.xml", tree);

    repDuplicateTags(tree);



    return a.exec();
}
