#include "SideBarSettingPage.h"

SideBarSettingPage::SideBarSettingPage(QWidget *parent)
    : BasedSiderBarPage{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void SideBarSettingPage::initMemberVariable()
{
    m_labTitle->setText("更多设置");
}
// 初始化 布局
void SideBarSettingPage::initLayout()
{
    QVBoxLayout *containerLayout = new QVBoxLayout;
    containerLayout->setSpacing(10);
    containerLayout->setContentsMargins(5,0,5,0);
    m_frmContainer->setLayout(containerLayout);

    // 章节 标题可见 布局
    QHBoxLayout *chapterTitleLayout = new QHBoxLayout;
    chapterTitleLayout->setSpacing(0);
    chapterTitleLayout->setContentsMargins(0,0,0,0);
    containerLayout->addLayout(chapterTitleLayout);

    // 章节标题可见 提示标签
    QLabel *labChapterTitleVisiable = new QLabel(m_frmContainer);
    labChapterTitleVisiable->setProperty("WidgetStyleClass", "SideBarSettingPage_TipLabel");
    labChapterTitleVisiable->setText("章节标题");
    chapterTitleLayout->addWidget(labChapterTitleVisiable);

    // 添加弹簧
    chapterTitleLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding, QSizePolicy::Fixed));

    // 章节标题可见 开关按钮
    m_swbtnChapterTitleVisible = new SwitchButton(m_frmContainer);
    m_swbtnChapterTitleVisible->setProperty("WidgetStyleClass", "SideBarSettingPage_SwitchButton");
    m_swbtnChapterTitleVisible->setButtonStyle(SwitchButtonStyle::circleInButton);
    m_swbtnChapterTitleVisible->setSpace(3); // 设置 滑块与滑槽的 间距
    m_swbtnChapterTitleVisible->setChecked(true); // 设置 开关按钮 默认 为打开状态
    m_swbtnChapterTitleVisible->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_swbtnChapterTitleVisible->setFixedSize(SIDEBARSETTINGPAGE_SWITCHBUTTON_WIDTH, SIDEBARSETTINGPAGE_SWITCHBUTTON_HEIGHT); // 设置 固定尺寸
    chapterTitleLayout->addWidget(m_swbtnChapterTitleVisible);

    // 添加弹簧
    containerLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Fixed, QSizePolicy::Expanding));
}
// 初始化 绑定
void SideBarSettingPage::initConnect()
{
    connect(m_swbtnChapterTitleVisible, &SwitchButton::checkedChanged,this,[=](bool checked){
        emit chapterTitleVisible_changed(checked);
    });
}
