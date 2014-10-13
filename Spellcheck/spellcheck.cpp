/*
spellcheck.cpp
Author: Kelvin Lin
Sept 18 2014
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <time.h>

#include "hash.h"

using namespace std;

int main()
{
  string dictionary, ifilename, ofilename;
  cout << "Enter name of dictionary: ";
  cin >> dictionary;

  int line = 0;
  ifstream dict(dictionary.c_str());
  char buffer[256];
  hashTable* dictHash = new hashTable();
  int i = 0;
  char c = '\0';

  // read dictionary
  time_t start = time(0);
  while (dict.good())
    {
      //read word in regardless of length
      for (i = 0; dict.good() && i<256; i++)
	{
	  c = dict.get(); // read character
	  if (c >= 'A' && c <= 'Z') // correct capital letters
	    c += 32;	  
	  else if (c == '\n') // check if valid character
	    {
	      c = '\0';
	      buffer[i] = c;
	      break;
	    }
	  buffer[i] = c;
	}
      // ignore words that are too long
      // insert buffer into hashtable
       if (buffer[0] != '\0')
	 {
	   string sbuff(buffer);
	   if (sbuff.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-'") == std::string::npos)
	     dictHash->insert(buffer);
	 }
      
    }

  cout << "Total time (in seconds) to load dictionary: " << time(0) - start << endl;
  
  // load input file
  cout << "Enter name of input file: ";
  cin >> ifilename;
  ifstream doc(ifilename.c_str());
  cout << "Enter name of output file: ";
  cin >> ofilename;
  ofstream outfile(ofilename.c_str());

  line = 1;
  bool linebool = false;
  // parse document
  
  start = time(0);
  while (doc.good())
    {
      // read word in
      for (i = 0; doc.good(); i++)
	{
	  char c = doc.get();	  
	  if (c >= 'A' && c <= 'Z')
	    c += 32;	  
	  else if (!(c >= 'a' && c <= 'z') && 
		   c != '\'' && c != '-' && 
		   !(c >= '0' && c <= '9'))
	    {
	      if (c == '\n')
		{
		  linebool = true;
		}
	      c = '\0';
	      buffer[i] = c;
	      break;
	    }
	  buffer[i] = c;
	}

      buffer[i] = '\0';
      // check words against the dictionary
      if (buffer[0] != '\0')
	{
	  if (strlen(buffer) <= 20)
	    {
	      string sbuff(buffer);
	      if (sbuff.find_first_of("0123456789") == std::string::npos)
		if (!(dictHash->contains(buffer)))
		  outfile << "Unknown word at line " << line << ": "<< buffer << endl;
	    }
	  else
	    {
	      buffer[20] = '\0';
	      outfile << "Long word at line " << line << ", starts: " << buffer << endl;
	    }
	}
      if (linebool == true)
	{
	  line++;
	  linebool = false;
	}
    }
  cout << "Total time (in seconds) to check document: " << time(0) - start << endl;
  return 0;
}
