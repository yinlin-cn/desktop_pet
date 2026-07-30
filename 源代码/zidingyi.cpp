#include "zidingyi.h"
#include "ui_zidingyi.h"
#include<QPainter>
#include<filesystem>
#include<QDir>
#include<QFileDialog>
#include<QColor>
#include<QColorDialog>
#include<QCloseEvent>
zidingyi::zidingyi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::zidingyi)
{
    ui->setupUi(this);
    setVisible(false);
    setWindowTitle("自定义");
}

zidingyi::~zidingyi()
{
    delete ui;
}

void zidingyi::init(std::filesystem::path shezhibeijing1,
                    std::filesystem::path bofangqibeijing1,
                    std::filesystem::path yvshemusic11,
                    std::filesystem::path yvshemusic21,
                    std::filesystem::path jingzhidonghua1,
                    std::filesystem::path danjidonghua1,
                    std::filesystem::path tuodongdonghua1,QColor color1,int target1,int h1)
{
    shezhibeijing=shezhibeijing1;
    QString shezhi=QString::fromStdString(shezhibeijing.string());
    ui->lineEdit->setText(shezhi);
    bofangqibeijing=bofangqibeijing1;
    QString bofangqi=QString::fromStdString(bofangqibeijing.string());
    ui->lineEdit_2->setText(bofangqi);
    yvshemusic1=yvshemusic11;
    QString yvshe1=QString::fromStdString(yvshemusic1.string());
    ui->lineEdit_7->setText(yvshe1);
    yvshemusic2=yvshemusic21;
    QString yvshe2=QString::fromStdString(yvshemusic2.string());
    ui->lineEdit_9->setText(yvshe2);
    jingzhidonghua=jingzhidonghua1;
    QString jingzhi=QString::fromStdString(jingzhidonghua.string());
    ui->lineEdit_3->setText(jingzhi);
    danjidonghua=danjidonghua1;
    QString danji=QString::fromStdString(danjidonghua.string());
    ui->lineEdit_5->setText(danji);
    tuodongdonghua=tuodongdonghua1;
    QString tuodong=QString::fromStdString(tuodongdonghua.string());
    ui->lineEdit_4->setText(tuodong);
    color=color1;
    QString style = QString("QLineEdit { background-color: %1; }")
                        .arg(color.name());
    ui->lineEdit_6->setStyleSheet(style);
    beijing=new QImage(shezhi);
    target=target1;
    ui->spinBox->setValue(target/10);
    h=h1;
    ui->spinBox_2->setValue(h/10);
    update();
}

void zidingyi::paintEvent(QPaintEvent *event) {
    // 先调用父类的paintEvent，确保其他绘制正常
    QWidget::paintEvent(event);

    // 缩放图片（按窗口当前大小，保持比例扩展）
    QImage scaledPix = beijing->scaled(
        this->size(),  // 窗口当前大小
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation
        );

    // 绘制缩放后的图片，居中对齐（截取中间部分）
    QPainter painter(this);
    // 计算绘制位置：居中显示（图片可能比窗口大，只画中间区域）
    int x = (this->width() - scaledPix.width()) / 2;
    int y = (this->height() - scaledPix.height()) / 2;
    painter.drawImage(x, y, scaledPix);
}


void zidingyi::on_pushButton_8_clicked(bool checked)
{
    // 将std::string转换为std::filesystem::path
    //fs::path fs_path(str_path);  // 直接通过构造函数转换
    QString a=ui->lineEdit->text();
    std::filesystem::path fa(a.toStdString());
    shezhibeijing=fa;
    QString b=ui->lineEdit_2->text();
    std::filesystem::path fb(b.toStdString());
    bofangqibeijing=fb;
    QString c=ui->lineEdit_3->text();
    std::filesystem::path fc(c.toStdString());
    jingzhidonghua=fc;
    QString d=ui->lineEdit_4->text();
    std::filesystem::path fd(d.toStdString());
    tuodongdonghua=fd;
    QString e=ui->lineEdit_5->text();
    std::filesystem::path fe(e.toStdString());
    danjidonghua=fe;
    QString f=ui->lineEdit_7->text();
    std::filesystem::path ff(f.toStdString());
    yvshemusic1=ff;
    QString g=ui->lineEdit_9->text();
    std::filesystem::path fg(g.toStdString());
    yvshemusic2=fg;
    target=ui->spinBox->value()*10;
    h=ui->spinBox_2->value()*10;
    emit shezhixinghao( shezhibeijing,
                       bofangqibeijing,
                       yvshemusic1,
                       yvshemusic2,
                       jingzhidonghua,
                       danjidonghua,
                       tuodongdonghua,
                       color,target,h);
    hide();
}


void zidingyi::on_pushButton_3_clicked(bool checked)
{
    QString dirPath = QFileDialog::getOpenFileName(
        this,
        tr("选择GIF文件"),
        QDir::currentPath(),
        tr("GIF图片 (*.gif);;所有文件 (*)")
        );

    // 判断用户是否选择了文件夹（未选择则dirPath为空）
    if (!dirPath.isEmpty()) {
        qDebug() << "选中的文件夹路径：" << dirPath;
        jingzhidonghua=dirPath.toStdString();
        ui->lineEdit_3->setText(dirPath);
    } else {
        qDebug() << "用户取消了选择";
        return;
    };
}


void zidingyi::on_pushButton_5_clicked(bool checked)
{
    QString dirPath = QFileDialog::getOpenFileName(
        this,
        tr("选择GIF文件"),
        QDir::currentPath(),
        tr("GIF图片 (*.gif);;所有文件 (*)")
        );

    // 判断用户是否选择了文件夹（未选择则dirPath为空）
    if (!dirPath.isEmpty()) {
        qDebug() << "选中的文件夹路径：" << dirPath;
        danjidonghua=dirPath.toStdString();
        ui->lineEdit_5->setText(dirPath);
    } else {
        qDebug() << "用户取消了选择";
        return;
    };
}


void zidingyi::on_pushButton_4_clicked(bool checked)
{
    QString dirPath = QFileDialog::getOpenFileName(
        this,
        tr("选择GIF文件"),
        QDir::currentPath(),
        tr("GIF图片 (*.gif);;所有文件 (*)")
        );

    // 判断用户是否选择了文件夹（未选择则dirPath为空）
    if (!dirPath.isEmpty()) {
        qDebug() << "选中的文件夹路径：" << dirPath;
        tuodongdonghua=dirPath.toStdString();
        ui->lineEdit_4->setText(dirPath);
    } else {
        qDebug() << "用户取消了选择";
        return;
    };
}


void zidingyi::on_pushButton_2_clicked(bool checked)
{
    QString dirPath = QFileDialog::getOpenFileName(
        this,
        tr("选择图片文件"),
        QDir::currentPath(),
        tr("图片文件 (*.png *.jpg *.jpeg);;所有文件 (*)")  // 过滤器：支持png、jpg、jpeg
        );

    // 判断用户是否选择了文件夹（未选择则dirPath为空）
    if (!dirPath.isEmpty()) {
        qDebug() << "选中的文件夹路径：" << dirPath;
        bofangqibeijing=dirPath.toStdString();
        ui->lineEdit_2->setText(dirPath);
    } else {
        qDebug() << "用户取消了选择";
        return;
    };
}


void zidingyi::on_pushButton_clicked(bool checked)
{
    QString dirPath = QFileDialog::getOpenFileName(
        this,
        tr("选择图片文件"),
        QDir::currentPath(),
        tr("图片文件 (*.png *.jpg *.jpeg);;所有文件 (*)")  // 过滤器：支持png、jpg、jpeg
        );

    // 判断用户是否选择了文件夹（未选择则dirPath为空）
    if (!dirPath.isEmpty()) {
        qDebug() << "选中的文件夹路径：" << dirPath;
        shezhibeijing=dirPath.toStdString();
        ui->lineEdit->setText(dirPath);
    } else {
        qDebug() << "用户取消了选择";
        return;
    };
}


void zidingyi::on_pushButton_7_clicked(bool checked)
{
    QString dirPath = QFileDialog::getOpenFileName(
        this,
        tr("选择WAV文件"),  // 标题可改为更贴合的"选择WAV文件"
        QDir::currentPath(),
        tr("WAV音频 (*.wav);;所有文件 (*)")  // 过滤器：只显示.wav文件
        );

    // 判断用户是否选择了文件夹（未选择则dirPath为空）
    if (!dirPath.isEmpty()) {
        qDebug() << "选中的文件夹路径：" << dirPath;
        yvshemusic1=dirPath.toStdString();
        ui->lineEdit_7->setText(dirPath);
    } else {
        qDebug() << "用户取消了选择";
        return;
    };
}


void zidingyi::on_pushButton_9_clicked(bool checked)
{
    QString dirPath = QFileDialog::getOpenFileName(
        this,
        tr("选择WAV文件"),  // 标题可改为更贴合的"选择WAV文件"
        QDir::currentPath(),
        tr("WAV音频 (*.wav);;所有文件 (*)")  // 过滤器：只显示.wav文件
        );

    // 判断用户是否选择了文件夹（未选择则dirPath为空）
    if (!dirPath.isEmpty()) {
        qDebug() << "选中的文件夹路径：" << dirPath;
        yvshemusic2=dirPath.toStdString();
        ui->lineEdit_9->setText(dirPath);
    } else {
        qDebug() << "用户取消了选择";
        return;
    };
}


void zidingyi::on_pushButton_6_clicked(bool checked)
{
    // 弹出颜色选择框，初始颜色为当前颜色，父窗口为this
    QColor color1 = QColorDialog::getColor(
        color,    // 初始颜色
        this,              // 父窗口
        "选择颜色"         // 对话框标题
        );

    // 判断用户是否选择了有效颜色（未点击"取消"）
    if (color1.isValid()) {
        color = color1;
        QString style = QString("QLineEdit { background-color: %1; }")
                            .arg(color.name());
        ui->lineEdit_6->setStyleSheet(style);
    }
}

void zidingyi::closeEvent(QCloseEvent *event)
{
    // 1. 隐藏窗口（替代默认的销毁行为）
    this->hide();

    // 2. 忽略关闭事件（告诉Qt：我们自己处理了，不要销毁窗口）
    event->ignore();
}

void zidingyi::on_pushButton_10_clicked(bool checked)
{
    emit taicishezhi();
}


void zidingyi::on_pushButton_11_clicked(bool checked)
{
    shezhibeijing=":/tubiao.png";
    bofangqibeijing=":/bofangqi.png";
    yvshemusic1=":/music.wav";
    yvshemusic2=":/elysiam realm.wav";
    jingzhidonghua=":/1758263119038(1).gif";
    danjidonghua=":/0334aad0-61f2-4854-8d35-0cf70a8b1472.gif";
    tuodongdonghua=":/1680313875248.gif";
    QColor pink2("pink");
    color=pink2;
    target=150;
    h=50;
    emit shezhixinghao( shezhibeijing,
                       bofangqibeijing,
                       yvshemusic1,
                       yvshemusic2,
                       jingzhidonghua,
                       danjidonghua,
                       tuodongdonghua,
                       color,
                       target,h);
    hide();
}



