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

treeHtml::treeHtml(QDomDocument & tree) {
    this->tree = tree;

    // добавление поддерживаемых замене на конструкцию ul-li тегов
    replaceableTags << "blockquote";
    replaceableTags << "div";
    replaceableTags << "h1";
    replaceableTags << "h2";
    replaceableTags << "h3";
    replaceableTags << "h4";
    replaceableTags << "h5";
    replaceableTags << "h6";
    replaceableTags << "h7";
    replaceableTags << "p";
    replaceableTags << "a";
    replaceableTags << "abbr";
    replaceableTags << "acronym";
    replaceableTags << "h12";
    replaceableTags << "b";
    replaceableTags << "cite";
    replaceableTags << "code";
    replaceableTags << "dfn";
    replaceableTags << "em";
    replaceableTags << "i";
    replaceableTags << "q";
    replaceableTags << "s";
    replaceableTags << "samp";
    replaceableTags << "span";
    replaceableTags << "strike";
    replaceableTags << "strong";
    replaceableTags << "sub";
    replaceableTags << "sup";
    replaceableTags << "textarea";
    replaceableTags << "tt";
    replaceableTags << "u";
}

void treeHtml::repDuplicateTags(const QStringList & repTags) {
    QDomNode root = tree.firstChild().nextSibling();

    preOrder(root);
    qprint << "-------------------------";
    printTree(root);
}

void treeHtml::preOrder(QDomNode node) {
    QVector<QDomNode> children;

    if(node.isNull())
        return;

    qprint << node.toElement().tagName();

    if(node.hasChildNodes()) {
        getChildren(node, children);
    }
    if(children.length() > 1) {
        replaceSequence(children);
    }

    for(int i = 0; i < children.length(); i++) {
        preOrder(children[i]);
    }

}

void treeHtml::getChildren(QDomNode & node, QVector<QDomNode> & children) {
    QDomNode child = node.firstChild();

    while(!child.isNull() && !child.isText()) {
        children.append(child);
        child = child.nextSibling();
    }
}

void treeHtml::replaceSequence(QVector<QDomNode> & nodes) {
    QVector<QDomNode> sequence;

    sequence.append(nodes[0]);
    QString sequenceTag = nodes[0].toElement().tagName();

    for(int i = 1; i < nodes.length(); i++) {
        // если последовательность продолжается
        if(nodes[i].toElement().tagName() == sequenceTag) {
            sequence.append(nodes[i]);
        } // если последовательность прервалась или не началась
        else {
            // если последовательность прервалась
            if(sequence.length() > 1) {
                insertUL_LI(sequence);
            }
            // начать новую последовательность
            sequence.clear();
            sequence.append(nodes[i]);
            sequenceTag = nodes[i].toElement().tagName();
        }
    }

    // если последний дочерний узел продолжает последовательность
    if(sequence.length() > 1) {
        insertUL_LI(sequence);
    }
}

void treeHtml::insertUL_LI(QVector<QDomNode> & sequence) {
    if(sequence.isEmpty()) {
        return;
    }

    QDomNode parent = sequence[0].parentNode();

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

    QDomNode beforeNode = sequence[0].previousSibling();
    // добавить связь родителя с тегом ul
    parent.insertBefore(ul, beforeNode);
}

void treeHtml::printTree(QDomNode node) {
    QVector<QDomNode> children;

    if(node.isNull())
        return;

    qprint << node.toElement().tagName();

    if(node.hasChildNodes()) {
        getChildren(node, children);
    }

    for(int i = 0; i < children.length(); i++) {
        preOrder(children[i]);
    }
}
























