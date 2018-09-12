#pragma once
#include <QtWidgets/QWidget>
#include <QtGui/QStandardItemModel>
#include "ui_DataLogarithm.h"

namespace Ui
{
    class DataLogarithm;
}

namespace app
{
    class DataLogarithm : public QWidget
    {
        Q_OBJECT

    public:
        DataLogarithm(QWidget* pParent = nullptr);
        void initTable();
        std::map<int, std::vector<QString>>& getRadioDatas();
        void getLogarithmDatas(std::map<int, std::vector<QString>>& logarithmDatas);

    signals:
        void sgnClickedBefore();
        void sgnClickedNext();
        void sgnClickedComentropy();

    private:
        void connectSgn();
        void calculateLogarithmDatas();
        void calculateTotalData();
        void updateModel();
        void addSourceToTable();

    private slots:
        void onClickedBefore();
        void onCliekedNext();
        void onClickedComentropy();

    private:
        Ui::DataLogarithm* ui;
        QStandardItemModel* m_model;
        std::map<int, std::vector<QString>> m_radioDatas;
        std::map<int, std::vector<QString>> m_logarithmDatas;
        QStringList m_lstHeader;
    };
}