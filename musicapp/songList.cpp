#include "songList.h"

QString song::getName()
{
    return Name;
}

void song::setName(QString nm) {
	this->Name = nm;
}

song *song::getNext()
{
    return next;
}

void song::setNext(song *nxt) {
	this->next = nxt;
}

song *songList::findSong(QString &Name)
{
    song *tmpSong = head->next;
    while(tmpSong != NULL && tmpSong->Name != Name)
        tmpSong = tmpSong->next;
    return tmpSong;
}

QString songList::getName()
{
    return Name;
}

songList *songList::getNext()
{
    return next;
}

void songList::setNext(songList *newNext) {
	next = newNext;
}

song *songList::getHead()
{
    return head;
}

song *songList::getTail() {
	return tail;
}

int songList::getNum()
{
    return num;
}

void songList::insertSong(song *s)
{
    tail->next = s;
    tail = s;
    num++;
}

void songList::deleteSong(int index)
{
	song *tmp = head, *pre = NULL;
	int i;
	if (index>num || index <= 0)
		return;
	num--;
	for (i = 0; i<index; i++)
	{
		pre = tmp;
		tmp = tmp->next;
	}
	if (tmp == tail)
		tail = pre;
	pre->next = tmp->next;
	delete(tmp);
}

void songList::disposeList()
{
    song *tmp = head->next, *nxt;
    while(tmp != NULL)
    {
        nxt = tmp->next;
        delete(tmp);
        tmp = nxt;
    }
    head->next = NULL;
}

void songList::outputList(QFile *fp)
{
    song *tmp = head;
    QTextStream txtOutput(fp);
    while(tmp != NULL)
    {
        txtOutput<<tmp->Name<<'@';
        tmp = tmp->next;
    }
}

songList *playList::getHead()
{
    return head;
}

songList *playList::findList(int index)
{
    songList *tmpList = head->next;
    int i;
    if(index > num)
        return NULL;
    for(i=1; i<index; i++)
        tmpList = tmpList->next;
    return tmpList;
}

void playList::setNum(int num)
{
    this->num = num;
}

int playList::getNum()
{
    return num;
}

int playList::getcurList()
{
    return curList;
}

void playList::setcurList(int curList)
{
    this->curList = curList;
}

int playList::getviewList()
{
    return viewList;
}

void playList::setviewList(int viewList)
{
    this->viewList = viewList;
}

void playList::insertList(songList *s)
{
    tail->next = s;
    tail = s;
    num++;
}

void playList::deleteList(int index)
{
	songList *tmp = head, *pre = NULL;
	int i;
	if (index>num || index <= 0)
		return;
	num--;
	for (i = 0; i<index; i++)
	{
		pre = tmp;
		tmp = tmp->next;
	}
	if (tmp == tail)
		tail = pre;
	pre->next = tmp->next;
	tmp->disposeList();
}

void playList::outputList(QFile *fp)
{
    songList *tmpList = head->next;
    QTextStream txtOutput(fp);
    if(!fp->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        cout << "Open failed." << endl;
    }
    while(tmpList != NULL)
    {
        tmpList->outputList(fp);
        txtOutput<<endl;
        tmpList = tmpList->next;
    }
}

void playList::initialize(QFile *fp)
{
    QTextStream txtInput(fp);
    QString lineStr, tmpSection;
    songList *tmpList;
    song *tmpSong;
    int i;
    if(!fp->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cout << "Open failed." << endl;
    }
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        tmpSection = lineStr.section('@',0,0);
        if(!tmpSection.isEmpty())
        {
            tmpList = new songList(tmpSection);
            this->insertList(tmpList);
            i = 1;
            tmpSection = lineStr.section('@',i,i);
            while(!tmpSection.isEmpty())
            {
                tmpSong = new song(tmpSection);
                tmpList->insertSong(tmpSong);
                i++;
                tmpSection = lineStr.section('@',i,i);
            }
        }
    }
}


void songList::sort(songList *start_ptr, int low, int high) {


	int pivotPoint;

	if (low > high)

	{

		// Get the pivot point.

		pivotPoint = partition(start_ptr, low, high);

		//Sort sublist A.

		sort(start_ptr, low, pivotPoint - 1);

		//Sort sublist B.

		sort(start_ptr, pivotPoint + 1, high);

	}
}
	

int songList::partition(songList *start_ptr, int low, int high)

	{

		song *ptr, *ptr2 = start_ptr->getHead(), *ptr3 = start_ptr->getHead();
		ptr = start_ptr->getHead();
		int  pivotIndex, mid, i;
		QString sName;


		mid = (low + high) / 2;

		for (i = 0; i < mid; i++)

		{

			ptr = ptr->next;

		}

		swap(start_ptr->getHead()->Name, ptr->Name);

		pivotIndex = low;

		sName = start_ptr->getHead()->Name;

		for (int scan = low + 1; scan <= high; scan++)

		{

			if (ptr2->Name < sName)

			{

				pivotIndex++;

				ptr3 = ptr3->next;

				swap(ptr3->Name, ptr2->Name);

			}

		}

		swap(start_ptr->getHead()->Name, ptr3->Name);

		return pivotIndex;

	}

	/*int num, x, y, z;
	song *i, *j; song *temp; song* prev; song* agla;
	i = songs->getHead();
	prev = songs->getHead();
	agla = new song();
	while (i->next != NULL)
	{
		cout << "\nhello 1";
		j = i;
		agla = j->next;
		
		temp = new song();
		temp = NULL;
		while (agla != NULL)
		{
			
			cout << "\nhello2";

			if (j->getName() > agla->getName())
			{
				cout << "\nloop1";
				temp = agla->next;
				agla->next = j;
				prev->next = agla;
				prev = agla;
				agla= j->next;
				j->next = temp;
			}
			else {
				cout << "\nloop2";
				prev = j;
				j = agla;
				agla->next = j->next;
			}
		}
		prev = i;
		i = i->next;
	}
	song *current = songs->getHead(), *current2 = current->getNext(), *temp;
	//if (songs->getHead() != NULL) {
		while (current->getNext()->next != NULL)
		{
			if (current->getName() > current->next->getName())
			{
				temp = current;
				current = current2;
				current2 = temp;
			}
			current = current->next;
		}
	//}*/
	
/*
void Insertion_sort(List * list, List * sortedlist, FILE *fout)
{
	Node * indexNode;
	Node * nextNode;
	Node * firstNode;

	indexNode = list->first;
	nextNode = indexNode->next;

	for(indexNode = list->first; indexNode->next != NULL; indexNode = indexNode->next)
	{
		nextNode = indexNode->next;

		if(indexNode->ID < nextNode->ID)
		{
			if(sortedlist->first == NULL)
			{
				firstNode = indexNode;
				sortedlist->first = firstNode;
				sortedlist->last = firstNode;
			} else {
				firstNode->next = indexNode;
				indexNode->prev = firstNode;
				sortedlist->last = indexNode;
			}

		} else {
			nextNode->prev = indexNode;
			indexNode->next = nextNode;
			sortedlist->last = nextNode;
		}

	}

	DLList_print(sortedlist, fout);
}*/