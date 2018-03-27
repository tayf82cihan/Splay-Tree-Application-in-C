#ifndef RedBlack_TREE_H
#define RedBlack_TREE_H

#define DEBUG

#include "General.h"

struct SPLAY_TREE_ROOT;

typedef struct SPLAY_TREE_ROOT* Splay_Tree_Root;

typedef struct _Object_func {
    COMPARISON_FUNC compare_component;
    COPY_FUNC copy_component;
    DESTRUCTION_FUNC destroy_component;
} Splay_Tree_Component_Funcs;

extern Splay_Tree_Root Splay_Tree_Create(Splay_Tree_Component_Funcs);
extern void Splay_Tree_Destroy(Splay_Tree_Root*);
extern void Splay_Tree_InsertElement(const Splay_Tree_Root, const Object);
extern Object Splay_Tree_DeleteElement(const Splay_Tree_Root, Object);
extern int Splay_Tree_GetNumberOfElements(const Splay_Tree_Root);
extern Object Splay_Tree_GetElement(const Splay_Tree_Root, const Object);
extern Object Splay_Tree_FindLargestElement(const Splay_Tree_Root);
extern Object Splay_Tree_FindSmallestElement(const Splay_Tree_Root);

#ifdef DEBUG
extern void Splay_Tree_PreorderTraversal(const Splay_Tree_Root);
extern void Splay_Tree_InorderTraversal(const Splay_Tree_Root);
extern void Splay_Tree_PostorderTraversal(const Splay_Tree_Root);
#endif

extern Object Splay_Tree_BFS_Search(const Splay_Tree_Root this, const Object info);

#endif
