#include <functions.h>

void windows1251ToUnicode(QByteArray & inputData, QString & string) {
    QTextCodec * defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    QTextDecoder *decoder = new QTextDecoder(defaultTextCodec);
    string = decoder->toUnicode(inputData);
}

InputData::InputData(int argc, char *argv[]) {
    supportedParams << "-i";
    supportedParams << "-o";

    if(argc != CORRECT_NUMBER_ARGS) {
        throw QString("Incorrect number of arguments to run programm");
    }

    // обработка входных параментров
    for(int i = 1; i < argc; i += 2) {
        if(supportedParams.contains(argv[i])) {
            params.insert(argv[i], argv[i + 1]);
        }
        else {
            throw QString("Unknown flag: ") + argv[i];
        }
    }
}

bool InputData::getHtml(const QString openFrom, const QString outputFilename) {
    QUrl url(openFrom);
    QString html;
    QByteArray htmlBa;

    // если html-разметка разположена на компьютере то
    if(!(openFrom.contains("http://") || openFrom.contains("https://"))) {
        // преобразовать url в локальный
        url = QUrl::fromLocalFile(QDir().absoluteFilePath(openFrom));
    }

    // Загрузка html-разметки
    QNetworkAccessManager manager; // объект для запроса
    QNetworkReply *response = manager.get(QNetworkRequest(url)); // выполняем запрос
    QEventLoop event;
    QObject::connect(response,SIGNAL(finished()),&event,SLOT(quit())); // сигнал загрузки
    event.exec();

    // Если не удалось загрузить html
    if(response->error() != QNetworkReply::NoError) {
        throw QString("function dowloadHTML: Unable to access html file at input URL");
    }

    // Получить html в виде строки
    htmlBa = response->readAll();
    windows1251ToUnicode(htmlBa, html);

    //html = response->readAll();

    // Cохранение html
    QFile outputHtml(outputFilename);

    // Если удалось открыть файл для записи
    if(outputHtml.open(QIODevice::WriteOnly)) {
        QTextStream out(&outputHtml);
        out << html.toUtf8();
        out.flush();
    }
    else {
        outputHtml.close();
        throw QString("function dowloadHTML: Unable to save file");
    }

    outputHtml.close();

    return true;
}

bool InputData::htmlToXml(const QString htmlFilename, const QString xmlFilename) {
    QStringList params; // параметры для запуска xmllint

    params << "-html" << "-xmlout" << htmlFilename << "-output" << xmlFilename;

    if(QProcess::execute("xmllint", params) != QProcess::NormalExit) {
        throw QString("function htmlToXml: Unable to convert html to xml");
    }

    return true;
}

bool InputData::parsingXml(const QString xmlFilename, QDomDocument & tree) {
    QString errorMsg;
    int errorLine, errorColumn;

    // Открыть xml для чтения
    QFile xml(xmlFilename);
    if (!xml.open(QIODevice::ReadOnly))
        throw QString("function parsingXml: Unable to open xml file");
    // Парсинг xml
    if (!tree.setContent(&xml, &errorMsg, &errorLine, &errorColumn)) {
        xml.close();
        throw QString("function parsingXml: Unable to parsing xml file. Error message: ") + errorMsg +
                " from line " + QString::number(errorLine) + ", column " + QString::number(errorColumn);
    }
    xml.close();

    return true;
}

QString InputData::getFlagValue(const QString flag) {
    return params[flag];
}

Ulli::Ulli(QDomDocument & tree) {
    this->tree = tree;

    // получить список поддерживаемых тегов
    getListOfTags(supportedTagsFilename, supportedTags);

    // получить список пользовательских земеняемых тегов
    getListOfTags(userTagsFilename, replaceableUserTags);

    // получить список заменяемых тегов
    excludeUnsupportedTags();
}

void Ulli::repDuplicateTags() {
    // получить узел с тегом body
    QDomNode root = tree.elementsByTagName("body").at(0);

    if(root.isNull()) {
        qprint << "function repDuplicateTags: Warning: tag body not found";
        return;
    }

    // Обойти дерево и заменить повторяемые теги
    preOrder(root);
}

void Ulli::preOrder(QDomNode node) {
    QVector<QDomNode> children; // cписок детей

    // если узел нулевой то прекратить обход этого узла
    if(node.isNull())
        return;

    // получить детей узла
    if(node.hasChildNodes()) {
        getChildren(node, children);
    }

    // найти и заменить последовательности повторяющихся тегов
    if(children.length() > 1) {
        replaceSequence(children);
    }

    // продолжить обход дерева
    for(int i = 0; i < children.length(); i++) {
        preOrder(children[i]);
    }

}

void Ulli::getChildren(QDomNode & node, QVector<QDomNode> & children) {
    QDomNode child = node.firstChild(); // текущий ребенок

    // пока у узла есть дети
    while(!child.isNull()) {
        // если ребенок не текст
        if(!child.isText()) {
            // добавить ребенка в список детей
            children.append(child);
        }
        // перейти к следующему ребенку
        child = child.nextSibling();
    }
}

void Ulli::replaceSequence(QVector<QDomNode> & nodes) {
    if(nodes.isEmpty()) {
        return;
    }

    QVector<QDomNode> sequence;

    sequence.append(nodes[0]);
    QString sequenceTag = nodes[0].toElement().tagName();

    for(int i = 1; i < nodes.length(); i++) {
        // если последовательность продолжается
        if(nodes[i].toElement().tagName() == sequenceTag) {
            sequence.append(nodes[i]);
        } // если последовательность прервалась или не началась
        else {
            // если последовательность прервалась и последовательность подвергается замене
            if(sequence.length() > 1 && checkReplaceableTags(sequenceTag)) {
                insertUL_LI(sequence);
            }
            // начать новую последовательность
            sequence.clear();
            sequence.append(nodes[i]);
            sequenceTag = nodes[i].toElement().tagName();
        }
    }

    // если последний дочерний узел продолжает последовательность и последовательность подвергается замене
    if(sequence.length() > 1 && checkReplaceableTags(sequenceTag)) {
        insertUL_LI(sequence);
    }
}

void Ulli::insertUL_LI(QVector<QDomNode> & sequence) {
    if(sequence.isEmpty()) {
        return;
    }

    QDomNode parent = sequence[0].parentNode();
    QDomNode beforeNode = sequence[0].previousSibling();

    // создать тег ul
    QDomNode ul = tree.createElement("ul");

    for(int i = 0; i < sequence.length(); i++) {
        // удалить связь узла с родителем
        parent.removeChild(sequence[i]);
        // добавить связь узла с ul
        ul.appendChild(sequence[i]);
        // заменить тег узла на li
        sequence[i].toElement().setTagName("li");
    }

    // добавить связь родителя с тегом ul
    if(beforeNode.isNull()) {
        parent.insertBefore(ul, beforeNode);
    }
    else {
        parent.insertAfter(ul, beforeNode);
    }
}

bool Ulli::checkReplaceableTags(const QString sequenceTag) {
    return replaceableTags.contains(sequenceTag);
}

void Ulli::excludeUnsupportedTags() {
    replaceableTags = (QSet<QString>::fromList(supportedTags) & QSet<QString>::fromList(replaceableUserTags)).toList();

    if(replaceableTags.length() < replaceableUserTags.length()) {
        qprint << "Warning: some input tags are not supported (appendix \"A\")";
    }
}


bool Ulli::saveXml(QString xmlFilename) {
    QString xml;
    QFile file(xmlFilename);

    // Открыть xml для записи
    if (!file.open(QIODevice::WriteOnly)) {
        throw QString("function saveXml: Unable to create/write xml file");
    }

    file.write(tree.toByteArray());
    file.close();

    return true;
}

bool Ulli::getListOfTags(const QString filename, QStringList & tags) {
    QFile file(filename);
    QTextStream stream(&file);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw QString("functions getListOfTags: Unable to open file with tags");
    }

    while(!stream.atEnd()) {
        tags << stream.readLine();
    }

    file.close();
    stream.flush();

    return true;
}

bool OutputData::xmlToHtml(const QString xmlFilename, const QString htmlFilename) {
    QProcess process;

    QStringList params; // параметры для запуска xmllint

    params << "-html" << "-noout" << "-htmlout" << xmlFilename << "-output" << htmlFilename;

    // Открывать процесс в дочернем окне
    process.setProcessChannelMode(QProcess::MergedChannels);
    // Запуск процесса
    process.start("xmllint", params);
    // Ждать пока процесс не выполнится
    process.waitForFinished(-1);

    if(process.exitStatus() != QProcess::NormalExit) {
        throw QString("function xmlToHtml: Bad exit process status");
    }

    return true;
}

bool OutputData::removeXmlHeader(const QString htmlFilename) {
    QFile in(htmlFilename);
    QFile out(htmlFilename);

    QString html;

    if (!in.open(QIODevice::ReadWrite | QIODevice::Text)) {
        throw QString("functions removeXmllintHeader: Unable to open html file");
    }

    html = in.readAll();
    html.remove(0, html.indexOf("\n") + 1);
    html.remove(0, html.indexOf("\n") + 1);

    in.close();

    if (!out.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
        throw QString("functions removeXmllintHeader: Unable to open html file");
    }

    out.write(html.toUtf8());

    out.close();

    return true;
}
