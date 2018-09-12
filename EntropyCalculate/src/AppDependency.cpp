#include "AppDependency.h"

namespace app
{
    AppDependency::AppDependency()
        : m_loginUiPtr{nullptr}
        , m_mainWidget{nullptr}
    {

    }

    bool AppDependency::initAppInstance()
    {
        m_loginUiPtr = std::make_unique<operation::CLoginUI>();
        m_loginUiPtr->init();
        int dlgCode = m_loginUiPtr->exec();

        if (QDialog::Rejected == dlgCode)
        {
            return false;
        }

        m_mainWidget = std::make_unique<MainWidget>();
        m_mainWidget->show();

        return true;
    }

} // namespace app