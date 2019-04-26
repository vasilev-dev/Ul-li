#ifndef FUNCTIONS_H
#define FUNCTIONS_H

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

class treeHtml {
public:
    QDomDocument tree; // html в виде дерева

    QVector<QDomNodeList> levels;

    /*!
    * Рекурсивный постфиксный обход дерева в глубину
    *\param [in] node - узел дерева
    */
    void postOrderDFS(QDomNode & node);

    /*!
    * Проверяет можно ли произвести замену повтоящихся тегов на конструкцию маркированного списка ul-li
    *\param [in] node - узел дерева
    */
    bool canReplaceDuplicateTags(QDomNode & node) const;

    /*!
    * Считает количество повтоящихся тегов
    *\param [in] node - узел дерева
    */
    uint countDuplicateTags(QDomNode & node) const;
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
