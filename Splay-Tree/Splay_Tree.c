#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#include "General.h"
#include "Splay_Tree.h"
#include "Queue.h"

#define NOTHING    0
#define LEFT_NODE  1
#define RIGHT_NODE 2

struct _NODE {
    Object info;
    struct _NODE* parent;
    struct _NODE* left;
    struct _NODE* right;
};

typedef struct _NODE* _Node;   

struct SPLAY_TREE_ROOT {
    COMPARISON_FUNC compare_component;
    COPY_FUNC copy_component;
    DESTRUCTION_FUNC destroy_component;
    int num_of_elements;    
    struct _NODE* childs;
};

static _Node Tree_FindLargestElement(const _Node);
static _Node Tree_FindSmallestElement(const _Node);

static _Node root_child;

static _Node getGrantParent(const _Node this) {

    _Node gParent = NULL;
     
    if ((this) && (this->parent))
        gParent = this->parent->parent;
    
    return gParent;
    
} /* end of static _Node getGrantParent(const _Node) */

static void rotate_left(const _Node this) {
    
    _Node right_child = this->right, parent = this->parent;
    
    if (right_child == NULL) return;
          
    if (parent) {
        if (parent->left == this)
            parent->left = right_child;
        else 
            parent->right = right_child;
    } /* end of if (parent) */
	
    right_child->parent = parent;
    
    if (right_child->left)
        right_child->left->parent = this;
		
    this->right = right_child->left;
    right_child->left = this;
    this->parent = right_child;
	
    if (right_child->parent == NULL)
        root_child = right_child;
    
} /* end of static void rotate_left(const _Node) */

static void rotate_right(const _Node this) {
    
    _Node left_child = this->left, parent = this->parent;
    
    if (left_child == NULL) return;
          
    if (parent) {
        if (parent->left == this)
            parent->left = left_child;
        else 
            parent->right = left_child;
    } /* end of if (parent) */
	
    left_child->parent = parent;
    
    if (left_child->right)
        left_child->right->parent = this;
		
    this->left = left_child->right;
    left_child->right = this;
    this->parent = left_child;
	
    if (left_child->parent == NULL)
        root_child = left_child;

} /* end of static void rotate_right(const _Node) */

static void zig_step(const _Node this) {

    if (getGrantParent(this)) return;
    
    /* this->parent is root */
    if (this->parent->left == this)
        rotate_right(this->parent);
    else
        rotate_left(this->parent);
    
} /* end of static void zig_step(const _Node) */

static void zigzig_step(const _Node this) {

    _Node g, p;
    
    if ((g = getGrantParent(this)) == NULL) return;
    
    if ((g->left) && (g->left->left == this)) {
        rotate_right(g);
        rotate_right(this->parent);
        return;
    }
    
    if ((g->right) && (g->right->right == this)) {
        rotate_left(g);
        rotate_left(this->parent);
        return;
    }
    
} /* end of static void zigzig_step(const _Node this) */

static void zigzag_step(const _Node this) {

    _Node g;
    
    if ((g = getGrantParent(this)) == NULL) return;
    
    if ((g->left) && (g->left->right == this)) {
        rotate_left(this->parent);
        rotate_right(g);
        return;
    }
    
    if ((g->right) && (g->right->left == this)) {
        rotate_right(this->parent);
        rotate_left(g);
        return;
    }
    
} /* end of static void zigzag_step(const _Node this) */

/* splay the tree at (_Node) this */
static void perform_splaying(const Splay_Tree_Root root, const _Node this) {

   if (this == NULL) return;

    while (this->parent) {
    
      zig_step(this); 
      zigzig_step(this);
      zigzag_step(this);
      
    } /* end of  while (this->parent) */
    
    if (root_child)
        root->childs = root_child;
    root_child = NULL;
    
} /* end of static void perform_splaying(const Splay_Tree_Root, const _Node) */

static _Node create_Node(const Object info) {

    _Node this = (_Node) malloc(sizeof(struct _NODE));
	
    if (!this) {
        fprintf(stderr, "Out of memory for creating _Node...\n");
        return NULL;
    } /* end of if (!this) */
    
    this->info = info;
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
    
    return this;
    
} /* end of static _Node create_Node(const Object) */


Splay_Tree_Root Splay_Tree_Create(Splay_Tree_Component_Funcs funcs) {
    Splay_Tree_Root ret_root;
    
    if ((ret_root = (Splay_Tree_Root) malloc(sizeof(struct SPLAY_TREE_ROOT))) == NULL) {
        fprintf(stderr, "Out of memory for creating SPLAY tree!!!\n");
        return NULL;
    } /* end of if ((ret_root = (Splay_Tree_Root) malloc(sizeof(struct SPLAY_TREE_ROOT))) == NULL) */
	
    ret_root->compare_component = funcs.compare_component;
    ret_root->copy_component = funcs.copy_component;
    ret_root->destroy_component = funcs.destroy_component;
    ret_root->num_of_elements = 0;
    ret_root->childs = NULL;
    
    return ret_root;
    
} /* end of Splay_Root Splay_Tree_Create(Splay_Tree_Component_Funcs) */

int Splay_Tree_GetNumberOfElements(const Splay_Tree_Root root) {

    return ((root != NULL) && (root->childs != NULL)) ? root->num_of_elements : 0;
    
} /* end of int Splay_Tree_GetNumberOfElements(const Splay_Tree_Root) */

Object Splay_Tree_GetElement(const Splay_Tree_Root this, const Object info) {

    _Node node, last_non_null_node;
    Object ret_info = NULL;
    if (!this) return NULL;
    
    node = this->childs;
    
    while ((node && node->info) && (!ret_info)) {
	      last_non_null_node = node;
        int cmp_result = this->compare_component(info, node->info);
        
        if (cmp_result > 0) node = node->left;
        else if (cmp_result < 0) node = node->right;
        else ret_info = node->info;
		
    } /* end of while ((node && node->info) && (!ret_info)) */
    
    if (ret_info)
        perform_splaying(this, node);
    else
        perform_splaying(this, last_non_null_node);

/*    if (root_child)
        this->childs = root_child; */
    
    return ret_info;
    
} /* end of Object Splay_Tree_GetElement(const Splay_Tree_Root, const Object) */

void Tree_Destroy(_Node *this, DESTRUCTION_FUNC destruction_func) {

    if (*this != NULL) {        
        Tree_Destroy(&((*this)->left), destruction_func);
        Tree_Destroy(&((*this)->right), destruction_func);        
        destruction_func((*this)->info);
        (*this)->info = NULL;
        (*this)->parent = NULL;
        free(*this);
        *this = NULL;
    } /* end of if (*this != NULL) */
	
} /* end of void Tree_Destroy(Node *this, DESTRUCTION_FUNC) */

void Splay_Tree_Destroy(Splay_Tree_Root *this) {

    if (*this != NULL) {
        Tree_Destroy(&((*this)->childs), (*this)->destroy_component);        
        free(*this);
        *this = NULL;
    } /* end of if (*this != NULL) */
	
} /* end of void Splay_Tree_Destroy(Splay_Tree_Root*) */

static BOOL insert_node(const Splay_Tree_Root root, _Node this, const _Node new_node) {

    COMPARISON_FUNC cmp = root->compare_component;   
    _Node parent;
    int which_node;
    
    if (this == NULL) return FALSE;    
    
    while (this) {
	
        parent = this;
        if (cmp(new_node->info, this->info) > 0) {
            which_node = LEFT_NODE;
            this = this->left;
        } else {
            which_node = RIGHT_NODE;
            this = this->right;
        } /* end of if-else */
		
    } /* end of while (this) */
    
    this = parent;
    
    if (which_node == LEFT_NODE) {
        this->left = new_node;
        new_node->parent = this;
    } else {
        this->right = new_node;
        new_node->parent = this;
    } /* end of if-else */   
    
    perform_splaying(root, new_node);
    
    return TRUE;

} /* end of static BOOL insert_node(const Splay_Tree_Root, const _Node, const _Node) */

void Splay_Tree_InsertElement(const Splay_Tree_Root this, const Object info) {

    _Node temp_node;        
    
    if ((temp_node = create_Node(info)) == NULL) return;

    if (!this->childs) {        
        this->childs = temp_node;
        this->num_of_elements++;      
        return;
    } /* end of if (!this->childs) */
    
    if (insert_node(this, this->childs, temp_node) == TRUE) {
 /*       if (root_child)
            this->childs = root_child; */        
        this->num_of_elements++;
    } /* end of if (insert_node(this, this->childs, temp_node) == TRUE) */
    
    return;
    
} /* end of void Splay_Tree_Insert(const Splay_Tree_Root, const Object) */

/*

Following are the different cases to delete a key k from splay tree.

1. If Root is NULL: We simply return the root.
2. Else Splay the given key k. If k is present, then it becomes the new root. If not present, 
   then last accessed leaf node becomes the new root.
3. If new root’s key is not same as k, then return the root as k is not present.
4. Else the key k is present.
   . Split the tree into two trees Tree1 = root’s left subtree and Tree2 = root’s right subtree and 
     delete the root node.
   . Let the root’s of Tree1 and Tree2 be Root1 and Root2 respectively.
   . If Root1 is NULL: Return Root2.
   . Else, Splay the maximum node (node having the maximum value) of Tree1.
   . After the Splay procedure, make Root2 as the right child of Root1 and return Root1.

*/

Object Splay_Tree_DeleteElement(const Splay_Tree_Root this, const Object info) {

    COMPARISON_FUNC cmp = this->compare_component;
    _Node node, tree_root1, tree_root2;
    Object ret_info = NULL;
    
    if ((!this) || (this->num_of_elements == 0)) return NULL;
          
    ret_info = Splay_Tree_GetElement(this, info);
        
    if (!ret_info) return this->childs->info;
    
    if (cmp(this->childs->info, ret_info) != 0) return this->childs->info;
    
    tree_root1 = this->childs->left;
    if (tree_root1)
        tree_root1->parent = NULL;
    
    tree_root2 = this->childs->right;
    if (tree_root2)
        tree_root2->parent = NULL;
    
    free(this->childs);
    root_child = NULL;
    
    if (tree_root1 == NULL) {
        this->childs = tree_root2;
        return ret_info;
    }   

    this->childs = tree_root1;     
    
    if (tree_root2 == NULL)        
        return ret_info;     
     
    perform_splaying(this, Tree_FindLargestElement(this->childs));
/*    if (root_child)
        this->childs = root_child; */ 
//    if (tree_root2)
    tree_root2->parent = this->childs; 
    this->childs->right = tree_root2;
        
    if ((--this->num_of_elements) == 0) 
        this->childs = NULL;
		
    return ret_info;
    
} /* end of Object Splay_Tree_DeleteElement(const Splay_Tree_Root, const Object) */

Object Splay_Tree_FindLargestElement(const Splay_Tree_Root this) {

    _Node node;
	
    if (!this) return NULL; 
	
    node = Tree_FindLargestElement(this->childs);
    
    return (node != NULL) ? node->info : NULL;
    
} /* end of Object Splay_Tree_FindLargestElement(const Splay_Tree_Root, const Object) */ 

Object Splay_Tree_FindSmallestElement(const Splay_Tree_Root this) {

    _Node node;
	
    if (!this) return NULL;    
	
    node = Tree_FindSmallestElement(this->childs);
    
    return (node != NULL) ? node->info : NULL;
    
} /* end of Object Splay_Tree_FindSmallestElement(const Splay_Tree_Root, const Object) */ 

static _Node Tree_FindLargestElement(const _Node this) {

    _Node ret_node = this;
	
    if (ret_node == NULL) return NULL;    
	
    while (ret_node->right) ret_node = ret_node->right;
        
    return ret_node;
        
} /* end of static _Node Tree_FindLargestElement(const _Node) */

static _Node Tree_FindSmallestElement(const _Node this) {

    _Node ret_node = this;
	
    if (ret_node == NULL) return NULL;    
	
    while (ret_node->left) ret_node = ret_node->left;
        
    return ret_node;    

} /* end of static _Node Tree_FindSmallestElement(const _Node) */

void empty_destroy(Object to_be_destroyed) {}

Object Splay_Tree_BFS_Search(const Splay_Tree_Root this, const Object info) {

    Queue_Component_Funcs queue_comp_funcs = { (COMPARISON_FUNC)this->compare_component, (COPY_FUNC)this->copy_component, (DESTRUCTION_FUNC) empty_destroy };
    Queue queue;    
    _Node node;
    Object ret_info = NULL;
	
    if ((this == NULL) || (this->childs == NULL)) return NULL;
	
	queue = Queue_Create(DEFAULT, queue_comp_funcs);    
    node = this->childs;
	
#ifdef DEBUG
    printf("\n Node\tParent\n");
	printf("------\t------\n");
#endif

#ifdef DEBUG
    while (node) {
#else    
    while (node && !ret_info) {
#endif	
#ifdef DEBUG     
		printf(" %d \t%d\n", 
               *((Integer) node->info), (node->parent != NULL) ? *((Integer) node->parent->info) : 0);
#endif

        if (this->compare_component(info ,node->info) == 0) { 
            ret_info = node->info;
#ifndef DEBUG			
			continue;
#endif
		} /* end of if (this->compare_component(info ,node->info) == 0) */
            
        if (node->left) Queue_Add(queue, (Object)(node->left));
        if (node->right) Queue_Add(queue, (Object)(node->right));
        node =  (Queue_IsEmpty(queue) == FALSE) ? (_Node) Queue_Remove(queue) : NULL;
        
    } /* end of while (node && !ret_info) */
    
    Queue_Destroy(&queue);
    
    return ret_info;

} /* end of Object Splay_Tree_BFS_Search(const Splay_Tree_Root, const Object) */

#ifdef DEBUG

static void Tree_PreorderTraversal(_Node this) {

    if (this != NULL) {
        printf(" %d \t%d\n", *((Integer) this->info), (this->parent != NULL) ? *((Integer) this->parent->info) : 0);
        Tree_PreorderTraversal(this->left);    
        Tree_PreorderTraversal(this->right);    
    } /* end of if (this != NULL) */
	
} /* end of static void Tree_PreorderTraversal(Node) */

static void Tree_InorderTraversal(_Node this) {

    if (this != NULL) {
        Tree_InorderTraversal(this->left);    
        printf(" %d \t%d\n", *((Integer) this->info), (this->parent != NULL) ? *((Integer) this->parent->info) : 0);
        Tree_InorderTraversal(this->right);    
    } /* end of if (this != NULL) */
	
} /* end of static void Tree_InorderTraversal(Node) */

static void Tree_PostorderTraversal(_Node this) {

    if (this != NULL) {    
        Tree_PostorderTraversal(this->left);    
        Tree_PostorderTraversal(this->right); 
        printf(" %d \t%d\n", *((Integer) this->info), (this->parent != NULL) ? *((Integer) this->parent->info) : 0);
    } /* end of if (this != NULL) */

} /* end of static void Tree_PostorderTraversal(Node) */

void Splay_Tree_PreorderTraversal(const Splay_Tree_Root this) {

    printf("\n Pre-Order Traversal");
	  printf("\n----------------------");
    printf("\n Node\tParent\n");
	  printf("------\t------\n");

    if (this->childs != NULL) 
        Tree_PreorderTraversal(this->childs); 
   
} /* end of void void Splay_Tree_PreorderTraversal(const Splay_Tree_Root) */

void Splay_Tree_InorderTraversal(const Splay_Tree_Root this) {

    printf("\n In-Order Traversal");
	  printf("\n----------------------");
    printf("\n Node\tParent\n");
	  printf("------\t------\n");

    if (this->childs != NULL) 
        Tree_InorderTraversal(this->childs); 
   
} /* end of void Splay_Tree_InorderTraversal(const Splay_Tree_Root) */

void Splay_Tree_PostorderTraversal(const Splay_Tree_Root this) {

    printf("\n Post-Order Traversal");
	  printf("\n----------------------");
    printf("\n Node\tParent\n");
	  printf("------\t------\n");

    if (this->childs != NULL) 
        Tree_PostorderTraversal(this->childs); 
   
} /* end of void Splay_Tree_PostorderTraversal(const Splay_Tree_Root) */

#endif


