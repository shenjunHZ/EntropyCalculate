#pragma once

#include <QtWidgets/QWidget>
#include <QtGui/QStandardItemModel>
#include <memory>
#include "ui_EntropyCalculate.h"
#include "ExcelFileOperationFactory.h"

namespace Ui
{
    class EntropyCalculateClass;
}
namespace operation 
{
    class IExcelFileOperation;
}

namespace app
{
    class WaitingWidget;
    class EntropyCalculate : public QWidget
    {
        Q_OBJECT

    public:
        EntropyCalculate(QWidget *parent = nullptr);
        void getSourceDatas(std::map<int, std::vector<QString>>& sourceDatas);
        
    private:
        void connectSgn();
        void importExcel(const QString& strFile);
        void parseData();
        void updateModel();
        void addSourceToTable();

    signals:
        void sgnClickNext();
        void sgnClickDataStandardization();

    private slots:
        void onClickImport();
        void onClickNext();
        void onClickDataStandardization();

    private:
        std::unique_ptr<Ui::EntropyCalculateClass> ui;
        std::unique_ptr<operation::ExcelFileOperationFactory> m_excelFileOperationFactory;
        std::unique_ptr<WaitingWidget> m_waitingWidget;
        QList<QList<QVariant>> m_listData;
        std::map<int, std::vector<QString>> m_sourceDatas;
        QStandardItemModel*        m_model;
        QStringList                m_lstHeader;
    };

} // namespace app
