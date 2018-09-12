#pragma once
#include "ExcelFileOperation.h"

namespace operation
{
    class ExcelFileOperationFactory
    {
    public:
        ExcelFileOperationFactory();
        std::unique_ptr<ExcelFileOperation> createExcelFileOperation();
    };
}