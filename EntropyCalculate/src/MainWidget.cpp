#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "DataStandardization.h"
#include "DataLogarithm.h"
#include "DataWeight.h"
#include "DataGrade.h"

#include <QtWidgets/QDesktopWidget>

const int g_iWidthWidow = 1024;
const int g_iHeighWidow = 824;

namespace app 
{
    MainWidget::MainWidget(QWidget* pParent /*= nullptr*/)
        : DSGUI::DSDialog(pParent, CloseButtonHint /*| MinimizeButtonHint | MaximizeButtonHint*/)
        , m_pUi(nullptr)
        , m_EntropyCalculate{nullptr}
        , m_DataStandardization{nullptr}
        , m_DataRadio{nullptr}
        , m_DataLogarithm{nullptr}
        , m_DataWeight{nullptr}
        , m_DataGrade{nullptr}
    {
        m_pUi.reset(new Ui::MainWidget());
        m_pUi->setupUi(this->GetCentralWidget());
        setAttribute(Qt::WA_StyledBackground);

        this->setAttribute(Qt::WA_TranslucentBackground, false);
        this->resize(g_iWidthWidow, g_iHeighWidow);

        this->setObjectName("MainWidget");
        this->SetCloseAnimaEnable(true);
        //this->setWindowFlags(windowFlags() | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
        this->SetOpenDoubleClickTitle(true);
        this->SetTitle("Entropy Calculate");

        // move to middle
        QDesktopWidget* pDeskWidget = QApplication::desktop();
        int iScreen = pDeskWidget->primaryScreen(); // use primary screen for show
        QWidget* pPrimaryScreen = pDeskWidget->screen(iScreen);

        int iWidth = pPrimaryScreen->width();
        int iHeight = pPrimaryScreen->height();
        this->move((iWidth - width()) / 2, (iHeight - height()) / 2);

        m_pUi->funcStackedWidget->setCurrentWidget(m_pUi->entropyCalculateWidget);
        m_EntropyCalculate = m_pUi->entropyCalculateWidget;
        m_DataStandardization = m_pUi->dataStandardizationWidget;
        m_DataRadio = m_pUi->dataRadioWidget;
        m_DataLogarithm = m_pUi->dataLogarithmWidget;
        m_DataWeight = m_pUi->dataWeightWidget;
        m_DataGrade = m_pUi->dataGradeWidget;
        connectSgn();
    }

    MainWidget::~MainWidget()
    {

    }

    void MainWidget::onEntropyCalculateClickNext()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_DataStandardization);
    }

    void MainWidget::onClickDataStandardization()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_DataStandardization);
        m_EntropyCalculate->getSourceDatas(m_DataStandardization->getSourceDatas());
        m_DataStandardization->initTable();
    }

    void MainWidget::onDataStandardizationClickedBefore()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_EntropyCalculate);
    }

    void MainWidget::onDataStandardizationClickedNext()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_DataRadio);
    }

    void MainWidget::onDataStandardizationClickedRatio()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_DataRadio);
        m_DataStandardization->getStandardizationDatas(m_DataRadio->getStandardizationDatas());
        m_DataRadio->initTable();
    }

    void MainWidget::onDataRadioClickedBefore()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_DataStandardization);
    }

    void MainWidget::onDataRadioClickedNext()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_DataLogarithm);
    }

    void MainWidget::onDataRadioClickedlogarithm()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_DataLogarithm);
        m_DataRadio->getRadioDatas(m_DataLogarithm->getRadioDatas());
        m_DataLogarithm->initTable();
    }

    void MainWidget::onDataLogarithmClickedBefore()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_DataRadio);
    }

    void MainWidget::onDataLogarithmClickedNext()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_DataWeight);
    }

    void MainWidget::onDataLogarithmClickedComentropy()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_DataWeight);
        m_DataLogarithm->getLogarithmDatas(m_DataWeight->getLogarithmDatas());
        m_DataWeight->initTable();
    }

    void MainWidget::onDataWeightClickedBefore()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_DataLogarithm);
    }

    void MainWidget::onDataWeightClickedNext()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_DataGrade);
    }

    void MainWidget::onDataWeightClickedGrade()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_DataGrade);
        m_DataWeight->getWeightDatas(m_DataGrade->getWeightDatas());
        m_EntropyCalculate->getSourceDatas(m_DataGrade->getSourceDatas());
        m_DataGrade->initTable();
    }

    void MainWidget::onDataGradeCkickedBefore()
    {
        m_pUi->funcStackedWidget->setCurrentWidget(m_DataWeight);
    }

    void MainWidget::connectSgn()
    {
        connect(m_EntropyCalculate,    SIGNAL(sgnClickNext()),                this, SLOT(onEntropyCalculateClickNext()));
        connect(m_EntropyCalculate,    SIGNAL(sgnClickDataStandardization()), this, SLOT(onClickDataStandardization()));
        connect(m_DataStandardization, SIGNAL(sgnClickedBefore()),            this, SLOT(onDataStandardizationClickedBefore()));
        connect(m_DataStandardization, SIGNAL(sgnClickedNext()),              this, SLOT(onDataStandardizationClickedNext()));
        connect(m_DataStandardization, SIGNAL(sgnClickedRatio()),             this, SLOT(onDataStandardizationClickedRatio()));
        connect(m_DataRadio,           SIGNAL(sgnClickedBefore()),            this, SLOT(onDataRadioClickedBefore()));
        connect(m_DataRadio,           SIGNAL(sgnClickedNext()),              this, SLOT(onDataRadioClickedNext()));
        connect(m_DataRadio,           SIGNAL(sgnClickedLogarithm()),         this, SLOT(onDataRadioClickedlogarithm()));
        connect(m_DataLogarithm,       SIGNAL(sgnClickedBefore()),            this, SLOT(onDataLogarithmClickedBefore()));
        connect(m_DataLogarithm,       SIGNAL(sgnClickedNext()),              this, SLOT(onDataLogarithmClickedNext()));
        connect(m_DataLogarithm,       SIGNAL(sgnClickedComentropy()),        this, SLOT(onDataLogarithmClickedComentropy()));
        connect(m_DataWeight,          SIGNAL(sgnClickedBefore()),            this, SLOT(onDataWeightClickedBefore()));
        connect(m_DataWeight,          SIGNAL(sgnClickedNext()),              this, SLOT(onDataWeightClickedNext()));
        connect(m_DataWeight,          SIGNAL(sgnClickedGrade()),             this, SLOT(onDataWeightClickedGrade()));
        connect(m_DataGrade,           SIGNAL(sgnClickedBefore()),            this, SLOT(onDataGradeCkickedBefore()));
    }

}