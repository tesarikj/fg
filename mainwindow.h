#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QLayout>
#include <QResizeEvent>

#include "gridlines.h"
#include "libstructure.h"

class QPrinter;
class QComboBox;
class QFontComboBox;
class QToolButton;
class QGraphicsView;
class QGraphicsScene;

class QString;
class QAction;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QResizeEvent;

class MouseAction;
class Scene;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void grid();                                        //akce pro zobrazení mřížky
    void gridContext();                                 //akce pro zobrazení mřížky - v kontextovém menu
    void about();                                       //o programu
    void about_Qt();                                    //o Qt
    void visible_Toolbars();                            //vypínání toolbarů

    void exportPDF();                                   //export do PDF
    void exportJPG();                                   //export do JPG
    void exportPNG();                                   //export do PNG

    void deleteItem();                                  //pro vymazání objektu
    void insertItem();                                  //pro vložení objektu
    void itemInserted(LibStructure *item);              //akce po vložení objektu
    void itemSelected(QGraphicsItem *item);             //je vybrán objekt
    void moveItem();                                    //přesun objektu
    void moveItemContext();                             //přesun objektu - v kontextovém menu
   // void insertLine();
    //void insertLineContext();
    void sceneScaleChanged(const QString &scale);       //lupa

    void insertText();                                  //akce pro vložení textu
    void textInserted(QGraphicsTextItem *item);         //akce po vložení textu
    void currentFontChanged(const QFont &font);         //pro změnu fontu
    void fontSizeChanged(const QString &size);          //pro změnu velikosti
    void handleFontChange();                            //pro změnu písma - velikost, bold, italic, font

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void createToolBars();              //vytvoření toolbarů
    void createActions();               //vytvoření akcí (delete, ....)
    void createGrid();                  //vytvoření mřížky

    QString nameFile;                   //jméno souboru
    Scene *sceneArea;
    MouseAction *mouseAction;
    GridLines * gridLines;

    QToolBar *editToolBar;              //vytvoření toolbaru - aby bylo možné vložit fontCombobox a fontSizeCombobox
    QComboBox *fontSizeCombo;
    QComboBox *sceneScaleCombo;
    QFontComboBox *fontComboBox;        //

    virtual void resizeEvent(QResizeEvent *);               //změna okna =>změna mřížky
};


#endif // MAINWINDOW_H
