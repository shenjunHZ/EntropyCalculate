#pragma once

#include <QtWidgets/QDialog>
#include <QtGui/QPalette>
#include <QtCore/QString>
#include <QtGui/QMouseEvent>
#include <QtGui/QImage>
#include <QtGui/QBitmap>
#include <QtGui/QPixmap>
#include <QtGui/QIcon>
#include <QtCore/QThread>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <memory>

namespace Ui 
{
    class CLoginUI;
};
namespace common
{
    class CLocalIniFile;
}

namespace operation
{
    class NoticeFrame : public QFrame
    {
        Q_OBJECT
        Q_PROPERTY(bool notice READ notice WRITE setNotice)
    public:
        NoticeFrame(QWidget*parent = 0):QFrame(parent)
            ,m_bNotice(false)
        {
        }
        bool notice() const{return m_bNotice;}
        void setNotice(bool n){m_bNotice = n;}
    private:
        bool  m_bNotice;
    };

    class CLoginUI : public QDialog
    {
        Q_OBJECT

    public:
        CLoginUI(QWidget *parent = 0);
        ~CLoginUI();

        int init();
        void initCommSkinInfo();

    protected:
        virtual bool eventFilter(QObject *obj, QEvent *event);
        virtual void moveEvent(QMoveEvent *event);

    private slots:
        void OnBtnLogin();
        void OnBtnQuit();
        void OnBtnStretch(bool);
        void OnClearInput();
        void OnTextChanged(const QString &);
        void OnTextEdited(const QString &);

    private:
        void SetErrorMsg(const QString& tipMsg);  
        void UpdateLoginState(bool bLogin);

    private:
        std::unique_ptr<Ui::CLoginUI>					ui;
        std::unique_ptr<common::CLocalIniFile>          m_pLocalIniFile;

        QLabel*                                         m_msgLabel;
        NoticeFrame*									frUserName;
        NoticeFrame*									frUserPwd;
        NoticeFrame*									frAddress;
        NoticeFrame*									frPort;

        QPoint											m_mousePos;
        bool											m_bmousePressed; 
    };
}

