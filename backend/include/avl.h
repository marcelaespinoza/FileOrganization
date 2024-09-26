#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <vector>
#include <fstream>
#include "record.h"

using namespace std;

class AVLFile {
private:
    string filename;
    long pos_root;

public:
    AVLFile(string filename);
    void insert(Record record);
    void delete_by_key(int cod);
    Record search_by_key(int cod);
    vector<Record> inorder();
    vector<Record> rangeSearch(int start, int end);

private:
    void insert_and_balance(long& pos_node, Record record, fstream& file);
    void balance(fstream& file, long pos_node);
    int BalancingFactor(long pos_node, fstream& file);
    long rightRotate(fstream& file, long y_pos);
    long leftRotate(fstream& file, long x_pos);
    int height(long pos_node, fstream& file);
    void updateHeight(long pos_node, fstream& file);
    void inorder(long pos_node, vector<Record>& result, fstream& file);
    void rangeSearch(long pos_node, int start, int end, vector<Record>& results, fstream& file);
    long deleteNode(long pos_node, int cod, fstream& file);
};

#endif // AVL_H

