
#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include "frp_file_expose_widget.h"
#include <QPushButton>
#include <QTabBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

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
    // frp file
    FrpFileExposeWidget* frpFileExposeWidget;
    // frp service
    QWidget* frpServiceExposeWidget;
    // H264 change
    QWidget* toH264Mp4Widget;

private:
    FrpFileExposeWidget* createFrpFileExposeWidget();
    QWidget*             createFrpServiceExposeWidget();
    QWidget*             createToH264Mp4Widget();

signals:
};

#endif  // INPUTWIDGET_H
