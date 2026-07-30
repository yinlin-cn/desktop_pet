#ifndef ZIDINGYI_H
#define ZIDINGYI_H

#include <QWidget>
#include<filesystem>

/*关于自定义模块的开发，要达成的目标是可以完全自定义桌宠的一切，包括台词、动画、背景图片和动画，要有保存和打开不同方案的功能，实现宠物形态的快速切换
目前菜单栏的使用没有很好掌握，因此开发只能从熟悉的开始，后面再添加，但是为了避免再次出现和之前一样的大规模修改的问题，需要做好功能预留的设计*/

namespace Ui {
class zidingyi;
}

class zidingyi : public QWidget
{
    Q_OBJECT

public:
    explicit zidingyi(QWidget *parent = nullptr);
    ~zidingyi();
    void init(std::filesystem::path shezhibeijing1,
              std::filesystem::path bofangqibeijing1,
              std::filesystem::path yvshemusic11,
              std::filesystem::path yvshemusic21,
              std::filesystem::path jingzhidonghua1,
              std::filesystem::path danjidonghua1,
              std::filesystem::path tuodongdonghua1,
              QColor color1,int target1,int h1);
protected:
    void paintEvent(QPaintEvent *event)override;
    void closeEvent(QCloseEvent *event)override;
private slots:
    void on_pushButton_8_clicked(bool checked);

    void on_pushButton_3_clicked(bool checked);

    void on_pushButton_5_clicked(bool checked);

    void on_pushButton_4_clicked(bool checked);

    void on_pushButton_2_clicked(bool checked);

    void on_pushButton_clicked(bool checked);

    void on_pushButton_7_clicked(bool checked);

    void on_pushButton_9_clicked(bool checked);

    void on_pushButton_6_clicked(bool checked);

    void on_pushButton_10_clicked(bool checked);

    void on_pushButton_11_clicked(bool checked);

private:
    Ui::zidingyi *ui;
    std::filesystem::path shezhibeijing;
    std::filesystem::path bofangqibeijing;
    std::filesystem::path yvshemusic1;
    std::filesystem::path yvshemusic2;
    std::filesystem::path jingzhidonghua;
    std::filesystem::path danjidonghua;
    std::filesystem::path tuodongdonghua;
    QImage* beijing;
    QColor color;
    int target;
    int h;

signals:
    void  shezhixinghao(std::filesystem::path shezhibeijing1,
                       std::filesystem::path bofangqibeijing1,
                       std::filesystem::path yvshemusic11,
                       std::filesystem::path yvshemusic21,
                       std::filesystem::path jingzhidonghua1,
                       std::filesystem::path danjidonghua1,
                       std::filesystem::path tuodongdonghua1,
                       QColor color1,int target1,int h1);
    void taicishezhi();
};

#endif // ZIDINGYI_H

