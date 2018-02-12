#ifndef FLVDISPALY_H
#define FLVDISPALY_H

#include <QTreeWidget>
#include <QTextEdit>
#include <QtGlobal>
#include "flv/FLVParser.h"

class FLVDispaly
{
public:
    FLVDispaly();

public:
    void Display(QTreeWidget* tree, QTextEdit* edit, FLVParser* parser);

private:
    void DisplayFlvHeader(QTreeWidgetItem* root, FLVParser* parser);
    void DisplayFlvTags(QTreeWidgetItem* root, FLVParser* parser);
};

#endif // FLVDISPALY_H
