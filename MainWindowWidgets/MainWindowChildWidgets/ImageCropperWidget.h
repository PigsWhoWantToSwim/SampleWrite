#ifndef IMAGECROPPERWIDGET_H
#define IMAGECROPPERWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QPixmap>
#include <QPen>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QDebug>
#include <QBitmap>
#include <QMessageBox>
#include "GlobalClass/AppEnumClass.h"

class ImageCropperWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor ImageCropperWidgetBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor ImageCropBoxBorderColor READ getBorderColor WRITE setBorderColor)
    Q_PROPERTY(double ImageCropBoxBorderWidth READ getBorderWidth WRITE setBorderWidth)
    Q_PROPERTY(QColor ImageCropBoxControlPointColor READ getControlPointColor WRITE setControlPointColor)
    Q_PROPERTY(double ImageCropBoxControlPointSize READ getControlPointSize WRITE setControlPointSize)
    Q_PROPERTY(double ImageCropperWidgetOpacity READ getOpacity WRITE setOpacity)
public:
    explicit ImageCropperWidget(QWidget *parent = nullptr);
    // 构造函数，指定宽高
    explicit ImageCropperWidget(int width, int height,QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor &backgroundColor);
    QColor getBackgroundColor() const;

    // 设置 和 获取 裁剪框颜色
    void setBorderColor(const QColor &borderColor);
    QColor getBorderColor() const;

    // 设置 和 获取 裁剪框宽度
    void setBorderWidth(const double borderWidth);
    double getBorderWidth() const;

    // 设置 和 获取 控制点颜色
    void setControlPointColor(const QColor &controlPointColor);
    QColor getControlPointColor() const;

    // 设置 和 获取 控制点尺寸
    void setControlPointSize(const double controlPointSize);
    double getControlPointSize() const;

    // 设置 和 获取 透明度
    void setOpacity(const double opacity);
    double getOpacity() const;

    // 设置要裁剪的图片
    void setPixmapToCrop(QPixmap croppedPixmap);
    // 获取截取后的图片
    QPixmap getCroppedPixmap();

    // QWidget interface
protected:
    // 重写 事件
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

private:
    // 初始化
    void initMemberVariable();

    // 重新设置截取框位置
    void resetCropBoxPosition();
    // 获取一个点在截取框里的坐标
    ImageCropBoxPosition getOrientationInCropBoxRect(const QPoint &point);
    // 两点是否相近
    bool areTwoPointsClose(const QPoint &point1, const QPoint &point2, int effectiveDistance);
    // 改变光标形状
    void changeCursor();

    // 绘制空白区
    void drawBlankBackground(QPainter *painter);
    // 绘制背景图片
    void drawPixmapBackground(QPainter *painter);
    // 绘制矩形截取框对应的遮罩区
    void drawMaskArea(QPainter *painter);
    // 绘制 裁剪框
    void  drawCropBox(QPainter *painter);
    // 绘制截取框的控制点
    void drawAllControlPoints(QPainter *painter);
    // 绘制一个填充矩形（控制点），传入图片控制点的坐标，边长
    void drawFillSquare(QPainter *painter, QPoint centerPos, int controlPointSize);

signals:

private:
    // 成员变量
    // 背景颜色
    QColor m_backgroundColor;
    // 透明度
    double m_opacity;

    // 边框画笔

    int m_borderPenWidth;
    QColor m_borderPenColor;

    // 控制点
    int m_controlPointSize;
    QColor m_controlPointColor;

    // 截取框最小尺寸
    int m_cropBoxMinWidth;
    int m_cropBoxMiHeight;

    // 显示的图片(缩放后的整张大图)所在矩形
    QRect m_recDisplayedPixmap;
    // 截取后的图片（从原始图片截取下，未缩放，不是显示的尺寸）
    QRect m_recCroppedIPixmap;
    // 截取框所在矩形
    QRect m_recCropBox;
    // 缩放比例
    double m_scaledRate;

    // 原始图片和临时图片（显示在控件上的图片）
    QPixmap m_pixReal;
    QPixmap m_pixShow;

    // 鼠标左键是否按下，默认未按下
    bool m_isLeftBtnPressed;
    // 光标位置是否计算，默认未计算
    bool m_isCursorPosCalculated;
    // 光标在截取框的位置，默认在截取框外部
    ImageCropBoxPosition m_cursorPosInCropBox;
    // 上一次坐标
    QPoint m_lastPos;
    // 当前坐标
    QPoint m_currPos;

};

#endif // IMAGECROPPERWIDGET_H
