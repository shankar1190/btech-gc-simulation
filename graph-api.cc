// Author: Shankar, shankar1190@gmail.com

#include "graph-api.h"

void MSGraphUtil :: TriggerGC() {
  for (int i = 0; i < roots.size(); i++) {
    DFSMark(roots[i]);
  }
  Sweep(first, NULL);
}

void MSGraphUtil :: DFSMark (Object* root) {
  if (root == NULL) return;
  root->seen = true;
  DFSMark(root->child);
}

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
