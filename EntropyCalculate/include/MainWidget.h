#pragma once

#include <QtWidgets/QWidget>
#include <memory>

#include "Dialog/Dialog.h"

namespace Ui
{
    class MainWidget;
}

namespace app
{
    typedef enum class mainwidgetType
    {
        MAIN_WIDGET_TYPE_Main = 0,
        MAIN_WIDGET_TYPE_RecipeElement = 1,
    }mainwidgetType_e;

    class EntropyCalculate;
    class DataStandardization;
    class DataRadio;
    class DataLogarithm;
    class DataWeight;
    class DataGrade;

    class MainWidget : public DSGUI::DSDialog
    {
        Q_OBJECT

    public:
        MainWidget(QWidget* pParent = nullptr);
        ~MainWidget();

    private slots:
        void onEntropyCalculateClickNext();
        void onClickDataStandardization();

        void onDataStandardizationClickedBefore();
        void onDataStandardizationClickedNext();
        void onDataStandardizationClickedRatio();

        void onDataRadioClickedBefore();
        void onDataRadioClickedNext();
        void onDataRadioClickedlogarithm();

        void onDataLogarithmClickedBefore();
        void onDataLogarithmClickedNext();
        void onDataLogarithmClickedComentropy();

        void onDataWeightClickedBefore();
        void onDataWeightClickedNext();
        void onDataWeightClickedGrade();

        void onDataGradeCkickedBefore();

    private:
        void connectSgn();

    private:
        std::unique_ptr<Ui::MainWidget>      m_pUi;
        EntropyCalculate*    m_EntropyCalculate;
        DataStandardization* m_DataStandardization;
        DataRadio*           m_DataRadio;
        DataLogarithm*       m_DataLogarithm;
        DataWeight*          m_DataWeight;
        DataGrade*           m_DataGrade;
    };
}