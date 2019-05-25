#include <functions.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    treeHtml html;

    try {
        parsingXml("example.html", html.tree);
    }
    catch(QString & err) {
        qprint << err;
    }

    QDomNode root = html.tree.firstChild().nextSibling();
    html.preOrder(root);

    return a.exec();
}
