#include "EditWorkspace.h"


/********************************************************************
 ****************   编辑工作区    ************************************
*********************************************************************/

EditWorkspace::EditWorkspace(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void EditWorkspace::initMemberVariable()
{
    m_backgroundColor = QColor(245, 246, 249);
}
// 初始化 布局
void EditWorkspace::initLayout()
{
    // 主布局，垂直布局，
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);

    // 栈容器
    m_stkContainer = new QStackedWidget(this);
    mainLayout->addWidget(m_stkContainer); // 将 栈容器 添加到 主布局

    // 空白页
    m_blankPage = new BlankPage(m_stkContainer);
    m_blankPage->setTipsText("未打开章节或相关");
    m_stkContainer->addWidget(m_blankPage); // 将空白页添加到 栈容器

    // 编辑页
    m_textPage = new TextEditWorkspacePage(m_stkContainer);
    m_stkContainer->addWidget(m_textPage); // 将空白页添加到 栈容器

    m_stkContainer->setCurrentIndex(int(EditWorkspacePage::BlankPage));
}
// 初始化 绑定
void EditWorkspace::initConnect()
{
    // 关闭 文本编辑页面
    connect(m_textPage, &TextEditWorkspacePage::textEditPage_close, this, &EditWorkspace::do_textEditPage_close);

    // 字数改变
    connect(m_textPage, &TextEditWorkspacePage::chapterWordCount_changed, this, [=](int wordCount){
        emit chapterWordCount_changed(wordCount);
    });

    // 文本编辑控件 改变 章节标题，发送当前打开的章节ID和新章节标题
    connect(m_textPage, &TextEditWorkspacePage::chapterTitle_changed, this, [=](QString chapterID,QString newChapterTitle){
        emit chapterTitle_changed(chapterID,newChapterTitle);
    });
    // 文本编辑控件 布局参数 初始化
    connect(m_textPage, &TextEditWorkspacePage::contentLayoutValues_init, this, [=](int firstLineIndent, int wordSpacing,
                                                                                         int lineSpacing, int leftRightMargin,
                                                                                         int bottomMargin){
        emit layoutSliderValues_init(firstLineIndent,wordSpacing,lineSpacing,leftRightMargin,bottomMargin);
    });

    // 更新历史版本页面
    connect(m_textPage, &TextEditWorkspacePage::updateHistoricalVersionPage, this, [=](){
        qDebug()<<"中转 更新备份文件 信号";
        emit historicalVersionPage_update();
    });
}

// 设置 和 获取 背景颜色
void EditWorkspace::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
QColor EditWorkspace::getBackgroundColor() const
{
    return m_backgroundColor;
}

// 工具栏 按钮 点击处理槽函数，处理撤销、重做、自动排版
void EditWorkspace::do_editToolBarButton_clicked(EditToolBarButton toolButton, bool checked)
{
    Q_UNUSED(checked);

    if(toolButton == EditToolBarButton::UndoButton) // 进行 撤销
    {
        m_textPage->undo();
    }
    else if(toolButton == EditToolBarButton::RedoButton) // 进行 重做
    {
        m_textPage->redo();
    }
    else if(toolButton == EditToolBarButton::TypeSettingButton) // 进行 自动排版
    {
        m_textPage->autoTypeSetting();
    }
}

// 双击打开 文件 处理槽函数
void EditWorkspace::do_openChapter(QString chapterID, QString chapterTitle)
{
    m_openChapterID = chapterID; // 记录 当前打开章节ID
    m_stkContainer->setCurrentIndex(int(EditWorkspacePage::TextPage)); // 切换到 文本编辑页
    m_textPage->openChapterFile(chapterID,chapterTitle); // 打开文件，获取数据
}
// 已经打开章节 重命名 处理槽函数
void EditWorkspace::do_chapterItem_renamed(QString newOpenChapterTitle)
{
    m_textPage->setLineEditNewTitle(newOpenChapterTitle);
}
// 保存 当前打开的 章节
void EditWorkspace::do_beingOpenedChapter_save()
{
    m_textPage->saveAsMainChapterFile();
}
// 章节标题 显示隐藏 处理槽函数
void EditWorkspace::do_chapterTitleVisible_changed(bool checked)
{
    // 设置 标题输入框 是否 可见
    m_textPage->setLineEditTitleVisible(checked);
}
// 字体改变 处理槽函数
void EditWorkspace::do_font_changed(TextEditArea editArea, QFont font, QColor fontColor)
{
    // if(m_openChapterID != "") // 有章节 被打开，可以进行编辑
    // {
    if(editArea == TextEditArea::TitleArea)
    {
        // 标题字体 改变
        m_textPage->setTitleFont(font,fontColor);
    }
    else if(editArea == TextEditArea::ContentArea)
    {
        // 内容字体 改变
        m_textPage->setContentFont(font,fontColor);
    }
    // }

}
// 标题对齐方式改变  处理槽函数
void EditWorkspace::do_titleAlign_changed(Qt::Alignment flag)
{
    m_textPage->setTitleAlignment(flag);
}
// 排版按钮 点击 处理槽函数
void EditWorkspace::do_tbtnTypesetting_clicked()
{
    m_textPage->autoTypeSetting();
}
// 间距边距 改变，设置布局，处理槽函数
void EditWorkspace::do_contentLayout_changed(int firstLineIndent, int wordSpacing, int lineSpacing, int leftRightMargin, int bottomMargin)
{
    m_textPage->setContentLayout(firstLineIndent, wordSpacing, lineSpacing, leftRightMargin, bottomMargin);
}
// 初始化 历史版本信息
void EditWorkspace::do_historicalVersionInfo_init(QString latestVersionID, int backupFileCount)
{
    m_textPage->setHistoricalVersionInfo(latestVersionID, backupFileCount);
}
// 恢复 历史版本 处理槽函数
void EditWorkspace::do_historicalVersion_restore(QString filePath)
{
    qDebug()<<"接收 中转信号";
    m_textPage->restoreHistoricalVersion(filePath);
}
// 撤销
void EditWorkspace::do_tbtnUndo_clicked()
{
    m_textPage->undo();
}
// 重做
void EditWorkspace::do_tbtnRedo_clicked()
{
    m_textPage->redo();
}
// 搜索查找内容 处理槽函数
void EditWorkspace::do_searchText(QString text)
{
    m_textPage->searchText(text);
}
// 搜索上一个查找内容 处理槽函数
void EditWorkspace::do_searchText_previous()
{
    m_textPage->searchPreviousText();
}
// 搜索下一个查找内容 处理槽函数
void EditWorkspace::do_searchText_next()
{
    m_textPage->SearchNextText();
}
// 搜索查找内容 清空 处理槽函数
void EditWorkspace::do_searchText_cleared()
{
    m_textPage->clearSearchText();
}
// 替换 当前 处理槽函数
void EditWorkspace::do_replaceCurrent(QString replaceText)
{
    m_textPage->replaceCurrent(replaceText);
}
// 替换 全部 处理槽函数
void EditWorkspace::do_replaceAll(QString replaceText)
{
    m_textPage->replaceAll(replaceText);
}

// 绘制 背景
void EditWorkspace::drawBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    painter->setBrush(QBrush(m_backgroundColor)); // 设置 画刷

    painter->drawRect(QRect(0,0,this->width(),this->height())); // 绘制 控件尺寸 的矩形

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}

// 重写 绘画事件
void EditWorkspace::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
}

// 关闭 文本编辑页面 处理槽函数
void EditWorkspace::do_textEditPage_close()
{
    m_stkContainer->setCurrentIndex(int(EditWorkspacePage::BlankPage)); // 切换到 空白页面
    m_openChapterID = "";
    emit editWorkspace_noFile();
}
