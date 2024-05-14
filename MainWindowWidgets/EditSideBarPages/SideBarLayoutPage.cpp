#include "SideBarLayoutPage.h"

SideBarLayoutPage::SideBarLayoutPage(QWidget *parent)
    : BasedSiderBarPage{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void SideBarLayoutPage::initMemberVariable()
{
    m_labTitle->setText("布局");
}
// 初始化 布局
void SideBarLayoutPage::initLayout()
{
    // 主布局，垂直布局，依次为 侧边栏标题区，标题可见区，标题对齐区，字间距区，行间距区，左右边距区，底部间距区
    QVBoxLayout *containerLayout = new QVBoxLayout; // 新建 布局
    containerLayout->setSpacing(10); // 设置 控件间距
    containerLayout->setContentsMargins(5,0,5,0); // 设置 边距
    m_frmContainer->setLayout(containerLayout);


    // 标题可见区，左边为标签
    QHBoxLayout *titleVisibleLayout = new QHBoxLayout; // 新建 布局
    titleVisibleLayout->setSpacing(0); // 设置 控件间距
    titleVisibleLayout->setContentsMargins(0,0,0,0); // 设置 边距
    containerLayout->addLayout(titleVisibleLayout); // 将 标题可见区 添加到 主布局

    // 标题可见提示标签
    QLabel *labTitleVisible = new QLabel("文章标题",m_frmContainer); // 新建 标签
    labTitleVisible->setProperty("WidgetStyleClass", "SideBarLayoutPage_ChildTitleLabel");
    titleVisibleLayout->addWidget(labTitleVisible); // 将 标题可见提示标签 添加到 标题可见区


    // 标题对齐区，左边为 居左，中间为 居中，右边为 居右
    QHBoxLayout *titleAlignLayout = new QHBoxLayout; // 新建 布局
    titleAlignLayout->setSpacing(2); // 设置 控件间距
    titleAlignLayout->setContentsMargins(0,0,0,0); // 设置 边距
    containerLayout->addLayout(titleAlignLayout); // 将 标题对齐区 添加到 主布局
    // 互斥按钮组
    m_btngTitleAlignment = new QButtonGroup(m_frmContainer); // 新建 互斥按钮组
    m_btngTitleAlignment->setExclusive(true); // 设置 组内按钮 互斥
    // 居左按钮
    m_btnTitleAlignLeft = new QPushButton(m_frmContainer); // 新建 按钮
    m_btnTitleAlignLeft->setProperty("WidgetStyleClass", "SideBarLayoutPage_AlignButton");
    m_btnTitleAlignLeft->setText("居左");
    m_btnTitleAlignLeft->setCheckable(true); // 设置 可选中
    m_btnTitleAlignLeft->setChecked(true); // 默认点击
    m_btnTitleAlignLeft->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnTitleAlignLeft->setMinimumWidth(40); // 设置 最小宽度
    m_btnTitleAlignLeft->setFixedHeight(30); // 设置 固定高度
    m_btngTitleAlignment->addButton(m_btnTitleAlignLeft); // 添加到 互斥按钮组
    titleAlignLayout->addWidget(m_btnTitleAlignLeft);  // 将 按钮 添加到 标题对齐区
    // 居中按钮
    m_btnTitleAlignCenter = new QPushButton(m_frmContainer); // 新建 按钮
    m_btnTitleAlignCenter->setProperty("WidgetStyleClass", "SideBarLayoutPage_AlignButton");
    m_btnTitleAlignCenter->setText("居中");
    m_btnTitleAlignCenter->setCheckable(true); // 设置 可选中
    m_btnTitleAlignCenter->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnTitleAlignCenter->setMinimumWidth(40); // 设置 最小宽度
    m_btnTitleAlignCenter->setFixedHeight(30); // 设置 固定高度
    m_btngTitleAlignment->addButton(m_btnTitleAlignCenter); // 添加到 互斥按钮组
    titleAlignLayout->addWidget(m_btnTitleAlignCenter);  // 将 按钮 添加到 标题对齐区
    // 居右按钮
    m_btnTitleAlignRight = new QPushButton(m_frmContainer); // 新建 按钮
    m_btnTitleAlignRight->setProperty("WidgetStyleClass", "SideBarLayoutPage_AlignButton");
    m_btnTitleAlignRight->setText("居右");
    m_btnTitleAlignRight->setCheckable(true); // 设置 可选中
    m_btnTitleAlignRight->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnTitleAlignRight->setMinimumWidth(40); // 设置 最小宽度
    m_btnTitleAlignRight->setFixedHeight(30); // 设置 固定高度
    m_btngTitleAlignment->addButton(m_btnTitleAlignRight); // 添加到 互斥按钮组
    titleAlignLayout->addWidget(m_btnTitleAlignRight);  // 将 按钮 添加到 标题对齐区



    // 首行缩进提示标签
    QLabel *labFirstLineIndent = new QLabel("首行缩进",m_frmContainer); // 新建 标签
    labFirstLineIndent->setProperty("WidgetStyleClass", "SideBarLayoutPage_ChildTitleLabel");
    containerLayout->addWidget(labFirstLineIndent); // 将 字间距提示标签 添加到 主布局


    // 首行缩进区，左边为标签，右边为切换按钮
    QHBoxLayout *firstLineIndentLayout = new QHBoxLayout; // 新建 布局
    firstLineIndentLayout->setSpacing(5); // 设置 控件间距
    firstLineIndentLayout->setContentsMargins(0,0,0,0); // 设置 边距
    containerLayout->addLayout(firstLineIndentLayout); // 将 首行缩进区 添加到 主布局

    // 滑动条
    m_sldFirstLineIndent = new QSlider(m_frmContainer); // 新建 滑动条
    m_sldFirstLineIndent->setProperty("WidgetStyleClass", "SideBarLayoutPage_Slider");
    m_sldFirstLineIndent->setOrientation(Qt::Horizontal); // 设置为 水平滑动条
    m_sldFirstLineIndent->setRange(0,100); // 设置 取值范围
    m_sldFirstLineIndent->setSingleStep(1); // 设置 步长
    m_sldFirstLineIndent->setValue(0); // 设置 当前值
    firstLineIndentLayout->addWidget(m_sldFirstLineIndent); // 将 滑动条 添加到 字间距区
    // 值输入控件
    m_spbFirstLineIndent = new QSpinBox(m_frmContainer); // 新建 值输入控件
    m_spbFirstLineIndent->setProperty("WidgetStyleClass", "SideBarLayoutPage_SpinBox");
    m_spbFirstLineIndent->setRange(0,100); // 设置 取值范围
    m_spbFirstLineIndent->setValue(0); // 设置 当前值
    firstLineIndentLayout->addWidget(m_spbFirstLineIndent); // 将 值输入控件 添加到 字间距区


    // 字间距提示标签
    QLabel *labWordSpacing = new QLabel("字间距",m_frmContainer); // 新建 标签
    labWordSpacing->setProperty("WidgetStyleClass", "SideBarLayoutPage_ChildTitleLabel");
    containerLayout->addWidget(labWordSpacing); // 将 字间距提示标签 添加到 主布局

    // 字间距区，左边为 滑动条，右边为 值输入控件
    QHBoxLayout *wordSpacingLayout = new QHBoxLayout; // 新建 布局
    wordSpacingLayout->setSpacing(5); // 设置 控件间距
    wordSpacingLayout->setContentsMargins(0,0,0,0); // 设置 边距
    containerLayout->addLayout(wordSpacingLayout); // 将 字间距区 添加到 主布局
    // 滑动条
    m_sldWordSpacing = new QSlider(m_frmContainer); // 新建 滑动条
    m_sldWordSpacing->setProperty("WidgetStyleClass", "SideBarLayoutPage_Slider");
    m_sldWordSpacing->setOrientation(Qt::Horizontal); // 设置为 水平滑动条
    m_sldWordSpacing->setRange(0,100); // 设置 取值范围
    m_sldWordSpacing->setSingleStep(1); // 设置 步长
    m_sldWordSpacing->setValue(0); // 设置 当前值
    wordSpacingLayout->addWidget(m_sldWordSpacing); // 将 滑动条 添加到 字间距区
    // 值输入控件
    m_spbWordSpacing = new QSpinBox(m_frmContainer); // 新建 值输入控件
    m_spbWordSpacing->setProperty("WidgetStyleClass", "SideBarLayoutPage_SpinBox");
    m_spbWordSpacing->setRange(0,100); // 设置 取值范围
    m_spbWordSpacing->setValue(0); // 设置 当前值
    wordSpacingLayout->addWidget(m_spbWordSpacing); // 将 值输入控件 添加到 字间距区



    // 行间距提示标签
    QLabel *labLineSpacing = new QLabel("行间距",m_frmContainer); // 新建 标签
    labLineSpacing->setProperty("WidgetStyleClass", "SideBarLayoutPage_ChildTitleLabel");
    containerLayout->addWidget(labLineSpacing); // 将 行间距提示标签 添加到 主布局

    // 行间距区，左边为 滑动条，右边为 值输入控件
    QHBoxLayout *lineSpacingLayout = new QHBoxLayout; // 新建 布局
    lineSpacingLayout->setSpacing(5); // 设置 控件间距
    lineSpacingLayout->setContentsMargins(0,0,0,0); // 设置 边距
    containerLayout->addLayout(lineSpacingLayout); // 将 行间距区 添加到 主布局

    // 滑动条
    m_sldLineSpacing = new QSlider(m_frmContainer); // 新建 滑动条
    m_sldLineSpacing->setProperty("WidgetStyleClass", "SideBarLayoutPage_Slider");
    m_sldLineSpacing->setOrientation(Qt::Horizontal); // 设置为 水平滑动条
    m_sldLineSpacing->setRange(0,100); // 设置 取值范围
    m_sldLineSpacing->setSingleStep(1); // 设置 步长
    m_sldLineSpacing->setValue(30); // 设置 当前值
    lineSpacingLayout->addWidget(m_sldLineSpacing); // 将 滑动条 添加到 行间距区
    // 值输入控件
    m_spbLineSpacing = new QSpinBox(m_frmContainer); // 新建 值输入控件
    m_spbLineSpacing->setProperty("WidgetStyleClass", "SideBarLayoutPage_SpinBox");
    m_spbLineSpacing->setRange(0,100); // 设置 取值范围
    m_spbLineSpacing->setValue(30); // 设置 当前值
    lineSpacingLayout->addWidget(m_spbLineSpacing); // 将 值输入控件 添加到 行间距区



    // 左右边距提示标签
    QLabel *labLeftRightMargin = new QLabel("左右边距",m_frmContainer); // 新建 标签
    labLeftRightMargin->setProperty("WidgetStyleClass", "SideBarLayoutPage_ChildTitleLabel");
    containerLayout->addWidget(labLeftRightMargin); // 将 左右边距提示标签 添加到 主布局

    // 左右边距区，左边为 滑动条，右边为 值输入控件
    QHBoxLayout *leftRightMarginLayout = new QHBoxLayout; // 新建 布局
    leftRightMarginLayout->setSpacing(5); // 设置 控件间距
    leftRightMarginLayout->setContentsMargins(0,0,0,0); // 设置 边距
    containerLayout->addLayout(leftRightMarginLayout); // 将 左右间距区 添加到 主布局

    // 滑动条
    m_sldLeftRightMargin = new QSlider(m_frmContainer); // 新建 滑动条
    m_sldLeftRightMargin->setProperty("WidgetStyleClass", "SideBarLayoutPage_Slider");
    m_sldLeftRightMargin->setOrientation(Qt::Horizontal); // 设置为 水平滑动条
    m_sldLeftRightMargin->setRange(0,100); // 设置 取值范围
    m_sldLeftRightMargin->setSingleStep(1); // 设置 步长
    m_sldLeftRightMargin->setValue(0); // 设置 当前值
    leftRightMarginLayout->addWidget(m_sldLeftRightMargin); // 将 滑动条 添加到 左右边距区
    // 值输入控件
    m_spbLeftRightMargin = new QSpinBox(m_frmContainer); // 新建 值输入控件
    m_spbLeftRightMargin->setProperty("WidgetStyleClass", "SideBarLayoutPage_SpinBox");
    m_spbLeftRightMargin->setRange(0,100); // 设置 取值范围
    m_spbLeftRightMargin->setValue(0); // 设置 当前值
    leftRightMarginLayout->addWidget(m_spbLeftRightMargin); // 将 值输入控件 添加到 左右边距区



    // 底部边距提示标签
    QLabel *labBottomMargin = new QLabel("底部安全距离",m_frmContainer); // 新建 标签
    labBottomMargin->setProperty("WidgetStyleClass", "SideBarLayoutPage_ChildTitleLabel");
    containerLayout->addWidget(labBottomMargin); // 将 底部边距提示标签 添加到 主布局

    // 底部边距区，左边为 滑动条，右边为 值输入控件
    QHBoxLayout *bottomMarginLayout = new QHBoxLayout; // 新建 布局
    bottomMarginLayout->setSpacing(5); // 设置 控件间距
    bottomMarginLayout->setContentsMargins(0,0,0,0); // 设置 边距
    containerLayout->addLayout(bottomMarginLayout); // 将 底部边距区 添加到 主布局
    // 滑动条
    m_sldBottomMargin = new QSlider(m_frmContainer); // 新建 滑动条
    m_sldBottomMargin->setProperty("WidgetStyleClass", "SideBarLayoutPage_Slider");
    m_sldBottomMargin->setOrientation(Qt::Horizontal); // 设置为 水平滑动条
    m_sldBottomMargin->setRange(0,100); // 设置 取值范围
    m_sldBottomMargin->setSingleStep(1); // 设置 步长
    m_sldBottomMargin->setValue(0); // 设置 当前值
    bottomMarginLayout->addWidget(m_sldBottomMargin); // 将 滑动条 添加到 底部边距区
    // 值输入控件
    m_spbBottomMargin = new QSpinBox(m_frmContainer); // 新建 值输入控件
    m_spbBottomMargin->setProperty("WidgetStyleClass", "SideBarLayoutPage_SpinBox");
    m_spbBottomMargin->setRange(0,100); // 设置 取值范围
    m_spbBottomMargin->setValue(0); // 设置 当前值
    bottomMarginLayout->addWidget(m_spbBottomMargin); // 将 值输入控件 添加到 底部边距区

    // 添加弹簧
    containerLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Expanding));

}
// 初始化 绑定
void SideBarLayoutPage::initConnect()
{
    // 绑定 居左、居中和居右按钮，发送标题对齐信号
    connect(m_btnTitleAlignLeft,&QPushButton::clicked,this,[=](){
        emit titleAlign_changed(Qt::AlignLeft);
    });
    connect(m_btnTitleAlignCenter,&QPushButton::clicked,this,[=](){
        emit titleAlign_changed(Qt::AlignCenter);
    });
    connect(m_btnTitleAlignRight,&QPushButton::clicked,this,[=](){
        emit titleAlign_changed(Qt::AlignRight);
    });

    // 绑定 滑动条 值改变，发送信号，绑定滑动条和值输入框
    // 首行缩进
    connect(m_sldFirstLineIndent,&QSlider::valueChanged,this,&SideBarLayoutPage::on_sliderSpinBoxValues_changed);
    connect(m_sldFirstLineIndent,&QSlider::valueChanged,m_spbFirstLineIndent,&QSpinBox::setValue);
    connect(m_spbFirstLineIndent,&QSpinBox::valueChanged,m_sldFirstLineIndent,&QSlider::setValue);
    // 字间距
    connect(m_sldWordSpacing,&QSlider::valueChanged,this,&SideBarLayoutPage::on_sliderSpinBoxValues_changed);
    connect(m_sldWordSpacing,&QSlider::valueChanged,m_spbWordSpacing,&QSpinBox::setValue);
    connect(m_spbWordSpacing,&QSpinBox::valueChanged,m_sldWordSpacing,&QSlider::setValue);
    // 行间距
    connect(m_sldLineSpacing,&QSlider::valueChanged,this,&SideBarLayoutPage::on_sliderSpinBoxValues_changed);
    connect(m_sldLineSpacing,&QSlider::valueChanged,m_spbLineSpacing,&QSpinBox::setValue);
    connect(m_spbLineSpacing,&QSpinBox::valueChanged,m_sldLineSpacing,&QSlider::setValue);
    // 左右边距
    connect(m_sldLeftRightMargin,&QSlider::valueChanged,this,&SideBarLayoutPage::on_sliderSpinBoxValues_changed);
    connect(m_sldLeftRightMargin,&QSlider::valueChanged,m_spbLeftRightMargin,&QSpinBox::setValue);
    connect(m_spbLeftRightMargin,&QSpinBox::valueChanged,m_sldLeftRightMargin,&QSlider::setValue);
    // 底部边距
    connect(m_sldBottomMargin,&QSlider::valueChanged,this,&SideBarLayoutPage::on_sliderSpinBoxValues_changed);
    connect(m_sldBottomMargin,&QSlider::valueChanged,m_spbBottomMargin,&QSpinBox::setValue);
    connect(m_spbBottomMargin,&QSpinBox::valueChanged,m_sldBottomMargin,&QSlider::setValue);
}

// 设置控件的值
void SideBarLayoutPage::setSlidersValues(int firstLineIndent, int wordSpacing, int lineSpacing, int leftRightMargin, int bottomMargin)
{
    m_sldFirstLineIndent->setValue(firstLineIndent);
    m_sldWordSpacing->setValue(wordSpacing);
    m_sldLineSpacing->setValue(lineSpacing);
    m_sldLeftRightMargin->setValue(leftRightMargin);
    m_sldBottomMargin->setValue(bottomMargin);
}

// 关联 滑动条和输入框
void SideBarLayoutPage::on_sliderSpinBoxValues_changed(int value)
{
    Q_UNUSED(value);

    int firstLineIndet = m_sldFirstLineIndent->value();
    int wordSpacing = m_sldWordSpacing->value();
    int lineSpacing = m_sldLineSpacing->value();
    int leftRightMargin = m_sldLeftRightMargin->value();
    int bottomMargin = m_sldBottomMargin->value();

    emit layoutValuesChanged(firstLineIndet, wordSpacing, lineSpacing, leftRightMargin, bottomMargin);
}



