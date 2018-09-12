#pragma once
#include "ui_DataGrade.h"
#include <QtWidgets/QWidget>
#include <QtGui/QStandardItemModel>

namespace Ui
{
    class DataGrade;
}

namespace app
{
    class DataGrade : public QWidget
    {
        Q_OBJECT

    public:
        DataGrade(QWidget* pParent = nullptr);
        std::map<int, std::vector<QString>>& getWeightDatas();
        std::map<int, std::vector<QString>>& getSourceDatas();
        void initTable();

    signals:
        void sgnClickedBefore();

    private:
        void connectSgn();
        void updateModel();

    private slots:
        void onClickedBefore();
        void addSourceToTable();

    private:
        Ui::DataGrade* ui;
        QStandardItemModel* m_model;
        QStringList m_lstHeader;
        std::map<int, std::vector<QString>> m_weightDatas;
        std::map<int, std::vector<QString>> m_soureceDatas;
    };
}