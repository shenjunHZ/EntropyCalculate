#include "DataGrade.h"
#include "Definition.h"

namespace app
{

    DataGrade::DataGrade(QWidget* pParent /*= nullptr*/)
        : ui{nullptr}
        , m_model{nullptr}
    {
        ui = new Ui::DataGrade();
        ui->setupUi(this);
        connectSgn();
    }

    std::map<int, std::vector<QString>>& DataGrade::getWeightDatas()
    {
        return m_weightDatas;
    }

    std::map<int, std::vector<QString>>& DataGrade::getSourceDatas()
    {
        return m_soureceDatas;
    }

    void DataGrade::initTable()
    {
        updateModel();
        addSourceToTable();
    }

    void DataGrade::updateModel()
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
                m_lstHeader << QObject::tr("Grade");
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

    void DataGrade::addSourceToTable()
    {
        for (auto datas : m_soureceDatas)
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
            float fGrade = 0.0;
            std::vector<QString> targets = m_weightDatas[2];
            for (auto data : datas.second)
            {
                // project
                iIndex++;
                m_model->setData(m_model->index(nRow, static_cast<int>(ElementViewColumn::Element_View_Column_Num) + iIndex), data);

                if (1 != iIndex)
                {
                    fGrade += data.toFloat() * targets[iIndex - 1].toFloat();
                }
            }
            m_model->setData(m_model->index(nRow, static_cast<int>(ElementViewColumn::Element_View_Column_Num) + iIndex + 1), QString("%1").arg(fGrade));
        }
    }

    void DataGrade::connectSgn()
    {
        connect(ui->m_btnBefore, SIGNAL(clicked()), this, SLOT(onClickedBefore()));
    }

    void DataGrade::onClickedBefore()
    {
        emit sgnClickedBefore();
    }

}