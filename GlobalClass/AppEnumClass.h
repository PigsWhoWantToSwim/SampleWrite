#ifndef APPENUMCLASS_H
#define APPENUMCLASS_H

/******************************************************
 *  本头文件用于 定义 一些常量和枚举类
******************************************************/

/************************************** 头文件 *********************************************************/
#include <QWidget>
#include <QPixmap>
#include <QRandomGenerator>
#include <QSettings>
#include <QDir>


/****************************************** 控件常量 ***************************************************/

// 首页
#define HOMEWORKSPACEHOMEPAGE_CONTAINERLAYOUT_MARGINLEFT 50
#define HOMEWORKSPACEHOMEPAGE_CONTAINERLAYOUT_MARGINRIGHT 0
#define HOMEWORKSPACEHOMEPAGE_CONTAINERLAYOUT_MARGINTOP 50
#define HOMEWORKSPACEHOMEPAGE_CONTAINERLAYOUT_MARGINBOTTOM 30
#define HOMEWORKSPACEHOMEPAGE_CONTAINERLAYOUT_HORIZONTALSPACING 60

#define HOMEWORKSPACEHOMEPAGE_NOVELWIDGET_HORIZONTALSPACING 80
#define HOMEWORKSPACEHOMEPAGE_NOVELWIDGET_VERTICALSPACING 30

// 首页 标题栏
#define HOMETITLEBAR_HEIGHT 80

// 首页 导航栏
#define HOMENAVIGATIONBAR_WIDTH 220

// 设置页
#define HOMEWORKSPACESETTINGSPAGE_CONTAINERLAYOUT_MARGINSLEFT 40
#define HOMEWORKSPACESETTINGSPAGE_CONTAINERLAYOUT_MARGINSRIGHT 40
#define HOMEWORKSPACESETTINGSPAGE_CONTAINERLAYOUT_MARGINSTOP 40
#define HOMEWORKSPACESETTINGSPAGE_CONTAINERLAYOUT_MARGINSNBOTTOM 40

#define HOMEWORKSPACESETTINGSPAGE_THEMELAYOUT_MARGINSLEFT 50
#define HOMEWORKSPACESETTINGSPAGE_THEMELAYOUT_MARGINSRIGHT 30
#define HOMEWORKSPACESETTINGSPAGE_THEMELAYOUT_MARGINSTOP 30
#define HOMEWORKSPACESETTINGSPAGE_THEMELAYOUT_MARGINSNBOTTOM 30

#define HOMEWORKSPACESETTINGSPAGE_THEMEBUTTON_HORIZONTALSPACING 40
#define HOMEWORKSPACESETTINGSPAGE_THEMEBUTTON_VERTICALSPACING 40

// 主页 主题按钮
#define HOME_THEMETICKBUTTON_WIDTH 120
#define HOME_THEMETICKBUTTON_HEIGHT 75
// 编辑页 主题按钮
#define EDIT_THEMETICKBUTTON_WIDTH 72
#define EDIT_THEMETICKBUTTON_HEIGHT 45

// 小说控件
#define NOVELWIDGET_WIDTH 140
#define NOVELWIDGET_HEIGHT 230
#define NOVELWIDGET_SHADOW_WIDTH 10
#define NOVELWIDGET_COVER_RECTRADIUS 15
#define NOVELWIDGET_TITLE_HEIGHT 30
#define NOVELWIDGET_AUTHOR_HEIGHT 20

// 修改小说对话框
#define MODIFYNOVELDIALOG_WIDTH 480
#define MODIFYNOVELDIALOG_HEIGHT 300
#define MODIFYNOVELDIALOG_COVERCONTAINER_WIDTH 135
#define MODIFYNOVELDIALOG_COVERCONTAINER_HEIGHT 175
#define MODIFYNOVELDIALOG_COVER_WIDTH 120
#define MODIFYNOVELDIALOG_COVER_HEIGHT 160
#define MODIFYNOVELDIALOG_CANCELCOVERICON_MINSIZE 20
#define MODIFYNOVELDIALOG_CANCELCOVERICON_MAXSIZE 30

// 裁剪图片对话框
#define IMAGECROPPERDIALOG_WIDTH 720
#define IMAGECROPPERDIALOG_HEIGHT 480

// 图片裁剪控件
#define IMAGECROPPERWIDGET_WIDTH 640
#define IMAGECROPPERWIDGET_HEIGHT 360
#define IMAGECROPPERWIDGET_PEN_WIDTH 1
#define IMAGECROPPERWIDGET_CONTROLPOINT_SIZE 16

// 首页回站页面
#define HOMERECYCLEPAGE_TITLEBAR_HEIGHT 65
#define HOMERECYCLEPAGE_STATUSBAR_HEIGHT 80
// 回收作品控件
#define RECYCLEWORK_HEIGHT 65


// 编辑页 标题栏
#define EDITTITLEBAR_HEIGHT 30

// 编辑页 工具栏
#define EDITTOOLBAR_HEIGHT 65
#define EDITTOOLBAR_BUTTONICON_SIZE 20
#define EDITTOOLBAR_BUTTON_WIDTH 75
#define EDITTOOLBAR_BUTTON_HEIGHT 55

// 编辑页 目录栏
#define EDITCATALOGBAR_MAX_WIDTH 500
#define EDITCATALOGBAR_MIN_WIDTH 180

// 编辑栏 目录按钮及排序按钮
#define EDITCATALOGBAR_DIRECTORYBUTTON_WIDTH 40
#define EDITCATALOGBAR_SORT1BUTTON_WIDTH 20

// 重命名 节点对话框
#define RENAMEDIRECTORYITEMDIALOG_WIDTE 360
#define RENAMEDIRECTORYITEMDIALOG_HEIGHT 180

// 查找栏
#define FINDBAR_HEIGHT 40
#define FINDBAR_LAYOUT_LEFTMARGIN 5
#define FINDBAR_LAYOUT_RIGHTMARGIN 5
#define FINDBAR_LAYOUT_TOPMARGIN 0
#define FINDBAR_LAYOUT_BOTTOMMARGIN 0
#define FINDBAR_LAYOUT_SPACING 5

#define FINDBAR_LINEEDIT_HEIGHT 30
#define FINDBAR_SEARCHBUTTON_WIDTH 60
#define FINDBAR_BUTTON_SIZE 30
#define FINDBAR_SWITCHBUTTON_WIDTH 60

// 替换栏
#define REPLACEBAR_HEIGHT 40
#define REPLACEBAR_LAYOUT_LEFTMARGIN 5
#define REPLACEBAR_LAYOUT_RIGHTMARGIN 5
#define REPLACEBAR_LAYOUT_TOPMARGIN 0
#define REPLACEBAR_LAYOUT_BOTTOMMARGIN 0
#define REPLACEBAR_LAYOUT_SPACING 5

#define REPLACEBAR_LINEEDIT_HEIGHT 30
#define REPLACEBAR_BUTTON_WIDTH 80
#define REPLACEBAR_BUTTON_HEIGHT 30

// 文本编辑器
#define MAX_BACKUPFILE_NUMBER 50
#define BACKUPFILE_TIME 60000

// 状态栏
#define EDITSTATUSBAR_HEIGHT 30

// 侧边栏
#define SIDEBAR_MAX_WIDTH 500
#define SIDEBAR_MIN_WIDTH 160

// 侧边栏 主题页面
#define SIDEBARTHEMEPAGE_THEMEBUTTON_HORIZONTALSPACING 50

// 字体页面
#define SIDEBARFONTPAGE_BUTTON_HEIGHT 30;

// 恢复历史版本 对话框
#define RESTOREHISTORICALVERSIONDIALOG_WIDTH 600
#define RESTOREHISTORICALVERSIONDIALOG_HEIGHT 400

// 历史版本按钮
#define HISTORICALVERSIONBUTTON_HEIGHT 65

// 设置侧边栏 开关按钮
#define SIDEBARSETTINGPAGE_SWITCHBUTTON_WIDTH 50
#define SIDEBARSETTINGPAGE_SWITCHBUTTON_HEIGHT 25

// 一天的秒数
#define DAY_SECOND 86400

/********************************************* 枚举类 **************************************************/

// 被点击的 按钮 名称
enum class TitleBarClickedButton{
    MinimizedButton = 0, // 最小化窗口
    MaximizedButton = 1, // 最大化窗口
    RestoreButton = 2, // 向下还原窗口
    CloseButton = 3 // 关闭窗口
};

// 首页工作区 的页面名称
enum class HomeMainWindowPage{
    HomePage = 0,
    FindPage = 1,
    RecyclePage = 2,
    SettingsPage = 3,
    AboutPage = 4 //,
    // ThemePage = 5
};

// 作品类型
enum class RecycledWorkType{
    Novel = 0,
    Volume = 1,
    Chapter = 2,
    WorkRelated = 3
};

// 开关按钮 风格
enum class TickButtonStyle {
    rectButton = 0,   // 圆角矩形
    circleButton = 1, // 圆形
    rectImageButton = 2, // 圆角矩形图片
    circleImageButton = 3 // 圆形图片
};

// 开关按钮 风格
enum class SwitchButtonStyle {
    rectButton = 0,     // 圆角矩形
    circleInButton = 1, // 内圆形
    circleOutButton = 2,// 外圆形
    imageButton = 3     // 图片
};

// 光标在截取框的位置，包括内部，外部，及8个方位
enum class ImageCropBoxPosition{
    CROP_BOX_OUTSIZD = 0,
    CROP_BOX_INSIDE = 1,
    CROP_BOX_TOP_LEFT = 2,
    CROP_BOX_TOP = 3,
    CROP_BOX_TOP_RIGHT = 4,
    CROP_BOX_RIGHT = 5,
    CROP_BOX_BOTTOM_RIGHT = 6,
    CROP_BOX_BOTTOM = 7,
    CROP_BOX_BOTTOM_LEFT = 8,
    CROP_BOX_LEFT = 9
};

// 编辑页面工具按钮
enum class EditToolBarButton{
    ShelfButton = 0,
    CatalogButton = 1,
    UndoButton = 2,
    RedoButton = 3,
    TypeSettingButton = 4,
    FindReplaceButtton = 5,
    ThemeButton = 6,
    FontButton = 7,
    LayoutButton = 8,
    HistoricalButton = 9,
    SettingsButton = 10
};


// 目录页面
enum class CatalogPage{
    DirectoryTreePage = 0,
    DirectoryListPage = 1
};
// 目录控件 显示列名
enum class DirectoryTextColume{
    TitleColume = 0,
    CountColume = 1
};
// 目录控件 数据列名
enum class DirectoryDateColume{
    IDColume = 0,
    ParentIDColume = 1,
    IndexColume = 2
};
// 目录控件 节点类型
enum class DirectoryItemType{
    VolumeItem = 1001,
    ChapterItem = 1002
};

// 编辑页 工作区
enum class EditWorkspacePage{
    BlankPage = 0,
    TextPage = 1
};

// 文本编辑控件
enum class TextEditArea{
    TitleArea = 0,
    ContentArea = 1
};

// 侧边栏 页面
enum class SideBarPage{
    ThemePage = 0,
    FontPage = 1,
    LayoutPage = 2,
    HistoricalPage = 3,
    SettingPage = 4
};

// 单例对象
extern QSettings *settings;


/**************************************** 通用函数 *****************************************************/

/* 将RGB颜色 转变为 带#号 16进制字符串 */
QString converRGB16HexStr(QColor color);

/* 生成 指定长度 的大小写字母字符串 */
QString getRandomLetterString(int length);

/* 设置保存主题 */
void saveCurrentTheme(QString qssFilePath);


#endif // APPENUMCLASS_H
