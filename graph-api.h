// Author: Shankar, shankar1190@gmail.com

#ifndef GRAPH_API
#define GRAPH_API

#include <string>
#include <vector>
#include <iostream>

using namespace std;

const int max_objects = 100;

class Object {
 public:
  bool seen;
  Object* next;
  Object* child;
  string desc;
  Object() {
    next = NULL;
    child = NULL;
    desc = "";
  }
  Object (const string& description) {
    next = NULL;
    child = NULL;
    desc = description;
  }
};

class MSGraphUtil {
 public:
  MSGraphUtil(); 
  int num_objects;
  vector <Object*> roots;
  Object* first;
  Object* last;
  void DFSMark(Object* root);
  void Sweep(Object* current, Object* prev);
  void New(const string& desc, Object* parent);
  void NewReference(const string& desc);
  void EndLifetime(Object* reference);
  void TriggerGC();
};
#endif // GRAPH_API
