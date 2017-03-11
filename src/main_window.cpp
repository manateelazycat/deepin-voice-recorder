#include <DTitlebar>
#include <QStandardPaths>
#include <QProcess>
#include <QApplication>
#include "main_window.h"
#include "toolbar.h"
#include "utils.h"
#include <QDir>

#include <QDebug>

#include "home_page.h"
#include "record_page.h"
#include "list_page.h"
#include "edit_page.h"
#include <daboutdialog.h>

DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(DMainWindow *parent) : DMainWindow(parent)
{
    menu = new QMenu();
    newRecordAction = new QAction("New record", this);
    connect(newRecordAction, &QAction::triggered, this, &MainWindow::newRecord);
    openSaveDirectoryAction = new QAction("Open save directory", this);
    connect(openSaveDirectoryAction, &QAction::triggered, this, &MainWindow::openSaveDirectory);
    aboutAction = new QAction("About", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    helpAction = new QAction("Help", this);
    connect(helpAction, &QAction::triggered, this, &MainWindow::showHelpManual);
    exitAction = new QAction("Exit", this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exit);
    menu->addAction(newRecordAction);
    menu->addSeparator();
    menu->addAction(openSaveDirectoryAction);
    menu->addAction(aboutAction);
    menu->addAction(helpAction);
    menu->addAction(exitAction);

    this->titleBar()->setMenu(menu);
    this->titleBar()->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

    Toolbar *toolbar = new Toolbar();
    this->titleBar()->setCustomWidget(toolbar, Qt::AlignVCenter, false);
    this->setFixedSize(440, 550);

    layoutWidget = new QWidget();
    this->setCentralWidget(layoutWidget);

    QStringList filters;
    filters << "*.wav";
    QFileInfoList fileInfoList = QDir("/home/andy/Music/Deepin Voice Recorder").entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);

    if (fileInfoList.size() > 0) {
        showListPage();
    } else {
        showHomePage();
    }
}

void MainWindow::showHomePage()
{
    Utils::removeChildren(layoutWidget);

    homePage = new HomePage();
    connect(homePage->recordButton, SIGNAL(clicked()), this, SLOT(showRecordPage()));

    layoutWidget->setLayout(homePage->layout);
}

void MainWindow::showRecordPage()
{
    Utils::removeChildren(layoutWidget);

    recordPage = new RecordPage();
    connect(recordPage->finishButton, SIGNAL(clicked()), this, SLOT(showListPage()));

    layoutWidget->setLayout(recordPage->layout);
}

void MainWindow::showListPage()
{
    Utils::removeChildren(layoutWidget);

    listPage = new ListPage();
    connect(listPage->recordButton, SIGNAL(clicked()), this, SLOT(showRecordPage()));

    layoutWidget->setLayout(listPage->layout);
}

void MainWindow::showEditPage()
{
    Utils::removeChildren(layoutWidget);

    editPage = new EditPage();
    connect(editPage->editButton, SIGNAL(clicked()), this, SLOT(showListPage()));

    layoutWidget->setLayout(editPage->layout);
}

void MainWindow::newRecord()
{
    showRecordPage();
}

void MainWindow::openSaveDirectory()
{
    QDir musicDirectory = QDir(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());
    QString subDirectory = "Deepin Voice Recorder";
    QString recordDirectory = musicDirectory.filePath(subDirectory);
    QProcess::startDetached("gvfs-open", QStringList() << recordDirectory);
}

void MainWindow::showAbout()
{
    QString descriptionText = "Deepin Voice Recorder is a beautiful design and simple function voice recorder.";
    QString acknowledgementLink = "https://www.deepin.org/acknowledgments/deepin-voice-recorder#thanks";

    auto *aboutDlg = new Dtk::Widget::DAboutDialog();
    aboutDlg->setWindowModality(Qt::WindowModal);
    aboutDlg->setWindowIcon(QPixmap::fromImage(QImage(Utils::getImagePath("logo.png"))));
    aboutDlg->setProductIcon(QPixmap::fromImage(QImage(Utils::getImagePath("logo.png"))));
    aboutDlg->setProductName("Deepin Voice Recorder");
    aboutDlg->setVersion("Version: 1.0");
    aboutDlg->setDescription(descriptionText + "\n");
    aboutDlg->setAcknowledgementLink(acknowledgementLink);
    aboutDlg->show();
}

void MainWindow::showHelpManual()
{
    static QProcess *m_manual = nullptr;
    if (NULL == m_manual) {
        m_manual =  new QProcess();
        const QString pro = "dman";
        const QStringList args("deepin-voice-recorder");
        m_manual->connect(m_manual, static_cast<void(QProcess::*)(int)>(&QProcess::finished), this, [ = ](int) {
                m_manual->deleteLater();
                m_manual = nullptr;
            });
        m_manual->start(pro, args);
    }
}

void MainWindow::exit()
{
    QApplication::quit();
}
