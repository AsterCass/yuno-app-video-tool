
#include "input_widget.h"

InputWidget::InputWidget(QWidget* parent) : QWidget{ parent }
{
    // todo QMessageBox::information
    // item
    launchModel = new QTabWidget(this);
    launchModel->addTab(createFrpFileExposeWidget(), "FRP文件穿透");
    launchModel->addTab(createFrpServiceExposeWidget(), "FRP服务穿透");
    launchModel->addTab(createToH264Mp4Widget(), "H264视频文件转换");
    functionTabList = { frpFileExposeWidget, frpServiceExposeWidget, toH264Mp4Widget };
    {
        QString tabStyle = QString("QTabBar::tab#%1{font-size: 15px;color: rgb(86, 86, 86);}").arg(launchModel->tabBar()->objectName());
        launchModel->setStyleSheet(tabStyle);
    }
    connect(launchModel, QTabWidget::tabBarClicked, this, changeTab);
    // layout
    indexLayout = new QVBoxLayout(this);
    indexLayout->setSpacing(0);
    indexLayout->setContentsMargins(0, 0, 0, 0);
    indexLayout->addWidget(launchModel);
    // init
    currentFunction = frpFileExposeWidget;
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
    currentFunction->lanuch();
}

void InputWidget::changeTab(int index)
{
    currentFunction = functionTabList.at(index);
}

CoreFunction* InputWidget::createFrpFileExposeWidget()
{
    frpFileExposeWidget = new FrpFileExposeWidget();
    return frpFileExposeWidget;
}

CoreFunction* InputWidget::createFrpServiceExposeWidget()
{
    frpServiceExposeWidget = new FrpServiceExposeWidget();
    return frpServiceExposeWidget;
}

CoreFunction* InputWidget::createToH264Mp4Widget()
{
    toH264Mp4Widget = new ToH264Mp4Widget();
    return toH264Mp4Widget;
}
