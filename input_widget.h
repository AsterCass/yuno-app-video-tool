
#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include "function/core_function.h"
#include "function/frp_file_expose_widget.h"
#include "function/frp_service_expose_widget.h"
#include "function/to_h264_mp4_widget.h"
#include <QPushButton>
#include <QTabBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <vector>

class InputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InputWidget(QWidget* parent = nullptr);
    ~InputWidget() override;

public:
    void lanuch();

private:
    // index
    QVBoxLayout* indexLayout;
    QTabWidget*  launchModel;
    // current function
    CoreFunction*              currentFunction;
    std::vector<CoreFunction*> functionTabList;
    // frp file
    CoreFunction* frpFileExposeWidget;
    // frp service
    CoreFunction* frpServiceExposeWidget;
    // H264 change
    CoreFunction* toH264Mp4Widget;

private:
    CoreFunction* createFrpFileExposeWidget();
    CoreFunction* createFrpServiceExposeWidget();
    CoreFunction* createToH264Mp4Widget();

    void changeTab(int index);

signals:
};

#endif  // INPUTWIDGET_H
