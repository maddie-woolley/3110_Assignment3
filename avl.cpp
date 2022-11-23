#include <stdio.h>
#include <iostream>
#include "avl.hpp"
using namespace std;

#define IS_ROOT 0
#define IS_LEFT 1
#define IS_RIGHT 2

/**
 * Internal method to insert into a subtree.
 * x is the item to insert.
 * t is the node that roots the subtree.
 * Set the new root of the subtree.
 */
// aux method to figure max between 2 nodes
int max(int a, int b){
    if (a>b){
        return a;
    }
    else{
        return b;
    }
}
//Function: Depth
//Inputs: Node
//Returns : Height of inputting node
int depth(AvlNode *n){
  if (n==NULL){
      return 0;
  }
  else{
      return n->height;
  }
}

//Function: GetBalance
// Inputs: Root node
// Returns : Number representing balance factor , 0 = balanced
int getBalance(AvlNode *N){
    if (N==NULL){
        return 0;
    }
    else{
        return (depth(N->left)- depth(N->right));
    }
}
//Function: Min Value
//Inputs: Root Node
//Returns: Minimum node in left tree
AvlNode *minValueNode(AvlNode* node){
    AvlNode *current = node;
    while(current->left != NULL){
        current = current->left;
    }
    return current;
}
// right rotate the subtree rooted with y
AvlNode *rightRotate(AvlNode *y) {
    //make temp nodes
    AvlNode *x = y->left;
    AvlNode *T2 = x->right;

    //perform rotations:
    x->right =y;
    y->left = T2;

    //Update Heights
    y->height = max(depth(y->left), depth(y->right)) + 1;
    x->height = max(depth(x->left), depth(x->right)) + 1;

    //return new root
    //y = x;
    return x;
}

// left rotate the subtree rooted with x
AvlNode *leftRotate(AvlNode *x){
    //make temp nodes
    AvlNode *y = x->right;
    AvlNode *T2 = y->left;
    //perform rotations
    y->left = x;
    x->right = T2;
    //update heights
    x->height = max(depth(x->left), depth(x->right))+1;
    y->height = max(depth(y->left), depth(y->right))+1;
    //return new root
    return y;
}
AvlNode* & insert ( const int & info, AvlNode * & root) {

    if (root == NULL) {
        root = new AvlNode(info, NULL, NULL);
    }
    //for left keys
    if (info < root->element){
        root->left = insert(info, root->left);
    }
    //for right keys
    else if (info > root->element) {
        root->right = insert(info, root->right);
    }


    //Update height
    root->height = 1 + max(depth(root->left), depth(root->right));
    //check if tree is balanced
    int balance = getBalance(root);
    //if not balanced
    //LL Case
    if ((balance > 1) && (info < root->left->element)) {
        cout<<"balancing happening - LL"<<endl;
        root = rightRotate(root);
   }
    //RR case
    if ((balance < -1) && (info > root->element)) {
        cout<<"balancing happening --RR"<<endl;
        root = leftRotate(root);
    }
    //LR case
    if ((balance > 1) && (info > root->element)){
        cout<<"balancing happening --LR"<<endl;
        root->left = leftRotate(root->left);
        root = leftRotate(root);
    }
    //RL case
    if ((balance < -1) && (info < root->right->element)){
    cout<<"balancing happening --RL"<<endl;
        root->right = rightRotate(root->right);
        root = leftRotate(root);
    }
    //return the node pointer
    return root;
}

/**
 * Internal method to remove from a subtree.
 * info is the item to remove.
 * root is the node that roots the subtree.
 * Set the new root of the subtree.
 */


AvlNode* & remove(const int & info, AvlNode * & root){
    if (root ==NULL){
        return root;
    }
    //if the number is smaller than root
    if (info < root->element) {
        root->left = remove(info, root->left);
    }
    //if the number is bigger than root
    if (info > root->element) {
        root->right = remove(info, root->right);
    }
    //if the number is the root
    else {
        // node has 1 or 0 children
        if ((root->left == NULL)||(root->right ==NULL)) {
            AvlNode *temp = root->left ? root->left : root->right;
            // node has no children
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            // one child
            else {
                *root = *temp;
                free(temp);
            }
        }
        //node with 2 children, get the smallest in the right subtree
        else{
            AvlNode *temp = minValueNode(root->right);
            //copy the new root to the temp
            root->element = temp->element;
            // and delete the original node
            root->right = remove(info, root->right);
        }
    }
    // and return (for 1 child case)
    if (root==NULL){
        return root;
    }
    //update height
    root->height = 1 + max(depth(root->left), depth(root->right));
    //check balance
    int balance = getBalance(root);
    //if it's unbalanced:
    //LL Case
    if ((balance > 1) && (getBalance(root->left)>=0)){
        rightRotate(root);
    }
    //RR Case
    if (balance <-1 && getBalance(root->right)<=0){
        leftRotate(root);
    }
    //LR Case
    if (balance >1 && getBalance(root->left)<0) {
        root->left = leftRotate(root->left);
        rightRotate(root);
    }
    //RL Case
    if (balance <-1 && getBalance(root->right)>0){
        root->right = rightRotate(root->right);
        leftRotate(root);
    }
    return root;
}
/***
 * You will probably need auxiliary methods to
 *  - find the minimum of tree
 *  - rotate (single and double, in both directions
 *  - balance am AVL  tree
 */

/*
 * Print methods, do not change
 */
void print(AvlNode *root, int level, int type) {
    if (root == NULL) {
        return;
    }
    if (type == IS_ROOT) {
        std::cout << root -> element << "\n";
    } else {
        for (int i = 1; i < level; i++) {
            std::cout << "| ";
        }
        if (type == IS_LEFT) {
            std::cout << "|l_" << root -> element << "\n";
        } else {
            std::cout << "|r_" << root -> element << "\n";
        }
    }
    if (root -> left != NULL) {
        print(root -> left, level + 1, IS_LEFT);
    }
    if (root -> right != NULL) {
        print(root -> right, level + 1, IS_RIGHT);
    }
}
void print(AvlNode *root) {
    print(root, 0, IS_ROOT);
}
/*
 * END Print methods, do not change
 */


/* 
 * Main method, do not change
 */
int main(int argc, const char * argv[]) {
    AvlNode *root = NULL;
    std::string filename = argv[1];
    freopen(filename.c_str(), "r", stdin);
    std::string input;
    int data;
    while(std::cin >> input){
        if (input == "insert"){
            std::cin>>data;
            insert(data, root);
        } else if(input == "delete"){
            std::cin>>data;
            remove(data, root);
        } else if(input == "print"){
            print(root);
            std::cout << "\n";
        } else{
            std::cout<<"Data not consistent in file";
            break;
        }
    }
    return 0;
}
/*
 * END main method
 */

