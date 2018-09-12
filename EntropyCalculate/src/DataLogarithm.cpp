#include "DataLogarithm.h"
#include "Definition.h"
#include <math.h>

namespace app
{

    DataLogarithm::DataLogarithm(QWidget* pParent /*= nullptr*/)
        : ui{nullptr}
        , m_model{nullptr}
    {
        ui = new Ui::DataLogarithm();
        ui->setupUi(this);
        connectSgn();
    }

    void DataLogarithm::initTable()
    {
        m_logarithmDatas.clear();
        calculateLogarithmDatas();
        calculateTotalData();
        updateModel();
        addSourceToTable();
    }

    std::map<int, std::vector<QString>>& DataLogarithm::getRadioDatas()
    {
        return m_radioDatas;
    }

    void DataLogarithm::getLogarithmDatas(std::map<int, std::vector<QString>>& logarithmDatas)
    {
        logarithmDatas = m_logarithmDatas;
    }

    void DataLogarithm::connectSgn()
    {
        connect(ui->m_btnBefore,     SIGNAL(clicked()), this, SLOT(onClickedBefore()));
        connect(ui->m_btnNext,       SIGNAL(clicked()), this, SLOT(onCliekedNext()));
        connect(ui->m_btnComentropy, SIGNAL(clicked()), this, SLOT(onClickedComentropy()));
    }

    void DataLogarithm::calculateLogarithmDatas()
    {
        int iTotal = m_radioDatas.size() - 1;
        for (auto datas : m_radioDatas)
        {
            if (1 == datas.first)
            {
                m_logarithmDatas[datas.first] = datas.second;
                continue;
            }
            bool bTitel = true;
            std::vector<QString> targets;
            for (auto data : datas.second)
            {
                if (bTitel)
                {
                    targets.push_back(data);
                    bTitel = false;
                    continue;
                }
                float fData = 0.0;
                if (0 == data.toFloat())
                {
                    fData = 0.0;
                }
                else
                {
                    fData = data.toFloat() * log(data.toFloat()) / (-1 * log(iTotal));
                }
                targets.push_back(QString("%1").arg(fData));
            }
            m_logarithmDatas[datas.first] = targets;
        }
    }

    void DataLogarithm::calculateTotalData()
    {
        if (m_logarithmDatas.size() < 1)
        {
            return;
        }

        std::vector<QString> targets;
        std::map<int, float> totals;
        targets.push_back(QObject::tr("Comentropy"));
        for (auto datas : m_logarithmDatas)
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
        int nCount = m_logarithmDatas.size();
        m_logarithmDatas[nCount + 1] = targets;
    }

    void DataLogarithm::updateModel()
    {
        m_lstHeader.clear();
        if (nullptr != m_model)
        {
            m_model->removeRows(0, m_model->rowCount());
            delete m_model;
            m_model = nullptr;
        }
        for (auto datas : m_logarithmDatas)
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

    void DataLogarithm::addSourceToTable()
    {
        for (auto datas : m_logarithmDatas)
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

    void DataLogarithm::onClickedBefore()
    {
        emit sgnClickedBefore();
    }

    void DataLogarithm::onCliekedNext()
    {
        emit sgnClickedNext();
    }

    void DataLogarithm::onClickedComentropy()
    {
        emit sgnClickedComentropy();
    }

}