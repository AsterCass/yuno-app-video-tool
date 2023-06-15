
#include "input_widget.h"

InputWidget::InputWidget(QWidget* parent) : QWidget{ parent }
{
    // todo QMessageBox::information
    // item
    launchModel = new QTabWidget(this);
    launchModel->addTab(createFrpFileExposeWidget(), "FRP文件穿透");
    launchModel->addTab(createFrpServiceExposeWidget(), "FRP服务穿透");
    launchModel->addTab(createToH264Mp4Widget(), "H264视频文件转换");
    {
        QString tabStyle = QString("QTabBar::tab#%1{font-size: 15px;color: rgb(86, 86, 86);}").arg(launchModel->tabBar()->objectName());
        launchModel->setStyleSheet(tabStyle);
    }
    // layout
    indexLayout = new QVBoxLayout(this);
    indexLayout->setSpacing(0);
    indexLayout->setContentsMargins(0, 0, 0, 0);
    indexLayout->addWidget(launchModel);
}

InputWidget::~InputWidget()
{
    // tabs frp file
    delete frpFileExposeWidget;
    // tabs frp service
    delete frpServiceExposeWidget;
    // tabs h264 convert
    delete toH264Mp4Widget;
    // main
    delete indexLayout;
    delete launchModel;
}

void InputWidget::lanuch()
{
    frpFileExposeWidget->lanuch();
}

FrpFileExposeWidget* InputWidget::createFrpFileExposeWidget()
{
    frpFileExposeWidget = new FrpFileExposeWidget();
    return frpFileExposeWidget;
}

QWidget* InputWidget::createFrpServiceExposeWidget()
{
    frpServiceExposeWidget = new QWidget();
    // style
    {
        QPalette pal = QPalette();
        pal.setColor(QPalette::Window, QColor(80, 180, 80));
        frpServiceExposeWidget->setAutoFillBackground(true);
        frpServiceExposeWidget->setPalette(pal);
    }
    return frpServiceExposeWidget;
}

QWidget* InputWidget::createToH264Mp4Widget()
{
    toH264Mp4Widget = new QWidget();
    // style
    {
        QPalette pal = QPalette();
        pal.setColor(QPalette::Window, QColor(180, 80, 80));
        toH264Mp4Widget->setAutoFillBackground(true);
        toH264Mp4Widget->setPalette(pal);
    }
    return toH264Mp4Widget;
}
