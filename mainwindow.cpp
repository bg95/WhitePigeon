#include <QtWidgets>

#include "QRecentFilesMenu.h"
#include "OscilloscopeWindow.h"
//#include "musicscene.h"
//#include "musictextitem.h"
//#include "mainwindow.h"
//#include "musicarcitem.h"
//#include "getmusicinfodialog.h"
//#include "musicbracketitem.h"
//#include "musicdotitem.h"
//#include "musiclineitem.h"
#include "WPWindow.h"

#include "mainwindow.h"

MainWindow::MainWindow()
{
    /* Private widget settings */
    oscilloscopeWindow = 0;
    countNumber = 0;

    mdiArea = new QMdiArea;
    mdiArea->setParent(this);
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setActivationOrder(QMdiArea::CreationOrder);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateStatusBar(QMdiSubWindow*)));

    /* MainWindow settings */
    setWindowTitle(tr("WhitePigeon"));
    // setWindowIcon(QIcon(":/images/WhitePigeon.jpg"));
    setContextMenuPolicy(Qt::ActionsContextMenu);
    setCentralWidget(mdiArea);

    /* UI settings */
    createActions();
    createToolBar();
    createMenus();
    //createStatusBar();
    //drawMusic();
    readSettings();
}

MainWindow::~MainWindow()
{
    delete oscilloscopeWindow;
}

void MainWindow::createActions()
{
    newAction = new QAction(this);
    newAction->setText(tr("&New"));
    // newAction->setIcon(QIcon(":/images/new.jpg"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new file"));
    newAction->setToolTip(tr("Create a new file"));
    connect(newAction, SIGNAL(triggered()),
            this, SLOT(newFile()));

    oscilloscopeAction = new QAction(this);
    oscilloscopeAction->setText(tr("&Oscilloscope"));
    // oscilloscopeAction->setIcon(QIcon(":/images/oscilloscope.jpg"));
    // oscilloscopeAction->setShortcut();
    oscilloscopeAction->setStatusTip(tr("Show an oscilloscope"));
    oscilloscopeAction->setToolTip(tr("Show an oscilloscope"));
    //oscilloscopeAction->setCheckable(true);
    connect(oscilloscopeAction, SIGNAL(triggered()), this, SLOT(showOscilloscope()));

    newAction = new QAction(tr("&New"), this);
    newAction->setStatusTip("create a new music");
    newAction->setToolTip("create a new music");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, SIGNAL(triggered()), this, SLOT(getMusicInformation()));
}

void MainWindow::createMenus()
{
    /* Create all menus */
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(oscilloscopeAction);
}

void MainWindow::createToolBar()
{
    /* Create all toolbars */
    fileToolBar = new QToolBar;
    fileToolBar->addAction(newAction);
    fileToolBar->setToolTip(tr("&File"));

    toolBar = new QToolBar;
    toolBar->addAction(oscilloscopeAction);
    toolBar->setToolTip(tr("&Tools"));

    /* add the toolbars to the MainWindow */
    addToolBar(fileToolBar);
    addToolBar(toolBar);
}


/* private slots */

void MainWindow::newFile()
{
    ++countNumber;
    WPWindow *window = new WPWindow;
    window->setWindowTitle(tr("untitled %1").arg(countNumber));
    mdiArea->addSubWindow(window);
    window->show();
}

void MainWindow::showOscilloscope()
{
    if (!oscilloscopeWindow)
        oscilloscopeWindow = new OscilloscopeWindow(this);
    oscilloscopeWindow->show();
}

/*
void MainWindow::drawMusic()
{
    musicScene *scene = new musicScene(this);
    QGraphicsView *view = new QGraphicsView(scene);
    setCentralWidget(view);
    scene->setSceneRect(QRectF(QPointF(0, 0), QSizeF(800, 5000)));
    musicBracketItem *example = new musicBracketItem(QPointF(50, 50), QPointF(50, 100));
    scene->addItem(example);
    musicTextItem *item = new musicTextItem(8);
    scene->addItem(item);
    item->setPos(100, 100);
    //QFont font = QFont("Times", 30, QFont::Bold);
    //item->setFont(font);
    //musicDotItem *dotitem = new musicDotItem(item, musicDotItem::lower);
    //dotitem->setRadius(4);
    //scene->addItem(dotitem);
    item->arrangeDots();
    //musicDotItem *dotitem2 = new musicDotItem(item, musicDotItem::lower);
    //QList <musicTextItem *> items;
    //items.push_back(item);
    //musicLineItem *lineitem = new musicLineItem(item, item);
    //dotitem2->setRadius(4);
    //item->setinterval(20);
    //scene->addItem(dotitem2);
    item->arrangeDots();
    QVector <musicNumberItem *> example1(8);
    for (int i = 0; i < 8; ++i)
    {
        musicNumberItem *tmp = new musicNumberItem(i);
        example1[i] = tmp;
        example1[i]->setPos(60 + 20 * i, 50);
        scene->addItem(example1[i]);
    }
    //int tmp1 = fm.width("1  3  2  2  ");
    //int tmp2 = fm.width("1  3  2  2  1");
    //int tmp3 = fm.width("1  3  2  2  1  7  ");
    //int tmp4 = fm.width("1  3  2  2  1  7  1");

    musicArcItem *example2 = new musicArcItem(
                QPointF(60 + example1[0]->width + 8, 50),
            QPointF(example1[3]->width + 6 + 60 + 3 * 20, 50));
    scene->addItem(example2);
    musicArcItem *example3 = new musicArcItem(
                QPointF(60 + example1[4]->width + 8 + 4 * 20, 50),
            QPointF(example1[6]->width + 6 + 60 + 6 * 20, 50));
    scene->addItem(example3);
    musicArcItem *example3 = new musicArcItem(QPointF(60 + 4, 50),
                 QPointF(52 + 60, 50));
    scene->addItem(example3);
    musicDotItem *example4 = new musicDotItem(QPointF(60 + example1[0]->width + 8, 50 + example1[0]->height+ 5));
    scene->addItem(example4);
    musicDotItem *example5 = new musicDotItem(QPointF(60 + example1[0]->width + 8 + 3 * 20, 50 + example1[0]->height + 5));
    scene->addItem(example5);
    //QGraphicsEllipseItem *dot = new QGraphicsEclipseItem;


    //musicArcItem *example2 = new musicArcItem();
}
*/
/*
void MainWindow::display(WPScore *example)
{
    QGraphicsScene scene(this);
    QGraphicsView view(scene)
}
*/

void MainWindow::updateStatusBar(QMdiSubWindow* window)
{
    if (window)
        statusBar()->showMessage(window->windowTitle());
}


void MainWindow::readSettings()
{
    QSettings settings("QtProject", "WhitePigeon");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(1000, 800)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings("QtProject", "WhitePigeon");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

/*
void MainWindow::getMusicInformation()
{
    getMusicInfoDialog *dialog = new getMusicInfoDialog(this);
    dialog->show();
    //musicTextItem *text = new musicTextItem;
    //text->setPlainText(dialog->name);
    qDebug() << dialog->name;
    //text->setFont(QFont("Times", 20, QFont::Bold));
    //int titleWidth = (dialog->name).length();
    //qreal xpos = 400 - titleWidth * 20 / 2;
    //text->setPos(xpos, 0);
    //scene->addItem(text);
}
*/

/*
void MainWindow::showMusic()
{

}
*/

