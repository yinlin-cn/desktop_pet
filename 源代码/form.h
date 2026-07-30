#ifndef FORM_H
#define FORM_H
#include<filesystem>
#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();
    void init();
    void shezhislot(bool a,bool b,bool c,bool d,bool e,int f,std::filesystem::path g,std::filesystem::path h);

private slots:
    void on_pushButton_clicked();

    void on_checkBox_5_clicked(bool checked);

    void on_checkBox_6_clicked(bool checked);

    void on_checkBox_7_clicked(bool checked);

    void on_pushButton_2_clicked(bool checked);

    void on_pushButton_3_clicked(bool checked);

private:
    Ui::Form *ui;
    bool ailimove1;
    bool playwav1;
    bool ifshow1;
    void centerOnScreen();
    bool useownwenjianjia;
    std::filesystem::path zidingwenjianjiadizhi;
    bool xianshibofangqi;
    int bofangmoshi;
    QImage* beijing;
    std::filesystem::path tupianlujing;

signals:
    void shezhisignal(bool ailimove1, bool playwav1, bool ifshow1,bool xianshibofangqi,bool useownwenjianjia,int bofangmoshi,std::filesystem::path zidingwenjianjiadizhi);
    void xianshi();

protected:
    // 重写关闭事件（关键）
    void closeEvent(QCloseEvent *event) override;
    // 重写绘图事件：绘制所有拼图块（核心！）
    void paintEvent(QPaintEvent *event) override;
};

#endif // FORM_H
