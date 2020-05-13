#ifndef __REVERSESTRINGNODE_H__
#define __REVERSESTRINGNODE_H__

/**
 * struct definition of class and external definition of class table
 */
struct ReverseStringNode_class {
  struct StringNode_class* superClass;
  int  (*compareTo) (void*, void*);
  void (*printNode) (void*);
  void (*insert)    (void*, void*);
  void (*print)     (void*);
  void (*delete)    (void*);
};
extern struct ReverseStringNode_class ReverseStringNode_class_table;

/**
 * struct definition of object
 */
struct ReverseStringNode;

struct ReverseStringNode {
  struct StringNode_class* class;

  struct ReverseStringNode* left;
  struct ReverseStringNode* right;
  char* s;
};

/**
 * definition of methods implemented by this class
 */
void ReverseStringNode_ctor(void*, char*);
int ReverseStringNode_compareTo(void*, void*);

/**
 * definition of new for class
 */
void* new_ReverseStringNode(char*);

#endif /*__REVERSESTRINGNODE_H__*/
