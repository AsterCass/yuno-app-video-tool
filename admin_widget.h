
#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include "constants.h"
#include "input_widget.h"
#include "output_widget.h"
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QMenu>
#include <QPushButton>
#include <QSystemTrayIcon>

class AdminWidget : public QWidget
{
    Q_OBJECT
public:
    AdminWidget(QWidget* parent = nullptr);
    ~AdminWidget() override;

private:
    void initSysTrayIcon();
    void initAppMenu();

    QWidget* createIndexMainFrame();
    QWidget* createIndexBottomFrame();
    QWidget* createOutputWidget();
    QWidget* createInputWidget();

private:
    // index
    QVBoxLayout* m_indexLayout;
    // index bottom
    QWidget*     m_indexBottom;
    QHBoxLayout* m_indexBottomLayout;
    QSpacerItem* m_indexBottomSpacerLeft;
    QSpacerItem* m_indexBottomSpacerRight;
    QPushButton* m_startBtn;
    // index main
    QWidget*     m_indexMain;
    QHBoxLayout* m_indexMainLayout;
    //  trag
    QSystemTrayIcon* m_sysTrayIcon;
    QMenu*           m_menu;
    QAction*         m_showMainAction;
    QAction*         m_exitAppAction;
    // extend
    OutputWidget* m_outputWidget;
    InputWidget*  m_inputWidget;

private:
    void startLinkEvent();
    void systemIconActivatedEvent(QSystemTrayIcon::ActivationReason reason);
    void showAppMainEvent();
    void exitAppEvent();

protected:
    void closeEvent(QCloseEvent*) override;
};

#endif  // ADMINWIDGET_H
