#include "HomeWorkspaceSettingsPage.h"

HomeWorkspaceSettingsPage::HomeWorkspaceSettingsPage(QWidget *parent)
    : BasedHomeWorkspacePage{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}


// 初始化 成员变量
void HomeWorkspaceSettingsPage::initMemberVariable()
{
    // m_themeBtnAreaColumeCount = 1;

    m_frmThemeContainer = new QFrame(m_widContainer);

    // 主题页面 网格布局
    m_themeBtnAreaLayout = new FlowLayout;

    // 主题按钮
    // 默认主题
    ThemeTickButton *defualtTheme = new ThemeTickButton(m_frmThemeContainer);
    defualtTheme->setAboveColor(QColor(27, 178, 139));
    defualtTheme->setBelowColor(QColor(255,255,255));
    defualtTheme->setTextColor(QColor(27, 178, 139));
    defualtTheme->setText("默认", Qt::AlignLeft);
    defualtTheme->setQSSFilePath(":/theme/default.qss");
    defualtTheme->setRectRadius(20);
    defualtTheme->setBorderWidth(6);
    defualtTheme->setFixedSize(HOME_THEMETICKBUTTON_WIDTH,HOME_THEMETICKBUTTON_HEIGHT);
    m_listThemeTickButtons << defualtTheme;
    // defualtTheme->setChecked(true);

    // 暗黑主题
    ThemeTickButton *darkTheme = new ThemeTickButton(m_frmThemeContainer);
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
    ThemeTickButton *blueTheme = new ThemeTickButton(m_frmThemeContainer);
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
    ThemeTickButton *briarTheme = new ThemeTickButton(m_frmThemeContainer);
    briarTheme->setButtonStyle(TickButtonStyle::rectImageButton);
    briarTheme->setImage(":/image/briar.png");
    briarTheme->setTextColor(QColor(244, 245, 244));// 246, 162, 94
    briarTheme->setText("野蔷薇", Qt::AlignLeft);
    briarTheme->setQSSFilePath(":/theme/briar.qss");
    briarTheme->setRectRadius(20);
    briarTheme->setBorderWidth(6);
    briarTheme->setFixedSize(HOME_THEMETICKBUTTON_WIDTH,HOME_THEMETICKBUTTON_HEIGHT);
    m_listThemeTickButtons << briarTheme;

    // m_horizontalSpacer = new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed);

}
// 初始化 布局
void HomeWorkspaceSettingsPage::initLayout()
{
    // 主布局，垂直布局，上方为 主题区，下方为 （没想好）
    QVBoxLayout *containerainLayout = new QVBoxLayout;
    containerainLayout->setSpacing(30);
    containerainLayout->setContentsMargins(HOMEWORKSPACESETTINGSPAGE_CONTAINERLAYOUT_MARGINSLEFT,
                                           HOMEWORKSPACESETTINGSPAGE_CONTAINERLAYOUT_MARGINSTOP,
                                           HOMEWORKSPACESETTINGSPAGE_CONTAINERLAYOUT_MARGINSRIGHT,
                                           HOMEWORKSPACESETTINGSPAGE_CONTAINERLAYOUT_MARGINSNBOTTOM);

    // 主题区，有一个主题页面容器
    m_frmThemeContainer->setObjectName("HomeWorkspaceSettingsPage_frmThemeContainer");
    // m_frmThemeContainer->setProperty("WidgetStyleClass", "HomeWorkspaceSettingsPage_ChildContainer");
    containerainLayout->addWidget(m_frmThemeContainer);

    // 主题页面阴影效果
    QGraphicsDropShadowEffect *themeShadowEffect = new QGraphicsDropShadowEffect(m_widContainer); // 创建 阴影效果
    themeShadowEffect->setOffset(0, 0); // 阴影效果 不偏移
    themeShadowEffect->setColor(Qt::black); // 设置阴影效果 的颜色QColor(QStringLiteral("black"))
    themeShadowEffect->setBlurRadius(15); // 设置 模糊半径
    m_frmThemeContainer->setGraphicsEffect(themeShadowEffect); // 为 容器控件设置阴影效果

    // 主题区布局，垂直布局，上方为 标题标签，下方为 网格布局
    QVBoxLayout *themeContainerLayout = new QVBoxLayout;
    themeContainerLayout->setSpacing(20);
    themeContainerLayout->setContentsMargins(HOMEWORKSPACESETTINGSPAGE_THEMELAYOUT_MARGINSLEFT,
                                             HOMEWORKSPACESETTINGSPAGE_THEMELAYOUT_MARGINSTOP,
                                             HOMEWORKSPACESETTINGSPAGE_THEMELAYOUT_MARGINSRIGHT,
                                             HOMEWORKSPACESETTINGSPAGE_THEMEBUTTON_VERTICALSPACING);
    m_frmThemeContainer->setLayout(themeContainerLayout);
    // 主题页面 标题标签
    QLabel *themeTitle = new QLabel(m_frmThemeContainer);
    themeTitle->setProperty("WidgetStyleClass", "HomeWorkspaceSettingsPage_TitleLabel");
    themeTitle->setText("主题");
    themeTitle->setFixedHeight(30);
    themeContainerLayout->addWidget(themeTitle);

    // 主题按钮区 布局
    m_themeBtnAreaLayout->setSpacing(HOMEWORKSPACESETTINGSPAGE_THEMEBUTTON_HORIZONTALSPACING);
    m_themeBtnAreaLayout->setContentsMargins(0, 0, 0, 0);

    // 添加主题按钮
    // 读取 配置文件中的 当前主题
    settings->beginGroup("APP_THEME");
    QString currentTheme = settings->value("CURRENT_THEME",":/theme/default.qss").toString();
    settings->endGroup();

    // 遍历 每一个主题按钮
    foreach (ThemeTickButton* themeBtn, m_listThemeTickButtons) {
        if(themeBtn->getQSSFilePath() == currentTheme) // 是当前 主题
        {
            themeBtn->setChecked(true); // 设置 勾选
        }
        m_themeBtnAreaLayout->addWidget(themeBtn); // 添加到 布局
    }

    // 添加 弹簧 到最右边一列
    // m_themeBtnAreaLayout->addItem(m_horizontalSpacer, 0, m_themeBtnAreaColumeCount, (m_listThemeTickButtons.count() / m_themeBtnAreaColumeCount)+1, 1);

    themeContainerLayout->addLayout(m_themeBtnAreaLayout);

    containerainLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Expanding));

    m_widContainer->setLayout(containerainLayout);
    m_scrollAreaLayout->addWidget(m_widContainer);

    m_scrollArea->setLayout(m_scrollAreaLayout);
    m_scrollArea->setWidget(m_widContainer);


    m_mainLayout->addWidget(m_scrollArea);
    this->setLayout(m_mainLayout);

}
// 初始化 绑定
void HomeWorkspaceSettingsPage::initConnect()
{
    // 绑定 主题按钮 点击
    foreach (ThemeTickButton *btn, m_listThemeTickButtons) {
        connect(btn, SIGNAL(themeButton_clicked()), this, SLOT(do_themeTickButton_clicked()));
    }
}

// 更新 主题页面列数
// void HomeWorkspaceSettingsPage::updateThemeContainerLayout()
// {
//     // m_frmThemeContainer->resize(m_widContainer->width() - 40, m_frmThemeContainer->height());

//     // 计算列数
//     int colCount = floor(double(m_frmThemeContainer->width()
//                                 - HOMEWORKSPACESETTINGSPAGE_THEMELAYOUT_MARGINSLEFT
//                                 - HOMEWORKSPACESETTINGSPAGE_THEMELAYOUT_MARGINSRIGHT
//                                 + HOMEWORKSPACESETTINGSPAGE_THEMEBUTTON_HORIZONTALSPACING)
//                          /double(HOME_THEMETICKBUTTON_WIDTH + HOMEWORKSPACESETTINGSPAGE_THEMEBUTTON_HORIZONTALSPACING));

//     // 列数 要在 1和按钮总数之间
//     if (colCount > m_listThemeTickButtons.count())
//         colCount = m_listThemeTickButtons.count();
//     if (colCount < 1)
//         colCount = 1;


//     // 尺寸变化 引起 列数变化，更新布局
//     if(m_themeBtnAreaColumeCount != colCount)
//     {
//         m_themeBtnAreaColumeCount = colCount; // 设置 列数

//         // 遍历 历史版本按钮列表，从布局中 移除原布局
//         if(m_listThemeTickButtons.count() != 0)
//         {
//             // 遍历 主题按钮列表
//             foreach (ThemeTickButton *novelWidgettn, m_listThemeTickButtons)
//                 m_themeBtnAreaLayout->removeWidget(novelWidgettn); // 从布局中 移除按钮

//             // 移除弹簧
//             m_themeBtnAreaLayout->removeItem(m_horizontalSpacer);

//             int r,c; // 按钮行号、列号
//             // 重新 布局
//             for(int index = 0; index < m_listThemeTickButtons.count(); index++)
//             {
//                 // 计算 行号、列号
//                 r = index / m_themeBtnAreaColumeCount;
//                 c = index % m_themeBtnAreaColumeCount;
//                 // 将按钮 重新添加 到布局里
//                 m_themeBtnAreaLayout->addWidget(m_listThemeTickButtons[index], r, c, 1, 1);
//             }

//             // 将按钮添加到最左边一列
//             m_themeBtnAreaLayout->addItem(m_horizontalSpacer, 0, m_themeBtnAreaColumeCount, r+1, 1);

//             update(); // 更新
//         }
//     }

// }

// // 重写 显示事件，初始设置 滚动区、滚动区容器、主题页面容器宽度
// void HomeWorkspaceSettingsPage::showEvent(QShowEvent *event)
// {
//     // 调用 父类显示事件，初始化设置容器尺寸 与 滚动区尺寸相等
//     BasedHomeWorkspacePage::showEvent(event);

//     // 设置 主题页面宽度与滚动区容器 宽度相同
//     m_frmThemeContainer->resize(m_scrollArea->width() - 40*2, m_frmThemeContainer->height());

//     // 更新主题页面布局
//     updateThemeContainerLayout();
// }


// void HomeWorkspaceSettingsPage::resizeEvent(QResizeEvent *event)
// {
//     QWidget::resizeEvent(event);

//     // 更新 滚动区容器宽度 和 滚动区宽度 相等
//     BasedHomeWorkspacePage::updateContainerWidth();

//     // 设置 主题页面宽度与滚动区容器 宽度相同
//     m_frmThemeContainer->resize(m_scrollArea->width() - 40*2, m_frmThemeContainer->height());

//     // 更新主题页面布局
//     updateThemeContainerLayout();

// }

// 主题按钮点击 处理槽函数，实现互斥功能
void HomeWorkspaceSettingsPage::do_themeTickButton_clicked()
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
