// Author: Shankar, shankar1190@gmail.com

#include "graph-api.h"

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
    last->next = obj;
    obj = last;
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
