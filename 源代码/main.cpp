#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 设置程序名称（任务管理器、窗口标题等）
    a.setApplicationName("爱莉爱莉爱");         // 应用程序内部名称
    a.setApplicationDisplayName("爱莉爱莉爱");   // 显示给用户的友好名称
    a.setApplicationVersion("1.0.0");         // 版本号（可选）

    // 设置窗口图标（程序运行时窗口左上角的图标）
    a.setWindowIcon(QIcon(":/biao.ico"));  // 资源路径
    Widget w;
    w.show();
    return a.exec();
}
