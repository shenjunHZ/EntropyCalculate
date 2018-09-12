#include "EntropyCalculate.h"
#include "ExcelFileOperation.h"
#include "WaitingWidget.h"
#include "Definition.h"
#include <QtWidgets/QFileDialog>
#include <thread>
#include <Ole2.h>

namespace
{
    const int SaveFileWaitTime = 1 * 60;
}

namespace app
{
    EntropyCalculate::EntropyCalculate(QWidget *parent)
        : QWidget(parent)
        , m_waitingWidget{std::make_unique<WaitingWidget>(this)}
        , m_excelFileOperationFactory{ std::make_unique<operation::ExcelFileOperationFactory>() }
        , m_model{nullptr}
    {
        ui.reset(new Ui::EntropyCalculateClass());
        ui->setupUi(this);
        m_waitingWidget->SetTimeOut(SaveFileWaitTime);

        ui->m_editOfficialName->setEnabled(false);
        connectSgn();
    }

    void EntropyCalculate::getSourceDatas(std::map<int, std::vector<QString>>& sourceDatas)
    {
        sourceDatas = m_sourceDatas;
    }

    void EntropyCalculate::connectSgn()
    {
        connect(ui->m_btnImport,              SIGNAL(clicked()), this, SLOT(onClickImport()));
        connect(ui->m_btnNext,                SIGNAL(clicked()), this, SLOT(onClickNext()));
        connect(ui->m_btnDataStandardization, SIGNAL(clicked()), this, SLOT(onClickDataStandardization()));
    }

    void EntropyCalculate::importExcel(const QString& strFile)
    {
        HRESULT result = OleInitialize(0);
        if (!strFile.isEmpty() && nullptr != m_excelFileOperationFactory)
        {
            m_listData.clear();
            ui->m_editOfficialName->setText(strFile);
            ui->m_editOfficialName->setToolTip(strFile);

            std::unique_ptr<operation::IExcelFileOperation> excelFileOperationPrt{ std::move(m_excelFileOperationFactory->createExcelFileOperation()) };
            if(!excelFileOperationPrt->openExcelFile(strFile))
            {
                OleUninitialize();
                m_waitingWidget->StopWaiting();
                return;
            }
            excelFileOperationPrt->readSheetAllData(m_listData);
        }
        OleUninitialize();
        m_waitingWidget->StopWaiting();
    }

    void EntropyCalculate::parseData()
    {
        if (m_listData.isEmpty())
        {
            return;
        }

        m_sourceDatas.clear();
        int iIndex = 1;
        for (auto datas : m_listData)
        {
            std::vector<QString>& sourceDatas = m_sourceDatas[iIndex];
            for (auto data : datas)
            {
                sourceDatas.push_back(data.toString());
            }
            iIndex++;
        }
    }

    void EntropyCalculate::updateModel()
    {
        m_lstHeader.clear();
        if (nullptr != m_model)
        {
            m_model->removeRows(0, m_model->rowCount());
            delete m_model;
            m_model = nullptr;
        }
        for (auto datas : m_sourceDatas)
        {
            if (1 == datas.first)
            {
                m_lstHeader << "No.";
                for (auto data : datas.second)
                {
                    m_lstHeader << data;
                }
            }
            break;
        }
        m_model = new QStandardItemModel(0, m_lstHeader.count());
        ui->m_listViewElement->setModel(m_model);
        ui->m_listViewElement->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->m_listViewElement->setColumnWidth(static_cast<int>(ElementViewColumn::Element_View_Column_Num), 30);
        int begin = 0;
        for (auto name : m_lstHeader)
        {
            m_model->setHeaderData(begin, Qt::Horizontal, name);
            m_model->setHeaderData(begin, Qt::Horizontal, Qt::AlignCenter, Qt::TextAlignmentRole);
            begin++;
        }
    }

    void EntropyCalculate::addSourceToTable()
    {
        for (auto datas : m_sourceDatas)
        {
            if (1 == datas.first)
            {
                continue;
            }

            int nRow = m_model->rowCount();
            QStandardItem* item = new QStandardItem();
            m_model->insertRow(nRow, item);

            // NO.
            QString strText = QString("%1").arg(nRow + 1);
            m_model->setData(m_model->index(nRow, static_cast<int>(ElementViewColumn::Element_View_Column_Num)), strText);
            // Source
            int iIndex = 0;
            for (auto data : datas.second)
            {
                // project
                iIndex++;
                m_model->setData(m_model->index(nRow, static_cast<int>(ElementViewColumn::Element_View_Column_Num) + iIndex), data);
            }
        }
    }

    void EntropyCalculate::onClickImport()
    {
        QString strFilePath = QFileDialog::getOpenFileName(this, tr("Inport Recipe Table"), "", tr("*.xls*"));
        if (strFilePath.isEmpty())
        {
            return;
        }
        std::unique_ptr<std::thread> threadPrt = std::make_unique<std::thread>(&EntropyCalculate::importExcel, this, strFilePath);
        m_waitingWidget->StartWaiting();
        threadPrt->join();

        parseData();
        updateModel();
        addSourceToTable();
    }

    void EntropyCalculate::onClickNext()
    {
        emit sgnClickNext();
    }

    void EntropyCalculate::onClickDataStandardization()
    {
        emit sgnClickDataStandardization();
    }

}