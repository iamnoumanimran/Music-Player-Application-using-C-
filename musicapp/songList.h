#ifndef songList_h
#define songList_h
#include <iostream>
#include <QFile>
#include <QString>
#include <QTextStream>

using namespace std;
enum{aToz,zToa,recent,created};

struct song
{
    friend class songList;
    friend class playList;
private:
    QString Name;
    song *next;
public:
	song() :next(NULL) {}
	song(QString Name) :Name(Name), next(NULL) {}
    QString getName();
	void setName(QString);
    song *getNext();
	void setNext(song*);
};

class songList
{
    friend class playList;
private:    
	QString Name;
    song *head;
    song *tail;
    songList *next;
    int num;
    
public:
    songList() {Name = "";head = tail = new song(Name); num = 0; next = NULL;}
    songList(QString Name){this->Name = Name; head = tail = new song(Name); num = 0; next = NULL;}
    virtual ~songList() {delete(head);}
    song *getHead();
	song *getTail();
    song *findSong(QString &Name);
    QString getName();
    songList *getNext();
	void setNext(songList*);
	//void setNext(song*);
    int getNum();
    void insertSong(song *s);
    void deleteSong(int index);
    void disposeList();
    void outputList(QFile *fp);
	void sort(songList *, int, int);
	int  partition(songList*, int, int);
};

class playList
{
private:
    songList *head;
    songList *tail;
    int num;
    int curList;
    int viewList;
    
public:
    playList() {head = tail = new songList; num = 0; curList = 0; viewList = 0;}
    virtual ~playList() {delete(head);}
    songList *getHead();
    songList *findList(int index);
    int getNum();
    void setNum(int num);
    int getcurList();
    void setcurList(int curList);
    int getviewList();
    void setviewList(int viewList);
    void insertList(songList *s);
    void deleteList(int index);
    void outputList(QFile *fp);
    void initialize(QFile *fp);
	//void sort(playList *, int, int);
	//int  partition(playList*, int, int);
}; 



#endif /* songList_h */

