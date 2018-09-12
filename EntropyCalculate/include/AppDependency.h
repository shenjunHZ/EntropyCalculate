#pragma once
#include <memory>
#include "LoginUI.h"
#include "MainWidget.h"

namespace app
{
    class AppDependency
    {
    public:
        AppDependency();
        bool initAppInstance();

    private:
        std::unique_ptr<operation::CLoginUI> m_loginUiPtr;
        std::unique_ptr<MainWidget> m_mainWidget;
    };
}