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

typedef QVector<QDomNode> neighbors; // узлы-соседи

class treeHtml {
public:
    QDomDocument tree; // html в виде дерева

    QVector<neighbors> levels; // уровни дерева

    /*!
    * Заменяет повторяющие теги в html-разметке на конструкцию маркированного списка ul-li
    *\param [in] tree - разметка, представленная в виде дерева
    *\param [in] repTags - заменяемые теги
    */
    void repDuplicateTags(const QStringList & repTags);

    /*!
    * Получить соседей на всех этажах дерева (обход дерева в ширину)
    */
    void getNeighbors();

    /*!
    * Получить очередь из детей узла
    *\param [in] node - узел дерева
    *\param [out] childs - дети
    */
    void getChilds(QDomNode & node, QQueue<QDomNode> & childs);

    /*!
    * Вставить конструкцию маркированного списка ul-li (проверка одноуровенности тегов не осуществяется)
    *\param [in] duplicateList - список повторяющихся тегов
    */
    void insertUL_LI(QVector<QDomNode> duplicateList);


};

/*!
* Получает QString из стандартного потока ввода
*\param [out] out - полученная строка
*/
void getQString(QString & out);

/*!
* Скачивает html-разметку по URL
*\param [in] URL - URL-адрес для скачивания html
*\param [in] fullFilename - полное имя, скачиваемого html-файла
*\return - возвращает true, если html была успешно скачана и сохранена
*/
bool downloadHTML(const QString url, const QString fullFilename);

/*!
* Производит конвертацию html-разметки в xml-разметку средставами HTML Tidy
* Создает файл, содержащий предупреждения и ошибки в html-разметке, в папке с исполняемым файлом
*\param [in] htmlFilename - полное имя html-разметки
*\param [in] xmlFilename - полное имя xml-разметки
*\return - возвращает true, если конвертация произошла успешно
*/
bool htmlToXml(const QString htmlFilename, const QString xmlFilename);

/*!
* Парсинг xml-разметки
*\param [in] xmlFilename - полное имя xml-разметки
*\param [out] treeHtml - дерево
*\return - возвращает true, если парсинг произошел успешно
*/
bool parsingXml(const QString xmlFilename, QDomDocument & tree);

/*!
* Заменяет повторяющие теги в html-разметке на конструкцию маркированного списка ul-li
*\param [in] tree - разметка, представленная в виде дерева
*\param [in] repTags - заменяемые теги
*/
void repDuplicateTags(QDomDocument & tree, const QStringList & repTags);

/*!
* Производит конвертацию xml-разметки в html-разметку средставами HTML Tidy
*\param [in] xmlFilename - полное имя xml-разметки
*\param [in] htmlFilename - полное имя html-разметки
*\return - возвращает true, если конвертация произошла успешно
*/
bool xmlToHtml(const QString xmlFilename, const QString htmlFilename);

#endif // FUNCTIONS_H
