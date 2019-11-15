#include "myplaylist.h"

myPlayList:: ~myPlayList()
{
    if(head->next != NULL)
        clear();
    delete(head);
}

QString myPlayList::getTail()
{
    return tail->curSong;
}

node *myPlayList:: getHead() {
	return head;
}

node *myPlayList::getnext() {
	return next;
}

node *myPlayList::getCurr() {
	return curNode;
}

node *myPlayList::gettail() {
	return tail;
}

int myPlayList::getNum() {
	return this->num;
}
void myPlayList::setTail(node *tl) {
	this->tail = tl;
}

void myPlayList::setNext(node *nxt) {
	this->next = nxt;
}

void myPlayList::setHead(node *nxt) {
	this->head = nxt;
}

void myPlayList::setCurr(node *cur) {
	this->curNode = cur;
}
QString myPlayList::getNext()
{
	if (head->next == NULL)
		return NULL;
	if (curNode != NULL)
		curNode = curNode->next;
	if (curNode == NULL)
		curNode = head->next;
	return curNode->curSong;
}

QString myPlayList::getCur()
{
	if (curNode == NULL)
		return NULL;
	return curNode->curSong;
}

QString myPlayList::getPre()
{
	if (head->next == NULL)
		return NULL;
	if (curNode != head)
		curNode = curNode->previous;
	if (curNode == head)
		curNode = tail;
	return curNode->curSong;
}


void myPlayList::addMedia(QString path)
{
    node *tmpNode = new node(path);
    tmpNode->previous = tail;
    tail->next = tmpNode;
    tail = tmpNode;
    num++;
}

int myPlayList::currentIndex()
{
    int i;
    node *tmpNode = head->next;
    for(i=0; tmpNode!=curNode && tmpNode != NULL; i++)
        tmpNode = tmpNode->next;
    return i;
}

void myPlayList::setCurrentIndex(int index)
{
    int i;
    node *tmpNode = head->next;
    if(index > num)
        return;
    for(i=0; i<index; i++)
        tmpNode = tmpNode->next;
    curNode = tmpNode;
}

void myPlayList::clear()
{
    int i;
    node *tmpNode = head->next, *nxtNode;
    for(i=1; tmpNode!=NULL; i++)
    {
        nxtNode = tmpNode->next;
        delete tmpNode;
        tmpNode = nxtNode;
    }
    head->next = NULL;
    num = 0;
}

void myPlayList::removeMedia(int index)
{
	int i;
	node *tmpNode = head->next, *preNode = head;
	if (index > num)
		return;
	cout << index << endl;
	for (i = 0; i<index; i++)
	{
		preNode = tmpNode;
		tmpNode = tmpNode->next;
	}
	if (tail == tmpNode)
		tail = preNode;
	preNode->next = tmpNode->next;
	if (tmpNode->next != NULL)
		tmpNode->next->previous = preNode;
	delete tmpNode;
	num--;
}

void myPlayList::quickSort(myPlayList *start_ptr,int low,int high) {
	int pivotPoint;

	if (low > high)

	{
		cout << "sort";
		// Get the pivot point.

		pivotPoint = partition(start_ptr, low, high);

		//Sort sublist A.

		quickSort(start_ptr, low, pivotPoint - 1);

		//Sort sublist B.

		quickSort(start_ptr, pivotPoint + 1, high);

	}
}


int myPlayList::partition(myPlayList *start_ptr, int low, int high)

{

	node *ptr, *ptr2 = start_ptr->getHead(), *ptr3 = start_ptr->getHead();
	ptr = start_ptr->getHead();
	int  pivotIndex, mid, i;
	QString sName;
	cout << "partition";

	mid = (low + high) / 2;

	for (i = 0; i < mid; i++)

	{

		ptr = ptr->next;

	}

	swap(start_ptr->getCurr()->curSong, ptr->curSong);

	pivotIndex = low;

	sName = start_ptr->getCurr()->curSong;

	for (int scan = low + 1; scan <= high; scan++)

	{

		if (ptr2->curSong < sName)

		{
			cout << "qP";
			pivotIndex++;

			ptr3 = ptr3->next;

			swap(ptr3->curSong, ptr2->curSong);

		}

	}

	swap(start_ptr->getCurr()->curSong, ptr3->curSong);

	return pivotIndex;

}
/*void quickSort()
{
	myPlaylist *pLeft, IntegerList *pRight
	IntegerList *pStart;
	IntegerList *pCurrent;
	int nCopyInteger;

	// If the left and right pointers are the same, then return
	if (pLeft == pRight) return;

	// Set the Start and the Current item pointers
	pStart = pLeft;
	pCurrent = pStart->pNext;

	// Loop forever (well until we get to the right)
	while (1)
	{
		// If the start item is less then the right
		if (pStart->nInteger < pCurrent->nInteger)
		{
			// Swap the items
			nCopyInteger = pCurrent->nInteger;
			pCurrent->nInteger = pStart->nInteger;
			pStart->nInteger = nCopyInteger;
		}

		// Check if we have reached the right end
		if (pCurrent == pRight) break;

		// Move to the next item in the list
		pCurrent = pCurrent->pNext;
	}

	// Swap the First and Current items
	nCopyInteger = pLeft->nInteger;
	pLeft->nInteger = pCurrent->nInteger;
	pCurrent->nInteger = nCopyInteger;

	// Save this Current item
	IntegerList *pOldCurrent = pCurrent;

	// Check if we need to sort the left hand size of the Current point
	pCurrent = pCurrent->pPrev;
	if (pCurrent != NULL)
	{
		if ((pLeft->pPrev != pCurrent) && (pCurrent->pNext != pLeft))
			QuickSortList(pLeft, pCurrent);
	}

	// Check if we need to sort the right hand size of the Current point
	pCurrent = pOldCurrent;
	pCurrent = pCurrent->pNext;
	if (pCurrent != NULL)
	{
		if ((pCurrent->pPrev != pRight) && (pRight->pNext != pCurrent))
			QuickSortList(pCurrent, pRight);
	}
}*/