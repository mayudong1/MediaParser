#include "FLVDispaly.h"

FLVDispaly::FLVDispaly()
{

}

void FLVDispaly::Display(QTreeWidget* tree, QTextEdit* edit, FLVParser* parser)
{
    tree->clear();
    QTreeWidgetItem *root = new QTreeWidgetItem(QStringList("root"));
    tree->addTopLevelItem(root);
    root->setExpanded(true);

    DisplayFlvHeader(root, parser);
    DisplayFlvTags(root, parser);
}

void FLVDispaly::DisplayFlvHeader(QTreeWidgetItem* root, FLVParser* parser)
{
    QTreeWidgetItem *header = new QTreeWidgetItem(QStringList("FLV Header"));
    root->addChild(header);

    FLVStruct* flv = parser->flv;
    QVariant value = QVariant::fromValue((void *)&flv->header.FLVHeader_pos);
    header->setData(0, Qt::UserRole, value);

    QString strTmp;
    strTmp = QString::asprintf("signature: %s", flv->header.signature);
    QTreeWidgetItem *signature = new QTreeWidgetItem(QStringList(strTmp));
    header->addChild(signature);

    strTmp = QString::asprintf("version: %d", flv->header.version);
    QTreeWidgetItem *version = new QTreeWidgetItem(QStringList(strTmp));
    header->addChild(version);

    strTmp = QString::asprintf("has video: %d", flv->header.hasVideo);
    QTreeWidgetItem *hasVideo = new QTreeWidgetItem(QStringList(strTmp));
    header->addChild(hasVideo);

    strTmp = QString::asprintf("has audio: %d", flv->header.hasAudio);
    QTreeWidgetItem *hasAudio = new QTreeWidgetItem(QStringList(strTmp));
    header->addChild(hasAudio);

    strTmp = QString::asprintf("header size: %d", flv->header.headerLen);
    QTreeWidgetItem *headerSize = new QTreeWidgetItem(QStringList(strTmp));
    header->addChild(headerSize);

    strTmp = QString::asprintf("First Tag size: %d", flv->firstTagSize);
    QTreeWidgetItem *firstTagSize = new QTreeWidgetItem(QStringList(strTmp));
    root->addChild(firstTagSize);
}

void FLVDispaly::DisplayFlvTags(QTreeWidgetItem* root, FLVParser* parser)
{
    QString strTmp;

    FLVTag* tag = parser->flv->tagList->next;
    int videoIndex = 0;
    int audioIndex = 0;
    while (tag != NULL)
    {
        QString strTagType("Unknown Tag");
        switch (tag->header.type)
        {
        case 8:
            strTagType = QString::asprintf("Audio Tag%d", audioIndex++);
            break;
        case 9:
            strTagType = QString::asprintf("Video Tag%d", videoIndex++);
            break;
        case 18:
            strTagType = "Metadata Tag";
            break;
        default:
            break;
        }
        QTreeWidgetItem *tagItem = new QTreeWidgetItem(QStringList(strTagType));
        QVariant value = QVariant::fromValue((void *)&tag->FLVTag_pos);
        tagItem->setData(0, Qt::UserRole, value);
        root->addChild(tagItem);

//        displayFLVTagDetail(tagItem, tag);

        strTmp = QString::asprintf("preTagSize: %d", tag->preTagSize);
        QTreeWidgetItem *preTagSizeItem = new QTreeWidgetItem(QStringList(strTmp));
        QVariant preTagSizeItemValue = QVariant::fromValue((void *)&tag->preTagSize_pos);
        preTagSizeItem->setData(0, Qt::UserRole, preTagSizeItemValue);
        root->addChild(preTagSizeItem);
        tag = tag->next;
    }
}
