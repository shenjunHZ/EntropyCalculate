#include "ExcelFileOperationFactory.h"

namespace operation
{

    ExcelFileOperationFactory::ExcelFileOperationFactory()
    {

    }

    std::unique_ptr<operation::ExcelFileOperation> ExcelFileOperationFactory::createExcelFileOperation()
    {
        return std::make_unique<ExcelFileOperation>();
    }

}