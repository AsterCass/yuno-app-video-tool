
#include "output_widget.h"

QTextBrowser* OutputWidget::outputView = nullptr;

OutputWidget::OutputWidget(QWidget* parent) : QWidget{ parent }
{
    // item
    outputView = new QTextBrowser(this);
    {
        outputView->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        outputView->setObjectName("outputView");
        outputView->setStyleSheet("#outputView{background-color: rgba(25, 25, 25, 100%);font-size:15px;"
                                  "border: 2px solid grey;font-family: 'SimSun';}");
        outputView->setReadOnly(true);
    }
    // layout
    indexLayout = new QVBoxLayout(this);
    indexLayout->setSpacing(0);
    indexLayout->setContentsMargins(0, 0, 0, 0);
    indexLayout->addWidget(outputView);
}

OutputWidget::~OutputWidget()
{
    delete outputView;
    delete indexLayout;
}

void OutputWidget::clearConsole()
{
    outputView->clear();
    //    outputView->clearHistory();
}

void OutputWidget::printMessageWarn(QString str)
{
    outputView->moveCursor(QTextCursor::End);
    outputView->setTextColor(QColor(255, 255, 100));
    outputView->insertPlainText(str.append("\n\n"));
}

void OutputWidget::printMessageError(QString str)
{
    outputView->moveCursor(QTextCursor::End);
    outputView->setTextColor(QColor(255, 100, 100));
    outputView->insertPlainText(str.append("\n\n"));
}

void OutputWidget::printMessage(QString str)
{
    outputView->moveCursor(QTextCursor::End);
    outputView->setTextColor(QColor(100, 255, 100));
    outputView->insertPlainText(str.append("\n"));
    //    printf(str.toUtf8().data());
    //    fflush(NULL);
}
