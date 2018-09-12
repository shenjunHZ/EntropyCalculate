#include "ExcelFileOperation.h"
#include "MessageNotify/MessageNotify.h"
#include <QtCore/QFileInfo>

namespace
{
    const int g_iWorkSheetIndex = 1;
}

namespace operation
{
    
    ExcelFileOperation::ExcelFileOperation()
        : m_axObject{nullptr}
        , m_workBooks{nullptr}
        , m_workBook{nullptr}
        , m_workSheets{nullptr}
        , m_workSheet{nullptr}
        , m_filePath{""}
        , m_bNewFile{false}
    {
        //initObject();
    }

    void ExcelFileOperation::initObject()
    {
        m_axObject = std::make_unique<QAxObject>("Excel.Application", this);
        if (!m_axObject)
        {
            DSGUI::DSMessageNotify::Instance().AddTextNotification(QObject::tr("Application excel filed!"));
            return;
        }
        m_axObject->dynamicCall("SetVisible (bool Visible)", "false"); // ����ʾ����
        m_axObject->setProperty("DisplayAlerts", false);               // ����ʾ�κξ���
        m_workBooks = m_axObject->querySubObject("WorkBooks");        // ��ȡ����������
        if (!m_workBooks)
        {
            DSGUI::DSMessageNotify::Instance().AddTextNotification(QObject::tr("Excel get books filed!"));
        }
    }

    void ExcelFileOperation::saveFile()
    {
        if (nullptr != m_workBook && nullptr != m_workSheet
            && !m_filePath.isEmpty())
        {
            m_filePath.replace("/", "\\"); // must do it
            if (!m_bNewFile)
            {
                m_workBook->dynamicCall("Save()");
            }
            else
            {
                //m_pWorkBook->dynamicCall("SaveAs (const QString&)", m_filePathName);
                QVariant var = m_workBook->dynamicCall("SaveAs(const QString&, int, const QString&, const QString&, bool, bool)",
                    m_filePath, 56, QString(""), QString(""), false, false);
                //int iVar = var.toInt();
                //qDebug() <<"============================" <<iVar;
            }
        }
    }

    bool ExcelFileOperation::openExcelFile(const QString& strFilePath)
    {
        if (strFilePath.isEmpty())
        {
            DSGUI::DSMessageNotify::Instance().AddTextNotification(QObject::tr("Excel find filed!"));
            return false;
        }
        m_filePath = strFilePath;
        initObject(); // ���ڹ�����ʱ�����ʧ�ܵ��������Ҫ��ʱ�ٳ�ʼ

        QFile excelFile(strFilePath);
        if (!excelFile.exists())
        {
            m_workBooks->dynamicCall("Add");
            m_bNewFile = true;
            m_workBook = m_axObject->querySubObject("ActiveWorkBook");
        }
        else
        {
            m_workBook = m_workBooks->querySubObject("Open (const QString&)", m_filePath);
            m_bNewFile = false;

        }
        if (!m_workBook)
        {
            DSGUI::DSMessageNotify::Instance().AddTextNotification(QObject::tr("Open work book filed!"));
            return false;
        }
        QVariant varTitle = m_axObject->property("Caption");          // ��ȡ����
        QString strTitle = varTitle.toString();

        //m_pWorkBook = m_pAxObject->querySubObject("ActiveWorkBook"); // ��ȡ��ǰ������
        m_workSheets = m_workBook->querySubObject("Sheets"); // ��ȡ��������
        m_workSheet = m_workSheets->querySubObject("Item(int)", g_iWorkSheetIndex); //��ȡ�������ϵı�1

        return true;
    }

    bool ExcelFileOperation::closeExcelFile()
    {
        if (!m_workBook || !m_axObject)
        {
            return false;
        }
        saveFile();

        m_axObject->setProperty("DisplayAlerts", true);
        //m_pWorkBook->dynamicCall("Close()"); // close work book
        m_axObject->dynamicCall("Quit()"); // close excel
        m_workBooks = nullptr;
        m_workBook = nullptr;
        m_workSheets = nullptr;
        m_workSheet = nullptr;
        m_axObject.release();
        return true;
    }

    bool ExcelFileOperation::readSheetAllData(QList<QList<QVariant>>& listData)
    {
        if (!m_workSheet || m_workSheet->isNull())
        {
            DSGUI::DSMessageNotify::Instance().AddTextNotification(QObject::tr("Read excel filed!"));
            return false;
        }

        QAxObject* pUsedRange = m_workSheet->querySubObject("UsedRange"); // ��ȡ���з�Χ
        if (!pUsedRange || pUsedRange->isNull())
        {
            DSGUI::DSMessageNotify::Instance().AddTextNotification(QObject::tr("Read excel filed!"));
            return false;
        }

        QVariant var = pUsedRange->dynamicCall("Value");
        castVariantToList(var, listData);
        delete pUsedRange;
        pUsedRange = nullptr;

        closeExcelFile();
        return true;
    }

    bool ExcelFileOperation::writeSheetData(const QList<QList<QVariant>>& listData)
    {
        if (listData.size() <= 0)
        {
            return false;
        }
        if (nullptr == m_workSheet || m_workSheet->isNull())
        {
            return false;
        }
        int row = listData.size();
        int col = listData.at(0).size();
        QString rangStr;
        convertToColName(col, rangStr);
        rangStr += QString::number(row);
        rangStr = "A1:" + rangStr;
        //qDebug() << rangStr;
        QAxObject *range = m_workSheet->querySubObject("Range(const QString&)", rangStr);
        if (NULL == range || range->isNull())
        {
            return false;
        }
        bool succ = false;
        QVariant var;
        castListListVariant2Variant(listData, var);
        succ = range->setProperty("Value", var);
        delete range;
        return succ;
    }

    void ExcelFileOperation::castListListVariant2Variant(const QList<QList<QVariant>> &cells, QVariant& var)
    {
        //var.setValue<QList<QList<QVariant>>>(cells);
        QVariantList vars;
        const int rows = cells.size();
        for (int i = 0; i < rows; ++i)
        {
            vars.append(QVariant(cells[i]));
        }
        var = QVariant(vars);
    }

    bool ExcelFileOperation::castVariantToList(const QVariant& var, QList<QList<QVariant>>& listRes)
    {
        QVariantList varRows = var.toList();
        if (varRows.isEmpty())
        {
            return false;
        }

        int rowCount = varRows.size();
        for (int iIndex = 0; iIndex < rowCount; iIndex++)
        {
            QVariantList rowData = varRows[iIndex].toList();
            listRes.push_back(rowData); // accord to the row data
        }
        return true;
    }

    // brief ������ת��Ϊexcel����ĸ�к�
    // param data ����0����
    // return ��ĸ�кţ���1->A 26->Z 27 AA
    void ExcelFileOperation::convertToColName(int data, QString &res)
    {
        Q_ASSERT(data > 0 && data < 65535);
        int tempData = data / 26;
        if (tempData > 0)
        {
            int mode = data % 26;
            convertToColName(mode, res);
            convertToColName(tempData, res);
        }
        else
        {
            res = (to26AlphabetString(data) + res);
        }
    }

    // brief ����ת��Ϊ26��ĸ
    // 1->A 26->Z
    QString ExcelFileOperation::to26AlphabetString(int data)
    {
        QChar ch = data + 0x40;//A��Ӧ0x41
        return QString(ch);
    }

} // namespace operation