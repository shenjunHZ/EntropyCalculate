#pragma once
#include <QtWidgets/QWidget>
#include <QtGui/QStandardItemModel>
#include "ui_DataStandardization.h"

namespace Ui
{
    class DataStandardization;
}

namespace app
{
    class DataStandardization : public QWidget
    {
        Q_OBJECT

    public:
        DataStandardization(QWidget* pParent = nullptr);
        std::map<int, std::vector<QString>>& getSourceDatas();
        void getStandardizationDatas(std::map<int, std::vector<QString>>& datas);
        void initTable();

    private:
        void calculateStandardizationDatas();
        void calculateTotalData();
        void updateModel();
        void addSourceToTable();
        void connectSgn();
        void changeToCalculateDatas(std::map<int, std::vector<float>>& calculateDatas);

    signals:
        void sgnClickedBefore();
        void sgnClickedNext();
        void sgnClickedRatio();

    private slots:
        void onClickedBefore();
        void onClickedNext();
        void onClickedRatio();

    private:
        Ui::DataStandardization* ui;
        QStandardItemModel* m_model;
        std::map<int, std::vector<QString>> m_sourceDatas;
        std::map<int, std::vector<QString>> m_standardizationDatas;
        QStringList m_lstHeader;
    };
}