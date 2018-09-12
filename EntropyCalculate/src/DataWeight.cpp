#include "DataWeight.h"
#include "Definition.h"

namespace app
{

    DataWeight::DataWeight(QWidget* pParent /*= nullptr*/)
        : ui{nullptr}
        , m_model{nullptr}
    {
        ui = new Ui::DataWeight();
        ui->setupUi(this);
        connectSgn();
    }

    std::map<int, std::vector<QString>>& DataWeight::getLogarithmDatas()
    {
        return m_logarithmDatas;
    }

    void DataWeight::initTable()
    {
        m_weightDatas.clear();
        calculateLogarithmDatas();
        updateModel();
        addSourceToTable();
    }

    void DataWeight::getWeightDatas(std::map<int, std::vector<QString>>& weightDatas)
    {
        weightDatas = m_weightDatas;
    }

    void DataWeight::updateModel()
    {
        m_lstHeader.clear();
        if (nullptr != m_model)
        {
            m_model->removeRows(0, m_model->rowCount());
            delete m_model;
            m_model = nullptr;
        }
        for (auto datas : m_weightDatas)
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

    void DataWeight::addSourceToTable()
    {
        for (auto datas : m_weightDatas)
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

    void DataWeight::connectSgn()
    {
        connect(ui->m_btnBefore, SIGNAL(clicked()), this, SLOT(onClickedBefore()));
        connect(ui->m_btnNext,   SIGNAL(clicked()), this, SLOT(onClickedNext()));
        connect(ui->m_btnGrade,  SIGNAL(clicked()), this, SLOT(onClickedGrade()));
    }

    void DataWeight::calculateLogarithmDatas()
    {
        float fTotalData = 0.0;
        for (auto datas : m_logarithmDatas)
        {
            if (1 == datas.first)
            {
                m_weightDatas[datas.first] = datas.second;
                continue;
            }
            else if (datas.first == m_logarithmDatas.size() - 1)
            {
                bool bTitle = true;
                std::vector<QString> targets;
                for (auto data : datas.second)
                {
                    if (bTitle)
                    {
                        bTitle = false;
                        targets.push_back(QObject::tr("weight")); // 权重
                        continue;
                    }
                    fTotalData += data.toFloat(); // 熵值求和
                }
                bTitle = true;
                for (auto data : datas.second)
                {
                    if (bTitle)
                    {
                        bTitle = false;
                        continue;
                    }
                    float fWeight = (1 - data.toFloat()) / ((datas.second.size() - 1) - fTotalData);
                    targets.push_back(QString("%1").arg(fWeight));
                }

                m_weightDatas[2] = targets;
            }
        }
    }

    void DataWeight::onClickedBefore()
    {
        emit sgnClickedBefore();
    }

    void DataWeight::onClickedNext()
    {
        emit sgnClickedNext();
    }

    void DataWeight::onClickedGrade()
    {
        emit sgnClickedGrade();
    }

}