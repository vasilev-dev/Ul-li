﻿#include "functions.h"

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
    QProcess process;   // создаем новый процесс
    QStringList params; // параметры для запуска tidy.exe

    params << "-config" << "config.txt" << "-o" << xmlFilename << "-asxml" << htmlFilename;

    // Открывать процесс в дочернем окне
    process.setProcessChannelMode(QProcess::MergedChannels);
    // Запуск процесса
    process.start("tidy.exe", params);
    // Ждать пока процесс не выполнится
    process.waitForFinished(-1);

    if(process.exitStatus() != QProcess::NormalExit) {
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

void repDuplicateTags(QDomDocument & tree, QStringList repTags) {

}

uint treeHtml::getNeighbors() {


}

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


}

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



























