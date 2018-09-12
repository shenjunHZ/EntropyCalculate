#pragma once
#include <memory>
#include "IExcelFileOperation.h"

namespace operation
{
    class ExcelFileOperation : public IExcelFileOperation
    {
    public:
        ExcelFileOperation();
        bool openExcelFile(const QString& strFilePath);
        bool closeExcelFile();
        bool readSheetAllData(QList<QList<QVariant>>& listData);
        bool writeSheetData(const QList<QList<QVariant>>& listData);

    private:
        void initObject();
        void saveFile();
        bool castVariantToList(const QVariant& var, QList<QList<QVariant>>& listRes);
        void castListListVariant2Variant(const QList<QList<QVariant>> &cells, QVariant& var);
        void convertToColName(int data, QString &res);
        QString to26AlphabetString(int data);

    private:
        std::unique_ptr<QAxObject> m_axObject;
        QAxObject* m_workBooks;
        QAxObject* m_workBook;
        QAxObject* m_workSheets;
        QAxObject* m_workSheet;
        QString m_filePath;
        bool m_bNewFile;
    };
}