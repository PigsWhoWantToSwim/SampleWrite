#ifndef MASKFRAME_H
#define MASKFRAME_H

#include <QWidget>
#include <QApplication>

class MaskFrame : public QWidget
{
    Q_OBJECT
public:
    explicit MaskFrame(QWidget *parent = nullptr);

    //注册要显示在遮罩中的窗口
    void setUpperWidget(QWidget *upperWidget);

    //设置 被遮罩的控件
    void setMaskedWidget(QWidget *maskedWidget);

    //设置遮罩颜色、透明度
    void setMaskColor(const QColor &color,float opacity);


    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    // QWidget interface
protected:
    virtual void showEvent(QShowEvent *event) override;

private:
    void initWidgets(); // 初始化控件

signals:

private:
    QWidget *m_upperWidget;
    QWidget *m_maskedWidget;
};

#endif // MASKFRAME_H
