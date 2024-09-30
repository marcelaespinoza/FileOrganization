#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;

template<typename Key, typename T>
class AVLFile {
private:
    string filename;
    long pos_root;

public:
    AVLFile(string filename);
    void insert(T record, int& reads, int& writes);
    void delete_by_key(Key key, int& reads, int& writes);
    T search_by_key(Key key, int& reads, int& writes);
    vector<T> inorder(int& reads, int& writes);
    vector<T> rangeSearch(Key key_begin, Key key_end, int& reads, int& writes);

private:
    void insert_and_balance(long& pos_node, T record, fstream& file, int& reads, int& writes);
    void balance(fstream& file, long pos_node, int& reads, int& writes);
    int BalancingFactor(long pos_node, fstream& file, int& reads, int& writes);
    long rightRotate(fstream& file, long y_pos, int& reads, int& writes);
    long leftRotate(fstream& file, long x_pos, int& reads, int& writes);
    int height(long pos_node, fstream& file, int& reads, int& writes);
    void updateHeight(long pos_node, fstream& file,int& reads, int& writes);
    void inorderP(long pos_node, vector<T>& result, fstream& file, int& reads, int& writes);
    void rangeSearchP(long pos_node, int start, int end, vector<T>& results, fstream& file, int& reads, int& writes);
    long deleteNode(long pos_node, Key key, fstream& file, int& reads, int& writes);
};

template<typename Key, typename T>
AVLFile<Key, T>::AVLFile(string filename) : filename(filename) {
    fstream file;
    file.open(this->filename, ios::in | ios::binary);
    if (!file.is_open()) {
        this->pos_root = -1;
    } else {
        this->pos_root = 0; // Si el archivo existe, inicializamos la raíz
    }
    file.close();
}

template<typename Key, typename T>
void AVLFile<Key, T>::insert(T record, int& reads, int& writes) {
    fstream file;
    file.open(this->filename, ios::in | ios::out | ios::binary | ios::ate);

    if (!file.is_open()) {
        ofstream createFile(filename, ios::out | ios::binary);
        createFile.close();
        file.open(this->filename, ios::in | ios::out | ios::binary | ios::ate);
    }

    if (this->pos_root == -1) {
        file.seekp(0, ios::beg);
        file.write((char*)&record, sizeof(T));
        writes++;
        this->pos_root = 0;
    } else {
        insert_and_balance(this->pos_root, record, file, reads, writes);
    }

    file.close();
}

template<typename Key, typename T>
void AVLFile<Key, T>::delete_by_key(Key key, int& reads, int& writes) {
    fstream file(this->filename, ios::in | ios::out | ios::binary);
    if (this->pos_root != -1) {
        this->pos_root = deleteNode(this->pos_root, key, file, reads, writes);
    } else {
        cout << "No hay elementos registrados" << endl;
    }
    file.close();
}

template<typename Key, typename T>
vector<T> AVLFile<Key, T>::inorder(int& reads, int& writes) {
    fstream file(this->filename, ios::in | ios::binary);
    vector<T> result;
    if (this->pos_root != -1) {
        inorderP(this->pos_root, result, file, reads, writes);
    } else {
        cout << "No hay elementos registrados" << endl;
    }
    file.close();
    return result;
}

template<typename Key, typename T>
vector<T> AVLFile<Key, T>::rangeSearch(Key key_start, Key key_end, int& reads, int& writes) {
    fstream file(this->filename, ios::in | ios::binary);
    vector<T> results;
    if (this->pos_root != -1) {
        rangeSearchP(this->pos_root, key_start, key_end, results, file, reads, writes);
    } else {
        cout << "No hay elementos registrados" << endl;
    }
    file.close();
    return results;
}

template<typename Key, typename T>
void AVLFile<Key, T>::insert_and_balance(long& pos_node, T record, fstream& file, int& reads, int& writes) {
    if (pos_node == -1) {
        file.seekp(0, ios::end);
        pos_node = file.tellp() / sizeof(T);
        file.write((char*)&record, sizeof(T));
        writes++;
        return;
    }

    T node;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&node, sizeof(T));
    reads++;

    if (record.key < node.key) {
        insert_and_balance(node.left, record, file, reads, writes);
    } else if (record.key > node.key) {
        insert_and_balance(node.right, record, file, reads, writes);
    } else {
        cout << "El nodo con código " << record.key << " ya existe." << endl;
        return;
    }

    file.seekp(pos_node * sizeof(T));
    file.write((char*)&node, sizeof(T));
    writes++;
    balance(file, pos_node, reads, writes);
}

template<typename Key, typename T>
void AVLFile<Key, T>::balance(fstream& file, long pos_node, int& reads, int& writes) {
    if (pos_node == -1) return;

    T node;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&node, sizeof(T));
    reads++;

    updateHeight(pos_node, file, reads, writes);
    int balanceFactor = BalancingFactor(pos_node, file, reads, writes);

    if (balanceFactor > 1) {
        if (BalancingFactor(node.left, file, reads, writes) < 0) {
            node.left = leftRotate(file, node.left, reads, writes);
        }
        pos_node = rightRotate(file, pos_node, reads, writes);
    } else if (balanceFactor < -1) {
        if (BalancingFactor(node.right, file, reads, writes) > 0) {
            node.right = rightRotate(file, node.right, reads, writes);
        }
        pos_node = leftRotate(file, pos_node, reads, writes);
    }

    file.seekp(pos_node * sizeof(T));
    file.write((char*)&node, sizeof(T));
    writes++;
}

template<typename Key, typename T>
int AVLFile<Key, T>::BalancingFactor(long pos_node, fstream& file, int& reads, int& writes) {
    if (pos_node == -1) return 0;

    T record;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&record, sizeof(T));
    reads++;

    return height(record.left, file, reads, writes) - height(record.right, file, reads, writes);
}

template<typename Key, typename T>
long AVLFile<Key, T>::rightRotate(fstream& file, long y_pos, int& reads, int& writes) {
    T y, x;
    file.seekg(y_pos * sizeof(T));
    file.read((char*)&y, sizeof(T));
    reads++;
    file.seekg(y.left * sizeof(T));
    file.read((char*)&x, sizeof(T));
    reads++;

    long T2 = x.right; 

    x.right = y_pos; 
    y.left = T2; 

    file.seekp(y_pos * sizeof(T));
    file.write((char*)&y, sizeof(T));
    writes++;
    file.seekp(x.left * sizeof(T)); 
    file.write((char*)&x, sizeof(T));
    writes++;

    return x.left; 
}

template<typename Key, typename T>
long AVLFile<Key, T>::leftRotate(fstream& file, long x_pos, int& reads, int& writes) {
    T x, y;
    file.seekg(x_pos * sizeof(T));
    file.read((char*)&x, sizeof(T));
    reads++;
    file.seekg(x.right * sizeof(T));
    file.read((char*)&y, sizeof(T));
    reads++;

    long T2 = y.left; 

    y.left = x_pos;
    x.right = T2; 

    file.seekp(x_pos * sizeof(T));
    file.write((char*)&x, sizeof(T));
    writes++;
    file.seekp(y.left * sizeof(T)); 
    file.write((char*)&y, sizeof(T));
    writes++;

    return y.left; 
}

template<typename Key, typename T>
int AVLFile<Key, T>::height(long pos_node, fstream& file, int& reads, int& writes) {
    if (pos_node == -1) return 0;

    T record;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&record, sizeof(T));
    reads++;
    return record.height;
}

template<typename Key, typename T>
void AVLFile<Key, T>::updateHeight(long pos_node, fstream& file, int& reads, int& writes) {
    if (pos_node == -1) return;

    T record;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&record, sizeof(T));
    reads++;
    record.height = 1 + max(height(record.left, file, reads, writes), height(record.right, file, reads, writes));

    file.seekp(pos_node * sizeof(T));
    file.write((char*)&record, sizeof(T));
    writes++;
}

template<typename Key, typename T>
void AVLFile<Key, T>::inorderP(long pos_node, vector<T>& result, fstream& file, int& reads, int& writes) {
    if (pos_node == -1) return;

    T record;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&record, sizeof(T));
    reads++;
    inorderP(record.left, result, file, reads, writes);
    result.push_back(record);
    inorderP(record.right, result, file, reads, writes);
}

template<typename Key, typename T>
void AVLFile<Key, T>::rangeSearchP(long pos_node, int start, int end, vector<T>& results, fstream& file, int& reads, int& writes) {
    if (pos_node == -1) return;

    T record;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&record, sizeof(T));
    reads++;

    if (record.key > start) {
        rangeSearchP(record.left, start, end, results, file, reads, writes);
    }

    if (record.key >= start && record.key <= end) {
        results.push_back(record);
    }

    if (record.key < end) {
        rangeSearchP(record.right, start, end, results, file, reads, writes);
    }
}

template<typename Key, typename T>
long AVLFile<Key, T>::deleteNode(long pos_node, Key key, fstream& file, int& reads, int& writes) {
    if (pos_node == -1) {
        return pos_node; // Nodo no encontrado
    }

    T node;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&node, sizeof(T));
    reads++;

    if (key < node.key) {
        node.left = deleteNode(node.left, key, file, reads, writes);
    } else if (key > node.key) {
        node.right = deleteNode(node.right, key, file, reads, writes);
    } else {
        // Encontramos el nodo a eliminar
        if (node.left == -1) {
            return node.right; // Reemplazar con hijo derecho
        } else if (node.right == -1) {
            return node.left; // Reemplazar con hijo izquierdo
        }

        // Encontrar el sucesor 
        long successor_pos = node.right;
        while (true) {
            file.seekg(successor_pos * sizeof(T));
            file.read((char*)&node, sizeof(T));
            reads++;
            if (node.left != -1) {
                successor_pos = node.left; 
            } else {
                break; 
            }
        }

        T successor;
        file.seekg(successor_pos * sizeof(T));
        file.read((char*)&successor, sizeof(T));
        reads++;
        long tmp_right = node.right;
        std::memcpy(&node, &successor, sizeof(T));
        node.right = tmp_right;
        node.right = deleteNode(node.right, successor.key, file, reads, writes);
    }

    // Actualizar y equilibrar
    file.seekp(pos_node * sizeof(T));
    file.write((char*)&node, sizeof(T));
    reads++;
    balance(file, pos_node, reads, writes);

    return pos_node;
}

template<typename Key, typename T>
T AVLFile<Key, T>::search_by_key(Key key, int& reads, int& writes) {
    fstream file(this->filename, ios::in | ios::binary);
    if (this->pos_root == -1) {
        cout << "No hay elementos registrados" << endl;
        return T(); 
    }

    long result_pos = this->pos_root;
    while (result_pos != -1) {
        T node;
        file.seekg(result_pos * sizeof(T));
        file.read((char*)&node, sizeof(T));
        reads++;

        if (key < node.key) {
            result_pos = node.left; 
        } else if (key > node.key) {
            result_pos = node.right; 
        } else {
            file.close();
            return node; 
        }
    }

    cout << "Registro con código " << key << " no encontrado." << endl;
    file.close();
    return T(); 
}


#endif // AVL_H

