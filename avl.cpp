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

//aux method to figure depth of a subtree

int depth(AvlNode *n){
  if (n==NULL){
      return 0;
  }
  else{
      return n->height;
  }
}

// aux method to check if tree is balanced
int getBalance(AvlNode *N){
    if (N==NULL){
        return 0;
    }
    else{
        return (depth(N->left)- depth(N->right));
    }
}
//minvalue
AvlNode *minValueNode(AvlNode* node){
    AvlNode *current = node;
    while(current->left != NULL){
        current = current->left;
    }
    return current;
}
// right rotate the subtree rooted with y
AvlNode *rightRotate(AvlNode *y) {
    AvlNode *x = y->left;
    AvlNode *T2 = x->right;

    //perform rotations:
    x->right =y;
    y->left = T2;

    //Update Heights
    y->height = max(depth(y->left), depth(y->right)) + 1;
    x->height = max(depth(x->left), depth(x->right)) + 1;

    //return new root
    return x;
}

// left rotate the subtree rooted with x
AvlNode *leftRotate(AvlNode *x){

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
void insert ( const int & info, AvlNode * & root) {

    if (root == NULL) {
        root = new AvlNode(info, NULL, NULL, 1 );
    }
    //for left keys
    if (info < root->element){
        insert(info, root->left);
    }
    //for right keys
    else if (info > root->element) {
        insert(info, root->right);
    }

    //Unlock this section to see how the rotations are working
    //Update height of ancestor node
    root->height = 1 + max(depth(root->left), depth(root->right));
    int balance = getBalance(root);

    //LL case--you are a problem, depth and such are good
    if ((balance > 1) && (info < root->left->element)) {
        cout<<"balancing happening - LL"<<endl;
        rightRotate(root);
   }

    //RR case
    if ((balance < -1) && (info > root->element)) {
        cout<<"balancing happening --RR"<<endl;
        leftRotate(root);
    }
    //LR case
    if ((balance > 1) && (info > root->element)){
        cout<<"balancing happening --LR"<<endl;
        leftRotate(root->left);
        leftRotate(root);
    }
    //RL case
    if ((balance < -1) && (info < root->right->element)){
    cout<<"balancing happening --RL"<<endl;
        rightRotate(root->right);
        leftRotate(root);
    }
}

/**
 * Internal method to remove from a subtree.
 * info is the item to remove.
 * root is the node that roots the subtree.
 * Set the new root of the subtree.
 */


void remove (const int & info, AvlNode * & root ) {
    if (info < root->element) {
        remove(info, root->left);
    }
    if (info > root->element) {
        remove(info, root->right);
    }
    else {
        if (root->left == NULL) {
            AvlNode *temp = root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
                free(temp);
            }
        }
        if (root->right == NULL) {
            AvlNode *temp = root->left;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else {
                *root = *temp;
                free(temp);
            }
        }
        else{
            AvlNode *temp = minValueNode(root->right);
            root->element = temp->element;
            remove(temp->element,root->right);
        }
    }

    root->height = 1 + max(depth(root->left), depth(root->right));
    int balance = getBalance(root);

    if ((balance > 1) && (getBalance(root->left)>=0)){
        rightRotate(root);
    }
    if (balance >1 && getBalance(root->left)<0){
        leftRotate(root->left);
        rightRotate(root);
    }
    if (balance <-1 && getBalance(root->right)<=0){
        leftRotate(root);
    }
    if (balance <-1 && getBalance(root->right)>0){
        root->right = rightRotate(root->right);
        leftRotate(root);
    }
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

