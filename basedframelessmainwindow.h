#ifndef BASEDFRAMELESSMAINWINDOW_H
#define BASEDFRAMELESSMAINWINDOW_H

#include <QMainWindow>

class BasedFramelessMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    BasedFramelessMainWindow(QWidget *parent = nullptr);
    ~BasedFramelessMainWindow();
};
#endif // BASEDFRAMELESSMAINWINDOW_H
