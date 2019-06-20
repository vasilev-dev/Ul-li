﻿#include <functions.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try {
        InputData inputData(argc, argv);
        QDomDocument html;
        QString inputFile = inputData.getFlagValue("-i");
        QString outputFile = inputData.getFlagValue("-o");

        InputData::getHtml(inputFile, outputFile);
        InputData::htmlToXml(outputFile, outputFile);
        InputData::parsingXml(outputFile, html);

        Ulli ulli(html);
        ulli.repDuplicateTags();
        ulli.saveXml(outputFile);

        OutputData::xmlToHtml(outputFile, outputFile);
        OutputData::removeXmlHeader(outputFile);

    } catch (QString err) {
        qprint << err;
        exit(-1);
    }

    return 0;
}
