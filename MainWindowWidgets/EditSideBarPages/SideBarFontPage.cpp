#include "SideBarFontPage.h"

SideBarFontPage::SideBarFontPage(QWidget *parent)
    : BasedSiderBarPage{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void SideBarFontPage::initMemberVariable()
{
    m_labTitle->setText("字体");
}
// 初始化 布局
void SideBarFontPage::initLayout()
{
    // 容器布局
    QVBoxLayout *containerLayout = new QVBoxLayout;
    containerLayout->setSpacing(20);
    containerLayout->setContentsMargins(10,10,10,10);
    m_frmContainer->setLayout(containerLayout);

    // 章节标题编辑框
    QFrame *frmChapterTitle = new QFrame(m_frmContainer);
    frmChapterTitle->setProperty("WidgetStyleClass", "SiderBarPage_ContainerFrame");
    containerLayout->addWidget(frmChapterTitle);

    // 章节标题编辑区，垂直布局，上侧为 提示标签，中间为 字体字号颜色区，下侧为 加粗斜体下划线区
    QVBoxLayout *chapterTitleLayout = new QVBoxLayout;
    chapterTitleLayout->setSpacing(5);
    chapterTitleLayout->setContentsMargins(0,0,0,0);
    frmChapterTitle->setLayout(chapterTitleLayout);

    // 章节标题提示标签
    QLabel *labChapterTitle = new QLabel("标题字体",frmChapterTitle);
    labChapterTitle->setProperty("WidgetStyleClass", "SideBarFontPage_ChildTitleLabel");
    chapterTitleLayout->addWidget(labChapterTitle);

    // 章节标题字体下拉框
    m_fcmbTitleFontFamily = new QFontComboBox(frmChapterTitle);
    m_fcmbTitleFontFamily->setProperty("WidgetStyleClass", "SideBarFontPage_FamilyComboBox");
    m_fcmbTitleFontFamily->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_fcmbTitleFontFamily->setEditable(false); // 不可编辑
    chapterTitleLayout->addWidget(m_fcmbTitleFontFamily);

    // 章节标题字号颜色区
    QHBoxLayout *chapterTitleFontLayout = new QHBoxLayout;
    chapterTitleFontLayout->setSpacing(5);
    chapterTitleFontLayout->setContentsMargins(0,0,0,0);
    chapterTitleLayout->addLayout(chapterTitleFontLayout);

    // 章节标题字号下拉框
    m_cmbTitleFontSize = new QComboBox(frmChapterTitle);
    m_cmbTitleFontSize->setProperty("WidgetStyleClass", "SideBarFontPage_SizeComboBox");
    m_cmbTitleFontSize->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    for(int fontSize=10;fontSize<=30;fontSize++)
        m_cmbTitleFontSize->addItem(QString::number(fontSize));
    m_cmbTitleFontSize->addItems({"36","48","60","72"});
    m_cmbTitleFontSize->setCurrentIndex(8);
    chapterTitleFontLayout->addWidget(m_cmbTitleFontSize);

    // 章节标题字体颜色按钮
    m_btnTitleFontColor = new QPushButton(frmChapterTitle);
    m_btnTitleFontColor->setProperty("WidgetStyleClass", "SideBarFontPage_ColorButton");
    m_btnTitleFontColor->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    // m_btnTitleFontColor->setStyleSheet("background-color: black;");
    chapterTitleFontLayout->addWidget(m_btnTitleFontColor);

    // 加粗斜体下划线区
    QHBoxLayout *chapterTitleBIULayout = new QHBoxLayout;
    chapterTitleBIULayout->setSpacing(2);
    chapterTitleBIULayout->setContentsMargins(0,0,0,0);
    chapterTitleLayout->addLayout(chapterTitleBIULayout); // 将 章节标题加粗斜体下划线区 添加到 章节标题编辑区

    // 章节标题加粗按钮
    m_btnTitleBlod = new QPushButton(frmChapterTitle);
    m_btnTitleBlod->setObjectName("SideBarFontPage_btnTitleBlod");
    m_btnTitleBlod->setProperty("WidgetStyleClass", "SideBarFontPage_FontEditButton");
    m_btnTitleBlod->setCheckable(true);
    m_btnTitleBlod->setChecked(true);
    m_btnTitleBlod->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnTitleBlod->setMinimumWidth(40);
    chapterTitleBIULayout->addWidget(m_btnTitleBlod);

    // 章节标题斜体按钮
    m_btnTitleItalics = new QPushButton(frmChapterTitle);
    m_btnTitleItalics->setObjectName("SideBarFontPage_btnTitleItalics");
    m_btnTitleItalics->setProperty("WidgetStyleClass", "SideBarFontPage_FontEditButton");
    m_btnTitleItalics->setCheckable(true);
    m_btnTitleItalics->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnTitleItalics->setMinimumWidth(40);
    chapterTitleBIULayout->addWidget(m_btnTitleItalics);

    // 章节标题下划线按钮
    m_btnTitleUnderline = new QPushButton(frmChapterTitle);
    m_btnTitleUnderline->setObjectName("SideBarFontPage_btnTitleUnderline");
    m_btnTitleUnderline->setProperty("WidgetStyleClass", "SideBarFontPage_FontEditButton");
    m_btnTitleUnderline->setCheckable(true);
    m_btnTitleUnderline->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnTitleUnderline->setMinimumWidth(40);
    chapterTitleBIULayout->addWidget(m_btnTitleUnderline);


    // 章节内容编辑框
    QFrame *frmChapterContent = new QFrame(m_frmContainer);
    frmChapterContent->setProperty("WidgetStyleClass", "SiderBarPage_ContainerFrame");
    containerLayout->addWidget(frmChapterContent);

    // 章节内容编辑区，垂直布局，上侧为 提示标签，中间为 字体字号区，下侧为 加粗斜体下划线区
    QVBoxLayout *chapterContentLayout = new QVBoxLayout;
    chapterContentLayout->setSpacing(5);
    chapterContentLayout->setContentsMargins(0,0,0,0);
    frmChapterContent->setLayout(chapterContentLayout);

    // 提示标签
    QLabel *labChapterContent = new QLabel("正文字体",frmChapterContent);
    labChapterContent->setProperty("WidgetStyleClass", "SideBarFontPage_ChildTitleLabel");
    chapterContentLayout->addWidget(labChapterContent);

    // 章节内容字体下拉框
    m_fcmbContentFontFamily = new QFontComboBox(frmChapterContent);
    m_fcmbContentFontFamily->setProperty("WidgetStyleClass", "SideBarFontPage_FamilyComboBox");
    m_fcmbContentFontFamily->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_fcmbContentFontFamily->setEditable(false); // 不可编辑
    chapterContentLayout->addWidget(m_fcmbContentFontFamily);

    // 字号颜色区
    QHBoxLayout *chapterContentFontLayout = new QHBoxLayout;
    chapterContentFontLayout->setSpacing(5);
    chapterContentFontLayout->setContentsMargins(0,0,0,0);
    chapterContentLayout->addLayout(chapterContentFontLayout);

    // 章节内容字号下拉框
    m_cmbContentFontSize = new QComboBox(frmChapterContent);
    m_cmbContentFontSize->setProperty("WidgetStyleClass", "SideBarFontPage_SizeComboBox");
    m_cmbContentFontSize->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    for(int fontSize=10;fontSize<=30;fontSize++)
        m_cmbContentFontSize->addItem(QString::number(fontSize));
    m_cmbContentFontSize->addItems({"36","48","60","72"});
    m_cmbTitleFontSize->setCurrentIndex(4);
    chapterContentFontLayout->addWidget(m_cmbContentFontSize);

    // 章节内容字体颜色按钮
    m_btnContentFontColor = new QPushButton(frmChapterContent);
    m_btnContentFontColor->setProperty("WidgetStyleClass", "SideBarFontPage_ColorButton");
    m_btnContentFontColor->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    // m_btnContentFontColor->setStyleSheet("background-color: black;");
    chapterContentFontLayout->addWidget(m_btnContentFontColor);

    // 加粗斜体下划线区
    QHBoxLayout *chapterContentBIULayout = new QHBoxLayout;
    chapterContentBIULayout->setSpacing(2);
    chapterContentBIULayout->setContentsMargins(0,0,0,0);
    chapterContentLayout->addLayout(chapterContentBIULayout); // 将 加粗斜体下划线区 添加到 章节内容编辑区

    // 章节内容加粗按钮
    m_btnContentBlod = new QPushButton(frmChapterContent);
    m_btnContentBlod->setObjectName("SideBarFontPage_btnContentBlod");
    m_btnContentBlod->setProperty("WidgetStyleClass", "SideBarFontPage_FontEditButton");
    m_btnContentBlod->setCheckable(true);
    m_btnContentBlod->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnContentBlod->setMinimumWidth(40);
    chapterContentBIULayout->addWidget(m_btnContentBlod);

    // 章节内容斜体按钮
    m_btnContentItalics = new QPushButton(frmChapterContent);
    m_btnContentItalics->setObjectName("SideBarFontPage_btnContentItalics");
    m_btnContentItalics->setProperty("WidgetStyleClass", "SideBarFontPage_FontEditButton");
    m_btnContentItalics->setCheckable(true);
    m_btnContentItalics->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnContentItalics->setMinimumWidth(40);
    chapterContentBIULayout->addWidget(m_btnContentItalics);

    // 章节内容下划线按钮
    m_btnContentUnderline = new QPushButton(frmChapterContent);
    m_btnContentUnderline->setObjectName("SideBarFontPage_btnContentUnderline");
    m_btnContentUnderline->setProperty("WidgetStyleClass", "SideBarFontPage_FontEditButton");
    m_btnContentUnderline->setCheckable(true);
    m_btnContentUnderline->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnContentUnderline->setMinimumWidth(40);
    chapterContentBIULayout->addWidget(m_btnContentUnderline);

    // 添加 弹簧
    containerLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Fixed,QSizePolicy::Expanding));

}
// 初始化 绑定
void SideBarFontPage::initConnect()
{
    // 标题 字体区 字体变化
    connect(m_fcmbTitleFontFamily,&QFontComboBox::currentIndexChanged,this,[=](int index){
        Q_UNUSED(index);
        acceptFont(TextEditArea::TitleArea);
    });
    connect(m_cmbTitleFontSize,&QComboBox::currentIndexChanged,this,[=](int index){
        Q_UNUSED(index);
        acceptFont(TextEditArea::TitleArea);
    });
    connect(m_btnTitleFontColor,&QPushButton::clicked,this,[=](){
        // 获取当前按钮背景颜色
        QColor btnColor = m_btnTitleFontColor->palette().color(QWidget::backgroundRole());

        // 创建颜色对话框对象，设置当前颜色
        QColorDialog *dialog=new QColorDialog(btnColor);
        dialog->setWindowTitle("选择颜色");
        dialog->exec(); //以模态方式运行对话框

        //获取新的颜色
        QColor newBtnColor = dialog->currentColor();
        QString newBtnColorString = converRGB16HexStr(newBtnColor);

        // 设置按钮颜色
        m_btnTitleFontColor->setStyleSheet(QString("background-color: %1;").arg(newBtnColorString));

        // 释放 颜色对话框
        if(dialog != nullptr)
            delete dialog;

        // 调用接收字体
        acceptFont(TextEditArea::TitleArea);
    });
    connect(m_btnTitleBlod,&QPushButton::clicked,this,[=](){
        // 调用接收字体
        acceptFont(TextEditArea::TitleArea);
    });
    connect(m_btnTitleItalics,&QPushButton::clicked,this,[=](){
        // 调用接收字体
        acceptFont(TextEditArea::TitleArea);
    });
    connect(m_btnTitleUnderline,&QPushButton::clicked,this,[=](){
        // 调用接收字体
        acceptFont(TextEditArea::TitleArea);
    });


    // 标题 字体区 字体变化
    connect(m_fcmbContentFontFamily,&QFontComboBox::currentIndexChanged,this,[=](int index){
        Q_UNUSED(index);
        acceptFont(TextEditArea::ContentArea);
    });
    connect(m_cmbContentFontSize,&QComboBox::currentIndexChanged,this,[=](int index){
        Q_UNUSED(index);
        acceptFont(TextEditArea::ContentArea);
    });
    connect(m_btnContentFontColor,&QPushButton::clicked,this,[=](){
        // 获取当前按钮背景颜色
        QColor btnColor = m_btnContentFontColor->palette().color(QWidget::backgroundRole());

        // 创建颜色对话框对象，设置当前颜色
        QColorDialog *dialog=new QColorDialog(btnColor);
        dialog->setWindowTitle("选择颜色");
        dialog->exec(); //以模态方式运行对话框

        //获取新的颜色
        QColor newBtnColor = dialog->currentColor();
        QString newBtnColorString = converRGB16HexStr(newBtnColor);

        // 设置按钮颜色
        m_btnContentFontColor->setStyleSheet(QString("background-color: %1;").arg(newBtnColorString));


        // 释放 颜色对话框
        if(dialog != nullptr)
            delete dialog;

        // 调用接收字体
        acceptFont(TextEditArea::ContentArea);
    });
    connect(m_btnContentBlod,&QPushButton::clicked,this,[=](){
        // 调用接收字体
        acceptFont(TextEditArea::ContentArea);
    });
    connect(m_btnContentItalics,&QPushButton::clicked,this,[=](){
        // 调用接收字体
        acceptFont(TextEditArea::ContentArea);
    });
    connect(m_btnContentUnderline,&QPushButton::clicked,this,[=](){
        // 调用接收字体
        acceptFont(TextEditArea::ContentArea);
    });
}

// 接收 字体，并发送
void SideBarFontPage::acceptFont(TextEditArea editArea)
{
    QFont font; // 字体对象
    QColor fontColor; // 字体颜色
    font.setStyleStrategy(QFont::PreferAntialias); // 设置 字体抗锯齿
    // 根据区域 进行 处理
    if(editArea == TextEditArea::TitleArea)
    {
        // 标题字体区 发生 变化
        font.setFamily(m_fcmbTitleFontFamily->currentText()); // 设置 字族
        // 中文字号VS英文字号(磅)VS像素值的对应关系：
        // 八号＝5磅(5pt) ==(5/72)*96=6.67 =6px
        // 七号＝5.5磅 ==(5.5/72)*96=7.3 =7px
        font.setPointSize((m_cmbTitleFontSize->currentText().toInt()*96)/72); // 设置 字号

        // 获取 字体颜色
        fontColor = m_btnTitleFontColor->palette().color(QWidget::backgroundRole());

        // 是否 勾选 加粗，斜体，下划线
        if(m_btnTitleBlod->isChecked())
            font.setBold(true);  // 设置 加粗
        if(m_btnTitleItalics->isChecked())
            font.setItalic(true); // 设置 斜体
        if(m_btnTitleUnderline->isChecked())
            font.setUnderline(true); // 设置 下划线
    }
    else if(editArea == TextEditArea::ContentArea)
    {
        // 内容字体区 发生 变化
        font.setFamily(m_fcmbContentFontFamily->currentText()); // 设置 字族
        font.setPointSize((m_cmbContentFontSize->currentText().toInt()*96)/72); // 设置 字体大小
        // 获取 字体颜色
        fontColor = m_btnContentFontColor->palette().color(QWidget::backgroundRole());

        // 是否 勾选 加粗，斜体，下划线
        if(m_btnContentBlod->isChecked())
            font.setBold(true);  // 设置 加粗
        if(m_btnContentItalics->isChecked())
            font.setItalic(true); // 设置 斜体
        if(m_btnContentUnderline->isChecked())
            font.setUnderline(true); // 设置 下划线
    }

    // 发送 字体
    emit font_changed(editArea, font, fontColor);
}
