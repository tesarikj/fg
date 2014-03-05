#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "scene.h"
#include "textitem.h"
#include "gridlines.h"
#include "libstructure.h"
#include "connection.h"
#include "mouseaction.h"

#include <QtGui>
#include <QLabel>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sceneArea = new Scene(ui->menuEdit);

    //nastavení stejné velikosti scéně i zobrazení scény
    sceneArea->setSceneRect(0,0,ui->graphicsView->width()*2,ui->graphicsView->height()*2);
    ui->graphicsView->setScene(sceneArea);

    mouseAction = new MouseAction(this);
    mouseAction->install(sceneArea);

    createActions();
    createToolBars();
    createGrid();

    connect(sceneArea, SIGNAL(itemInserted(LibStructure*)), this, SLOT(itemInserted(LibStructure*)));
    connect(sceneArea, SIGNAL(textInserted(QGraphicsTextItem*)), this, SLOT(textInserted(QGraphicsTextItem*)));
    connect(sceneArea, SIGNAL(itemSelected(QGraphicsItem*)), this, SLOT(itemSelected(QGraphicsItem*)));

    setCentralWidget(ui->horizontalLayoutWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//------export do různých formátů-------------------------------------------------------
//--------------------------------------------------------------------------------------
//------export do pdf-------------------------------------------------------------------
void MainWindow::exportPDF()
{
    nameFile = QFileDialog::getSaveFileName(this,
      QString::fromUtf8("Exportovat scénu jako.."), "./scena.pdf", "Portable Document Format (*.pdf)");

    if (!nameFile.isEmpty())
    {
      QPrinter tiskarna(QPrinter::HighResolution);
      tiskarna.setOrientation(QPrinter::Landscape);
      tiskarna.setOutputFormat(QPrinter::PdfFormat);
      tiskarna.setOutputFileName(nameFile);

      QPainter painter(&tiskarna);
      painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);  // kvalita vykreslování
      sceneArea->render(&painter); }
}
//------export do jpg------------------------------------------------------------------
void MainWindow::exportJPG()
{
  QPixmap pixmapaSceny(int(sceneArea->width()), int(sceneArea->height()));
  QPainter painter(&pixmapaSceny);

  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing); // kvalita vykreslování
  sceneArea->render(&painter);

  nameFile = QFileDialog::getSaveFileName(this,
    QString::fromUtf8("Exportovat scénu jako.."), "./scena.jpg", "Joint Photographic Expert Group (*.jpg)");

  if (!nameFile.isEmpty())
  { pixmapaSceny.save(nameFile, "JPG"); }
}
//------export do png-------------------------------------------------------------------
void MainWindow::exportPNG()
{
  QPixmap pixmapaSceny(int(sceneArea->width()), int(sceneArea->height()));
  QPainter painter(&pixmapaSceny);

  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing); // kvalita vykreslování
  sceneArea->render(&painter);

  nameFile = QFileDialog::getSaveFileName(this,
    QString::fromUtf8("Exportovat scénu jako.."), "./scena.png", "Portable Network Graphics (*.png)");

  if (!nameFile.isEmpty())
  { pixmapaSceny.save(nameFile, "PNG"); }
}

//-----------vytvoření mřížky------------------------------------------------------------
void MainWindow::createGrid()
{
    gridLines = new GridLines (ui->graphicsView->width(), ui->graphicsView->height(),10);       //číslo 10 je rozteč mřížky;
    sceneArea->addItem(gridLines);
    gridLines->setVisible(false);
}

void MainWindow::resizeEvent(QResizeEvent * )
{
 /*při změně velikosti se změní lupa a tím se zavolá funkce
  *sceneScaleChanged, která vykresluje podle nastavené lupy určitou mřížku*/
    int s = sceneScaleCombo->currentIndex();
    sceneScaleCombo->setCurrentIndex(s-1);
    sceneScaleCombo->setCurrentIndex(s);

    /*QSize wsize = ui->graphicsView->frameSize();
    if (gridLines) gridLines->handleWindowSizeChanged(wsize.width(), wsize.height());     //wsize.width()*10
    qDebug() << "cw frame height" << wsize.height() << " width: " << wsize.width();
    */
}

void MainWindow::grid()             //pouze zobrazení mřížky
{
    if (ui->actionGrid->isChecked()){
        gridLines->setVisible(true);
        ui->actionGrid_2->setChecked(true);
    } else {
        gridLines->setVisible(false);
        ui->actionGrid_2->setChecked(false);
    }
}
void MainWindow::gridContext()      //funkce mřížky v kontextovém menu
{
    if (ui->actionGrid_2->isChecked()){
        ui->actionGrid->setChecked(true);
    } else {
        ui->actionGrid->setChecked(false);
    }
    grid();
}

void MainWindow::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = ui->graphicsView->matrix();
    ui->graphicsView->resetMatrix();
    ui->graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
    ui->graphicsView->scale(newScale, newScale);

//----když se změní lupa, tak je potřeba zvětšit i plochu, kde se bude vykreslovat mřížka
    QSize wsize = ui->graphicsView->frameSize();
    if (gridLines) gridLines->handleWindowSizeChanged(wsize.width()*2, wsize.height()*2);
}

void MainWindow::createActions()            //propojení signálů a slotů pro komunikaci v rámci aplikace
{ 
//-------MENU--------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------FILE MENU---------------------------------------------------------------
    connect(ui->actionExportPDF, SIGNAL(triggered()), this, SLOT(exportPDF()));
    connect(ui->actionExportJPG, SIGNAL(triggered()), this, SLOT(exportJPG()));
    connect(ui->actionExportPNG, SIGNAL(triggered()), this, SLOT(exportPNG()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
//-------EDIT MENU---------------------------------------------------------------
    connect(ui->actionMove_2, SIGNAL(triggered()),this, SLOT(moveItemContext()));
    connect(ui->actionDelete_2,SIGNAL(triggered()),this, SLOT(deleteItem()));
    connect(ui->actionGrid_2, SIGNAL(triggered()),this, SLOT(gridContext()));
    connect(ui->actionLine_2,SIGNAL(triggered()),this, SLOT(insertLineContext()));
//-------TOOLS---------------------------------------------------------------------------------
    connect(ui->actionVisible_EditToolbar,SIGNAL(triggered()),this, SLOT(visible_Toolbars()));
    connect(ui->actionVisible_TextToolbar,SIGNAL(triggered()),this, SLOT(visible_Toolbars()));
//-------ABOUT---------------------------------------------------------------------------------
    connect(ui->actionAbout, SIGNAL(triggered()),this, SLOT(about()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()),this, SLOT(about_Qt()));

//-------TOOLBARS------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//
    connect(ui->actionMove, SIGNAL(triggered()),this, SLOT(moveItem()));
    connect(ui->actionLine,SIGNAL(triggered()),this, SLOT(insertLine()));
    connect(ui->actionDelete,SIGNAL(triggered()),this, SLOT(deleteItem()));
    connect(ui->actionGrid, SIGNAL(triggered()),this, SLOT(grid()));
    connect(ui->actionInsert,SIGNAL(triggered()),this, SLOT(insertItem()));

    connect(ui->actionText, SIGNAL(triggered()),this, SLOT(insertText()));                            //vkládání textu
    connect(ui->actionBold, SIGNAL(triggered()),this, SLOT(handleFontChange()));
    connect(ui->actionItalic, SIGNAL(triggered()),this, SLOT(handleFontChange()));
    connect(ui->actionUnderline, SIGNAL(triggered()),this, SLOT(handleFontChange()));
}

void MainWindow::createToolBars()
{
//-----combobox pro změnu písma
    fontComboBox = new QFontComboBox();
    QFont font = fontComboBox->currentFont();
    fontComboBox->setMaxVisibleItems(5);
    fontComboBox->setStatusTip("Change font");
    connect(fontComboBox, SIGNAL(currentFontChanged(QFont)),this, SLOT(currentFontChanged(QFont)));

//-----combobox pro velikost písma
    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    fontSizeCombo->setStatusTip("Change text size");
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),this, SLOT(fontSizeChanged(QString)));

//-----combobox pro lupu
    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);        //nastaveny index v comboboxu
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),this, SLOT(sceneScaleChanged(QString)));

//-----vytvoření textového toolbaru
    ui->textToolBar->addAction(ui->actionText);
    ui->textToolBar->addWidget(fontComboBox);
    ui->textToolBar->addWidget(fontSizeCombo);
    ui->textToolBar->addAction(ui->actionBold);
    ui->textToolBar->addAction(ui->actionItalic);
    ui->textToolBar->addAction(ui->actionUnderline);
    ui->textToolBar->addWidget(sceneScaleCombo);
}


void MainWindow::about()
{
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));     //pro zobrazení háčků, čárek
    QMessageBox::about(this, trUtf8("Filter generator"),trUtf8("O programu pro generování VHDL popisu filtrů."));
}

void MainWindow::about_Qt()
{
    QMessageBox::aboutQt(this, "O Qt");
}

//------VIDITELNOST TOOLBARŮ-------------------------------------------------
void MainWindow::visible_Toolbars()                 //viditelnost toolbarů
{
    if(ui->actionVisible_TextToolbar->isChecked()){
        ui->textToolBar->setVisible(true);
    }else {
        ui->textToolBar->setVisible(false);
    }
    if(ui->actionVisible_EditToolbar->isChecked()){
        ui->editToolBar->setVisible(true);
    }else {
        ui->editToolBar->setVisible(false);
    }
}

void MainWindow::textInserted(QGraphicsTextItem *)
{
    ui->actionText->setChecked(false);
    sceneArea->setMode(Scene::MoveItem);                 //po vložení textu se přepne scéna do módu přesunu objektů
}

void MainWindow::insertText()
{
    sceneArea->setMode(Scene::InsertText);
}

void MainWindow::currentFontChanged(const QFont &)      //změna fontu písma
{
    handleFontChange();
}

void MainWindow::fontSizeChanged(const QString &)       //změna velikosti písma
{
    handleFontChange();
}

void MainWindow::handleFontChange()                         //nastavení písma
{
    QFont font = fontComboBox->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(ui->actionBold->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(ui->actionItalic->isChecked());
    font.setUnderline(ui->actionUnderline->isChecked());

    sceneArea->setFont(font);
}

void MainWindow::itemSelected(QGraphicsItem *item)
{
    TextItem *textItem = qgraphicsitem_cast<TextItem *>(item);

    QFont font = textItem->font();
    fontComboBox->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    ui->actionBold->setChecked(font.weight() == QFont::Bold);
    ui->actionItalic->setChecked(font.italic());
    ui->actionUnderline->setChecked(font.underline());
}

void MainWindow::insertLine()
{
    if(ui->actionLine->isChecked()){
         sceneArea->setMode(Scene::InsertLine);
         ui->actionLine_2->setChecked(true);
         ui->actionMove->setChecked(false);
         ui->actionMove_2->setChecked(false);
    }else{
        sceneArea->setMode(Scene::Mode(-1));
        ui->actionLine_2->setChecked(false);
    }
}

void MainWindow::insertLineContext()
{
    if(ui->actionLine_2->isChecked()){                  //propojení akce v kontextovém menu s toolbarem
        ui->actionLine->setChecked(true);
    }else{
        ui->actionLine->setChecked(false);
    }
    insertLine();
}

void MainWindow::deleteItem()
{
    foreach (QGraphicsItem *item, sceneArea->selectedItems()) {
        if (item->type() == Connection::Type || item->type() == LibStructure::Type)
              delete item;
    }
}

void MainWindow::insertItem(){

    sceneArea->setMode(Scene::InsertItem);              //"zapne" mód pro vkládání objektů
}

void MainWindow::itemInserted(LibStructure *item)
{
    sceneArea->setMode(Scene::MoveItem);
    ui->actionInsert->setChecked(false);
    ui->actionMove->setChecked(true);
    ui->actionMove_2->setChecked(true);
    ui->actionLine->setChecked(false);
}

void MainWindow::moveItem()
{
    sceneArea->setMode(Scene::MoveItem);
    if(ui->actionMove->isChecked()){
        sceneArea->setMode(Scene::MoveItem);
        ui->actionMove_2->setChecked(true);
        ui->actionLine->setChecked(false);
        ui->actionLine_2->setChecked(false);
    }else{
        sceneArea->setMode(Scene::Mode(-1));
        ui->actionMove_2->setChecked(false);
    }
}

void MainWindow::moveItemContext()                      //akce pro "Move" v kontextovém menu
{
    if(ui->actionMove_2->isChecked()){                  //propojení akce v kontextovém menu s toolbarem
        ui->actionMove->setChecked(true);
    }else{
        ui->actionMove->setChecked(false);
    }
    moveItem();
}

