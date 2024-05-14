#include "BasedHomeWorkspacePage.h"

BasedHomeWorkspacePage::BasedHomeWorkspacePage(QWidget *parent)
    : QWidget{parent}
{
    // 主布局
    m_mainLayout = new QHBoxLayout;
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0,0,0,0);

    // 滚动区
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setProperty("HomeWorkspaceWidgets", "ScrollArea");
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setAlignment(Qt::AlignCenter);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setWidgetResizable(true);

    // 滚动区布局
    m_scrollAreaLayout = new QVBoxLayout;
    m_scrollAreaLayout->setSpacing(0);
    m_scrollAreaLayout->setContentsMargins(0, 0, 0, 0);

    // 用于 放置按钮的容器
    m_widContainer = new QWidget(m_scrollArea);
    // 用于 放置按钮的容器
    m_widContainer->setProperty("HomeWorkspaceWidgets", "Container");

    // m_scrollAreaLayout->addWidget(m_widContainer);

    // m_scrollArea->setLayout(m_scrollAreaLayout);
    // m_scrollArea->setWidget(m_widContainer);

    // m_mainLayout->addWidget(m_scrollArea);
    // this->setLayout(m_mainLayout);
}

void BasedHomeWorkspacePage::updateContainerWidth()
{
    m_widContainer->resize(m_scrollArea->width(), m_widContainer->height());

}

// 重写 显示事件，在初始时 设置容器宽度与滚动区相等
void BasedHomeWorkspacePage::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updateContainerWidth();
}
