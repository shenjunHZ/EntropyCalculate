#pragma once
#include <QtWidgets/QWidget>
#include <QtGui/QStandardItemModel>
#include "ui_DataRadio.h"

namespace Ui
{
    class DataRadio;
}

namespace app
{
    class DataRadio : public QWidget
    {
        Q_OBJECT

    public:
        DataRadio(QWidget* pParent = nullptr);
        std::map<int, std::vector<QString>>& getStandardizationDatas();
        void initTable();
        void getRadioDatas(std::map<int, std::vector<QString>>& radioDatas);

    signals:
        void sgnClickedBefore();
        void sgnClickedNext();
        void sgnClickedLogarithm();

    private:
        void connectSgn();
        void updateModel();
        void addSourceToTable();
        void calculateRadioData();

    private slots:
        void onClickedBefore();
        void onClickedNext();
        void onClickedLogarithm();

    private:
        Ui::DataRadio* ui;
        QStandardItemModel* m_model;
        QStringList m_lstHeader;
        std::map<int, std::vector<QString>> m_standardizationDatas;
        std::map<int, std::vector<QString>> m_radioDatas;
    };
}