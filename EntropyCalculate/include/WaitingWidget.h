#ifndef INCLUDE_SMARTTRACK_WAITING_WND_H
#define INCLUDE_SMARTTRACK_WAITING_WND_H

#include <QtWidgets//QDialog>
#include <QtGui/QMovie>
#include <QtWidgets/QLabel>

#define LOADING_GIF		"./Skin/DustyBlue/WaitingWidget/loading.gif"
namespace app
{
    class WaitingWidget : public QDialog
    {
        Q_OBJECT

    public:
        explicit WaitingWidget(QWidget *parent = 0);
        ~WaitingWidget();

        void StartWaiting();
        void StopWaiting(bool bIsFinished = true);
        void SetTimeOut(int nTime);

        bool GetIfCanceled();
        void SetPressESCToCancel(bool);

    protected:
        virtual void keyPressEvent(QKeyEvent * e);

    private:
        QLabel *m_label;
        QMovie *m_movie;
        QTimer *m_tmrStop;
        int m_nSecsToTimeOut;
        bool m_bIfCanceled;
        bool m_bTimeOutToCancel;

    signals:
        void sgnUnNormalExit();
        void sgnReadyToStopWaiting();

    private slots:
        void OnTimeOutToStopWaiting();
    };

}

#endif // INCLUDE_SMARTTRACK_WAITING_WND_H
