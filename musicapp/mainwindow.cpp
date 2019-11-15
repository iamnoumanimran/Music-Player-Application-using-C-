#include "mainwindow.h"
#include "ui_mainwindow.h"


static const int timeConst = 125; 
static const int barConst = 32;	  


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	this->setWindowTitle(QStringLiteral("SmartMusicPlayer"));
    srcFile = "./Picture/123.txt";
    Position = 0;

    QFile fp(srcFile);
	l = new playList;
    l->initialize(&fp);

    setFixedSize(this->width(), this->height());

    QIcon icon("./Picture/icon.png");	
    setWindowIcon(icon);				

    QPixmap pixmap("./Picture/1.jpg");
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Active,QPalette::Window,QBrush(pixmap));
    setPalette(palette);			
    setAutoFillBackground(true);		 

	ui->button_Play->setStyleSheet("background-color:transparent");
	ui->button_Import->setStyleSheet("background-color:transparent");
	ui->button_Next->setStyleSheet("background-color:transparent");
	ui->button_Previous->setStyleSheet("background-color:transparent");
	ui->button_Return->setStyleSheet("background-color:transparent");
	ui->uiSongList->setStyleSheet("background-color:transparent");
	ui->Loop->setStyleSheet("background-color:transparent");
	ui->button_Pause->setStyleSheet("background-color:transparent");
	ui->Sequential->setStyleSheet("background-color:transparent");
	ui->CurrentItemInLoop->setStyleSheet("background-color:transparent");
	ui->CurrentItemOnce->setStyleSheet("background-color:transparent");
	ui->Random->setStyleSheet("background-color:transparent");
	ui->pushButton->setStyleSheet("background-color:transparent");
	ui->sortAlgo->setStyleSheet("background-color:transparent");
	ui->Random->setToolTip(tr("Random"));
	ui->Sequential->setToolTip("Sequential");
	ui->Loop->setToolTip(tr("Loop"));
	ui->CurrentItemInLoop->setToolTip("CurrentItemInLoop");
	ui->CurrentItemOnce->setToolTip(tr("CurrentItemOnce"));	


    MyPlayer = new QMediaPlayer(this);
	MyplayList = new myPlayList;
	playBackMode = Loop;
	ui->button_Play->hide();
	ui->CurrentItemInLoop->hide();
	ui->CurrentItemOnce->hide();
	ui->Random->hide();
	ui->Sequential->hide();

    ui->uiSongList->setColumnCount(1);
    ui->uiSongList->setRowCount(0); 
    ui->uiSongList->setStyleSheet("background-color:rgba(209,238,238,50)"); 
    ui->uiSongList->horizontalHeader()->setStretchLastSection(true);
    ui->uiSongList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->uiSongList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->uiSongList->verticalHeader()->setHidden(true);
	ui->uiSongList->setShowGrid(false);
	ui->uiSongList->setWindowFlags(Qt::FramelessWindowHint);


    ui->slider_volme->setRange(0, 100);
    QObject::connect(ui->slider_volme, SIGNAL(valueChanged(int)), this, SLOT(setVolume())); 
    ui->slider_volme->setValue(20); 
	QObject::connect(ui->slider_song, SIGNAL(sliderPressed()), this, SLOT(setPosition())); 
	QObject::connect(ui->slider_song, SIGNAL(sliderReleased()), this, SLOT(setPosition()));
	QObject::connect(MyPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChange()));
    QObject::connect(MyPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChange()));
	MyPlayer->duration();

	lrc = new MyLrc(this);

    createAction(); 
    createTray(); 
    createMenu(); 
	showplayList();
	
	rawData = nullptr;
	wav = new Wave();
	timer = new QTimer();
	SpecGraph = new Spectrum(this);
	this->SpecGraph->move(425, 0);
	
	connect(MyPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(sampling()));
	connect(timer, SIGNAL(timeout()), this, SLOT(slottimedone()));
	this->SpecGraph->show();
	
}

MainWindow::~MainWindow()
{
    delete ui;
	delete MyPlayer;
	delete  MyplayList;
	delete lrc;
	delete wav;
	delete timer;
	delete[] rawData;
	delete SpecGraph;
}

void MainWindow::createAction()
{
    action_playStop = new QAction("Play/Pause",this);
    action_playStop->setShortcut(tr("ctrl+p"));
    connect(action_playStop, SIGNAL(triggered()), this, SLOT(PlayOrStop()));
    connect(ui->button_Play, SIGNAL(clicked()), this, SLOT(PlayOrStop())); 
	connect(ui->button_Pause, SIGNAL(clicked()), this, SLOT(PlayOrStop()));


    action_playChoosen = new QAction("PlayChoosen",this);
    action_playChoosen->setShortcut(tr("ctrl+shift+p"));
    connect(action_playChoosen, SIGNAL(triggered()), this, SLOT(playChoosen()));


    action_Import = new QAction("ImportSongs",this);
    action_Import->setShortcut(tr("ctrl+i"));
    connect(action_Import, SIGNAL(triggered()), this, SLOT(Import()));
    connect(ui->button_Import, SIGNAL(clicked()), this, SLOT(Import()));

    action_Next = new QAction("NextSong",this);
    action_Next->setShortcut(tr("ctrl+n"));
    connect(action_Next, SIGNAL(triggered()), this, SLOT(nextSong()));
    connect(ui->button_Next, SIGNAL(clicked()), this, SLOT(nextSong()));
 

    action_Previous = new QAction("PreSong",this);
    action_Previous->setShortcut(tr("ctrl+l"));
    connect(action_Previous, SIGNAL(triggered()), this, SLOT(preSong()));
    connect(ui->button_Previous, SIGNAL(clicked()), this, SLOT(preSong()));

	connect(ui->uiSongList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(itemClicked()));

    action_Exit = new QAction("Exit",this);
    action_Exit->setShortcut(tr("ctrl+e"));
    connect(action_Exit, SIGNAL(triggered()), this, SLOT(close()));

    action_Minimize = new QAction("Minimize",this);
    action_Minimize->setShortcut(tr("ctrl+m"));
    connect(action_Minimize, SIGNAL(triggered()), this, SLOT(showMinimized()));


    action_VolumeUp = new QAction("Volume+10%",this);
    action_VolumeUp->setShortcut(tr("up"));
    connect(action_VolumeUp, SIGNAL(triggered()), this, SLOT(VolumeUp()));
    

    action_VolumeDown = new QAction("Volume-10%",this);
    action_VolumeDown->setShortcut(tr("down"));
    connect(action_VolumeDown, SIGNAL(triggered()), this, SLOT(VolumeDown()));
    

    action_ChangeMode = new QAction("ChangeMode",this);
    action_ChangeMode->setShortcut(tr("ctrl+c"));
    connect(action_ChangeMode, SIGNAL(triggered()), this, SLOT(ChangeMode()));

    connect(ui->Loop, SIGNAL(clicked()), this, SLOT(ChangeMode()));

	connect(ui->Random, SIGNAL(clicked()), this, SLOT(ChangeMode()));
	
	connect(ui->Sequential, SIGNAL(clicked()), this, SLOT(ChangeMode()));

	connect(ui->CurrentItemInLoop, SIGNAL(clicked()), this, SLOT(ChangeMode()));

	connect(ui->CurrentItemOnce, SIGNAL(clicked()), this, SLOT(ChangeMode()));


    action_Remove = new QAction("Remove",this);
    action_Remove->setShortcut(tr("delete"));
    connect(action_Remove, SIGNAL(triggered()), this, SLOT(Remove()));

    action_Return = new QAction("Return",this);
    action_Return->setShortcut(tr("ctrl+r"));
    connect(action_Return, SIGNAL(triggered()), this, SLOT(showplayList()));
    connect(ui->button_Return, SIGNAL(clicked()), this, SLOT(showplayList()));

    action_createList = new QAction("createList",this);
    action_createList->setShortcut(tr("ctrl+o"));
    connect(action_createList, SIGNAL(triggered()), this, SLOT(createList()));

	action_sort = new QAction("Sort", this);
	action_sort->setShortcut(tr("ctrl+s"));
	connect(action_sort, SIGNAL(triggered()), this, SLOT(sort()));
	connect(ui->sortAlgo, SIGNAL(clicked()), this, SLOT(sort()));

	action_LRC = new QAction(("Lrc"), this);
	action_LRC->setShortcut(tr("F2"));
	connect(action_LRC, SIGNAL(triggered()), this, SLOT(LrcShow()));
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(LrcShow()));
	connect(MyPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(UpdateTime(qint64)));

	connect(MyPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(ChangeLrc()));

}

void MainWindow::createMenu()
{
    ui->uiSongList->addAction(action_playStop);
    ui->uiSongList->addAction(action_playChoosen);
    ui->uiSongList->addAction(action_Import);
    ui->uiSongList->addAction(action_Remove);
    ui->uiSongList->addAction(action_Next);
    ui->uiSongList->addAction(action_Previous);
    ui->uiSongList->addAction(action_VolumeUp);
    ui->uiSongList->addAction(action_VolumeDown);
    ui->uiSongList->addAction(action_ChangeMode);
    ui->uiSongList->addAction(action_Minimize);
    ui->uiSongList->addAction(action_Exit);
    ui->uiSongList->addAction(action_createList);
	ui->uiSongList->addAction(action_LRC);
    ui->uiSongList->setContextMenuPolicy(Qt::ActionsContextMenu);
	this->addAction(action_LRC);
    this->addAction(action_playStop);
    this->addAction(action_playChoosen);
    this->addAction(action_Import);
    this->addAction(action_Next);
    this->addAction(action_Previous);
    this->addAction(action_VolumeUp);
    this->addAction(action_VolumeDown);
    this->addAction(action_ChangeMode);
    this->addAction(action_Minimize);
    this->addAction(action_Exit);
    this->addAction(action_createList);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createTray()
{
    QIcon icon("./Picture/icon.png");
    SystemTray = new QSystemTrayIcon();
    connect(SystemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(iconActived(QSystemTrayIcon::ActivationReason)));
    SystemTray->setIcon(icon);
    SystemTray->setToolTip("Tray");
    TrayMenu = new QMenu(this);
    action_TrayShow = new QAction("Show", this);
    action_TrayShow->setShortcut(tr("ctrl+s"));
    TrayMenu->addAction(action_TrayShow);
    connect(action_TrayShow, SIGNAL(triggered()), this, SLOT(showNormal()));
    TrayMenu->addAction(action_playStop);
    TrayMenu->addAction(action_Import);
    TrayMenu->addAction(action_Next);
    TrayMenu->addAction(action_Previous);
    TrayMenu->addAction(action_VolumeUp);
    TrayMenu->addAction(action_VolumeDown);
    TrayMenu->addAction(action_ChangeMode);
    TrayMenu->addAction(action_Exit);
    SystemTray->setContextMenu(TrayMenu);
    SystemTray->show();
}


void MainWindow::PlayOrStop()
{
    if(MyPlayer->state() == QMediaPlayer::PausedState)
    {
		ui->button_Pause->hide();
		ui->button_Play->show();
        MyPlayer->setPosition(Position);
        MyPlayer->play();
		resolve_lrc();
    }
    else if(MyPlayer->state() == QMediaPlayer::PlayingState)
    {
		ui->button_Pause->show();
		ui->button_Play->hide();
        Position = MyPlayer->position();
        MyPlayer->pause();
		if (!lrc_map.isEmpty()) 
		{
			lrc->stop_lrc_mask();
		}
    }
	else
	{
		if (l->getviewList() != 0)
		{
			ui->button_Pause->hide();
			ui->button_Play->show();
			playChoosen();
		}
	}
}

void MainWindow::sort() {
	songList *tmpList;
	songList *tail;
	tmpList = l->findList(l->getcurList());
	//tmpList->sort(tmpList , 0, tmpList->getNum());
	MyplayList->quickSort(MyplayList,1,MyplayList->getNum());
	ui->uiSongList->clear();
	QModelIndex indexlist = ui->uiSongList->currentIndex();
	int i = indexlist.row();
	l->setviewList(i + 1);
	showplayList();
	showsongList(i +1);
}

void MainWindow::playChoosen()
{
    QModelIndex indexlist = ui->uiSongList->currentIndex();
    int row = indexlist.row();
    if(MyPlayer->state() == QMediaPlayer::StoppedState||MyplayList->currentIndex() != row)
    {
		if (l->getviewList() != l->getcurList())
		{
			MyplayList->clear();
			l->setcurList(l->getviewList());
			songList *tmpList = l->findList(l->getcurList());
			int num = tmpList->getNum();
			song *tmpSong = tmpList->getHead()->getNext();
			for (int j = 0; j<num; j++)
			{
				MyplayList->addMedia(tmpSong->getName());
				tmpSong = tmpSong->getNext();
			}
		}
		MyplayList->setCurrentIndex(row);
		MyPlayer->setMedia(QUrl::fromLocalFile(MyplayList->getCur()));
		preprocess();
		ui->button_Pause->hide();
		ui->button_Play->show();
		MyPlayer->play();
		Position = 0;
    }
    else if(MyPlayer->state() == QMediaPlayer::PausedState)
    {
        MyPlayer->setPosition(Position);
//		preprocess();		
		ui->button_Pause->hide();
		ui->button_Play->show();
        MyPlayer->play();
    }
}


void MainWindow::Import()
{
    QModelIndex indexlist = ui->uiSongList->currentIndex();
    int row = indexlist.row(), tarList;
    if(l->getviewList()!=0)
        tarList = l->getviewList();
    else if(row>=0 && row<l->getNum())
        tarList = row+1;			
    else
        return;
    int i, num;
    QStringList path=QFileDialog::getOpenFileNames(this,tr("Choose wav files"),".","*.wav");
    songList *tmpList = l->findList(tarList);
    song *tmpSong;
    num = tmpList->getNum();
    for(i=0; i<path.size(); i++)
    {
        if(tmpList->findSong(path[i]) != NULL)
            continue;
        ui->uiSongList->setRowCount(ui->uiSongList->rowCount()+1);
        tmpSong = new song(path[i]);
        tmpList->insertSong(tmpSong);
        if(tarList == l->getviewList())
        {
            QFileInfo fileInfo;
            fileInfo = QFileInfo(path[i]);
            QString fileName = fileInfo.baseName();
			QDateTime filedate = fileInfo.lastModified();
			QDateTime recent = fileInfo.lastRead();
			QMediaMetaData::AlbumArtist.toStdString();
            ui->uiSongList->setItem(tmpList->getNum()-1,0, new QTableWidgetItem(fileName));
        }
        if(l->getcurList() == l->getviewList())
			MyplayList->addMedia(path[i]); 
    }
}

void MainWindow::showplayList()
{
    int i, num;
    l->setviewList(0);
    num = l->getNum();
    songList *tmpList = l->getHead()->getNext();
    ui->uiSongList->clear();
    QStringList header;
    header<<"Songlist";
    ui->uiSongList->setHorizontalHeaderLabels(header);
    ui->uiSongList->setRowCount(num);
    for(i=0;i<num;i++)
    {
        ui->uiSongList->setItem(i,0, new QTableWidgetItem(tmpList->getName()));
        tmpList = tmpList->getNext();
    }
}

void MainWindow::showsongList(int viewList)
{
    int i, num;
    l->setviewList(viewList);
    ui->uiSongList->clear();
    songList *tmpList = l->findList(viewList);
    num = tmpList->getNum();
    QStringList header;
    header<<tmpList->getName();
    ui->uiSongList->setHorizontalHeaderLabels(header);
    ui->uiSongList->setRowCount(num);
    song *tmpSong = tmpList->getHead()->getNext();
    QFileInfo fileInfo;
    QString fileName;
	QDateTime fileDate;
    for(i=0;i<num;i++)
    {
        fileInfo=QFileInfo(tmpSong->getName());
		fileName = fileInfo.baseName();
        fileDate=fileInfo.birthTime();
        ui->uiSongList->setItem(i,0, new QTableWidgetItem(fileName));
        tmpSong = tmpSong->getNext();
    }
}

void MainWindow::nextSong()
{
	QString tmpStr = MyplayList->getNext();
	if (tmpStr != NULL)
	{
		MyPlayer->setMedia(QUrl::fromLocalFile(tmpStr));
		MyPlayer->play();
	}
}

void MainWindow::preSong() 
{
	QString tmpStr = MyplayList->getPre();
	if (tmpStr != NULL)
	{
		MyPlayer->setMedia(QUrl::fromLocalFile(tmpStr));
		MyPlayer->play();
	}
}

void MainWindow::itemClicked()
{
	qDebug() << l->getviewList();
    if(l->getviewList() == 0)
        listClicked();
	else
		songClicked(); 
}

void MainWindow::listClicked()
{
	QModelIndex indexlist = ui->uiSongList->currentIndex();
	int i = indexlist.row();
	l->setviewList(i + 1);
	showsongList(i + 1);
}


void MainWindow::setVolume()
{
    Volume = ui->slider_volme->value();
    MyPlayer->setVolume(Volume);
}

void MainWindow::setPosition()
{
    MyPlayer->setPosition(ui->slider_song->value());
}

void MainWindow::positionChange()
{
	ui->slider_song->setSliderPosition(MyPlayer->position());
	if (MyPlayer->position() != 0 && MyPlayer->position() == MyPlayer->duration())
		checkMode();
}

void MainWindow::durationChange() 
{
    ui->slider_song->setRange(0,MyPlayer->duration());
}

void MainWindow::iconActived(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
       showNormal();
} //show the window then double-clicking the tray icon

void MainWindow::closeEvent(QCloseEvent *event) 
{
    if(this->isVisible())
   {
        event->ignore();
        SystemTray->showMessage(tr("hint"), tr("Background operation"));
        this->hide();
    }
    else
    {
        QFile fp(srcFile);
        l->outputList(&fp);
		exit(1);
        event->accept();
    }
}

void MainWindow::VolumeUp() 
{
    if(Volume > 90)
    {
        Volume = 100;
        SystemTray->showMessage(tr("hint"), tr("Volume can't be greater"));
    }
    else
        Volume += 10;
    ui->slider_volme->setValue(Volume);
    MyPlayer->setVolume(Volume);
}

void MainWindow::VolumeDown()
{
    if(Volume < 10)
    {
        Volume = 0;
        SystemTray->showMessage(tr("hint"), tr("Volume can't be smaller"));
    }
    else
        Volume -= 10;
    ui->slider_volme->setValue(Volume);
    MyPlayer->setVolume(Volume);
}

void MainWindow::ChangeMode() 
{
    switch(playBackMode)
    {
        case CurrentItemOnce: playBackMode = CurrentItemInLoop; 
			ui->CurrentItemInLoop->show();
			ui->CurrentItemOnce->hide();
		break;
        case CurrentItemInLoop: playBackMode = Sequential;
			ui->Sequential->show();
			ui->CurrentItemInLoop->hide();
		break;
        case Sequential: playBackMode = Loop;
			ui->Loop->show();
			ui->Sequential->hide();
		break;
        case Loop: playBackMode = CurrentItemOnce;
			ui->CurrentItemOnce->show();
			ui->Loop->hide();
		break;
    }

}

void MainWindow::checkMode() 
{
	switch (playBackMode)
	{
	case CurrentItemOnce:
		ui->slider_song->setSliderPosition(0); break;
	case CurrentItemInLoop:
		MyPlayer->setMedia(QUrl::fromLocalFile(MyplayList->getCur()));
		preprocess();		
		MyPlayer->play(); 
		ui->button_Pause->hide();
		ui->button_Play->show(); 
		break;
	case Sequential:
		if (MyplayList->getCur() != MyplayList->getTail()) {
			MyPlayer->setMedia(QUrl::fromLocalFile(MyplayList->getNext()));
			preprocess();		
			ui->button_Pause->hide();
			ui->button_Play->show();
			MyPlayer->play();
		}
		else
			ui->slider_song->setSliderPosition(0);
		break;
	case Loop:
		MyPlayer->setMedia(QUrl::fromLocalFile(MyplayList->getNext()));
		preprocess();					
		ui->button_Pause->hide();
		ui->button_Play->show();
		MyPlayer->play(); 
		break;
	}
}

void MainWindow::songClicked() 
{
	QModelIndex indexlist = ui->uiSongList->currentIndex();
	int row = indexlist.row(), j, num;
	if (l->getviewList() != l->getcurList())
	{
		MyplayList->clear();
		l->setcurList(l->getviewList());
		songList *tmpList = l->findList(l->getcurList());
		num = tmpList->getNum();
		song *tmpSong = tmpList->getHead()->getNext();
		for (j = 0; j<num; j++)
		{
			MyplayList->addMedia(tmpSong->getName());
			tmpSong = tmpSong->getNext();
		}
	}
	MyplayList->setCurrentIndex(row);
	MyPlayer->setMedia(QUrl::fromLocalFile(MyplayList->getCur()));
	preprocess();
	ui->button_Pause->hide();
	ui->button_Play->show();
	MyPlayer->play();
	Position = 0;
}

void MainWindow::createList()
{
    bool ok = false;
    QString tmpStr = QInputDialog::getText(NULL,
                tr( "Create a new songlist" ),
                tr( "Please enter the name:" ),
                QLineEdit::Normal, QString::null, &ok);
    if(tmpStr.isEmpty())
    {
        QMessageBox::warning(NULL, "Create a new songlist", "No input", QMessageBox::Ok);
        return;
    }
    songList *tmpList = new songList(tmpStr);
    l->insertList(tmpList);
    showplayList();
}

void MainWindow::Remove() 
{
	QModelIndex indexlist = ui->uiSongList->currentIndex();
	int row = indexlist.row();
	if (l->getviewList() != 0 && row >= 0)
	{
		ui->uiSongList->removeRow(row);
		songList *tmpList = l->findList(l->getviewList());
		tmpList->deleteSong(row + 1);
		if (l->getcurList() == l->getviewList())
		{
			int curIndex = MyplayList->currentIndex();
			if (curIndex == row)
			{
				ui->button_Play->hide();
				ui->button_Pause->show();
				MyPlayer->stop();
			}
			MyplayList->removeMedia(row);
		}
	}
	else if (row > 0)
	{
		ui->uiSongList->removeRow(row);
		cout << row << endl;
		QTableWidgetItem *item = ui->uiSongList->currentItem();
		QString tmpStr = item->text();
		l->deleteList(row + 1);
		if (l->getcurList() == l->getviewList())
			MyplayList->clear();
	}
}


void MainWindow::preprocess() {

	qDebug() << "haha";
	timer->stop();
	ifstream fs;

	QString filePath = MyplayList->getCur();

	string str;
	str = filePath.toStdString();
	cout << str << endl;

	QByteArray temp = filePath.toLatin1();
	fs.open(temp.data(), ios::binary | ios::in);

	fs.seekg(0, ios::end);
	wav->file_size = fs.tellg();

	bool appExist;
	unsigned int filter;
	fs.seekg(0x10);
	fs.read((char*)&filter, sizeof(unsigned int));
	appExist = filter == 32;

	fs.seekg(0x16);
	fs.read((char*)&wav->channel, sizeof(wav->channel));

	fs.seekg(0x18);
	fs.read((char*)&wav->frequency, sizeof(wav->frequency));

	fs.seekg(0x1c);
	fs.read((char*)&wav->Bps, sizeof(wav->Bps));

	fs.seekg(0x22);
	fs.read((char*)&wav->sample_num_bit, sizeof(wav->sample_num_bit));

	unsigned long int addr = 0x24;
	if (appExist)
		addr += 2;

	char flag[5];
	fs.seekg(addr);
	fs.read(flag, 4 * sizeof(char));
	flag[4] = '\0';

	bool isBegin;
	if (flag == "data")
		isBegin = true;
	else
		isBegin = false;

	if (isBegin == false) {
		addr += 4;
		unsigned int interval;
		fs.seekg(addr);
		fs.read((char*)&interval, sizeof(unsigned int));
		addr += (4 + interval);
	}

	addr += 4;
	fs.seekg(addr);
	fs.read((char*)&wav->data_size, sizeof(wav->data_size));

	if (rawData != nullptr) {
		qDebug() << "delete!";
		delete [] rawData;
	}
	rawData = new unsigned char[wav->data_size];

	addr += 4;
	fs.seekg(addr);
	fs.read((char *)rawData, sizeof(char)*wav->data_size);

	fs.close();

	wav->wav_time = double(wav->data_size) / wav->Bps * 1000;
}


void MainWindow::sampling() {
	if (MyPlayer->state() == QMediaPlayer::PlayingState) {
		timer->start(timeConst);
	}
	else if (MyPlayer->state() == QMediaPlayer::PausedState
		|| QMediaPlayer::StoppedState) {
		timer->stop();
	}
	
}

void MainWindow::slottimedone() {
	timer->stop();
	quint64 current = MyPlayer->position();
	unsigned long startFrame = current * wav->data_size / wav->wav_time + wav->Bps / 10;
	short* sampleData;
	unsigned long sampleCount;

	if (wav->channel == 1) {
		sampleCount = barConst;
		sampleData = new short[barConst];
	}
	else {
		sampleCount = barConst * 2;
		sampleData = new short[2 * barConst];
	}

	double* power[barConst];	
	if (wav->channel == 1) {
		for (int i = 0; i < sampleCount; i++) {
			unsigned long pos1, pos2;
			unsigned short data_low, data_high;
			pos1 = startFrame + wav->Bps / barConst / (1000 / timeConst) * i;
			pos2 = startFrame + wav->Bps / barConst / (1000 / timeConst) * i + 1;
			if (pos1 < wav->data_size)
				data_low = short(rawData[pos1]);
			else
				data_low = 0;
			if (pos2 < wav->data_size)
				data_high = short(rawData[pos2]);
			else
				data_high = 0;

			unsigned short data_true = data_high * 256 + data_low;
			unsigned short data_complement;

			int my_sign = (int)(data_high / 128);
			if (my_sign == 1) {
				data_complement = data_true - 65536;
			}
			else {
				data_complement = data_true;
			}
			sampleData[i] = (short)data_complement;
		}
	}

	else {
		for (int i = 0; i < sampleCount / 2; i++) {
			unsigned long pos1, pos2;
			unsigned short data_low, data_high;
			pos1 = startFrame + wav->Bps / barConst / (1000 / timeConst) * i;
			pos2 = startFrame + wav->Bps / barConst / (1000 / timeConst) * i + 1;
			if (pos1 < wav->data_size)
				data_low = short(rawData[pos1]);
			else
				data_low = 0;
			if (pos2 < wav->data_size)
				data_high = short(rawData[pos2]);
			else
				data_high = 0;

			unsigned short data_true = data_high * 256 + data_low;
			unsigned short data_complement;

			int my_sign = (int)(data_high / 128);
			if (my_sign == 1) {
				data_complement = data_true - 65536;
			}
			else {
				data_complement = data_true;
			}
			sampleData[2 * i] = (short)data_complement;
		}
		for (int i = 0; i < sampleCount / 2; i++) {
			unsigned long pos1, pos2;
			unsigned short data_low, data_high;
			pos1 = startFrame + wav->Bps / barConst / (1000 / timeConst) * i + 2;
			pos2 = startFrame + wav->Bps / barConst / (1000 / timeConst) * i + 3;
			if (pos1 < wav->data_size)
				data_low = short(rawData[pos1]);
			else
				data_low = 0;
			if (pos2 < wav->data_size)
				data_high = short(rawData[pos2]);
			else
				data_high = 0;

			unsigned short data_true = data_high * 256 + data_low;
			unsigned short data_complement;

			int my_sign = (int)(data_high / 128);
			if (my_sign == 1) {
				data_complement = data_true - 65536;
			}
			else {
				data_complement = data_true;
			}
			sampleData[2 * i + 1] = (short)data_complement;
		}
	}
	
	SpecGraph->powerSpectrum(sampleData, sampleCount, wav->channel, SpecGraph->FFT_result);
	SpecGraph->update();
	delete[] sampleData;
	timer->start(timeConst);
}

void MainWindow::LrcShow() 
{
	if (lrc->isHidden())
		lrc->show();
	else
		lrc->hide();
}

void MainWindow::resolve_lrc()
{
	lrc_map.clear();

	QString lrc_file_name = file_Name + ".lrc";

	QFile file(lrc_file_name);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		lrc->setText(/*QFileInfo(media_object->currentSource().fileName()).baseName()+ */tr("LRC NOT EXISTED"));
		return;
	}
	QTextStream in(&file);
	QString all_text = QString(in.readAll());
	file.close();
	QStringList lines = all_text.split("\n");


	QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");
	foreach(QString oneline, lines) {
		QString temp = oneline;
		temp.replace(rx, "");
		int pos = rx.indexIn(oneline, 0);
		while (pos != -1) { 
			QString cap = rx.cap(0);
			QRegExp regexp;
			regexp.setPattern("\\d{2}(?=:)");
			regexp.indexIn(cap);
			int minute = regexp.cap(0).toInt();
			regexp.setPattern("\\d{2}(?=\\.)");
			regexp.indexIn(cap);
			int second = regexp.cap(0).toInt();
			regexp.setPattern("\\d{2}(?=\\])");
			regexp.indexIn(cap);
			int millisecond = regexp.cap(0).toInt();
			qint64 totalTime = minute * 60000 + second * 1000 + millisecond * 10;
			lrc_map.insert(totalTime, temp);
			pos += rx.matchedLength();
			pos = rx.indexIn(oneline, pos);
		}
	}

	if (lrc_map.isEmpty()) {
		lrc->setText(tr("  LRC NOT EXISTED"));
		return;
	}
}

void MainWindow::UpdateTime(qint64 time)
{
	qint64 total_time_value = MyPlayer->duration();
	if (!lrc_map.isEmpty()) {
		qint64 previous = 0;
		qint64 later = 0;
		foreach(qint64 value, lrc_map.keys()) {
			if (time >= value) {
				previous = value;
			}
			else {
				later = value;
				break;
			}
		}

		if (later == 0)
			later = total_time_value;

		QString current_lrc = lrc_map.value(previous);

		if (current_lrc != lrc->text()) {
			lrc->setText(current_lrc);
			//top_label->setText(current_lrc);
			qint64 interval_time = later - previous;
			lrc->start_lrc_mask(interval_time);
		}
	}
}

void MainWindow::ChangeLrc()
{
	if (MyPlayer->duration() != 0) {
		index = MyplayList->currentIndex();
		file_Name = ui->uiSongList->item(index, 0)->text();
		resolve_lrc();
	}
}
