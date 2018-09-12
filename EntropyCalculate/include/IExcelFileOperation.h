#pragma once
#include <ActiveQt/QAxObject>

namespace operation
{
    class IExcelFileOperation : public QAxObject
    {
        Q_OBJECT

    public:
        virtual ~IExcelFileOperation() {};
        virtual bool openExcelFile(const QString& strFilePath) = 0;
        virtual bool closeExcelFile() = 0;
        virtual bool readSheetAllData(QList<QList<QVariant>>& listData) = 0;
        virtual bool writeSheetData(const QList<QList<QVariant>>& listData) = 0;
    };
}