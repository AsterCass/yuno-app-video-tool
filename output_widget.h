
#ifndef OUTPUTWIDGET_H
#define OUTPUTWIDGET_H

#include <QTextBrowser>
#include <QVBoxLayout>
#include <QWidget>

class OutputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OutputWidget(QWidget* parent = nullptr);
    ~OutputWidget() override;

public:
    static void clearConsole();
    static void printMessage(QString msg);
    static void printMessageError(QString msg);
    static void printMessageWarn(QString msg);

private:
    QVBoxLayout*         indexLayout;
    static QTextBrowser* outputView;

signals:
};

#endif  // OUTPUTWIDGET_H
