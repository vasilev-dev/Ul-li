#include <QtTest>
#include <QPair>
#include "functions.h"

// add necessary includes here

class tests : public QObject
{
    Q_OBJECT

public:

private:
    bool compareHtml(const QString leftFilename, const QString rightFilename);
    void launchUL_LI(const QString input, const QString output);

private slots:
    void testMain();
};

void tests::launchUL_LI(const QString input, const QString output) {
    QStringList params;
    params << "-i" << input << "-o" << output;

    QProcess::execute("UL_LI.exe", params);
}

bool tests::compareHtml(const QString leftFilename, const QString rightFilename) {
    QFile left(leftFilename);
    QFile right(rightFilename);
    QString leftHtml, rightHtml;

    // Открыть xml для записи
    if (!(left.open(QIODevice::ReadOnly) || left.open(QIODevice::ReadOnly))) {
        throw QString("function compareHtml: Unable to write file");
    }

    leftHtml = left.readAll();
    rightHtml = right.readAll();

    return (leftHtml == rightHtml);
}


void tests::testMain() {
   QPair<QString, QString> launchParams;
   launchParams << qMakePair("main/attribute.html", "main/attribute.html");







}


QTEST_APPLESS_MAIN(tests)

#include "tst_tests.moc"
