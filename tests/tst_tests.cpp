#include <QtTest>
#include "functions.h"

#define NOT_ERROR -1

// add necessary includes here

class tests : public QObject
{
    Q_OBJECT

public:

private:
    /*!
    * Получить название тестов, лежащих в папке
    *\param [in] path - путь к тестам
    *\param [out] listOfTests - наименования тестов
    */
    void getListTests(const QString path, QStringList & listOfTests);

    /****** FOR TESTING MAIN FUNCTIONS ******/

    /*!
    * Сравнить html
    *\param [in] leftHtml - входной html
    *\param [int] rightHtml - выходной html
    *\return - возвращает true, если html-деревья равны
    */
    bool compareHtml(const QString leftHtml, const QString rightHtml);

    /*!
    * Запуск тестов
    *\param [in] input - входной html
    *\param [out] output - выходной html
    */
    void startUL_LI(const QString input, const QString output);

    /*!
    * Сравнить узлы html-дерева
    *\param [in] leftNode - первый узел
    *\param [in] rightNode - второй узел
    *\return - возвращает true, если узлы равны
    */
    bool compareNode(const QDomNode & leftNode, const QDomNode & rightNode);

    /*!
    * Префиксный рекурсивный обход дерева для сравнения
    *\param [in] leftNode - первый узел
    *\param [in] rightNode - второй узел
    *\param [in|out] isEquals - равенство деревьев
    *\return - возвращает true, если текущие узлы равны
    */
    bool preOrderCompare(const QDomNode leftNode, const QDomNode rightNode, bool & isEquals);

    /*!
    * Получить список детей узла node
    *\param [in] node - узел
    *\param [out] children - список детей узла
    */
    void getChildren(const QDomNode & node, QVector<QDomNode> & children);

    /****** FOR TESTING CONVERTER FUNCTIONS ******/


private slots:
    void testMain();
    void testConverter();
    void testReplaceSequence();
};

void tests::getListTests(const QString path, QStringList & listOfTests) {
    QDir dir(path);
    listOfTests = dir.entryList(QStringList() << "*.html");
}

void tests::startUL_LI(const QString input, const QString output) {
    QProcess process;
    process.start("UL_LI.exe", QStringList() << "-i" << input << "-o" << output);
    process.waitForFinished();

    if(process.exitStatus() != QProcess::NormalExit) {
        throw QString("function startUL_LI: Unable to start programm");
    }
}

bool tests::compareHtml(const QString leftHtml, const QString rightHtml) {
    QDomDocument lTree, rTree;
    QDomNode lRoot, rRoot;
    bool isEqual = true;

    try {
        InputData::parsingXml(leftHtml, lTree);
    } catch (const QString err) {
        qprint << "Test " + leftHtml + " caused exception";
        qprint << err;
    }
    try {
        InputData::parsingXml(rightHtml, rTree);
    } catch (const QString err) {
        qprint << "Test " + rightHtml + " caused exception";
        qprint << err;
    }

    lRoot = lTree.elementsByTagName("html").at(0);
    rRoot = rTree.elementsByTagName("html").at(0);

    preOrderCompare(lRoot, rRoot, isEqual);

    return isEqual;
}

bool tests::compareNode(const QDomNode & leftNode, const QDomNode & rightNode) {
    const QDomElement lElement = leftNode.toElement();
    const QDomElement rElement = rightNode.toElement();

    return ((lElement.tagName() == rElement.tagName())
             && (lElement.text() == rElement.text()));
}

bool tests::preOrderCompare(const QDomNode leftNode, const QDomNode rightNode, bool & isEquals) {
    QVector<QDomNode> lChildren; // cписок детей первого узла
    QVector<QDomNode> rChildren; // cписок детей второго узла

    // если деревья не равны то прекратить обход этого узла
    if(!isEquals)
        return (isEquals = false);

    // если узел нулевой то прекратить обход этого узла
    if(leftNode.isNull() && rightNode.isNull())
        return true;

    // получить детей левого узла
    if(leftNode.hasChildNodes()) {
        getChildren(leftNode, lChildren);
    }

    // получить детей правого узла
    if(rightNode.hasChildNodes()) {
        getChildren(rightNode, rChildren);
    }

    // сравнить детей
    if(lChildren.length() != rChildren.length()) {
        return (isEquals = false);
    }
    for(int i = 0; i < lChildren.length(); i++) {
        if(!compareNode(lChildren[i], rChildren[i])) {
            qprint << "Tags " + lChildren[i].toElement().tagName() + " with text \"" + lChildren[i].toElement().text() + "\" and tag " +
                      rChildren[i].toElement().tagName() + " with text \"" + rChildren[i].toElement().text() + "\" are not equal";
            return (isEquals = false);
        }
    }

    // продолжить обход дерева
    for(int i = 0; i < lChildren.length(); i++) {
        preOrderCompare(lChildren[i], rChildren[i], isEquals);
    }

    return true;
}

void tests::getChildren(const QDomNode &node, QVector<QDomNode> &children) {

    QDomNode child = node.firstChild(); // текущий ребенок

    // пока у узла есть дети
    while(!child.isNull()) {
        // добавить ребенка в список детей
        children.append(child);
        // перейти к следующему ребенку
        child = child.nextSibling();
    }
}

void tests::testMain() {
    QStringList namesOfTests;
    getListTests("main/tests", namesOfTests);

    foreach(QString test, namesOfTests) {
        bool passed;

        try {
            startUL_LI("main/tests/" + test, "main/result/" + test);
        } catch (const QString err) {
            qprint << "Test " + test + " caused exception";
            qprint << err;
        }

        passed = compareHtml("main/reference/" + test, "main/result/" + test);

        if(passed) {
            qprint << "Test " + test  + " passed";
        }

        QVERIFY2(passed, QString("Test " + test + " is failed!").toLocal8Bit().data());
    }
}

void tests::testConverter() {
    QStringList namesOfTests;
    getListTests("htmlToXml/tests", namesOfTests);

    foreach(QString test, namesOfTests) {
        bool passed;

        try {
           InputData::htmlToXml("htmlToXml/tests/" + test, "htmlToXml/result/" + test);
           OutputData::xmlToHtml("htmlToXml/result/" + test, "htmlToXml/result/" + test);
           OutputData::removeXmlHeader("htmlToXml/result/" + test);
        } catch (const QString err) {
            qprint << "Test " + test + " caused exception";
            qprint << err;
        }

        passed = compareHtml("htmlToXml/reference/" + test, "htmlToXml/result/" + test);

        if(passed) {
            qprint << "Test " + test  + " passed";
        }

        QVERIFY2(passed, QString("Test " + test + " is failed!").toLocal8Bit().data());
    }
}

void tests::testReplaceSequence() {
    QStringList namesOfTests;
    getListTests("replaceSequence/tests", namesOfTests);

    foreach(QString nameOfTest, namesOfTests) {
        bool passed;

        try {
            QDomDocument test;

            // Конвертация теста в xml
            InputData::htmlToXml("replaceSequence/tests/" + nameOfTest, "replaceSequence/result/" + nameOfTest);

            // Парсинг теста
            InputData::parsingXml("replaceSequence/result/" + nameOfTest, test);
            Ulli res(test);

            // Получить список узлов
            QDomNode root = res.tree.elementsByTagName("body").at(0);
            QVector<QDomNode> nodes;
            res.getChildren(root, nodes);

            // Заменить повторяющиехся теги в списке узлов
            res.replaceSequence(nodes);

            // Cохранить результат
            res.saveXml("replaceSequence/result/" + nameOfTest);

            // Конвертация эталонного теста в xml
            InputData::htmlToXml("replaceSequence/reference/" + nameOfTest, "replaceSequence/referenceXml/" + nameOfTest);

        } catch (const QString err) {
            qprint << "Test " + nameOfTest + " caused exception";
            qprint << err;
        }

        passed = compareHtml("replaceSequence/referenceXml/" + nameOfTest, "replaceSequence/result/" + nameOfTest);

        if(passed) {
            qprint << "Test " + nameOfTest  + " passed";
        }

        QVERIFY2(passed, QString("Test " + nameOfTest + " is failed!").toLocal8Bit().data());
    }
}



QTEST_APPLESS_MAIN(tests)

#include "tst_tests.moc"
