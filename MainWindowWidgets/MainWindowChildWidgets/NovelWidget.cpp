#include "NovelWidget.h"

/* 小说控件 */
NovelWidget::NovelWidget(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void NovelWidget::initMemberVariable()
{
    // 设置 控件属性
    this->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    this->setFixedSize(NOVELWIDGET_WIDTH, NOVELWIDGET_HEIGHT);
    this->setContextMenuPolicy(Qt::CustomContextMenu); // 设置 自定义 弹出 菜单


    // 设置 小说 信息
    m_novelID = "";
    m_novelTitle = "新建作品";
    m_author = "未知";
    m_novelCoverPath = ":/image/default book cover.png";
    m_novelType = "新作品";

    m_volumesCount = 0;
    m_chaptersCount = 0;
    m_wordsCount = 0;
    m_novelIndex = 0;

    // 设置 控件参数
    m_backgroundColor = QColor(255, 255, 255, 0);
    m_labelKeyWordsColor = QColor(229, 249, 1);
    m_strLabelKeyWordsColor = converRGB16HexStr(m_labelKeyWordsColor);
    m_novelCoverRectRadius = NOVELWIDGET_COVER_RECTRADIUS;
}
// 初始化 布局
void NovelWidget::initLayout()
{
    // 主布局，垂直布局，从上到下依次为 阴影容器，小说名标签，作者名标签
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);

    // 阴影容器
    m_widShadowEffect = new QWidget(this);
    m_widShadowEffect->setAttribute(Qt::WA_TranslucentBackground); // 设置 为透明
    mainLayout->addWidget(m_widShadowEffect);

    // 阴影容器尺寸
    QSize shadowEffectRect(NOVELWIDGET_WIDTH, NOVELWIDGET_HEIGHT - NOVELWIDGET_TITLE_HEIGHT -NOVELWIDGET_AUTHOR_HEIGHT);

    // 阴影容器布局
    QVBoxLayout *shadowEffectLayout = new QVBoxLayout;
    shadowEffectLayout->setSpacing(0);
    shadowEffectLayout->setContentsMargins(NOVELWIDGET_SHADOW_WIDTH, NOVELWIDGET_SHADOW_WIDTH, NOVELWIDGET_SHADOW_WIDTH, NOVELWIDGET_SHADOW_WIDTH);
    m_widShadowEffect->setLayout(shadowEffectLayout);

    // 封面标签尺寸
    QSize novelCoverSize(shadowEffectRect.width() - NOVELWIDGET_SHADOW_WIDTH*2, shadowEffectRect.height()- NOVELWIDGET_SHADOW_WIDTH*2);


    // shadowEffectLayout->addWidget(m_labNovelCover);

    m_swiNovelCover = new SwitchImageWhenEnteringWidget(m_widShadowEffect);
    m_swiNovelCover->setFixedSize(novelCoverSize);
    m_swiNovelCover->setNormalBackgroundPath(":/image/default book cover.png");
    shadowEffectLayout->addWidget(m_swiNovelCover);

    // 阴影效果
    QGraphicsDropShadowEffect *coverShadowEffect = new QGraphicsDropShadowEffect(m_widShadowEffect); // 创建 阴影效果
    coverShadowEffect->setOffset(0, 0); // 阴影效果 不偏移
    coverShadowEffect->setColor(Qt::black); // 设置阴影效果 的颜色QColor(QStringLiteral("black"))
    coverShadowEffect->setBlurRadius(NOVELWIDGET_SHADOW_WIDTH); // 设置 模糊半径
    // m_labNovelCover->setGraphicsEffect(coverShadowEffect); // 为 容器控件设置阴影效果
    m_swiNovelCover->setGraphicsEffect(coverShadowEffect);

    // 小说信息区，垂直布局，从上往下依次为 小说类别，分卷数、章节数、字数
    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->setSpacing(0);
    infoLayout->setContentsMargins(10,10,10,10);
    m_swiNovelCover->setLayout(infoLayout);

    // 小说类别标签
    m_labNovelType = new QLabel(m_swiNovelCover);
    // m_labNovelType->setAlignment(Qt::AlignCenter);
    m_labNovelType->setText(QObject::tr("<font color = %1 size=4>%2</font>").arg(m_strLabelKeyWordsColor).arg(m_novelType));
    m_labNovelType->setVisible(false);
    infoLayout->addWidget(m_labNovelType);
    // 分卷标签
    m_labVolumesCount = new QLabel(m_swiNovelCover);
    m_labVolumesCount->setProperty("WidgetStyleClass", "NovelWidget_InfoLabel");
    m_labVolumesCount->setText(QObject::tr("分卷：<font color = %1 size=4>%2</font>卷").arg(m_strLabelKeyWordsColor).arg(m_volumesCount));
    m_labVolumesCount->setVisible(false);
    infoLayout->addWidget(m_labVolumesCount);
    // 章节标签
    m_labChaptersCount = new QLabel(m_swiNovelCover);
    m_labChaptersCount->setProperty("WidgetStyleClass", "NovelWidget_InfoLabel");
    m_labChaptersCount->setText(QObject::tr("章节：<font color = %1 size=4>%2</font>章").arg(m_strLabelKeyWordsColor).arg(m_chaptersCount));
    m_labChaptersCount->setVisible(false);
    infoLayout->addWidget(m_labChaptersCount);
    // 字数标签
    m_labWordsCount = new QLabel(m_swiNovelCover);
    m_labWordsCount->setProperty("WidgetStyleClass", "NovelWidget_InfoLabel");
    m_labWordsCount->setText(QObject::tr("字数：<font color = %1 size=4>%2</font>字").arg(m_strLabelKeyWordsColor).arg(m_wordsCount));
    m_labWordsCount->setVisible(false);
    infoLayout->addWidget(m_labWordsCount);

    // 作品名称标签
    m_labNovelTitle = new QLabel(this);
    m_labNovelTitle->setProperty("WidgetStyleClass","NovelWidget_TitleLabel");
    m_labNovelTitle->setText(m_novelTitle);
    m_labNovelTitle->setAlignment(Qt::AlignCenter);
    m_labNovelTitle->setAttribute(Qt::WA_TranslucentBackground); // 设置 为透明
    m_labNovelTitle->setFixedSize(NOVELWIDGET_WIDTH, NOVELWIDGET_TITLE_HEIGHT);
    mainLayout->addWidget(m_labNovelTitle);

    // 作者名标签
    m_labAuthor = new QLabel(this);
    m_labAuthor->setProperty("WidgetStyleClass","NovelWidget_AuthorLabel");
    m_labAuthor->setText(m_author);
    m_labAuthor->setAlignment(Qt::AlignCenter);
    m_labAuthor->setAttribute(Qt::WA_TranslucentBackground); // 设置 为透明
    m_labAuthor->setFixedSize(NOVELWIDGET_WIDTH, NOVELWIDGET_AUTHOR_HEIGHT);
    mainLayout->addWidget(m_labAuthor);
}
// 初始化 绑定
void NovelWidget::initConnect()
{

}

// 设置 背景颜色
void NovelWidget::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
// 获取 背景颜色
QColor NovelWidget::getBackgroundColor() const
{
    return m_backgroundColor;
}
// 设置 标签关键字颜色
void NovelWidget::setLabelKeyWordsColor(const QColor &labelKeyWordsColor)
{
    m_labelKeyWordsColor = labelKeyWordsColor;
    m_strLabelKeyWordsColor = converRGB16HexStr(labelKeyWordsColor);
    update();
}
// 获取 标签关键字颜色
QColor NovelWidget::getLabelKeyWordsColor() const
{
    return m_labelKeyWordsColor;
}


// 设置 小说控件信息
void NovelWidget::setNovelInfo(QString novelID, QString novelTitle, QString author,
                               QString novelCoverPath, QString novelType, int volumesCount,
                               int chaptersCount, int wordsCount, int novelIndex)
{
    // 设置 成员变量
    m_novelID = novelID;
    m_novelTitle = novelTitle;
    m_author = author;

    m_novelCoverPath = novelCoverPath;
    if(!QFile::exists(novelCoverPath)) // 封面不存在，使用默认封面
        m_novelCoverPath = QDir::currentPath() + "/Config/NovelCover/default book cover.png";

    m_novelType = novelType;
    m_volumesCount = volumesCount;
    m_chaptersCount = chaptersCount;
    m_wordsCount = wordsCount;
    m_novelIndex = novelIndex;

    // 设置 标签
    m_swiNovelCover->setNormalBackgroundPath(m_novelCoverPath);
    m_labNovelTitle->setText(novelTitle);
    m_labAuthor->setText(author);
    m_labNovelType->setText(QObject::tr("<font color = %1 size=4>%2</font>").arg(m_strLabelKeyWordsColor).arg(novelType));
    m_labVolumesCount->setText(QObject::tr("分卷：<font color = %1 size=4>%2</font>卷").arg(m_strLabelKeyWordsColor).arg(volumesCount));
    m_labChaptersCount->setText(QObject::tr("章节：<font color = %1 size=4>%2</font>章").arg(m_strLabelKeyWordsColor).arg(chaptersCount));
    m_labWordsCount->setText(QObject::tr("字数：<font color = %1 size=4>%2</font>字").arg(m_strLabelKeyWordsColor).arg(wordsCount));

    // 更新
    update();
}

// 导出成TXT
bool NovelWidget::exportNovelWidgetAsTXT(QString savedFilePath)
{
    bool flag = true;

    // 1、新建 导出的TXT文件
    // 新建 文件
    QFile novelFile(savedFilePath); // 文件操作对象
    novelFile.open(QIODevice::WriteOnly|QIODevice::Text); // 创建文件
    // 使用文本流 写入文件
    QTextStream writeStream(&novelFile);

    // 2、打开数据库
    if(AppDatabase::getInstance()->openDatabase()) // 打开数据库成功
    {
        // 写入 小说标题
        writeStream << m_novelTitle << "\n" << m_author << " 著\n" << m_novelType <<"\n";

        // 3、根据 小说ID 按分卷索升序 获取 所有未删除的 分卷ID、标题
        // 准备sql语句
        QSqlQuery volumes_sql;	//执行操作类对象
        // 准备 sql 语句
        volumes_sql.prepare("SELECT volume_id, volume_title FROM volumes WHERE novel_id = :novelID AND is_deleted = 0 ORDER BY volume_index ASC; "); // 使用排序
        volumes_sql.bindValue(":novelID", m_novelID); // 绑定值
        // 执行sql语句
        if(volumes_sql.exec())
        {
            // 读取每一条记录
            while(volumes_sql.next())
            {
                // 获取 分卷ID
                QString volumeID = volumes_sql.value("volume_id").toString();
                QString volumeTitle = volumes_sql.value("volume_title").toString();

                // 写入 分卷标题
                writeStream << volumeTitle << "\n";

                // 4、根据分卷ID 按章节索引升序 获取 所有未删除的 标题、文件路径
                QSqlQuery chapters_sql;	//执行操作类对象
                // 准备 sql 语句
                chapters_sql.prepare("SELECT chapter_title, file_pathURL FROM chapters WHERE volume_id = :volumeID AND is_deleted = 0 ORDER BY chapter_index ASC"); // 使用排序
                chapters_sql.bindValue(":volumeID", volumeID); // 绑定值
                // 执行sql语句
                if(chapters_sql.exec())
                {
                    // 读取每一条 chapter记录
                    while(chapters_sql.next())
                    {

                        QString chapterTitle = chapters_sql.value("chapter_title").toString();
                        QString chapterFilePath = chapters_sql.value("file_pathURL").toString();

                        // 5、根据 章节文件路径 获取内容，并加入 导出文件
                        // 打开文件
                        QFile chapterFile(chapterFilePath); // 通过文件路径创建文件对象
                        chapterFile.open(QFile::ReadOnly|QIODevice::Text); // 文件打开方式
                        QString chapterContent = chapterFile.readAll(); // 获取 章节文件中的 全部字符
                        chapterFile.close(); // 关闭文件

                        // 写入 章节标题，章节内容，并与下一章 再空一行
                        writeStream << chapterTitle << "\n" << chapterContent << "\n\n";

                       // 6、循环4和5 直到全部添加
                    }
                }
                else
                    flag = false;
            }
        }
        else
            flag = false;
    }
    else // 打开数据库 失败
    {
        QMessageBox::warning(this, "错误", "打开数据库失败");
        flag = false;
    }

    // 7、关闭文件、关闭数据库、返回成功
    // 关闭数据库
    AppDatabase::getInstance()->quit();
    // 关闭 文件
    novelFile.close();

    return flag;
}
void NovelWidget::setNovelID(QString novelID)
{
    m_novelID = novelID;
    update();
}
void NovelWidget::setNovelTitle(QString novelTitle)
{
    m_novelTitle = novelTitle;
    m_labNovelTitle->setText(novelTitle);
    update();
}
void NovelWidget::setAuthor(QString author)
{
    m_author = author;
    m_labAuthor->setText(author);
    update();
}
void NovelWidget::setNovelCoverPath(QString novelCoverPath)
{
    m_novelCoverPath = novelCoverPath;
    m_swiNovelCover->setNormalBackgroundPath(novelCoverPath);
    update();
}
void NovelWidget::setNovelType(QString novelType)
{
    m_novelType = novelType;
    m_labNovelType->setText(QObject::tr("<font color = %1 size=4>%2</font>").arg(m_strLabelKeyWordsColor).arg(novelType));
    update();
}
void NovelWidget::setVolumesCount(int volumesCount)
{
    m_volumesCount = volumesCount;
    m_labVolumesCount->setText(QObject::tr("分卷：<font color = %1 size=4>%2</font>卷").arg(m_strLabelKeyWordsColor).arg(volumesCount));
    update();
}
void NovelWidget::setChaptersCount(int chaptersCount)
{
    m_chaptersCount = chaptersCount;
    m_labChaptersCount->setText(QObject::tr("章节：<font color = %1 size=4>%2</font>章").arg(m_strLabelKeyWordsColor).arg(chaptersCount));
    update();
}
void NovelWidget::setWordsCount(int wordsCount)
{
    m_wordsCount = wordsCount;
    m_labWordsCount->setText(QObject::tr("字数：<font color = %1 size=4>%2</font>字").arg(m_strLabelKeyWordsColor).arg(wordsCount));
    update();
}
void NovelWidget::setIndex(int novelIndex)
{
    m_novelIndex = novelIndex;
    update();
}
QString NovelWidget::getNovelID()
{
    return m_novelID;
}
QString NovelWidget::getNovelTitle()
{
    return m_novelTitle;
}
QString NovelWidget::getAuthor()
{
    return m_author;
}
QString NovelWidget::getNovelCoverPath()
{
    return m_novelCoverPath;
}
QString NovelWidget::getNovelType()
{
    return m_novelType;
}
int NovelWidget::getVolumesCount()
{
    return m_volumesCount;
}
int NovelWidget::getChaptersCount()
{
    return m_chaptersCount;
}
int NovelWidget::getWordsCount()
{
    return m_wordsCount;
}
int NovelWidget::getIndex()
{
    return m_novelIndex;
}

// 重写鼠标按下，发送小说控件被点击信号
void NovelWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit novelWidget_mouseLeft_clicked(m_novelID, m_novelTitle);

    QWidget::mousePressEvent(event);
}
// 重写鼠标进入，显示信息标签
void NovelWidget::enterEvent(QEnterEvent *event)
{
    m_swiNovelCover->setEnter(true);

    m_labNovelType->setVisible(true);
    m_labVolumesCount->setVisible(true);
    m_labChaptersCount->setVisible(true);
    m_labWordsCount->setVisible(true);
}
// 重写鼠鼠标离开，隐藏信息标签
void NovelWidget::leaveEvent(QEvent *event)
{
    m_swiNovelCover->setEnter(false);

    m_labNovelType->setVisible(false);
    m_labVolumesCount->setVisible(false);
    m_labChaptersCount->setVisible(false);
    m_labWordsCount->setVisible(false);
}
