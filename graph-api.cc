// Author: Shankar, shankar1190@gmail.com

#include "graph-api.h"

// default constructor
MSGraphUtil :: MSGraphUtil() {
  first = NULL;
  last = NULL;
  num_objects = 0;
}

// This is triggered when there is not enough space on the heap. In our
// simulation, that is when num_objects equals max_objects.
void MSGraphUtil :: TriggerGC() {
  for (int i = 0; i < roots.size(); i++) {
    DFSMark(roots[i]);
  }
  Sweep(first, NULL);
}

// Simple Depth First Search to mark the nodes.
void MSGraphUtil :: DFSMark (Object* root) {
  if (root == NULL) return;
  root->seen = true;
  DFSMark(root->child);
}

// Sweeping the entire heap. We traverse the linked list in our simulation,
// resetting the "seen" flag then and there. Any object that does not have
// the seen flag set will be removed. Basic deletion of elements from a linked
// list.
void MSGraphUtil :: Sweep (Object* current, Object* prev) {
  if (current == NULL) return;

  if (current->seen == true) {
    current->seen = false;
    Sweep(current->next, current);
  } else {
    if (current->next != NULL & prev != NULL) {
      // Object to be removed is neither the
      // first nor the last item
      Object* temp = current;
      prev->next = current->next;
      current = current->next;
      delete temp;
      Sweep(current, current->next);
    } else if (current->next == NULL) {
      // Object to be removed is the last item
      last = prev;
      last->next = NULL;
      Object* temp = current;
      delete temp;
    } else {
      // Object to be removed is the first item
      first = current->next;
      Object* temp = current;
      current = current->next;
      delete temp;
      Sweep(current, current->next);
    }
  }
}

// Creating a new root item. 
void MSGraphUtil :: NewReference(const string& desc) {
  if (num_objects == max_objects) {
    TriggerGC();
  }
  
  if (num_objects < max_objects) {
    num_objects++;
    Object* obj = new Object(desc);
    roots.push_back(obj);
    if(first==NULL)
    {
      first=obj;
    }
    if(last==NULL)
    {
      last=obj;
    }
    else
    {
    last->next = obj;
    obj = last;
    }
  } else {
    cout << "Error! Unable to allocate memory!\n";
  }
}

// New object that would be pointed to by an existing pointer.
void MSGraphUtil :: New(const string& desc, Object* parent) {
  if (num_objects == max_objects) {
    TriggerGC();
  }

  if (num_objects < max_objects) {
    num_objects++;
    Object* obj = new Object(desc);
    parent->child = obj;
    last->next = obj;
    obj = last;
  } else {
    cout << "Error! Unable to allocate memory!\n";
  }
}

// Deleting root items for when they fall out of scope.
void MSGraphUtil :: EndLifetime(Object* obj) {
  int pos = -1;
  for (int i = 0; i < roots.size(); i++) {
    if (roots[i] == obj) {
      pos = i;
      break;
    }
  }
  if (pos != -1) roots.erase(roots.begin() + pos);
}


// The code below is the Stop-Copy GC. The util function definitions

// Default constructor
StopCopyUtil :: StopCopyUtil() {
  heap1_objects = 0;
  heap2_objects = 0;
  heap1_first = NULL;
  heap2_first = NULL;
  heap1_last = NULL;
  heap2_last = NULL;
}

// Creating a new root item. 
void StopCopyUtil :: NewReference(const string& desc, int ind) {
  int num; int max=50;
  SCObject* first, *last;
  if(ind==1)
  {
    num=heap1_objects;
    first=heap1_first;
    last=heap1_last;
  }
  if(ind==2)
  {
    num=heap2_objects;
    first=heap2_first;
    last=heap2_last;
  }
  if (num==max) {
    TriggerGC();
  }
  
  if (num < max) {
    num++;
    SCObject* obj = new SCObject(desc);
    if(first==NULL)
    {
      first=obj;
    }
    if(last==NULL)
    {
      last=obj;
    }
    else
    {
    last->next = obj;
    obj = last;
    }
    if(ind==1)
    {
      heap1_first=first;
      heap1_last=last;
      active.erase(active.begin(),active.end());
      active.insert(heap1_first);
    }
    if(ind==2)
    {
      heap2_first=first;
      heap2_last=last;
      inactive.erase(inactive.begin(),inactive.end());
      inactive.insert(heap2_first);
    }

  } else {
    cout << "Error! Unable to allocate memory!\n";
  }
}

// New object that would be pointed to by an existing pointer.
void StopCopyUtil :: New(const string& desc, SCObject* parent, int ind) {
  int num; int max=50;
  if(ind==1)
  {
    num=heap1_objects;
    last=heap1_last;
  }
  if(ind==2)
  {
    num=heap2_objects;
    last=heap2_last;
  }
  if (num == max) {
    TriggerGC();
  }

  if (num < max) {
    num++;
    SCObject* obj = new SCObject(desc);
    parent->child = obj;
    last->next = obj;
    obj = last;
    if(ind==1)
    {
      active.erase(active.begin(),active.end());
      active.insert(heap1_first);
    }
    if(ind==2)
    {
      inactive.erase(inactive.begin(),inactive.end());
      inactive.insert(heap2_first);
    }
  } else {
    cout << "Error! Unable to allocate memory!\n";
  }
}


// delete all elements from a linked list.
// TODO: Figure out a more elegant way to do this. Maybe we have to change the
// implementation of Stop-Copy completely.
int FlushHeap(vector <SCObject*> & v) {
  if(!v.empty())
  {
    v.erase(v.begin(), v.begin()+v.size());//this works, verified on dev-cpp
  }
  return v.empty();
  /*Object* delnode = heap1_first;
  Object* temp = delnode;
  while (temp != NULL) {
    temp = temp->next;
    delete delnode;
    delnode = temp;
  }
  heap1_first = NULL;*/
}

/*void FlushHeap2() {
  Object* delnode = heap2_first;
  Object* temp = delnode;
  while (temp != NULL) {
    temp = temp->next;
    delete delnode;
    delnode = temp;
  }
  heap1_first = NULL;
}*/

void Copy(vector<SCObject*> & from, vector <SCObject*> & to,SCObject* first, SCObject* last)
{
  int r=1;
  if(!to.empty())
  {
    r=FlushHeap(to);
  }
  if(r)
  {
    SCObject* temp=new SCObject;
    for (int i=0;i<from.size();i++)
    {
        temp=from.at(i);
        while(temp!=NULL)
        {
           if(temp.alive==1){
           last->next=temp;
           last=temp;
           to.erase(to.begin(),to.end());
           to.push_back(first);
           }
           temp=temp->next;
        }
    }
    from.erase(from.begin(),from.begin()+from.size());
  }
}

void CallCopy()
{
  if(heap1_objects==max)
     Copy(active, inactive,heap2_first,heap2_last);
  if(heap2_objects==max)
     Copy(inactive, active, heap1_first, heap1_last);
}
  
}