// Author: Shankar, shankar1190@gmail.com
// Class Object: This is a mock object and a node in our simulation graph.
// Any object that is allocated is in the form of this node in our
// simulation. We have a class MSGraphUtil that has the basic utilities
// of the standard Mark and Sweep. Roots are maintained in a vector
// and all the allocated objects are manintained on a linked list. "Sweeping"
// is done by traversing that linked list.

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
