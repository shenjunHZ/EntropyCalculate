#include "WaitingWidget.h"
#include <QtGui/QPixmap>
#include <QtGui/QBitmap>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>

namespace app
{
    WaitingWidget::WaitingWidget(QWidget *parent)
        : QDialog(parent)
    {
        m_label = new QLabel(this);
        m_movie = new QMovie(LOADING_GIF);

        m_nSecsToTimeOut = -1;
        m_bIfCanceled = false;
        m_bTimeOutToCancel = false;

        m_tmrStop = new QTimer(this);
        connect(m_tmrStop, SIGNAL(timeout()), this, SLOT(OnTimeOutToStopWaiting()));

        QPixmap bkImg(LOADING_GIF);
        this->setMinimumSize(bkImg.size());
        this->setMaximumSize(bkImg.size());
        
        QPixmap p(LOADING_GIF,"gif");
        this->setMask(p.mask());

        m_label->setMovie(m_movie);
        setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    }

    WaitingWidget::~WaitingWidget()
    {
        m_tmrStop->stop();
        delete m_tmrStop;
        delete m_movie;
        delete m_label;
    }

    void WaitingWidget::StartWaiting()
    {
        m_label->setGeometry(this->rect());
        m_label->show();
        m_movie->start();

        if(m_nSecsToTimeOut != -1) 
        {
            m_tmrStop->start(m_nSecsToTimeOut *1000);
        }
        setAttribute(Qt::WA_Moved, false);
        exec();
    }

    void WaitingWidget::StopWaiting(bool bIsFinished)
    {
        m_movie->stop();
        m_label->hide();
        m_tmrStop->stop();
        close();

        if(bIsFinished == false) 
        {
            emit sgnUnNormalExit();
        }
    }

    void WaitingWidget::OnTimeOutToStopWaiting()
    {
        StopWaiting(false);
    }

    void WaitingWidget::SetTimeOut(int sec)
    {
        m_nSecsToTimeOut = sec;
    }

    bool WaitingWidget::GetIfCanceled()
    {
        return m_bIfCanceled;
    }

    void WaitingWidget::SetPressESCToCancel(bool bCancel)
    {
        m_bIfCanceled = bCancel;
    }

    void WaitingWidget::keyPressEvent( QKeyEvent * e )
    {
        if(!isHidden() && e->key() == Qt::Key_Escape)
        {
            if(m_bIfCanceled)
            {
                StopWaiting(false);
            }
            else
            {
                emit sgnReadyToStopWaiting();
            }
        }
    }
}