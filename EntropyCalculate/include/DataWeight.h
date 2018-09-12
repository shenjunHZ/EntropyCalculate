#pragma once
#include <QtWidgets/QWidget>
#include <QtGui/QStandardItemModel>
#include "ui_DataWeight.h"

namespace Ui
{
    class DataWeight;
}

namespace app
{
    class DataWeight : public QWidget
    {
        Q_OBJECT

    public:
        DataWeight(QWidget* pParent = nullptr);
        std::map<int, std::vector<QString>>& getLogarithmDatas();
        void initTable();
        void getWeightDatas(std::map<int, std::vector<QString>>& weightDatas);

    signals:
        void sgnClickedBefore();
        void sgnClickedNext();
        void sgnClickedGrade();

    private:
        void connectSgn();
        void calculateLogarithmDatas();
        void updateModel();
        void addSourceToTable();

    private slots:
        void onClickedBefore();
        void onClickedNext();
        void onClickedGrade();

    private:
        Ui::DataWeight* ui;
        std::map<int, std::vector<QString>> m_logarithmDatas;
        std::map<int, std::vector<QString>> m_weightDatas;
        QStringList m_lstHeader;
        QStandardItemModel*  m_model;
    };
}