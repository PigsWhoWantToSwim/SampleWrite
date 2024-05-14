#include "TextEditWorkspacePage.h"

// 构造函数
TextEditWorkspacePage::TextEditWorkspacePage(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}
// 析构函数
TextEditWorkspacePage::~TextEditWorkspacePage()
{
    // 保存章节内容到主文件
    saveAsMainChapterFile();
}

// 初始化 成员变量
void TextEditWorkspacePage::initMemberVariable()
{
    // 成员变量
    m_openChapterID = "";
    m_latestVersionID = "";
    m_backupFileCount = 0;

    // 布局相关 变量
    m_firstLineIndent = 20;
    m_wordSpacing = 0;
    m_lineSpacing = 30;
    m_leftRightMargin = 0;
    m_bottomMargin = 0;
    emit contentLayoutValues_init(m_firstLineIndent, m_wordSpacing,
                                  m_lineSpacing, m_leftRightMargin, m_bottomMargin);


    // 默认字体
    m_fontTitle = QFont("Microsoft YaHei UI",18,QFont::Bold);
    m_fontContent = QFont("Microsoft YaHei UI",13);

    m_timer = new QTimer(this); // 新建 定时器
    m_timer->setInterval(BACKUPFILE_TIME); //设置定时器时间间隔 为 60000ms，即60s

    // 搜索相关
    // 要搜索的内容，用于取消搜索后的高亮
    m_searchText = "";
    // 游标列表 记录 与要搜索内容匹配的内容 所在的 游标
    // m_matchedCursorList.clear();
    // 当前游标索引，用于记录当前 游标位置
    m_currentCursorIndex = -1;

    m_backgroundColor = QColor(245, 246, 249);

}
// 初始化 布局
void TextEditWorkspacePage::initLayout()
{
    // 主布局，垂直布局，上侧为 标题输入框，下侧为 内容文本编辑器
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(10,10,0,10);
    this->setLayout(mainLayout);  // 将 主布局 设置为 控件布局

    // 标题输入框
    m_ledTitle = new QLineEdit(this); // 新建 输入框
    m_ledTitle->setObjectName("TextEditWorkspacePage_ledWorkingTexTitle"); // 设置 对象名
    m_ledTitle->setMaxLength(128); // 设置 标题最大长度
    m_ledTitle->setPlaceholderText("标题"); // 设置 提示内容
    m_fontTitle.setLetterSpacing(QFont::PercentageSpacing,100); // 默认字间距为100
    m_ledTitle->setFont(m_fontTitle); // 设置 字体
    m_ledTitle->setAlignment(Qt::AlignLeft); // 设置 向左对齐
    mainLayout->addWidget(m_ledTitle); // 添加到 主布局

    // 内容文本编辑器
    m_tedContent = new QTextEdit(this); // 新建 文本框
    m_tedContent->setObjectName("TextEditWorkspacePage_tedWorkingTexContent"); // 设置 对象名
    m_fontContent.setLetterSpacing(QFont::PercentageSpacing,100); // 默认字间距为100
    m_tedContent->setFont(m_fontContent); // 设置 字体
    m_tedContent->setContextMenuPolicy(Qt::CustomContextMenu); // 自定义弹出菜单
    m_tedContent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 关闭 水平滚动条
    mainLayout->addWidget(m_tedContent); // 添加到 主布局

}
// 初始化 绑定
void TextEditWorkspacePage::initConnect()
{
    // 标题文本框 标题发生变化，将当前打开的章节ID和新标题 发送出去
    connect(m_ledTitle,&QLineEdit::textChanged,this,[=](QString newChapterTitle){
        emit chapterTitle_changed(m_openChapterID,newChapterTitle);
    });

    // 正文内容文本框 字数发送变化，将改变后的字数 发送出去
    connect(m_tedContent,&QTextEdit::textChanged,this,[=](){
        // 获取 文本框内容 字数，并发送
        int wordCount = getTextEditWordCount();
        emit chapterWordCount_changed(wordCount);
    });
    // 绑定 正文内容文本框 自定义弹出菜单 处理槽函数
    connect(m_tedContent, &QTextEdit::customContextMenuRequested,
            this, &TextEditWorkspacePage::do_tedContent_customContextMenuRequested);

    // 绑定定时器 到时
    connect(m_timer,&QTimer::timeout,this,[=](){
        this->saveAsMainChapterFile(); // 保存 主文件
        this->saveAsBackupChapterFile(); // 保存 备份文件
    });
}

// 设置 和 获取 背景颜色
void TextEditWorkspacePage::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
QColor TextEditWorkspacePage::getBackgroundColor() const
{
    return m_backgroundColor;
}

// 打开 章节文件
void TextEditWorkspacePage::openChapterFile(QString chapterID, QString chapterTitle)
{
    // 先关闭 定时器
    m_timer->stop();

    if(m_openChapterID != "") // 不是 第一次 打开章节
    {
        // 保存 当前章节 到备份文件
        saveAsBackupChapterFile();
        // 保存 当前章节 到主文件
        saveAsMainChapterFile();
    }
    // 设置 打开的章节ID
    m_openChapterID = chapterID;
    QString openChapterFilePath = QDir::currentPath() + "/Config/LocalChapters/" + chapterID + ".txt";

    // 打开文件
    QFile file(openChapterFilePath); //通过文件路径创建文件对象
    file.open(QFile::ReadOnly|QIODevice::Text); //文件打开方式
    QString textContent = file.readAll(); //获取qss中全部字符
    file.close(); // 关闭文件

    // 向 标题输入框和文本框 设置内容
    m_ledTitle->setText(chapterTitle);
    m_tedContent->setText(textContent);

    // 设置 初始的 布局 参数
    setContentLayout(m_firstLineIndent,m_wordSpacing,m_lineSpacing,m_leftRightMargin,m_bottomMargin);
    emit contentLayoutValues_init(m_firstLineIndent,m_wordSpacing,m_lineSpacing,m_leftRightMargin,m_bottomMargin);

    // 重新启动 定时器
    m_timer->start();
}
// 关闭 章节文件
void TextEditWorkspacePage::closeChapterFile()
{
    // 保存 文件
    saveAsMainChapterFile();
    saveAsBackupChapterFile();

    // 停止 定时器
    m_timer->stop();

    // 将参数置空
    m_openChapterID = "";
    m_latestVersionID = "";
    m_backupFileCount = 0;

    // 搜索相关
    // 要搜索的内容，用于取消搜索后的高亮
    m_searchText = "";
    // 游标列表 记录 与要搜索内容匹配的内容 所在的 游标
    m_matchedCursorList.clear();
    // 当前游标索引，用于记录当前 游标位置
    m_currentCursorIndex = -1;

    // 发送 关闭文本框 信号
    emit textEditPage_close();
}

// 设置 章节标题 显示或隐藏
void TextEditWorkspacePage::setLineEditTitleVisible(bool checked)
{
    m_ledTitle->setVisible(checked); // 显示 或 隐藏标题

}
// 设置 章节新标题
void TextEditWorkspacePage::setLineEditNewTitle(QString newTitle)
{
    m_ledTitle->setText(newTitle);
}

// 获取 文本框内容 字数
int TextEditWorkspacePage::getTextEditWordCount()
{
    // 获取 文本框内容
    QString content = m_tedContent->toPlainText().replace("\n","").replace(" ","");
    // 统计 字数
    int wordCount = content.size();

    // 返回 字数
    return wordCount;
}

// 保存 主文件
void TextEditWorkspacePage::saveAsMainChapterFile()
{
    // if(m_openChapterID != "") // 关闭时 有章节 被打开
    // {
    // 获取 文本框的全部内容
    qDebug()<<"获取 文本框内容";
    QString chapterContent = m_tedContent->toPlainText();

    qDebug()<<"打开的文件："<<m_openChapterID;

    // 以覆盖的方式 打开 主文件
    QString mainChapterFilePath = QDir::currentPath() + "/Config/LocalChapters/" + m_openChapterID + ".txt";
    QFile mainFile(mainChapterFilePath);
    if(mainFile.open(QIODevice::WriteOnly|QIODevice::Text)) // 文件打开成功
    {
        qDebug()<<"写入 文件";
        // 以文本流 写入文件
        QTextStream textStreamWrite(&mainFile);
        textStreamWrite << chapterContent; //写入文本
    }

    qDebug()<<"关闭 文件";
    // 关闭文件
    mainFile.close();
    //}
}
// 保存 备份文件
void TextEditWorkspacePage::saveAsBackupChapterFile()
{
    // 获取 文本框 当前章节内容
    QString chapterContent = m_tedContent->toPlainText();

    qDebug()<<"最新备份文件："<<m_latestVersionID;

    // 获取 最新备份版本 文件路径
    QString latestVersionFilePath = QDir::currentPath() + "/Config/LocalChapters/" + m_latestVersionID + ".txt";
    // 获取 最新备份版本文件 内容
    QFile file(latestVersionFilePath); // 通过文件路径创建文件对象
    file.open(QFile::ReadOnly|QIODevice::Text); // 文件打开方式
    QString latestVersionContent = file.readAll(); // 获取全部字符
    file.close(); // 关闭文件

    // 对比 文本框内容 和 最新备份文件内容
    if(chapterContent.trimmed() != latestVersionContent.trimmed()) // 不同
    {
        // 不同，新建备份文件，更新最新备份版本，更新数据库 备份章节表和章节表
        // 时间戳
        QDateTime dateTime = QDateTime::currentDateTime(); // 获取 当前时间
        QString timestamp_id = dateTime.toString("yyMMddhhmmss");// 字符串格式化
        QString timestamp_birthTime = dateTime.toString("yyyy-MM-dd hh:mm:ss"); // 创建时间

        // 新备份文件 ID
        QString newBackupFileID = "b"+ timestamp_id + getRandomLetterString(2);

        // 获取 当前章节 字数
        int wordCount = getTextEditWordCount();

        // 新 备份文件 文件路径
        QString newBackupFilePath = QDir::currentPath() + "/Config/LocalChapters/" + newBackupFileID + ".txt";

        // 写入 备份文件
        QFile newBackupFile(newBackupFilePath); // 通过文件路径创建文件对象
        // 新建 备份文件，并打开成功
        if(newBackupFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            // 以文本流 写入文件
            QTextStream textStreamWrite(&newBackupFile);
            textStreamWrite << chapterContent; // 将文本框内容写入新备份文件
        }
        newBackupFile.close(); // 关闭文件


        // 打开数据库
        if (AppDatabase::getInstance()->openDatabase()) // 数据库打开成功
        {
            QSqlQuery sqlQuery;	//执行操作类对象

            // 备份章节表 添加新数据
            // 准备sql语句
            sqlQuery.prepare("INSERT INTO backup_chapters ("
                             "version_id, chapter_id, birth_time, words_count, file_pathURL"
                             ") VALUES ("
                             ":newBackupFileID, :chapterID, :birthTime, :wordCount, :filePath);");

            // 绑定数据
            sqlQuery.bindValue(":newBackupFileID", newBackupFileID);
            sqlQuery.bindValue(":chapterID", m_openChapterID);
            sqlQuery.bindValue(":birthTime", timestamp_birthTime);
            sqlQuery.bindValue(":wordCount", wordCount);
            sqlQuery.bindValue(":filePath", newBackupFilePath);

            // 执行sql语句
            if(!sqlQuery.exec())
            {
                // 执行 sql语句 失败
                qDebug()<<sqlQuery.lastError();
            }

            // 更新 最新备份版本ID（虽然会自动更新）
            m_latestVersionID = newBackupFileID;
            // 备份文件 数量 +1
            m_backupFileCount += 1;

            // 超过 最大备份文件数量，删除 最旧版本（最小）
            if(m_backupFileCount > MAX_BACKUPFILE_NUMBER)
            {
                // 最旧版本 ID
                QString oldestVersionID;

                // 准备sql语句
                sqlQuery.prepare("SELECT MIN(version_id) AS id FROM backup_chapters WHERE chapter_id = :chapterID;");
                // 绑定数据
                sqlQuery.bindValue(":chapterID", m_openChapterID);
                // 执行sql语句
                if(!sqlQuery.exec()) // 执行 sql语句 失败
                    qDebug()<<sqlQuery.lastError();
                else
                {
                    // 读取 记录
                    while(sqlQuery.next())
                        oldestVersionID = sqlQuery.value("version_id").toString();
                }

                // 删除 数据库信息
                // 准备sql语句
                sqlQuery.prepare("DELETE FROM backup_chapters WHERE version_id = :oldestVersionID;");
                // 绑定数据
                sqlQuery.bindValue(":oldestVersionID", oldestVersionID);
                // 执行sql语句
                if(!sqlQuery.exec()) // 执行 sql语句 失败
                    qDebug()<<sqlQuery.lastError();

                // 删除 本地文件
                QString oldestVersionFilePath = QDir::currentPath() + "/Config/LocalChapters/" + oldestVersionID + ".txt";
                if(QFile::exists(oldestVersionFilePath)) // 最旧版本文件 存在
                    QFile::moveToTrash(oldestVersionFilePath); // 移动到回收站

                // 备份文件 数量 -1
                m_backupFileCount -= 1;
            }

            // 结束 数据库 语句
            sqlQuery.finish();
        }
        else // 数据库 打开 失败
            QMessageBox::warning(this, "错误", "打开数据库失败");

        // 关闭数据库
        AppDatabase::getInstance()->quit();


        qDebug()<<"发送 更新备份文件 信号";
        // 发送 更新备份文件 信号
        emit updateHistoricalVersionPage();
    } 
}

// 撤销 编辑
void TextEditWorkspacePage::undo()
{
    m_tedContent->undo();
}
// 重做
void TextEditWorkspacePage::redo()
{
    m_tedContent->redo();
}

// 设置标题字体（包含加粗、斜体、下划线）
void TextEditWorkspacePage::setTitleFont(QFont newTitleFont,QColor newTitleFontColor)
{
    // 设置 标题字体
    m_ledTitle->setFont(newTitleFont);

    QString newTitleFontColorString = converRGB16HexStr(newTitleFontColor);

    // 设置 标题字体颜色
    m_ledTitle->setStyleSheet(QString("color: %1;").arg(newTitleFontColorString));
}

// 设置内容字体（包含加粗、斜体、下划线）
void TextEditWorkspacePage::setContentFont(QFont newContentFont,QColor newContentFontColor)
{
    // 设置 内容字体
    m_tedContent->setFont(newContentFont);
    // 设置 内容字体颜色
    // m_tedContent->setTextColor(newContentFontColor); // 不生效
    m_tedContent->setStyleSheet(QString("color: rgb(%1,%2,%3);")
                                    .arg(newContentFontColor.red())
                                    .arg(newContentFontColor.green())
                                    .arg(newContentFontColor.blue())); // 生效
}

// 设置 标题对齐
void TextEditWorkspacePage::setTitleAlignment(Qt::Alignment flag)
{
    if(m_openChapterID != "") // 有章节 被打开
    {
        m_ledTitle->setAlignment(flag);
    }
}
// 设置 内容首行缩进
void TextEditWorkspacePage::setFirstLineIndent(int value)
{
    if(m_openChapterID != "") // 有章节 被打开
    {
        // 获取 文档
        QTextDocument *doc =  m_tedContent->document();
        // 获取 文本光标
        QTextCursor textcursor = m_tedContent->textCursor();
        // 获取 QTextEdit默认字体的字体度量
        QFontMetrics fm(doc->defaultFont());
        // qDebug()<<"字符平均宽度" << fm.averageCharWidth();      // 字体度量的平均宽度
        // 遍历 文本块
        for(QTextBlock it = doc->begin(); it !=doc->end();it = it.next())
        {
            // 获取 文本块 格式
            QTextBlockFormat tbf = it.blockFormat();
            // 设置 设置文本块的文本缩进
            tbf.setTextIndent(fm.averageCharWidth()*value/5);
            // 设置 文本光标 位置，将光标定位到 第几个字符
            textcursor.setPosition(it.position());
            // 为 文本块 设置 格式
            textcursor.setBlockFormat(tbf);
            // 设置文本光标
            m_tedContent->setTextCursor(textcursor);
        }
    }
}
// 设置 字间距
void TextEditWorkspacePage::setWordSpacing(int value)
{
    if(m_openChapterID != "") // 有章节 被打开
    {
        // 设置 标题 字间距
        QFont titleFont = m_ledTitle->font();
        titleFont.setLetterSpacing(QFont::PercentageSpacing,100+value); // 默认间距为100
        m_ledTitle->setFont(titleFont);

        // 设置 正文 字间距
        QFont contentFont = m_tedContent->font();
        contentFont.setLetterSpacing(QFont::PercentageSpacing,100+value); // 默认间距为100
        m_tedContent->setFont(contentFont);
    }
}
// 设置 行间距
void TextEditWorkspacePage::setLineSpacing(int value)
{
    if(m_openChapterID != "") // 有章节 被打开
    {
        // 获取 文档
        QTextDocument *doc =  m_tedContent->document();
        // 获取 文本光标
        QTextCursor textcursor = m_tedContent->textCursor();
        // 遍历 文本块
        for(QTextBlock it = doc->begin(); it !=doc->end();it = it.next())
        {
            // 获取 文本块 格式
            QTextBlockFormat tbf = it.blockFormat();
            // 设置 文本块 行高（在行与行之间添加指定的高度，以像素为单位。）
            tbf.setLineHeight(value/2,QTextBlockFormat::LineDistanceHeight);
            // 设置 文本光标 位置
            textcursor.setPosition(it.position());
            // 为 文本块 设置 格式
            textcursor.setBlockFormat(tbf);
            // 设置文本光标
            m_tedContent->setTextCursor(textcursor);
        }
    }
}
// 设置 标题和内容 左右边距
void TextEditWorkspacePage::setLeftRightMargin(int value)
{
    if(m_openChapterID != "") // 有章节 被打开
    {
        // 设置 标题 左右边距
        m_ledTitle->setTextMargins(value*2,0,value*2,0);

        // 设置 正文内容 左右间距
        // 获取 文档
        QTextDocument *doc =  m_tedContent->document();
        // 获取 文本光标
        QTextCursor textcursor = m_tedContent->textCursor();
        // 遍历 文本块
        for(QTextBlock it = doc->begin(); it !=doc->end();it = it.next())
        {
            // 获取 文本块 格式
            QTextBlockFormat tbf = it.blockFormat();
            // 设置 文本块 左右边距
            tbf.setLeftMargin(value*2); // 设置 左边距
            tbf.setRightMargin(value*2); // 设置 右边距
            // 设置 文本光标 位置
            textcursor.setPosition(it.position());
            // 为 文本块 设置 格式
            textcursor.setBlockFormat(tbf);
            // 设置文本光标
            m_tedContent->setTextCursor(textcursor);
        }
    }
}
// 设置 内容底部边距
void TextEditWorkspacePage::setContentBottomMargin(int value)
{
    if(m_openChapterID != "") // 有章节 被打开
    {
        // 获取 文档 根框架的格式
        QTextFrameFormat format = m_tedContent->document()->rootFrame()->frameFormat();
        // 设置 底部缩进，最少为160
        format.setBottomMargin(160+value*2.5);
        // 为 文档 根框架 设置格式
        m_tedContent->document()->rootFrame()->setFrameFormat(format);
    }
}
// 设置布局
void TextEditWorkspacePage::setContentLayout(int firstLineIndent, int wordSpacing, int lineSpacing,
                                             int leftRightMargin, int bottomMargin)
{
    m_firstLineIndent = firstLineIndent;
    m_wordSpacing = wordSpacing;
    m_lineSpacing = lineSpacing;
    m_leftRightMargin = leftRightMargin;
    m_bottomMargin = bottomMargin;

    setFirstLineIndent(firstLineIndent);
    setWordSpacing(wordSpacing);
    setLineSpacing(lineSpacing);
    setLeftRightMargin(leftRightMargin);
    setContentBottomMargin(bottomMargin);
}
// 自动排版
void TextEditWorkspacePage::autoTypeSetting()
{
    if(m_openChapterID != "") // 有章节 被打开
    {
        setContentLayout(20,0,30,0,0);

        // 发送 自动排版信息
        emit contentLayoutValues_init(20,0,30,0,0);
    }
}

// 搜索查找内容 处理槽函数
void TextEditWorkspacePage::searchText(QString text)
{
    if (text.trimmed().isEmpty())                                               // 查找内容 为空，直接返回
        return;
    else {                                                                      // 查找内容 不为空
        clearSearchText();                                                // 清除 上一次查找的高亮

        m_searchText = text.trimmed();                                          // 保存查找内容

        QTextDocument *document = m_tedContent->document();                     // 获取 文本框 文档
        QTextCursor highlight_cursor(document);                                 // 从文档获取 游标，用于 设置高亮
        QTextCursor cursor(document);                                           // 从文档获取 游标

        cursor.beginEditBlock();                                                //开始编辑 文本块

        bool firstFlag = true;

        while (!highlight_cursor.isNull() && !highlight_cursor.atEnd()) {       // 高亮游标 不为空，并且 未到最后
            highlight_cursor = document->find(m_searchText, highlight_cursor);  //查找指定的文本
            if (!highlight_cursor.isNull()) {                                   // 查找后，高亮游标处不为空（查找到内容）
                m_matchedCursorList << highlight_cursor;                        // 记录 匹配内容的游标
                QTextCharFormat color_format(highlight_cursor.charFormat());    // 获取 高亮处 字符格式
                if(firstFlag) {                                                 // 第一个匹配内容
                    firstFlag = false;                                          // 将第一标志置为false
                    color_format.setBackground(QColor(255, 150, 51));           // 将背景颜色为 当前匹配的橙色
                    m_currentCursorIndex = 0;                                   // 当前游标索引 默认为 0，即第一个
                }
                else                                                            // 其他匹配内容
                    color_format.setBackground(Qt::yellow);                     // 将背景颜色为 普通匹配的黄色
                highlight_cursor.mergeCharFormat(color_format);                 // 设置 高亮游标处 的字符格式
            }
        }
        cursor.endEditBlock();                                                  // 结束编辑 文本块
    }
}
// 搜索上一个查找内容 处理槽函数
void TextEditWorkspacePage::searchPreviousText()
{
    // 获取 当前高亮处 字符格式
    QTextCharFormat currentFormat(m_matchedCursorList[m_currentCursorIndex].charFormat());
    // 将背景颜色为 普通匹配的黄色
    currentFormat.setBackground(Qt::yellow);
    // 设置 普通匹配高亮
    m_matchedCursorList[m_currentCursorIndex].mergeCharFormat(currentFormat);

    // 当前游标索引 -1，即 上一个匹配游标的索引
    m_currentCursorIndex -= 1;

    if(m_currentCursorIndex < 0) // 索引 越界
    {
        // 将当前游标索引 设置为 最后一个 游标位置的索引
        m_currentCursorIndex = m_matchedCursorList.count()-1;
    }

    // 上一个 匹配游标处 的字符格式
    QTextCharFormat previousFormat(m_matchedCursorList[m_currentCursorIndex].charFormat());
    // 将背景颜色为 当前匹配的橙色
    previousFormat.setBackground(QColor(255, 150, 51));           // 设置背景颜色 为橙色
    // 设置 普通匹配高亮
    m_matchedCursorList[m_currentCursorIndex].mergeCharFormat(previousFormat);
}
// 搜索下一个查找内容 处理槽函数
void TextEditWorkspacePage::SearchNextText()
{
    // 获取 当前高亮处 字符格式
    QTextCharFormat currentFormat(m_matchedCursorList[m_currentCursorIndex].charFormat());
    // 将背景颜色为 普通匹配的黄色
    currentFormat.setBackground(Qt::yellow);
    // 设置 普通匹配高亮
    m_matchedCursorList[m_currentCursorIndex].mergeCharFormat(currentFormat);

    // 当前游标索引 +1，即 下一个匹配游标的索引
    m_currentCursorIndex += 1;

    if(m_currentCursorIndex > (m_matchedCursorList.count()-1)) // 索引 越界
    {
        // 将当前游标索引 设置为 第一个 游标位置的索引
        m_currentCursorIndex = 0;
    }

    // 上一个 匹配游标处 的字符格式
    QTextCharFormat previousFormat(m_matchedCursorList[m_currentCursorIndex].charFormat());
    // 将背景颜色为 当前匹配的橙色
    previousFormat.setBackground(QColor(255, 150, 51));
    // 设置 普通匹配高亮
    m_matchedCursorList[m_currentCursorIndex].mergeCharFormat(previousFormat);
}
// 搜索查找内容 清空 处理槽函数
void TextEditWorkspacePage::clearSearchText()
{
    m_searchText=m_searchText.trimmed(); // 去掉 查找内容 左右空格

    if (m_searchText.isEmpty())
        return; // 查找内容 为空，直接返回
    else{
        QTextDocument *document = m_tedContent->document(); // 获取 文本框 文档
        bool found = false; // 查找标志

        QTextCursor highlight_cursor(document); // 从文档获取 高亮游标
        QTextCursor cursor(document); // 从文档获取 游标

        cursor.beginEditBlock();  // 开始编辑 文本块

        // 高亮游标 不为空，并且 未到最后
        while (!highlight_cursor.isNull() && !highlight_cursor.atEnd()){
            highlight_cursor = document->find(m_searchText, highlight_cursor); //查找指定的文本

            if (!highlight_cursor.isNull()){ // 查找后，高亮游标处不为空（查找到内容）
                if (!found) // 将 查找标志 置 为真
                    found = true;

                QTextCharFormat color_format(highlight_cursor.charFormat()); // 获取 高亮处 字符格式
                // 清空背景颜色
                color_format.clearForeground();   //字体颜色
                color_format.clearBackground();  //背景颜色
                // 设置 高亮游标处 的字符格式
                highlight_cursor.setCharFormat(color_format);
            }
        }
        // 结束 编辑
        cursor.endEditBlock();
    }
    // 将 保存的查找内容 清空
    m_searchText = "";
    // 将 匹配游标列表清空
    m_matchedCursorList.clear();
    // 将 当前匹配游标 索引 置为 -1
    m_currentCursorIndex = -1;
}

// 替换 当前 处理槽函数
void TextEditWorkspacePage::replaceCurrent(QString replaceText)
{
    // 打开 确认对话框
    QMessageBox::StandardButton resultBtn = QMessageBox::warning(this,"替换内容","确认替换当前？",QMessageBox::Cancel|QMessageBox::Yes);

    // 点击 取消按钮
    if(resultBtn == QMessageBox::Cancel)
        return; // 直接结束，不做替换操作

    // 点击 确认
    QTextCursor cursor(m_tedContent->document()); // 从文档获取 游标

    // 开始 编辑文本块
    cursor.beginEditBlock();

    // 取消 当前游标处 的高亮，设置 下一个游标 为当前匹配高亮
    QTextCharFormat currentFormat(m_matchedCursorList[m_currentCursorIndex].charFormat()); // 获取 高亮处 字符格式
    // 清空背景颜色
    currentFormat.clearForeground();   //字体颜色
    currentFormat.clearBackground();  //背景颜色
    // 设置 高亮游标处 的字符格式
    m_matchedCursorList[m_currentCursorIndex].setCharFormat(currentFormat);

    // 替换 当前游标处 的匹配内容
    m_matchedCursorList[m_currentCursorIndex].insertText(replaceText);

    // 从 匹配游标列表中 移除当前游标
    m_matchedCursorList.removeAt(m_currentCursorIndex);

    if(m_matchedCursorList.isEmpty()) // 游标列表为空，即已经全部替换
    {
        // 当前 游标索引 置为 -1，即不存在索引
        m_currentCursorIndex = -1;

        // 结束 编辑文本块
        cursor.endEditBlock();
        return; // 退出
    }

    if(m_currentCursorIndex > (m_matchedCursorList.count()-1)) // 索引 越界
    {
        // 检查 当前游标索引，大于最后一个游标索引，即 替换最后一个，设置为最后的游标索引
        m_currentCursorIndex = m_matchedCursorList.count() - 1;
    }

    // 上一个 匹配游标处 的字符格式
    QTextCharFormat previousFormat(m_matchedCursorList[m_currentCursorIndex].charFormat());
    // 将背景颜色为 当前匹配的橙色
    previousFormat.setBackground(QColor(255, 150, 51));
    // 设置 当前匹配高亮
    m_matchedCursorList[m_currentCursorIndex].mergeCharFormat(previousFormat);
    // 结束 编辑文本块
    cursor.endEditBlock();
}
// 替换 全部 处理槽函数
void TextEditWorkspacePage::replaceAll(QString replaceText)
{
    int replaceCount = m_matchedCursorList.count();
    // 打开 确认对话框
    QMessageBox::StandardButton resultBtn = QMessageBox::warning(this,"替换内容",QString("确认替换全部？\n共%1处").arg(replaceCount),QMessageBox::Cancel|QMessageBox::Yes);

    // 点击 取消按钮
    if(resultBtn == QMessageBox::Cancel)
        return; // 直接结束，不做替换操作

    // 点击 确认
    QTextCursor cursor(m_tedContent->document()); // 从文档获取 游标

    // 开始 编辑文本块
    cursor.beginEditBlock();

    // 遍历 匹配游标 列表
    for(int index=0; index<m_matchedCursorList.count(); index++)
    {
        QTextCharFormat currentCursorFormat(m_matchedCursorList[index].charFormat()); // 获取 高亮处 字符格式
        // 清空背景颜色
        currentCursorFormat.clearForeground();   //字体颜色
        currentCursorFormat.clearBackground();
        // 设置 高亮游标处 的字符格式
        m_matchedCursorList[index].setCharFormat(currentCursorFormat);

        m_matchedCursorList[index].insertText(replaceText); // 替换 每一个匹配内容
    }

    // 结束 编辑文本块
    cursor.endEditBlock();

    // 将 保存的查找内容 清空
    m_searchText = "";
    // 将 匹配游标列表清空
    m_matchedCursorList.clear();
    // 将 当前匹配游标 索引 置为 -1
    m_currentCursorIndex = -1;
}

// 设置 初始历史版本信息
void TextEditWorkspacePage::setHistoricalVersionInfo(QString latestVersionID, int backupFileCount)
{
    // 设置 初始的 最新版本ID，版本数量
    m_latestVersionID = latestVersionID;
    m_backupFileCount = backupFileCount;
}
// 恢复 历史版本 处理槽函数
void TextEditWorkspacePage::restoreHistoricalVersion(QString filePath)
{
    qDebug()<<"开始 处理";

    qDebug()<<"开始 备份";
    // 备份 当前内容
    saveAsBackupChapterFile();

    qDebug()<<"获取 历史版本";
    // 打开历史版本文件
    QFile file(filePath); // 通过文件路径创建文件对象
    file.open(QFile::ReadOnly|QIODevice::Text); // 文件打开方式
    QString textContent = file.readAll(); // 获取历史版本文件中全部字符
    file.close(); // 关闭文件

    qDebug()<<"显示 历史版本内容";
    // 清空 文本框内容
    m_tedContent->clear();
    // 添加到 文本框
    QString s="hello";
    qDebug()<<s;
    qDebug()<<textContent;
    m_tedContent->insertPlainText(textContent);
    // m_tedContent->setText(textContent);

    // 设置 初始 行间距
    setLineSpacing(30);
    // 设置 初始 底部边距
    setContentBottomMargin(0);

    // 发送 布局信息信号
    // 首行缩进0，字间距0，行间距30，左右边距0，底部边距0

    qDebug()<<"保存 到主文件";
    // 保存 到主文件
    saveAsMainChapterFile();
}

// 正文内容文本框 右键 弹出自定义菜单 处理槽函数
void TextEditWorkspacePage::do_tedContent_customContextMenuRequested(const QPoint &pos)
{
    // 不使用 pos
    Q_UNUSED(pos);

    // 新建 菜单
    QMenu *menu=new QMenu(this);

    menu->addAction("全选",this,[=](){
        m_tedContent->selectAll();
    });
    menu->addAction("剪切",this,[=](){
        m_tedContent->cut();
    });
    menu->addAction("复制",this,[=](){
        m_tedContent->copy();
    });
    menu->addAction("粘贴",this,[=](){
        m_tedContent->paste();
    });
    menu->addSeparator(); // 添加分隔线
    menu->addAction("关闭",this, closeChapterFile);

    // 运行 弹出菜单
    menu->exec(QCursor::pos());
}


// 绘制 背景
void TextEditWorkspacePage::drawBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    painter->setBrush(QBrush(m_backgroundColor)); // 设置 画刷

    painter->drawRect(QRect(0,0,this->width(),this->height())); // 绘制 控件尺寸 的矩形

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}

// 绘画事件
void TextEditWorkspacePage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
}
