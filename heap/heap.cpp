/*
heap.cpp
author: Kelvin Lin
Oct. 12, 2014
*/

#include "heap.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

heap::heap(int cap)
{
  filled = 0;
  capacity = cap;
  mapping = new hashTable(capacity);
  node newnode;
  data.resize(capacity+1,newnode);
}

heap::~heap()
{
  vector<node>().swap(data);
  delete mapping;
}

int heap::getPos(node *pn)
{
  int pos = pn - &data[0];
  return pos;
}

void heap::percolateUp(int posCur) //used in insert and setKey
{
  // check if at a valid position
  if (posCur <= 1)
    return;
  int posParent = (int)posCur/2;
  if (posParent == 0)
    return;
  // check if current node violates min-heap
  if (data[posCur].key >= data[posParent].key) 
    return;
  node np = data[posCur];
  // shift nodes down until min-heap is satisfied
  while(np.key < data[posParent].key && posCur > 1) 
    {
      data[posCur] = data[posParent];
      mapping->setPointer(data[posCur].id,&data[posCur]);
      posCur = posParent;
      posParent = (int)posCur/2;
    }
  data[posCur] = np; // place node in appropriate slot
  mapping->setPointer(data[posCur].id,&data[posCur]);
  return;
}

void heap::percolateDown(int posCur) //used in setKey
{
  //check that min-heap is satisfied at posCur
  int len = filled;
  int smallestpos = posCur;
  bool b = true;
  //check left child is smaller & within the heap
  if (2*posCur <= len && data[smallestpos].key > data[2*posCur].key)
    smallestpos = 2*posCur;
  //check right child is smaller & within the heap
  if (2*posCur+1 <= len && data[smallestpos].key > data[2*posCur+1].key)
    smallestpos = 2*posCur + 1;
  //swap current node with smaller child if necessary
  if (smallestpos != posCur)
    {
      node tmp = data[posCur];
      data[posCur] = data[smallestpos];
      data[smallestpos] = tmp;
      mapping->setPointer(data[posCur].id,&data[posCur]);
      mapping->setPointer(data[smallestpos].id,&data[smallestpos]);
      //continue to percolate down
      percolateDown(smallestpos);
    }
  return;
}

int heap::insert(const std::string &id, int key, void *pv)
{
  if (filled == capacity)
    return 1; // heap full
  if (mapping->contains(id))
    return 2; // id exists
  filled++;
  data[filled].key = key;
  data[filled].id = id;
  data[filled].pData = pv;
  
  mapping->insert(data[filled].id,&data[filled]);
  percolateUp(filled);
  return 0;
}

int heap::setKey(const std::string &id, int key)
{
  if(!mapping->contains(id))
    return 1;
  node *np = static_cast<node *> (mapping->getPointer(id));
  int datpos = getPos(np);
  int oldkey = data[datpos].key;
  data[datpos].key = key;
  mapping->setPointer(data[datpos].id,&data[datpos]);
  if (key>oldkey)
    {
    percolateDown(datpos);
    }
  else if(key<oldkey)
    percolateUp(datpos);
  return 0;
}

int heap::remove(const std::string &id, int *pKey, void *ppData)
{
  if(!mapping->contains(id))
    return 1;
  node *np = static_cast<node *> (mapping->getPointer(id));
  if (pKey)
    *pKey = np->key;
  setKey(id,data[1].key-1);
  if (!deleteMin(NULL,NULL,ppData))
    return 0;
  return 1;
  
}

int heap::deleteMin(std::string *pId, int *pKey, void *ppData)
{
  if (pId)
    *pId = data[1].id;
  if (pKey)
    *pKey = data[1].key;
  if (ppData)
    //*ppData = data[1].pData;
    *(static_cast<void**>(ppData)) = data[1].pData;
  if (!mapping->remove(data[1].id))
    return 1;
  //move last item to top of the list to replace minimum
  data[1] = data[filled];
  mapping->setPointer(data[1].id,&data[1]);
  //lazy delete old data by decrementing filled.
  //data will be overwritten by the next insert command
  filled--;
  percolateDown(1);
  return 0;
}
