#include <functions.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QDomDocument html;
    QDomNode root;

    try {
        parsingXml("example.html", html);

        treeHtml treehtml(html);

        treehtml.repDuplicateTags(QStringList());
    }
    catch(QString & err) {
        qprint << err;
    }


    return a.exec();
}
