#ifndef SIDEBARLAYOUTPAGE_H
#define SIDEBARLAYOUTPAGE_H

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QButtonGroup>
#include "BasedSiderBarPage.h"

class SideBarLayoutPage : public BasedSiderBarPage
{
    Q_OBJECT
public:
    explicit SideBarLayoutPage(QWidget *parent = nullptr);

    // 设置控件的值
    void setSlidersValues(int firstLineIndent, int wordSpacing, int lineSpacing, int leftRightMargin, int bottomMargin);

    // BasedSiderBarPage interface
protected:
    virtual void initMemberVariable() override;
    virtual void initLayout() override;
    virtual void initConnect() override;

private slots:
    // 关联 滑动条和输入框
    void on_sliderSpinBoxValues_changed(int value);

signals:
    // 标题对齐 方式 改变
    void titleAlign_changed(Qt::Alignment flag);

    // 间距边距 数值改变
    void layoutValuesChanged(int firstLineIndent, int wordSpacing,
                             int lineSpacing, int leftRightMargin, int bottomMargin);


private:
    // 标题对齐 互斥按钮组
    QButtonGroup *m_btngTitleAlignment;
    QPushButton *m_btnTitleAlignLeft;
    QPushButton *m_btnTitleAlignCenter;
    QPushButton *m_btnTitleAlignRight;

    // 首行缩进
    QSlider *m_sldFirstLineIndent;
    QSpinBox *m_spbFirstLineIndent;

    // 字间距
    QSlider *m_sldWordSpacing;
    QSpinBox *m_spbWordSpacing;

    // 行间距
    QSlider *m_sldLineSpacing;
    QSpinBox *m_spbLineSpacing;

    // 左右边距，对标题和内容生效
    QSlider *m_sldLeftRightMargin;
    QSpinBox *m_spbLeftRightMargin;

    // 底部边距
    QSlider *m_sldBottomMargin;
    QSpinBox *m_spbBottomMargin;

};

#endif // SIDEBARLAYOUTPAGE_H
