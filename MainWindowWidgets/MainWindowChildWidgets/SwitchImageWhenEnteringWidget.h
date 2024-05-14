#ifndef SWITCHIMAGEWHENENTERINGWIDGET_H
#define SWITCHIMAGEWHENENTERINGWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include "GlobalClass/AppEnumClass.h"

class SwitchImageWhenEnteringWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchImageWhenEnteringWidget(QWidget *parent = nullptr);

    // 设置 和 获取 是否  进入
    void setEnter(bool isEnter);
    bool getEnter();

    // 设置 和 获取 正常显示的背景图片
    void setNormalBackgroundPath(QString pixmapPath);
    QString getNormalBackgroundPath();
    void setNormalBackgroundPixmap(QPixmap pixmap);
    QPixmap getNormalBackgroundPixmap();

    // 设置 和 获取 鼠标进入时显示的背景图片
    void setEnterBackgroundPath(QString pixmapPath);
    QString getEnterBackgroundPath();
    void setEnterBackgroundPixmap(QPixmap pixmap);
    QPixmap getEnterBackgroundPixmap();

    // 设置 和 获取 圆角半径
    void setRectRadius(int rectRadius);
    int getRectRadius();


    // QWidget interface
protected:
    // 重写绘画事件 绘制背景
    virtual void paintEvent(QPaintEvent *event) override;

private:
    // 绘制背景
    void drawNormalBackground(QPainter *painter);
    void drawEnterBackground(QPainter *painter);

    // 缩放图片填充矩形
    QPixmap scalePixmapFillRect(QString pixmapPath, QSize pixmapSize);
    QPixmap scalePixmapFillRect(QPixmap pixmap, QSize pixmapSize);

    // 获取 裁剪圆角矩形的路径
    QPainterPath getClipRectRadiusPath();

signals:

private:
    // 是否进入标志
    bool m_isEnter;

    // 正常显示的背景图片
    QString m_normalBgPath;
    QPixmap m_normalBgPix;
    // 鼠标进入时显示的背景图片
    QString m_enterBgPath;
    QPixmap m_enterBgPix;

    int m_rectRadius; // 封面圆角半径

};

#endif // SWITCHIMAGEWHENENTERINGWIDGET_H
