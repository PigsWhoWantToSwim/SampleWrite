#include "SideBarThemePage.h"

SideBarThemePage::SideBarThemePage(QWidget *parent)
    : BasedSiderBarPage{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void SideBarThemePage::initMemberVariable()
{
    m_labTitle->setText("主题");

    // m_themeBtnAreaColumeCount = 1;

    m_frmThemeButtonContainer = new QFrame(m_frmContainer);

    // 主题页面 网格布局
    m_themeBtnAreaLayout = new FlowLayout;

    // 主题按钮
    // 默认主题
    ThemeTickButton *defualtTheme = new ThemeTickButton(m_frmThemeButtonContainer);
    defualtTheme->setAboveColor(QColor(27, 178, 139));
    defualtTheme->setBelowColor(QColor(255,255,255));
    defualtTheme->setTextColor(QColor(27, 178, 139));
    defualtTheme->setQSSFilePath(":/theme/default.qss");
    defualtTheme->setFixedSize(EDIT_THEMETICKBUTTON_WIDTH, EDIT_THEMETICKBUTTON_HEIGHT);
    connect(defualtTheme, SIGNAL(themeButton_clicked()),
            this, SLOT(do_themeTickButton_clicked()));
    m_listThemeTickButtons << defualtTheme;
    // defualtTheme->setChecked(true);

    // 暗黑主题
    ThemeTickButton *darkTheme = new ThemeTickButton(m_frmThemeButtonContainer);
    darkTheme->setAboveColor(QColor(64,65,66));
    darkTheme->setBelowColor(QColor(191,191,191));
    darkTheme->setTextColor(QColor(64,65,66));
    darkTheme->setQSSFilePath(":/theme/dark.qss");
    darkTheme->setFixedSize(EDIT_THEMETICKBUTTON_WIDTH, EDIT_THEMETICKBUTTON_HEIGHT);
    connect(darkTheme, SIGNAL(themeButton_clicked()),
            this, SLOT(do_themeTickButton_clicked()));
    m_listThemeTickButtons << darkTheme;

    // 天蓝主题
    ThemeTickButton *blueTheme = new ThemeTickButton(m_frmThemeButtonContainer);
    blueTheme->setAboveColor(QColor(9,132,227));
    blueTheme->setBelowColor(QColor(166,216,246));
    blueTheme->setTextColor(QColor(9,132,227));
    blueTheme->setQSSFilePath(":/theme/sky blue.qss");
    blueTheme->setFixedSize(EDIT_THEMETICKBUTTON_WIDTH, EDIT_THEMETICKBUTTON_HEIGHT);
    connect(blueTheme, SIGNAL(themeButton_clicked()),
            this, SLOT(do_themeTickButton_clicked()));
    m_listThemeTickButtons << blueTheme;

    // 野蔷薇主题
    ThemeTickButton *briarTheme = new ThemeTickButton(m_frmThemeButtonContainer);
    briarTheme->setButtonStyle(TickButtonStyle::rectImageButton);
    briarTheme->setImage(":/image/briar.png");
    briarTheme->setTextColor(QColor(244, 245, 244));// 246, 162, 94
    briarTheme->setQSSFilePath(":/theme/briar.qss");
    briarTheme->setFixedSize(EDIT_THEMETICKBUTTON_WIDTH, EDIT_THEMETICKBUTTON_HEIGHT);
    connect(briarTheme, SIGNAL(themeButton_clicked()),
            this, SLOT(do_themeTickButton_clicked()));
    m_listThemeTickButtons << briarTheme;

    // m_horizontalSpacer = new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed);
}
// 初始化 布局
void SideBarThemePage::initLayout()
{
    QVBoxLayout *containerLayout = new QVBoxLayout;
    containerLayout->setSpacing(0);
    containerLayout->setContentsMargins(15,0,15,0);
    m_frmContainer->setLayout(containerLayout);


    m_frmThemeButtonContainer->setProperty("WidgetStyleClass", "SiderBarPage_ContainerFrame");
    containerLayout->addWidget(m_frmThemeButtonContainer);

    // 主题按钮区 布局
    m_themeBtnAreaLayout->setSpacing(SIDEBARTHEMEPAGE_THEMEBUTTON_HORIZONTALSPACING);
    m_themeBtnAreaLayout->setContentsMargins(10, 10, 10, 10);

    // 添加主题按钮
    // 读取 配置文件中的 当前主题
    settings->beginGroup("APP_THEME");
    QString currentTheme = settings->value("CURRENT_THEME",":/theme/default.qss").toString();
    settings->endGroup();

    // 遍历 每一个主题按钮
    foreach (ThemeTickButton* themeBtn, m_listThemeTickButtons) {
        if(themeBtn->getQSSFilePath() == currentTheme)  // 是当前 主题
        {
            themeBtn->setChecked(true); // 设置 勾选
        }
        m_themeBtnAreaLayout->addWidget(themeBtn); // 添加到 布局
    }


    // 添加 弹簧 到最右边一列
    // m_themeBtnAreaLayout->addItem(m_horizontalSpacer, 0, m_themeBtnAreaColumeCount, (m_listThemeTickButtons.count() / m_themeBtnAreaColumeCount)+1, 1);

    m_frmThemeButtonContainer->setLayout(m_themeBtnAreaLayout);

    containerLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Fixed,QSizePolicy::Expanding));

}
// 初始化 绑定
void SideBarThemePage::initConnect()
{

}

// 更新 主题页面列数
// void SideBarThemePage::updateThemeContainerLayout()
// {
//     // 计算列数
//     int colCount = floor(double(m_frmThemeButtonContainer->width() + SIDEBARTHEMEPAGE_THEMEBUTTON_HORIZONTALSPACING - 10 - 10)
//                          /double(EDIT_THEMETICKBUTTON_WIDTH + SIDEBARTHEMEPAGE_THEMEBUTTON_HORIZONTALSPACING));

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

// void SideBarThemePage::showEvent(QShowEvent *event)
// {
//     // 调用 父类显示事件
//     QWidget::showEvent(event);

//     // 更新主题页面布局
//     updateThemeContainerLayout();
// }

// void SideBarThemePage::resizeEvent(QResizeEvent *event)
// {
//     QWidget::resizeEvent(event);

//     // 更新主题页面布局
//     updateThemeContainerLayout();
// }

void SideBarThemePage::do_themeTickButton_clicked()
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
