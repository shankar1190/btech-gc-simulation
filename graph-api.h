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

const int heap1_max = 50;
const int heap2_max = 50;

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

class StopCopyUtil {
 public:
  StopCopyUtil();
  vector <Object*> roots1;
  vector <Object*> roots2;
  int heap1_objects;
  int heap2_objects;
  Object* heap1_first;
  Object* heap1_last;
  Object* heap2_first;
  Object* heap2_last;

  void FlushHeap1();
  void FlushHeap2();
  void Copy1to2();
  void Copy2to1();
  void TriggerGC();
};

#endif // GRAPH_API
