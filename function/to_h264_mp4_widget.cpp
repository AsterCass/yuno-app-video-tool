
#include "to_h264_mp4_widget.h"

ToH264Mp4Widget::ToH264Mp4Widget(QWidget* parent) : CoreFunction{ parent }
{
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(80, 180, 80));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

ToH264Mp4Widget::~ToH264Mp4Widget() {}

void ToH264Mp4Widget::lanuch()
{
    OutputWidget::printMessageWarn("视频转换开发中...");
}
