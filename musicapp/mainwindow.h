#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <algorithm>
#include <QSystemTrayIcon>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaMetaData>
#include <QAction>
#include <QMenu>
#include <QPalette>
#include <QPixmap>
#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QPushButton>
#include <QCloseEvent>
#include <QLineEdit>
#include <QInputDialog>
#include "songList.h"
#include "myplaylist.h"
#include <QAudioFormat>
#include "qaudiodecoder.h"
#include <QCloseEvent>
#include <QAudioBuffer>
#include <QByteArray>
#include <QTimer>
#include "spectrum.h"
#include "wave.h"
#include <QTextCodec>
#include <QTime>
#include "mylrc.h"
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QPoint>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QAction *action_TrayShow;
    QAction *action_playStop;
    QAction *action_playChoosen;
    QAction *action_Import;
    QAction *action_Next;
    QAction *action_Previous;
    QAction *action_Exit;
    QAction *action_Minimize;
    QAction *action_VolumeUp;
    QAction *action_VolumeDown;
    QAction *action_ChangeMode;
    QAction *action_Remove;
    QAction *action_Return;
    QAction *action_createList;
	QAction *action_sort;

	QAction *action_LRC;

    QSystemTrayIcon *SystemTray;
    QMenu *TrayMenu;
    QMenu *Menu;
    QMediaPlayer *MyPlayer;
	myPlayList *MyplayList;
    QString srcFile;
    playList *l;
    int Volume;
	qint64 Position;
	int index;				
	int songNum;

	MyLrc *lrc;
	QString file_Name;
	QMap<qint64, QString> lrc_map;
	void resolve_lrc();

	int playBackMode;
    void createMenu();
    void createAction();
    void createTray();
    void closeEvent(QCloseEvent *event);
    void listClicked();
	void checkMode();
	void songClicked();

	Wave* wav;					
	QTimer* timer;					
	unsigned char* rawData;			
	quint64 currentTime;			
	Spectrum *SpecGraph;		

private slots:
    void PlayOrStop();
    void playChoosen();
    void Import();
    void nextSong();
    void preSong();
    void itemClicked();
    void setVolume();
    void setPosition();
    void positionChange();
    void durationChange();
    void iconActived(QSystemTrayIcon::ActivationReason reason);
    void VolumeUp();
    void VolumeDown();
    void ChangeMode();
    void Remove();
    void showplayList();
    void showsongList(int viewList);
	void createList();
	void sort();
	void LrcShow();
	void UpdateTime(qint64 time);
	void ChangeLrc();

	void preprocess();
	void sampling();
	void slottimedone();  
};

#endif // MAINWINDOW_H
