#include "mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    http = new QNetworkAccessManager;
    mainWidget = new QWidget;
    layout = new QVBoxLayout;
    topLayout = new QHBoxLayout;
    contentLayout = new QHBoxLayout;
    comboBox = new QComboBox;
    numberEdit = new QLineEdit;
    searchButton = new QPushButton("查询");
    contentBox = new QPlainTextEdit;

    contentBox->setWordWrapMode(QTextOption::WrapAnywhere);

    contentBox->setReadOnly(true);

    topLayout->addWidget(comboBox);
    topLayout->addWidget(numberEdit);
    topLayout->addWidget(searchButton);

    contentLayout->addWidget(contentBox);

    layout->addLayout(topLayout);
    layout->addLayout(contentLayout);

    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);

    initComboBoxItem();

    connect(http, &QNetworkAccessManager::finished, this, &MainWindow::replyFinished);
    connect(numberEdit, &QLineEdit::returnPressed, this, &MainWindow::query);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::query);
}

MainWindow::~MainWindow()
{

}

void MainWindow::initComboBoxItem()
{
    comboBox->addItem("申通快递");
    comboBox->addItem("顺丰快递");
    comboBox->addItem("圆通快递");
    comboBox->addItem("中通快递");
    comboBox->addItem("EMS快递");
    comboBox->addItem("韵达快递");
    comboBox->addItem("天天快递");
}

void MainWindow::query()
{
    if (numberEdit->text().isEmpty())
        return;

    QNetworkRequest request;
    QString type;

    int current = comboBox->currentIndex();

    switch (current) {
    case 0:
        type = "shentong";
        break;
    case 1:
        type = "shunfeng";
        break;
    case 2:
        type = "yuantong";
        break;
    case 3:
        type = "zhongtong";
        break;
    case 4:
        type = "ems";
        break;
    case 5:
        type = "yunda";
        break;
    case 6:
        type = "tiantian";
        break;
    }
    request.setUrl(QString("http://api.kuaidi100.com/api?id=29833628d495d7a5&com=%1&nu=%2&show=0&muti=1&order=desc").arg(type).arg(numberEdit->text()));
    http->get(request);
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();
    int value = data.value("status").toString().toInt();

    QString allContent = "";

    if (value != 1) {
        allContent.append("：( 该单号暂无物流进展，请稍后再试，或检查公司和单号是否有误。");
    }else {
        QJsonArray array = data.value("data").toArray();

        for (int i=0; i<array.size(); ++i) {
            allContent.append(array.at(i).toObject().value("time").toString());
            allContent.append("\n");
            allContent.append(array.at(i).toObject().value("context").toString());
            allContent.append("\n\n");
        }
    }

    contentBox->setPlainText(allContent);
}
