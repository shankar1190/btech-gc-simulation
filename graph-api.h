// Author: Shankar, shankar1190@gmail.com
// Class Object: This is a mock object and a node in our simulation graph.
// Any object that is allocated is in the form of this node in our
// simulation. We have a class MSGraphUtil that has the basic utilities
// of the standard Mark and Sweep. Roots are maintained in a vector
// and all the allocated objects are manintained on a linked list. "Sweeping"
// is done by traversing that linked list.

// TODO (Neeraja and Radhika!): Very Important. Rewrite generetic utilities
// outside the MSGraphUtil class, viz., New(), NewReference(), EndLifetime().


#ifndef GRAPH_API
#define GRAPH_API

#include <string>
#include <vector>
#include <cprocess>
#include <cctype>
#include <iostream>

// Utility Macros. Not a good idea to define macros and all, but I still
// do it cos I'm a bad-ass.
#define CHECK(x) if(!(x)){cerr<<"Check not satisfied! Aborting...\n";exit(1);}

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
    first = NULL;
    last = NULL;
    
    desc = "";
  }
  Object (const string& description) {
    next = NULL;
    child = NULL;
    desc = description;
  }
};

//Stop Copy works on a distinction between live and dead objects
//Define a child class of objects with an extra field to indicate if the object is alive or dead.
//As such, i have no idea how thats decided. We could figure that out later( a system call that 
//could give some info on the scope of the object or something)
class SCObject : public Object
{
   public:
   bool alive;
   SCObject() {
    alive=1;
   }
}
class MSGraphUtil {
 public:
  MSGraphUtil(); 
  int num_objects;
  Object* first;
  Object* last;
  vector <Object*> roots;
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
  vector <SCObject*> active;
  vector <SCObject*> inactive;
  int heap1_objects;
  int heap2_objects;
  //SCObject* heap1_first;
  //SCObject* heap1_last;
  //SCObject* heap2_first;
  //SCObject* heap2_last;

  void FlushHeap(vector <SCObject*> &v);//Cant we delete the elements as they're getting copied
  //void Copy1to2();
  //void Copy2to1();
  void Copy(vector <SCObject*> &from, vector <SCObject*> &to);
  //A function to call copy depending on which region is exhausted.
  void CallCopy();
  void TriggerGC();
};

#endif // GRAPH_API
