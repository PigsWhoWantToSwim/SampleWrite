#include "HomeWorkspaceThemePage.h"
#include "GlobalClass/TickButton.h"

HomeWorkspaceThemePage::HomeWorkspaceThemePage(QWidget *parent)
    : BasedHomeWorkspacePage{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

void HomeWorkspaceThemePage::initMemberVariable()
{
    // 容器布局
    m_containerLayout = new QGridLayout;
    // 弹簧
    m_spacer = new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Expanding);
    // 初始 列数
    m_buttonColumeCount = 1;

    // 默认主题
    ThemeTickButton *defualtTheme = new ThemeTickButton(m_widContainer);
    defualtTheme->setAboveColor(QColor(27, 178, 139));
    defualtTheme->setBelowColor(QColor(255,255,255));
    defualtTheme->setTextColor(QColor(27, 178, 139));
    defualtTheme->setText("默认", Qt::AlignLeft);
    defualtTheme->setQSSFilePath(":/theme/default.qss");
    defualtTheme->setRectRadius(20);
    defualtTheme->setBorderWidth(6);
    defualtTheme->setFixedSize(HOME_THEMETICKBUTTON_WIDTH,HOME_THEMETICKBUTTON_HEIGHT);
    m_listThemeTickButtons << defualtTheme;
    defualtTheme->setChecked(true);

    // 暗黑主题
    ThemeTickButton *darkTheme = new ThemeTickButton(m_widContainer);
    darkTheme->setAboveColor(QColor(64,65,66));
    darkTheme->setBelowColor(QColor(191,191,191));
    darkTheme->setTextColor(QColor(64,65,66));
    darkTheme->setText("暗黑", Qt::AlignLeft);
    darkTheme->setQSSFilePath(":/theme/dark.qss");
    darkTheme->setRectRadius(20);
    darkTheme->setBorderWidth(6);
    darkTheme->setFixedSize(HOME_THEMETICKBUTTON_WIDTH,HOME_THEMETICKBUTTON_HEIGHT);
    m_listThemeTickButtons << darkTheme;

    // 天蓝主题
    ThemeTickButton *blueTheme = new ThemeTickButton(m_widContainer);
    blueTheme->setAboveColor(QColor(9,132,227));
    blueTheme->setBelowColor(QColor(166,216,246));
    blueTheme->setTextColor(QColor(9,132,227));
    blueTheme->setText("天蓝", Qt::AlignLeft);
    blueTheme->setQSSFilePath(":/theme/sky blue.qss");
    blueTheme->setRectRadius(20);
    blueTheme->setBorderWidth(6);
    blueTheme->setFixedSize(HOME_THEMETICKBUTTON_WIDTH,HOME_THEMETICKBUTTON_HEIGHT);
    m_listThemeTickButtons << blueTheme;

    // 野蔷薇主题
    ThemeTickButton *briarTheme = new ThemeTickButton(m_widContainer);
    briarTheme->setButtonStyle(TickButtonStyle::rectImageButton);
    briarTheme->setImage(":/image/briar.png");
    briarTheme->setTextColor(QColor(244, 245, 244));// 246, 162, 94
    briarTheme->setText("野蔷薇", Qt::AlignLeft);
    briarTheme->setQSSFilePath(":/theme/briar.qss");
    briarTheme->setRectRadius(20);
    briarTheme->setBorderWidth(6);
    briarTheme->setFixedSize(HOME_THEMETICKBUTTON_WIDTH,HOME_THEMETICKBUTTON_HEIGHT);
    m_listThemeTickButtons << briarTheme;

}

void HomeWorkspaceThemePage::initLayout()
{
    // m_scrollArea->setAttribute(Qt::WA_NativeWindow, true);

    // 用于 放置按钮的容器
    // m_widContainer->setAttribute(Qt::WA_NativeWindow, true);

    // 容器布局
    m_containerLayout->setHorizontalSpacing(THEMECONTAINERLAYOUT_VERTICAL_SPACING);
    m_containerLayout->setVerticalSpacing(THEMECONTAINERLAYOUT_VERTICAL_SPACING);
    m_containerLayout->setContentsMargins(THEMECONTAINERLAYOUT_MARGINS_LEFT,
                                          THEMECONTAINERLAYOUT_MARGINS_TOP,
                                          THEMECONTAINERLAYOUT_MARGINS_RIGHT,
                                          THEMECONTAINERLAYOUT_MARGINS_BUTTOM);


    // 小说控件 初始布局 为一列，resizeEvent时 自动调整
    for(int index = 0; index < m_listThemeTickButtons.count(); index++)
    {
        // 计算 行号、列号，并将 小说控件添加到布局里
        m_containerLayout->addWidget(m_listThemeTickButtons[index], index / m_buttonColumeCount, index % m_buttonColumeCount, 1, 1);
    }

    // 添加弹簧
    m_containerLayout->addItem(m_spacer,m_listThemeTickButtons.count(),0,1,1);

    m_widContainer->setLayout(m_containerLayout);
    // m_scrollArea->setWidget(m_widContainer);

    m_scrollAreaLayout->addWidget(m_widContainer);

    m_scrollArea->setLayout(m_scrollAreaLayout);
    m_scrollArea->setWidget(m_widContainer);

    m_mainLayout->addWidget(m_scrollArea);
    this->setLayout(m_mainLayout);

}
// 初始化 绑定
void HomeWorkspaceThemePage::initConnect()
{
    // 绑定 主题按钮 点击
    foreach (ThemeTickButton *btn, m_listThemeTickButtons) {
        connect(btn, SIGNAL(themeButton_clicked()), this, SLOT(do_themeTickButton_clicked()));
    }
}

void HomeWorkspaceThemePage::updateContainerLayout()
{
    // 计算列数
    int colCount = floor(double(m_widContainer->rect().width()
                                - THEMECONTAINERLAYOUT_MARGINS_LEFT
                                - THEMECONTAINERLAYOUT_MARGINS_RIGHT
                                + THEMECONTAINERLAYOUT_HORIZONTIAL_SPACING)
                         /double(HOME_THEMETICKBUTTON_WIDTH
                                  + THEMECONTAINERLAYOUT_HORIZONTIAL_SPACING));

    // 列数 要在 1和按钮总数之间
    if (colCount > m_listThemeTickButtons.count())
        colCount = m_listThemeTickButtons.count();
    if (colCount < 1)
        colCount = 1;

    // 尺寸变化 引起 列数变化，更新布局
    if(m_buttonColumeCount != colCount)
    {
        m_buttonColumeCount = colCount; // 设置 列数

        // 遍历 历史版本按钮列表，从布局中 移除原布局
        if(m_listThemeTickButtons.count() != 0)
        {
            // 遍历 主题按钮列表
            foreach (ThemeTickButton *novelWidgettn, m_listThemeTickButtons)
                m_containerLayout->removeWidget(novelWidgettn); // 从布局中 移除按钮

            // 移除弹簧
            m_containerLayout->removeItem(m_spacer);

            int r,c; // 按钮行号、列号
            // 重新 布局
            for(int index = 0; index < m_listThemeTickButtons.count(); index++)
            {
                // 计算 行号、列号
                r = index / m_buttonColumeCount;
                c = index % m_buttonColumeCount;
                // 将按钮 重新添加 到布局里
                m_containerLayout->addWidget(m_listThemeTickButtons[index], r, c, 1, 1);
            }

            // 将按钮添加到最下面一行，此时 r 已到小说控件布局的最后一行
            r += 1; // 下一行
            m_containerLayout->addItem(m_spacer,r, 0, 1, 1);

            update(); // 更新
        }
    }
}

// 重写 显示事件，在初始时 设置容器宽度与滚动区相等
void HomeWorkspaceThemePage::showEvent(QShowEvent *event)
{
    // 调用 父类显示事件，初始化设置容器尺寸 与 滚动区尺寸相等
    BasedHomeWorkspacePage::showEvent(event);
    updateContainerLayout();
}
// 重写 尺寸改变事件，调整主题布局
void HomeWorkspaceThemePage::resizeEvent(QResizeEvent *event)
{
    BasedHomeWorkspacePage::resizeEvent(event);
    updateContainerLayout();
}

// 主题按钮点击 处理槽函数，实现互斥功能
void HomeWorkspaceThemePage::do_themeTickButton_clicked()
{
    // 获取 发送信号的按钮
    ThemeTickButton *clickedBtn = qobject_cast<ThemeTickButton *>(sender());

    // 全部按钮 取消勾选
    foreach (ThemeTickButton *btn, m_listThemeTickButtons) {
        btn->setChecked(false);
    }

    // 重新设置被勾选的按钮
    clickedBtn->setChecked(true);

}
