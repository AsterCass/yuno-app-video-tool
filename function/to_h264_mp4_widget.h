
#ifndef TOH264MP4WIDGET_H
#define TOH264MP4WIDGET_H

#include "function/core_function.h"
#include <QWidget>

class ToH264Mp4Widget : public CoreFunction
{
    Q_OBJECT
public:
    explicit ToH264Mp4Widget(QWidget* parent = nullptr);
    ~ToH264Mp4Widget() override;

public:
    void lanuch();

signals:
};

#endif  // TOH264MP4WIDGET_H
