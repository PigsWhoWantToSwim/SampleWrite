#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPainterPath>
#include "AppEnumClass.h"

class SwitchButton : public QWidget
{
    Q_OBJECT
    // 颜色属性
    Q_PROPERTY(QColor SwitchButtonGrooveColorOff READ getGrooveColorOff WRITE setGrooveColorOff)
    Q_PROPERTY(QColor SwitchButtonGrooveColorOn READ getGrooveColorOn WRITE setGrooveColorOn)

    Q_PROPERTY(QColor SwitchButtonSliderColorOff READ getSliderColorOff WRITE setSliderColorOff)
    Q_PROPERTY(QColor SwitchButtonSliderColorOn READ getSliderColorOn WRITE setSliderColorOn)

    Q_PROPERTY(QColor SwitchButtonTextColorOff READ getTextColorOff WRITE setTextColorOff)
    Q_PROPERTY(QColor SwitchButtonTextColorOn READ getTextColorOn WRITE setTextColorOn)
public:
    // 构造函数
    explicit SwitchButton(QWidget *parent = nullptr);

    //设置 和 获取 滑槽颜色
    void setGrooveColorOff(QColor grooveColorOff);
    void setGrooveColorOn(QColor grooveColorOn);
    QColor getGrooveColorOff() const;
    QColor getGrooveColorOn() const;

    //设置 和 获取 滑块颜色
    void setSliderColorOff(QColor sliderColorOff);
    void setSliderColorOn(QColor sliderColorOn);
    QColor getSliderColorOff()const;
    QColor getSliderColorOn()const;

    //设置 和 获取 文本颜色
    void setTextColorOff(QColor textColorOff);
    void setTextColorOn(QColor textColorOn);
    QColor getTextColorOff() const;
    QColor getTextColorOn() const;

    //设置 和 获取 是否选中
    void setChecked(bool checked);
    bool getChecked() const;

    //设置 和 获取 风格样式
    void setButtonStyle(SwitchButtonStyle buttonStyle);
    SwitchButtonStyle getButtonStyle() const;

    //设置 和 获取 槽和滑块的间隔
    void setSpace(int space);
    int getSpace() const;

    //设置 和 获取 圆角半径
    void setRectRadius(int rectRadius);
    int getRectRadius() const;

    //设置 和 获取 文本
    void setText(QString textOff, QString textOn);
    void setTextOff(QString textOff);
    void setTextOn(QString textOn);
    QString getTextOff() const;
    QString getTextOn() const;

    //设置 和 获取 背景图片
    void setImagePath(QString imageOffPath, QString imageOnPath);
    void setImageOffPath(QString imageOffPath);
    void setImageOnPath(QString imageOnPath);
    QString getImageOffPath() const;
    QString getImageOnPath() const;


    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;


private:
    // 初始化 成员函数
    void initMemberVariable();
    // 初始化 布局
    void initLayout();
    // 初始化 绑定事件
    void initConnect();

    // 绘制
    void drawGroove(QPainter *painter); // 绘制 滑槽
    void drawSlider(QPainter *painter); // 绘制 滑块
    void drawText(QPainter *painter); // 绘制 文字
    void drawImage(QPainter *painter); // 绘制 图片

    // 缩放图片填充矩形
    QPixmap scalePixmapFillRect(QPixmap pix, QRect rect);

private slots:
    // 定时器到时 更新 滑块的起点和终点坐标值
    void do_slider_update();

signals:
    // 按钮状态 改变 信号
    void checkedChanged(bool checked);

private:
    // 成员控件
    QTimer *m_timer;              //定时器

    // 成员变量
    bool m_checked;               //是否选中
    SwitchButtonStyle m_buttonStyle;    //开关按钮样式

    QColor m_grooveColorOff;      //关闭时滑槽背景颜色
    QColor m_grooveColorOn;       //打开时滑槽背景颜色

    QColor m_sliderColorOff;      //关闭时滑块颜色
    QColor m_sliderColorOn;       //打开时滑块颜色

    QColor m_textColorOff;        //关闭时文本颜色
    QColor m_textColorOn;         //打开时文本颜色

    QString m_textOff;            //关闭时显示的文字
    QString m_textOn;             //打开时显示的文字

    QString m_imageOffPath;           //关闭时显示的图片
    QString m_imageOnPath;            //打开时显示的图片

    int m_space;                  //滑块离槽的间隔
    int m_rectRadius;             //圆角半径

    int m_step;                   //每次移动的步长
    int m_startX;                 //滑块开始X轴坐标
    int m_endX;                   //滑块结束X轴坐标

    qreal m_pixelRatio; // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

};

#endif // SWITCHBUTTON_H
