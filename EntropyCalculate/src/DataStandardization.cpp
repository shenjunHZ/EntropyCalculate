#include "DataStandardization.h"
#include "Definition.h"

namespace app
{

    DataStandardization::DataStandardization(QWidget* pParent /*= nullptr*/)
        : ui{new Ui::DataStandardization()}
        , m_model{nullptr}
    {
        ui->setupUi(this);

        connectSgn();
    }

    std::map<int, std::vector<QString>>& DataStandardization::getSourceDatas()
    {
        return m_sourceDatas;
    }

    void DataStandardization::getStandardizationDatas(std::map<int, std::vector<QString>>& datas)
    {
        datas = m_standardizationDatas;
    }

    void DataStandardization::initTable()
    {
        if (m_sourceDatas.size() < 1)
        {
            return;
        }
        m_standardizationDatas.clear();
        calculateStandardizationDatas();
        calculateTotalData();
        updateModel();
        addSourceToTable();
    }

    void DataStandardization::calculateStandardizationDatas()
    {
        if (m_sourceDatas.size() < 1)
        {
            return;
        }
        for (auto datas : m_sourceDatas)
        {
            std::vector<QString> targets = datas.second;
            bool bTitle = true;
            if (1 == datas.first)
            {
                m_standardizationDatas[datas.first] = targets;
            }
            else
            {
                targets.clear();
                std::map<int, std::vector<float>> calculateDatas;
                changeToCalculateDatas(calculateDatas);

                bTitle = true;
                int iIndex = 1;
                for (auto data : datas.second)
                {
                    if (bTitle)
                    {
                        bTitle = false;
                        targets.push_back(data); // ÏîÄ¿
                        continue;
                    }

                    float fMax = calculateDatas[iIndex][0];
                    float fMin = calculateDatas[iIndex][0];
                    for (auto data : calculateDatas[iIndex])
                    {
                        if (data > fMax)
                        {
                            fMax = data;
                        }
                        else if (data < fMin)
                        {
                            fMin = data;
                        }
                    }
                    iIndex++;

                    if (data.toFloat() == fMax)
                    {
                        targets.push_back(QString("%1").arg(1));
                    }
                    else
                    {
                        targets.push_back(QString("%1").arg((data.toFloat() - fMin)/(fMax - fMin)));
                    }
                }
                m_standardizationDatas[datas.first] = targets;
            }
        }
    }

    void DataStandardization::calculateTotalData()
    {
        if (m_standardizationDatas.size() < 1)
        {
            return;
        }

        std::vector<QString> targets;
        std::map<int, float> totals;
        targets.push_back(QObject::tr("total"));
        for (auto datas : m_standardizationDatas)
        {
            if (1 != datas.first)
            {
                float fTotal = 0.0;
                bool bTitle = true;
                int iIndex = 1;
                for (auto data : datas.second)
                {
                    if (bTitle)
                    {
                        bTitle = false;
                        continue;
                    }
                    totals[iIndex] += data.toFloat();
                    iIndex++;
                }
            }
        }
        for (auto total : totals)
        {
            targets.push_back(QString("%1").arg(total.second));
        }
        int nCount = m_standardizationDatas.size();
        m_standardizationDatas[nCount + 1] = targets;
    }

    void DataStandardization::updateModel()
    {
        m_lstHeader.clear();
        if (nullptr != m_model)
        {
            m_model->removeRows(0, m_model->rowCount());
            delete m_model;
            m_model = nullptr;
        }
        for (auto datas : m_standardizationDatas)
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

    void DataStandardization::addSourceToTable()
    {
        for (auto datas : m_standardizationDatas)
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

    void DataStandardization::connectSgn()
    {
        connect(ui->m_btnBefore, SIGNAL(clicked()), this, SLOT(onClickedBefore()));
        connect(ui->m_btnNext,   SIGNAL(clicked()), this, SLOT(onClickedNext()));
        connect(ui->m_btnRatio,  SIGNAL(clicked()), this, SLOT(onClickedRatio()));
    }

    void DataStandardization::changeToCalculateDatas(std::map<int, std::vector<float>>& calculateDatas)
    {
        if (m_sourceDatas.size() < 1)
        {
            return;
        }
        for (auto datas : m_sourceDatas)
        {
            if (1 != datas.first)
            {
                int iIndex = 1;
                bool bTitle = true;
                for (auto data : datas.second)
                {
                    if (bTitle)
                    {
                        bTitle = false;
                        continue;
                    }
                    std::vector<float>& fDatas = calculateDatas[iIndex];
                    fDatas.push_back(data.toFloat());
                    iIndex++;
                }
            }
        }
    }

    void DataStandardization::onClickedBefore()
    {
        emit sgnClickedBefore();
    }

    void DataStandardization::onClickedNext()
    {
        emit sgnClickedNext();
    }

    void DataStandardization::onClickedRatio()
    {
        emit sgnClickedRatio();
    }

}