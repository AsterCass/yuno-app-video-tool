
#include "admin_widget.h"

AdminWidget::AdminWidget(QWidget* parent) : QWidget(parent)
{
    // layout
    m_indexLayout = new QVBoxLayout;
    m_indexLayout->addWidget(createIndexMainFrame());
    m_indexLayout->addWidget(createIndexBottomFrame());
    m_indexLayout->setContentsMargins(0, 0, 0, 0);
    m_indexLayout->setStretch(0, 8);
    m_indexLayout->setStretch(1, 1);
    setLayout(m_indexLayout);
    // extend
    setWindowIcon(QIcon(K_ADMIN_ICON_PATH));
    setWindowTitle(K_ADMIN_APP_NAME);
    initSysTrayIcon();
    // size
    resize(960, 540);
}

AdminWidget::~AdminWidget()
{
    // extend
    delete m_inputWidget;
    delete m_outputWidget;
    // trag
    delete m_showMainAction;
    delete m_exitAppAction;
    delete m_menu;
    delete m_sysTrayIcon;
    // index bottom
    delete m_startBtn;
    delete m_indexBottomLayout;
    delete m_indexBottom;
    // index main
    delete m_indexMainLayout;
    delete m_indexMain;
    // main
    delete m_indexLayout;
}

QWidget* AdminWidget::createIndexMainFrame()
{
    m_indexMain = new QWidget();
    // style
    {
        QPalette pal = QPalette();
        pal.setColor(QPalette::Window, Qt::white);
        m_indexMain->setAutoFillBackground(true);
        m_indexMain->setPalette(pal);
    }
    // item
    m_outputWidget = new OutputWidget(this);
    m_inputWidget  = new InputWidget(this);
    // layout
    m_indexMainLayout = new QHBoxLayout(m_indexMain);
    m_indexMainLayout->addWidget(m_outputWidget);
    m_indexMainLayout->addWidget(m_inputWidget);
    m_indexMainLayout->setStretch(0, 1);
    m_indexMainLayout->setStretch(1, 1);
    // ret
    return m_indexMain;
}

QWidget* AdminWidget::createIndexBottomFrame()
{
    m_indexBottom = new QWidget();
    // style
    {
        QPalette pal = QPalette();
        pal.setColor(QPalette::Window, Qt::white);
        m_indexBottom->setAutoFillBackground(true);
        m_indexBottom->setPalette(pal);
    }
    // item
    m_indexBottomSpacerLeft  = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_indexBottomSpacerRight = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_startBtn               = new QPushButton(tr("启动"));
    {
        // font
        m_startBtn->setStyleSheet("QPushButton {font-size: 15px;color: rgb(86, 86, 86);}");
        // size
        m_startBtn->setMinimumSize(QSize(200, 40));
        connect(m_startBtn, &QPushButton::clicked, this, &AdminWidget::startLinkEvent);
    }
    // layout
    m_indexBottomLayout = new QHBoxLayout(m_indexBottom);
    m_indexBottomLayout->addItem(m_indexBottomSpacerLeft);
    m_indexBottomLayout->addWidget(m_startBtn);
    m_indexBottomLayout->addItem(m_indexBottomSpacerRight);
    // ret
    return m_indexBottom;
}

void AdminWidget::initSysTrayIcon()
{
    m_sysTrayIcon = new QSystemTrayIcon(this);
    // base
    {
        m_sysTrayIcon->setIcon(QIcon(K_ADMIN_ICON_PATH));
        m_sysTrayIcon->setVisible(true);
        m_sysTrayIcon->showMessage(K_ADMIN_APP_NAME, "工具箱已启动");
        m_sysTrayIcon->setToolTip(K_ADMIN_APP_NAME);
    }
    // left click
    connect(m_sysTrayIcon, &QSystemTrayIcon::activated, this, &AdminWidget::systemIconActivatedEvent);
    // right click
    initAppMenu();
    // show
    m_sysTrayIcon->show();
}

// 创建托盘菜单
void AdminWidget::initAppMenu()
{
    // action
    m_showMainAction = new QAction("主界面");
    connect(m_showMainAction, &QAction::triggered, this, &AdminWidget::showAppMainEvent);
    m_exitAppAction = new QAction("退出");
    connect(m_exitAppAction, &QAction::triggered, this, &AdminWidget::exitAppEvent);
    // menu
    m_menu = new QMenu();
    m_menu->addAction(m_showMainAction);
    m_menu->addSeparator();
    m_menu->addAction(m_exitAppAction);
    m_sysTrayIcon->setContextMenu(m_menu);
}

void AdminWidget::systemIconActivatedEvent(QSystemTrayIcon::ActivationReason reason)
{
    if (QSystemTrayIcon::Trigger == reason || QSystemTrayIcon::DoubleClick == reason)
    {
        this->show();
    }
}

void AdminWidget::showAppMainEvent()
{
    this->show();
}

void AdminWidget::exitAppEvent()
{
    QApplication::exit();
}

void AdminWidget::startLinkEvent()
{
    m_inputWidget->lanuch();
}

void AdminWidget::closeEvent(QCloseEvent*)
{
    this->hide();
}
