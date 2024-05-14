#ifndef TICKBUTTON_H
#define TICKBUTTON_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QFontMetrics>
#include <QGraphicsDropShadowEffect>
#include "AppEnumClass.h"

class TickButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor TickButtonBorderColor READ getBorderColor WRITE setBorderColor)
public:
    explicit TickButton(QWidget *parent = nullptr);
    ~TickButton();

    // 设置 获取 是否选中
    void setChecked(bool checked);
    bool getChecked() const;

    // 设置 获取 风格样式
    void setButtonStyle(TickButtonStyle buttonStyle);
    TickButtonStyle getButtonStyle() const;

    // 设置 获取 边框宽度
    void setBorderWidth(int borderWidth);
    int getBorderWidth() const;

    // 设置 获取 矩形按钮 圆角半径
    void setRectRadius(int rectRadius);
    int getRectRadius() const;

    // 设置 获取 背景图片路径
    void setImage(QString imagePath);
    QString getImagePath() const;

    // 设置 和 获取 按钮上下颜色
    void setAboveColor(const QColor &aboveColor);
    QColor getAboveColor() const;
    void setBelowColor(const QColor &belowColor);
    QColor getBelowColor() const;
    void setBorderColor(const QColor &borderColor);
    QColor getBorderColor() const;

protected:
    // 绘制
    void drawBackground(QPainter *painter); // 绘制 按钮
    void drawImage(QPainter *painter); // 绘制 图片
    void drawTick(QPainter *painter);  // 绘制 勾号
    void drawBorder(QPainter *painter); // 绘制 边框

    // 缩放图片填充矩形
    QPixmap scalePixmapFillRect(QPixmap pix, QRect rect);

    // QWidget interface
protected:
    // 重写 事件
    virtual void mousePressEvent(QMouseEvent *event) override; // 鼠标按下事件
    virtual void paintEvent(QPaintEvent *event) override; // 绘画事件


signals:
    // 按钮状态 改变 信号
    void themeButton_clicked();
    void themeButton_clicked(bool checked);

protected:
    bool m_checked;            // 是否选中
    TickButtonStyle m_buttonStyle; // 开关按钮样式

    QColor m_aboveColor; // 按钮上半部分颜色
    QColor m_belowColor; // 按钮下半部分颜色
    QColor m_borderColor; // 边框颜色

    QString m_imagePath; // 按钮背景图片 路径

    int m_borderWidth;     // 滑块离槽的间隔 默认为 2
    int m_rectRadius; // 圆角角度

    qreal m_pixelRatio; // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真
};

#endif // TICKBUTTON_H
