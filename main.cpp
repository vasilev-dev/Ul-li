#include <functions.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // -url
    QString url = "file:///C:/QtProjects/UL_LI/testing/example.html";
    QString downloadPath = "testing/example.html";

    // -input
    QString inputFile = "testing/example.html";

    // -output
    QString ouputFile = "testing/example2.html";

    // -tags
    QString fileWithTags = "";

    QStringList tags;
    tags << "p";

    QDomDocument tree;

    inputData::downloadHTML(url, downloadPath);
    inputData::htmlToXml(inputFile, ouputFile);
    inputData::parsingXml(ouputFile, tree);

    ulli html(tree);
    html.repDuplicateTags(tags);
    html.saveXml(ouputFile);

    outputData::xmlToHtml(ouputFile, ouputFile);

    return a.exec();
}
