#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define qprint qDebug().nospace().noquote()

#include <iostream>
#include <QtCore/QCoreApplication>
#include <QDomDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QVector>
#include <QList>
#include <QQueue>
#include <QSet>

/*!
* Получает QString из стандартного потока ввода
*\param [out] out - полученная строка
*/
void getQString(QString & out);

class inputData {
public:
    /*!
    * Получает html-разметку
    *\param [in] openFrom - имя файла на компьютере или URL-адрес для скачивания html
    *\param [in] outputFilename - имя, сохраняемого html-файла
    *\return - возвращает true, если html была успешно скачана и сохранена
    */
    static bool getHtml(const QString openFrom, const QString outputFilename);

    /*!
    * Производит конвертацию html-разметки в xml-разметку средставами xmllint
    * Создает файл, содержащий предупреждения и ошибки в html-разметке, в папке с исполняемым файлом
    *\param [in] htmlFilename - полное имя html-разметки
    *\param [in] xmlFilename - полное имя xml-разметки
    *\return - возвращает true, если конвертация произошла успешно
    */
    static bool htmlToXml(const QString htmlFilename, const QString xmlFilename);

    /*!
    * Парсинг xml-разметки
    *\param [in] xmlFilename - полное имя xml-разметки
    *\param [out] treeHtml - дерево
    *\return - возвращает true, если парсинг произошел успешно
    */
    static bool parsingXml(const QString xmlFilename, QDomDocument & tree);

    /*!
    * Обработать аргументы командной строки
    *\param [in] argc - количество аргументов командной строки
    *\param [in] argv - аргументы командной строки
    *\param [out] url - url-адрес
    *\param [out] downloadFilename - имя скачиваемого файла
    *\param [out] inputFile - имя входного файла
    *\param [out] outputFile - имя выходного файла
    *\return - возвращает true, если все входные данные успешно обработаны
    */
    static bool handlerCmdParams(int argc, char *argv[], QString & url, QString & downloadFilename, QString & inputFile, QString & outputFile);
};

class ulli {
public:
    void printTree(QDomNode node);

    /*!
    * Констуктор класса
    *\param [in] tree - разметка, представленная в виде дерева
    */
    ulli(QDomDocument & tree);

    /*!
    * Заменяет повторяющие теги в html-разметке на конструкцию маркированного списка ul-li
    *\param [in] tree - разметка, представленная в виде дерева
    *\param [in] repTagsUser - заменяемые теги(пользовательские)
    */
    void repDuplicateTags(const QStringList & repTagsUser);

    /*!
    * Сохраняет дерево в формате xml
    *\param [in] xmlFilename - разметка, представленная в виде дерева
    *\return - возвращает true, если xml файл был сохранен
    */
    bool saveXml(QString xmlFilename);

private:
    QStringList supportedTags;   // поддерживаемые замене на конструкцию ul-li теги
    QStringList replaceableTags; // заменяемые теги (объединение поддерживаемых и пользовательских тегов)

    QDomDocument tree; // html в виде дерева

    /*!
    * Вставить конструкцию маркированного списка ul-li (проверка одноуровенности тегов не осуществяется)
    *\param [in] sequence - список повторяющихся тегов
    */
    void insertUL_LI(QVector<QDomNode> & sequence);

    /*!
    * Рекурсивный префиксный обход дерева в глубину
    *\param [in] node - узел дерева
    */
    void preOrder(QDomNode node);

    /*!
    * Получить список детей узла node
    *\param [in] node - узел
    *\param [out] children - список детей узла
    */
    void getChildren(QDomNode & node, QVector<QDomNode> & children);

    /*!
    * Заменить последовательности тегов на конструкцию маркированного списка ul-li
    *\param [in] nodes - дочерние узлы
    */
    void replaceSequence(QVector<QDomNode> & nodes);

    /*!
    * Исключает из пользовательского списка заменяемых тегов неподдерживаемые теги
    *\param [in] repTagsUser - пользовательский список заменяемых тегов
    */
    void excludeUnsupportedTags(const QStringList & repTagsUser);

    /*!
    * Проверить подвергается ли последовательность дочерних тегов замене на констукцию ul-li
    *\param [in] sequenceTag - тег последовательности
    *\return - возвращает true, если тег последовательности может быть изменен
    */
    bool checkReplaceableTags(const QString sequenceTag);
};

class outputData {
public:
    /*!
    * Производит конвертацию xml-разметки в html-разметку средставами xmllint
    *\param [in] xmlFilename - полное имя xml-разметки
    *\param [in] htmlFilename - полное имя html-разметки
    *\return - возвращает true, если конвертация произошла успешно
    */
    static bool xmlToHtml(const QString xmlFilename, const QString htmlFilename);

};

#endif // FUNCTIONS_H
