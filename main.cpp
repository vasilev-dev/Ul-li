#include <functions.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QDomDocument html;
    QDomNode root;

    try {
        parsingXml("example.html", html);

        treeHtml treehtml(html);

        root = treehtml.tree.firstChild().nextSibling();

        treehtml.preOrder(root);
    }
    catch(QString & err) {
        qprint << err;
    }


    return a.exec();
}
