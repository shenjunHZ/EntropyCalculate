#include "DataRadio.h"
#include "Definition.h"

namespace app
{
    DataRadio::DataRadio(QWidget* pParent /*= nullptr*/)
        : ui{nullptr}
        , m_model{nullptr}
    {
        ui = new Ui::DataRadio();
        ui->setupUi(this);
        connectSgn();
    }

    std::map<int, std::vector<QString>>& DataRadio::getStandardizationDatas()
    {
        return m_standardizationDatas;
    }

    void DataRadio::initTable()
    {
        if (m_standardizationDatas.size() < 1)
        {
            return;
        }
        calculateRadioData();
        updateModel();
        addSourceToTable();
    }

    void DataRadio::getRadioDatas(std::map<int, std::vector<QString>>& radioDatas)
    {
        radioDatas = m_radioDatas;
    }

    void DataRadio::connectSgn()
    {
        connect(ui->m_btnBefore, SIGNAL(clicked()), this, SLOT(onClickedBefore()));
        connect(ui->m_btnNext,   SIGNAL(clicked()), this, SLOT(onClickedNext()));
        connect(ui->m_btnLogarithm,  SIGNAL(clicked()), this, SLOT(onClickedLogarithm()));
    }
    
    void DataRadio::updateModel()
    {
        m_lstHeader.clear();
        if (nullptr != m_model)
        {
            m_model->removeRows(0, m_model->rowCount());
            delete m_model;
            m_model = nullptr;
        }
        for (auto datas : m_radioDatas)
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

    void DataRadio::addSourceToTable()
    {
        for (auto datas : m_radioDatas)
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

    void DataRadio::calculateRadioData()
    {
        for (auto datas : m_standardizationDatas)
        {
            std::vector<QString> targets = datas.second;
            bool bTitle = true;
            if (1 == datas.first)
            {
                m_radioDatas[datas.first] = targets;
            }
            else if (m_standardizationDatas.size() == datas.first)
            {
                return;
            }
            else
            {
                targets.clear();
                int iIndex = 0;
                for (auto data : datas.second)
                {
                    if (bTitle)
                    {
                        bTitle = false;
                        targets.push_back(data); // ÏîÄ¿
                        continue;
                    }
                    iIndex++;
                    std::vector<QString> standardizationDatas = m_standardizationDatas[m_standardizationDatas.size()];
                    float fData =  data.toFloat() / standardizationDatas[iIndex].toFloat();
                    targets.push_back(QString("%1").arg(fData));
    
                }
                m_radioDatas[datas.first] = targets;
            }
        }
    }

    void DataRadio::onClickedBefore()
    {
        emit sgnClickedBefore();
    }

    void DataRadio::onClickedNext()
    {
        emit sgnClickedNext();
    }

    void DataRadio::onClickedLogarithm()
    {
        emit sgnClickedLogarithm();
    }

}