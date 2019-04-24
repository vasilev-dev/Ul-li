#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QtCore/QCoreApplication>
#include <QDomDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QTextStream>


class treeHtml {
public:
    QDomDocument tree; // html в виде дерева

    /*!
    * –екурсивный постфиксный обход дерева в глубину
    *\param [in] node - узел дерева
    */
    void postOrderDFS(QDomNode & node);
};

/*!
* ѕолучает QString из стандартного потока ввода
*\param [out] out - полученна€ строка
*/
void getQString(QString & out);

/*!
* —качивает html-разметку по URL
*\param [in] URL - URL-адрес дл€ скачивани€ html
*\param [in] fullFilename - полное им€, скачиваемого html-файла
*\return - возвращает true, если html была успешно скачана и сохранена
*/
bool downloadHTML(const QString url, const QString fullFilename);

/*!
* ѕроизводит конвертацию html-разметки в xml-разметку средставами HTML Tidy
* —оздает файл, содержащий предупреждени€ и ошибки в html-разметке, в папке с исполн€емым файлом
*\param [in] htmlFilename - полное им€ html-разметки
*\param [in] xmlFilename - полное им€ xml-разметки
*\return - возвращает true, если конвертаци€ произошла успешно
*/
bool htmlToXml(const QString htmlFilename, const QString xmlFilename);

/*!
* ѕарсинг xml-разметки
*\param [in] xmlFilename - полное им€ xml-разметки
*\param [out] treeHtml - дерево
*\return - возвращает true, если парсинг произошел успешно
*/
bool parsingXml(const QString xmlFilename, QDomDocument & tree);

/*!
* «амен€ет повтор€ющие теги в html-разметке на конструкцию маркированного списка ul-li
*\param [in] tree - разметка, представленна€ в виде дерева
*/
void repDuplicateTags(treeHtml & tree);

/*!
* ѕроизводит конвертацию xml-разметки в html-разметку средставами HTML Tidy
*\param [in] xmlFilename - полное им€ xml-разметки
*\param [in] htmlFilename - полное им€ html-разметки
*\return - возвращает true, если конвертаци€ произошла успешно
*/
bool xmlToHtml(const QString xmlFilename, const QString htmlFilename);


#endif // FUNCTIONS_H
