#include "functions.h"

void getQString(QString & out) {
    QTextStream s(stdin);
    out = s.readLine();
}

bool downloadHTML(const QString url, const QString fullFilename) {
    // Загрузка html-разметки
    QNetworkAccessManager manager; // объект для запроса
    QNetworkReply *response = manager.get(QNetworkRequest(QUrl(url))); // выполняем запрос
    QEventLoop event;
    QObject::connect(response,SIGNAL(finished()),&event,SLOT(quit())); // сигнал загрузки
    event.exec();

    // Если не удалось загрузить html
    if(response->error() != QNetworkReply::NoError) {
        throw QString("function dowloadHTML: Unable to access html file at input URL.");
    }

    // Поместить скачанные данные в QString
    QString html = response->readAll();

    // Cохранение html
    QFile outputHtml(fullFilename);

    // Если удалось открыть файл для записи
    if(outputHtml.open(QIODevice::WriteOnly)) {
        QTextStream out(&outputHtml);
        out << html;
    }
    else {
        outputHtml.close();
        throw QString("function dowloadHTML: Unable to save file.");
    }

    outputHtml.close();
    return true;
}

bool htmlToXml(const QString htmlFilename, const QString xmlFilename) {
    QStringList params; // параметры для запуска xmllint

    params << "-html" << "-xmlout" << htmlFilename << "-output" << xmlFilename;

    if(QProcess::execute("xmllint", params) != QProcess::NormalExit) {
        throw QString("function htmlToXml: Process error. Perhaps in the root folder is missing \"tide.exe\".");
    }

    return true;
}

bool parsingXml(const QString xmlFilename, QDomDocument & tree) {
    QString errorMsg;
    int errorLine, errorColumn;

    // Открыть xml для чтения
    QFile xml(xmlFilename);
    if (!xml.open(QIODevice::ReadOnly))
        throw QString("function parsingXml: Unable to open xml file.");
    // Парсинг xml
    if (!tree.setContent(&xml, &errorMsg, &errorLine, &errorColumn)) {
        xml.close();
        throw QString("function parsingXml: Unable to parsing xml file. Error message: ") + errorMsg +
                " from line " + QString::number(errorLine) + ", column " + QString::number(errorColumn) + ".";
    }
    xml.close();

    return true;
}

void treeHtml::repDuplicateTags(const QStringList & repTags) {
    // заполнить этажи соседями
    getNeighbors();

    // для всех этажей, начиная от самого нижнего
    for(int i = levels.length() - 1; i >= 0 ; i--) {

        QVector<QDomNode> duplicateList; // список повторяющихся тегов
        QString tag = ""; // тег текущей последовательности

        // для всех элементов на этаже
        for(int j = 0; j < levels[i].length(); j++) {
            // если последовательность тегов продолжается то
            if(levels[i][j].toElement().tagName() == tag) {
                // добавить элемент в список повторяющихся тегов
                duplicateList.append(levels[i][j]);
            }
            else if(duplicateList.length() > 0) { // иначе, если последовательсть прервалась
                // заменить последовательность на конструкцию ul-li
                //insertUL_LI(duplicateList);
                // обнулить список повторяющихся тегов
                duplicateList.clear();
                // начать новую последовательность
                tag = levels[i][j].toElement().tagName();
                duplicateList.append(levels[i][j]);
            }
            else { // иначе, если последовательность не образовалась
                // обнулить список повторяющихся тегов
                duplicateList.clear();
                // начать новую последовательность
                tag = levels[i][j].toElement().tagName();
            }
        }
    }
}

void treeHtml::getNeighbors() {

    QDomNode currentNode = tree.firstChild();
    QQueue<QDomNode> currentLevel;
    QQueue<QDomNode> childs;
    uint countLevels = 0;

    // Добавить первый этаж в очередь
    while(!currentNode.isNull()) {
        currentLevel.enqueue(currentNode);
        currentNode = currentNode.nextSibling();
    }

    // Пока все этажы дерева не пройдены
    while(!currentLevel.isEmpty()) {

        QVector<QDomNode> nodes;

        // Добавить всех детей текущего уровня в очередь
        while(!currentLevel.isEmpty()) {
            currentNode = currentLevel.dequeue();
            if(currentNode.isElement()) {
                nodes.append(currentNode);
            }
            getChilds(currentNode, childs);
        }

        // Перейти на следующий этаж
        currentLevel = childs;
        childs.clear();
        countLevels++;

        if(!nodes.isEmpty()) {
            levels.append(nodes);
        }
    }


}

/*
void treeHtml::bfs(QDomNode & node) {
    QDomNode currentNode = node;
    QQueue<QDomNode> currentLevel;
    QQueue<QDomNode> childs;
    uint countLevels = 0;

    // Добавить первый этаж в очередь
    while(!currentNode.isNull()) {
        currentLevel.enqueue(currentNode);
        currentNode = currentNode.nextSibling();
    }

    // Пока все этажы дерева не пройдены
    while(!currentLevel.isEmpty()) {

        qprint << "Level: " + QString::number(countLevels);

        // Добавить всех детей текущего уровня в очередь
        while(!currentLevel.isEmpty()) {
            currentNode = currentLevel.dequeue();
            if(currentNode.isElement()) {
                qprint << "   " +currentNode.toElement().tagName();
            }
            getChilds(currentNode, childs);
        }

        // Перейти на следующий этаж
        currentLevel = childs;
        childs.clear();
        countLevels++;
    }
}*/

void treeHtml::getChilds(QDomNode &node, QQueue<QDomNode> & childs) {
    // Получить первого ребенка
    QDomNode currentChild = node.firstChild();

    // Если у входного узла есть дети
    if(!currentChild.isNull()) {
        // Добавить первого ребенка в очередь
        childs.enqueue(currentChild);

        // Пока у текущего ребенка есть братья
        while(!currentChild.nextSibling().isNull()) {
            // Перейти к следущему ребенку
            currentChild = currentChild.nextSibling();
            // Добавить ребенка в очередь
            childs.enqueue(currentChild);
        }
    }
}



























