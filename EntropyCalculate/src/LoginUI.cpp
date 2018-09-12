#include "LoginUI.h"
#include "ui_LoginUI.h"
#include "LocalIniFile.h"
#include "commonControl.h"
#include "ErrorCode.h"
#include "LineEdit/LineEdit.h"
#include <libdsl/DStr.h>
#include <QtWidgets/QLineEdit>
#include <QtCore/QByteArray>
#include <QDesktopServices>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <libdsl/DNetUtil.h>
#include <Tools/Dir.h>
#include <QSettings>
#include <QProcess>
#include <iphlpapi.h>

using namespace DSGUI;

namespace operation
{
    static constexpr int MAX_USERNAME_LEN = 20;
    static constexpr int MAX_USERPWD_LEN = 16;
    
    static const QRegExp regExDns("([0-9A-Za-z\\-_\\.]+)\\.([a-z]{2,3}(\\.[a-z]{2})?)");
    static const QRegExp regExIp("^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$");
    const std::string strCode{ "txj" };

    CLoginUI::CLoginUI(QWidget *parent)
        : QDialog(parent, Qt::FramelessWindowHint)
        , m_pLocalIniFile(nullptr)
        , m_bmousePressed(false)
    {
        ui.reset( new Ui::CLoginUI() );
        ui->setupUi(this);
        setAttribute(Qt::WA_DeleteOnClose, false);
        setAttribute(Qt::WA_TranslucentBackground, true);
        // move to middle
        QDesktopWidget* pDeskWidget = QApplication::desktop();
        int iScreen = pDeskWidget->primaryScreen(); // use primary screen for show
        QWidget* pPrimaryScreen = pDeskWidget->screen(iScreen);

        int iWidth = pPrimaryScreen->width();
        int iHeight = pPrimaryScreen->height();
        this->move((iWidth - width()) / 2, (iHeight - height()) / 2);
        
        QString oemAppname = "Entropy Calculate";
        this->setWindowTitle(oemAppname);

        m_msgLabel = ui->msgLabel;
        m_msgLabel->setObjectName("msgLabel");
        m_msgLabel->hide();

        ui->userClearBtn->hide();
        ui->pwdClearBtn->hide();
        ui->leUserName->setTextMargins(24, 0, 20, 0);
        frUserName = new NoticeFrame(ui->leUserName);
        frUserName->setGeometry(QRect(8, (ui->leUserName->height()-20)/2, 20, 20));
        frUserName->setObjectName("LAB_USER");
        ui->leUserName->setMaxLength(MAX_USERNAME_LEN);
        ui->leUserPwd->setTextMargins(24, 0, 20, 0);
        ui->leUserPwd->setMaxLength(MAX_USERPWD_LEN);
        ui->leUserPwd->setEchoMode(QLineEdit::Password);
        ui->leUserPwd->setPlaceholderText(tr("Please input Password"));
        frUserPwd = new NoticeFrame(ui->leUserPwd);
        frUserPwd->setGeometry(QRect(8, (ui->leUserPwd->height()-20)/2, 20, 20));
        frUserPwd->setObjectName("LAB_PASSWORD");
        
        ui->leServerPort->setTextMargins(24, 0, 0, 0);
        ui->leServerPort->setPlaceholderText(tr("Please input Port"));
        frPort = new NoticeFrame(ui->leServerPort);
        frPort->setGeometry(QRect(8, (ui->leServerPort->height()-20)/2, 20, 20));
        frPort->setObjectName("LAB_PORT");

        ui->leServerIp->setTextMargins(24, 0, 0, 0);
        ui->leServerIp->setPlaceholderText(tr("Please input Ip"));
        frAddress = new NoticeFrame(ui->leServerIp);
        frAddress->setGeometry(QRect(8, (ui->leServerIp->height()-20)/2, 20, 20));
        frAddress->setObjectName("LAB_ADDRESS");
        
        ui->btnLogin->show();
        
        ui->btnExpand->setChecked(true);
        
        ui->leUserName->installEventFilter(this);
        ui->leUserPwd->installEventFilter(this);
        ui->CLoginUIBg->installEventFilter(this);
        setTabOrder(ui->leUserName,ui->leUserPwd);
        setTabOrder(ui->leUserPwd,ui->btnLogin);

        connect(ui->btnLogin,  SIGNAL(clicked()),     this, SLOT(OnBtnLogin()));
        connect(ui->btnQuit,   SIGNAL(clicked()),     this, SLOT(OnBtnQuit()));
        connect(ui->btnExpand, SIGNAL(toggled(bool)), this, SLOT(OnBtnStretch(bool)));

        connect(ui->leUserPwd,  SIGNAL(textChanged(const QString &)), this, SLOT(OnTextChanged(const QString &)));
        connect(ui->leUserPwd,  SIGNAL(textEdited(const QString &)), this, SLOT(OnTextEdited(const QString &)));
        connect(ui->leUserName, SIGNAL(textEdited(const QString &)), this, SLOT(OnTextEdited(const QString &)));
        connect(ui->userClearBtn,SIGNAL(pressed()),this,SLOT(OnClearInput()));
        connect(ui->pwdClearBtn,SIGNAL(pressed()),this,SLOT(OnClearInput())); 

        SET_MODULE_STYLE_BY_PATH("LoginUI");

        this->setModal(true);
        this->activateWindow();
    }

    CLoginUI::~CLoginUI()
    {

    }

    int CLoginUI::init()
    {
        m_pLocalIniFile = std::make_unique<common::CLocalIniFile>();

        initCommSkinInfo();
        return 0;
    }

    void CLoginUI::initCommSkinInfo()
    {	
        QVariant varValue = m_pLocalIniFile->ReadValue("Login", "userName", "Localcfg.ini");
        ui->leUserName->setText(varValue.toString());
        ui->leUserName->setReadOnly(true);

        ui->leServerIp->setText("");
        ui->leServerPort->setText("");
        ui->leServerIp->setDisabled(true);
        ui->leServerPort->setDisabled(true);
        ui->btnExpand->setChecked(false);
    }

    void CLoginUI::SetErrorMsg(const QString& tipMsg)
    {
        if (!this->isVisible())
        {
            QDialog::setVisible(true);
        }
        m_msgLabel->setText(tipMsg);
        if (tipMsg.isEmpty())
        {
            m_msgLabel->hide();
        }
        else
        {
            m_msgLabel->setText(tipMsg);
            m_msgLabel->show();
        }
    }

    void CLoginUI::OnBtnLogin()
    {
        SetErrorMsg(""); 

        QString strUserName = ui->leUserName->text().trimmed();
        QString strUserPwd = ui->leUserPwd->text();
            
        if ( strUserName.isEmpty() || strUserPwd.isEmpty() )
        {
            if (strUserName.isEmpty())
            {
                SetErrorMsg(tr("User Name can not be Empty"));
                frUserName->setNotice(true);
                frUserPwd->setNotice(false);
                style()->polish(frUserName);
                style()->polish(frUserPwd);
                ui->leUserName->setFocus();
            }
            else if (strUserPwd.isEmpty())
            {
                SetErrorMsg(tr("User Password can not be Empty"));
                frUserPwd->setNotice(true);
                frUserName->setNotice(false);
                style()->polish(frUserPwd);
                style()->polish(frUserName);
                ui->leUserPwd->setFocus();
            }
            return;
        }
        if (strUserPwd != QString::fromStdString(strCode))
        {
            SetErrorMsg(tr("Password is not right!"));
            return;
        }
        this->accept();
    }

    void CLoginUI::UpdateLoginState(bool bLogin)
    {
        ui->btnLogin->setText(bLogin ? tr("Logining...") : tr("Sign in"));

        ui->leUserPwd->setEnabled(!bLogin);
        ui->leUserName->setEnabled(!bLogin);
        ui->cbMemorizePwd->setEnabled(!bLogin);
        ui->btnLogin->setEnabled(!bLogin);

        if (bLogin)
        {
            ui->CLoginUIBg->setFocus();
        }
    }

    void CLoginUI::OnBtnQuit()
    {
        UpdateLoginState(false);
        this->close();
    }

    void CLoginUI::OnBtnStretch(bool bShow)
    {
        SetErrorMsg("");
        style()->polish(ui->btnExpand);

        if ( bShow )
        {	
            ui->leServerIp->show();
            ui->leServerPort->show();
            ui->btnExpand->setGeometry(350, 310, 80, 16);
            ui->wgtMask->show();
        }
        else
        {
            ui->leServerIp->hide();
            ui->leServerPort->hide();
            ui->btnExpand->setGeometry(350, 400, 80, 16);
            ui->wgtMask->hide();
        }
    }

    void CLoginUI::OnClearInput()
    {
        QWidget *obj = (QWidget*)sender();
        if (obj == ui->pwdClearBtn)
        {
            ui->leUserPwd->clear();
        }
        else if (obj == ui->userClearBtn)
        {
            ui->leUserName->clear();
        }
    }

    bool CLoginUI::eventFilter(QObject *obj, QEvent *event)
    {
        if (obj == ui->CLoginUIBg)
        {
            if (event->type() == QEvent::MouseButtonPress)
            {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                if (mouseEvent->button() == Qt::LeftButton)
                {
                    m_mousePos = mouseEvent->globalPos();
                    m_bmousePressed = true;
                    return true;
                }
            }
            else if (event->type() == QEvent::MouseMove)
            {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                if ((mouseEvent->buttons() & Qt::LeftButton) && m_bmousePressed)
                {
                    int xDis = mouseEvent->globalPos().x() - m_mousePos.x();
                    int yDis = mouseEvent->globalPos().y() - m_mousePos.y();
                    m_mousePos = mouseEvent->globalPos();
                    QPoint pos = mapToGlobal(QPoint(0,0));
                    move(pos.x() + xDis,pos.y() + yDis);
                    return true;
                }
            }
            else if (event->type() == QEvent::MouseButtonRelease)
            {
                if(m_bmousePressed)
                {
                    ui->CLoginUIBg->setFocus();
                }
                m_bmousePressed = false;

                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                if (mouseEvent->button() == Qt::LeftButton)
                {
                    return true;
                }
            }
        }
        else if(obj == ui->leUserPwd || obj == ui->leUserName)
        {
            if (event->type()  == QEvent::FocusIn)
            {
                if(obj == ui->leUserPwd)
                {
                    ui->pwdClearBtn->setVisible(!ui->leUserPwd->text().isEmpty());
                }
                else if(obj == ui->leUserName)
                {
                   // ui->userClearBtn->setVisible(!ui->leUserName->text().isEmpty());
                }
            }
            else if (event->type()  == QEvent::FocusOut)
            {
                if(obj == ui->leUserPwd)
                {
                    ui->pwdClearBtn->hide();
                }
                else
                {
                    ui->userClearBtn->hide();
                }			
            }
        }
        return QDialog::eventFilter(obj,event);
    }

    void CLoginUI::OnTextEdited( const QString & dat)
    {
        QWidget *obj = (QWidget*)sender();
        if(obj == ui->leUserName)
        {
            ui->userClearBtn->setVisible(!dat.isEmpty());
            frUserName->setNotice(false);
            style()->polish(frUserName);
        }
        else if(obj == ui->leUserPwd)
        {
            ui->pwdClearBtn->setVisible(!dat.isEmpty());
            frUserPwd->setNotice(false);
            style()->polish(frUserPwd);
        }
        
        SetErrorMsg("");
    }

    void CLoginUI::OnTextChanged( const QString & dat)
    {
        if(dat.isEmpty())
        {
            QFont ft = ui->leUserPwd->font();
            ft.setLetterSpacing(QFont::AbsoluteSpacing, ui->leUserName->font().letterSpacing());
            ui->leUserPwd->setFont(ft);
        }
        else
        {
            QFont ft = ui->leUserPwd->font();
            ft.setLetterSpacing(QFont::AbsoluteSpacing, 4);
            ui->leUserPwd->setFont(ft);
        }
    }

    void CLoginUI::moveEvent(QMoveEvent *event)
    {
        const int margin = 20;
        QPoint pt = event->pos();
        QRect rtWnd = QApplication::desktop()->availableGeometry(pt);
        if (pt.x()+this->width()<=margin)
        {
            pt = QPoint(-this->width()+margin, pt.y());
        }
        if (pt.x()>rtWnd.right()-margin)
        {
            pt = QPoint(rtWnd.right()-margin, pt.y());
        }
        if (pt.y()+this->height()<=margin)
        {
            pt = QPoint(pt.x(), -this->height()+margin);
        }
        if (pt.y()>rtWnd.bottom()-margin)
        {
            pt = QPoint(pt.x(), rtWnd.bottom()-margin);
        }
        move(pt);
    }
}