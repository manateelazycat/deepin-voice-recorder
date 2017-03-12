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
#include <daboutdialog.h>

DWIDGET_USE_NAMESPACE

const int MainWindow::PAGE_TYPE_HOME = 1;
const int MainWindow::PAGE_TYPE_RECORD = 2;
const int MainWindow::PAGE_TYPE_LIST = 3;

MainWindow::MainWindow(DMainWindow *parent) : DMainWindow(parent)
{
    menu = new QMenu();
    newRecordAction = new QAction(tr("New recording"), this);
    connect(newRecordAction, &QAction::triggered, this, &MainWindow::newRecord);
    openSaveDirectoryAction = new QAction(tr("Open the save directory"), this);
    connect(openSaveDirectoryAction, &QAction::triggered, this, &MainWindow::openSaveDirectory);
    aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    helpAction = new QAction(tr("Help"), this);
    connect(helpAction, &QAction::triggered, this, &MainWindow::showHelpManual);
    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, &QAction::triggered, this, &MainWindow::exit);
    menu->addAction(newRecordAction);
    menu->addSeparator();
    menu->addAction(openSaveDirectoryAction);
    menu->addAction(aboutAction);
    menu->addAction(helpAction);
    menu->addAction(quitAction);

    this->titleBar()->setMenu(menu);
    this->titleBar()->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

    Toolbar *toolbar = new Toolbar();
    this->titleBar()->setCustomWidget(toolbar, Qt::AlignVCenter, false);
    this->setFixedSize(440, 550);

    layoutWidget = new QWidget();
    this->setCentralWidget(layoutWidget);

    QFileInfoList fileInfoList = Utils::getRecordingFileinfos();

    if (fileInfoList.size() > 0) {
        showListPage("");
    } else {
        showHomePage();
    }
}

void MainWindow::showHomePage()
{
    pageType = PAGE_TYPE_HOME;
    
    Utils::removeChildren(layoutWidget);

    homePage = new HomePage();
    connect(homePage->recordButton, SIGNAL(clicked()), this, SLOT(showRecordPage()));

    layoutWidget->setLayout(homePage->layout);
}

void MainWindow::showRecordPage()
{
    pageType = PAGE_TYPE_RECORD;
    
    Utils::removeChildren(layoutWidget);

    recordPage = new RecordPage();
    connect(recordPage->finishButton, &DImageButton::clicked, [=] () {
            showListPage(recordPage->getRecordingFilepath());
        });

    layoutWidget->setLayout(recordPage->layout);
}

void MainWindow::showListPage(QString recordingPath)
{
    pageType = PAGE_TYPE_LIST;
    
    Utils::removeChildren(layoutWidget);

    listPage = new ListPage();
    connect(listPage, SIGNAL(clickRecordButton()), this, SLOT(showRecordPage()));
    
    if (recordingPath != "") {
        listPage->selectItemWithPath(recordingPath);
    }

    layoutWidget->setLayout(listPage->layout);
}

void MainWindow::newRecord()
{
    if (pageType == PAGE_TYPE_RECORD) {
        recordPage->stopRecord();
    }
    
    showRecordPage();
}

void MainWindow::openSaveDirectory()
{
    QProcess::startDetached("gvfs-open", QStringList() << Utils::getRecordingSaveDirectory());
}

void MainWindow::showAbout()
{
    QString descriptionText = tr("Deepin Voice Recorder is a beautiful design and simple function voice recorder.");
    QString acknowledgementLink = "https://www.deepin.org/acknowledgments/deepin-voice-recorder#thanks";

    auto *aboutDlg = new Dtk::Widget::DAboutDialog();
    aboutDlg->setWindowModality(Qt::WindowModal);
    aboutDlg->setWindowIcon(QPixmap::fromImage(QImage(Utils::getQrcPath("logo.png"))));
    aboutDlg->setProductIcon(QPixmap::fromImage(QImage(Utils::getQrcPath("logo.png"))));
    aboutDlg->setProductName(tr("Deepin Voice Recorder"));
    aboutDlg->setVersion(QString("%s: 1.0").arg(tr("Version")));
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
