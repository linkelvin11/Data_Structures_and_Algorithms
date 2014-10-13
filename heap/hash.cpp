/*
hash.cpp
Author: Kelvin Lin
Sept 17 2014
*/

#include "hash.h"
#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::cout;



hashTable::hashTable(int size)
{
  filled = 0; //start filled as 0
  capacity = getPrime(size); //use getprime to set the capacity
  hashItem hitem; hitem.isOccupied = false; hitem.isDeleted = false;
  data.resize(capacity,hitem); //initialize data to size capacity
  
}

int hashTable::insert(const string &key, void *pv)
{
  //check capacity and/or attempt rehash
  if (((double)filled)/((double)capacity)>0.5) 
    {
      //cout << "rehashing: new size is: " << getPrime(capacity)<< std::endl;
      if (!(rehash())) 
	{
	  //cout<< "rehash failed" << std::endl;
	  return 2; 
	}
      /*
      else
	cout << "rehash for size " << getPrime(capacity) << "success" << std::endl;
      */
    }
  
  //linear probing
  int index = hash(key) % data.size();
  while (data[index].isOccupied || data[index].isDeleted) // skip full slots
    {
      if (data[index].key == key) //if key exists don't insert
	return 1;
      index = (index + 1) % data.size();
    }
  //empty slot, fill with new item
  data[index].key = key;
  //cout << data[index].key << " at line" << index << std::endl;
  data[index].isOccupied = true;
  data[index].isDeleted = false;
  data[index].pv = pv;

  filled++; // keep track of # of elements
  //cout << "insert success" << std::endl;
  return 0; // success!
}

bool hashTable::contains(const string &key)
{
  if (findPos(key) == -1)
    return false;
  return true;
}

int hashTable::hash(const string &key)
{
  // credits to http://stackoverflow.com/questions/8317508/hash-function-for-a-string
  int sum = 0;
  int seed = 101;
  for (unsigned int i = 0; i < key.size(); i++)
    sum = sum*seed + key[i];
  return sum;
}

int hashTable::findPos(const string &key)
{
  int hpos = hash(key) % data.size();  
  //cycle through non-matching cells
  while((data[hpos].isOccupied && data[hpos].key != key && !data[hpos].isDeleted) || // check if the cell is occupied and if the key is not matched
	data[hpos].isDeleted) // check if cell is deleted
    hpos = (hpos+1) % data.size();
  if(data[hpos].isOccupied == false) // check if cell you land on is empty 
    return -1;
  return hpos;
}
  
bool hashTable::rehash()
{
  capacity = getPrime(capacity);
  vector<hashItem> newtable;
  hashItem hitem; hitem.isOccupied = false; hitem.isDeleted = false;
  try // may fail to allocate memory
    {
      newtable.resize(capacity, hitem);
    }
  catch (std::bad_alloc& ba)
    {
      return false;
    }
  vector<hashItem> old = data;
  data = newtable;
  filled = 0;
  vector<hashItem>::iterator it = old.begin();
  for(;it != old.end(); it++)
    if (it->isOccupied==true && it->isDeleted == false)
      insert(it->key,it->pv);

  return true; // success!
  
}

unsigned int hashTable::getPrime(int size)
{
  int primes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593,49157, 98317, 196613, 393241, 786433, 1000003, 1572869, 2000003};

  for(int i = 0; i<18; i++)
    if (primes[i]>size)
      return primes[i];
  return 50331653;
}

void *hashTable::getPointer(const string &key, bool *b)
{
  int i = findPos(key);
  if (i == -1)
    {
      if (b)
	*b = false;
      return NULL;
    }
  else
    {
      if (b)
	*b = true;
      return data[i].pv;
    }
}

int hashTable::setPointer(const std::string &key, void *pv)
{
  int i = findPos(key);
  if (i == -1)
    return -1;
  else
    data[i].pv = pv;
  return 0;
}

bool hashTable::remove(const string &key)
{
  int index = findPos(key);
  if (index == -1)
    return false;
  data[index].isDeleted = true;
  return true;
}
