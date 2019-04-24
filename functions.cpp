#include "functions.h"

void getQString(QString & out) {
    QTextStream s(stdin);
    out = s.readLine();
}

bool downloadHTML(const QString url, const QString fullFilename) {
    // Загрузка html
    QNetworkAccessManager manager; // объект для запроса
    QNetworkReply *response = manager.get(QNetworkRequest(QUrl(url))); // выполняем запрос
    QEventLoop event;
    QObject::connect(response,SIGNAL(finished()),&event,SLOT(quit())); // сигнал загрузки
    event.exec();

    // Если не удалось загрузить html
    if(response->error() != QNetworkReply::NoError) {
        return false;
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
        return false;
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

    return process.exitStatus() == QProcess::NormalExit;
}

bool parsingXml(const QString xmlFilename, QDomDocument & tree) {
    // Открыть xml для чтения
    QFile xml(xmlFilename);
    if (!xml.open(QIODevice::ReadOnly))
        return false;
    // Парсинг xml
    if (!tree.setContent(&xml)) {
        xml.close();
        return false;
    }
    xml.close();

    return true;
}

void repDuplicateTags(QDomDocument & tree) {

}

void postOrderDFS(QDomNode & node) {
    // Если текущий узел пустой то прекратить обход текущего узла
    if(node.isNull()) {
        return;
    }

    // Обойти первое (левое) поддерево
    QDomNode firstChild = node.firstChild();
    postOrderDFS(firstChild);

    qDebug() << node.toElement().tagName();

    // Обойти следущее поддерево(вправо)
    QDomNode nextChild = node.nextSibling();
    postOrderDFS(nextChild);

}













