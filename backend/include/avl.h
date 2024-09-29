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
    void insert(T record);
    void delete_by_key(Key key);
    T search_by_key(Key key);
    vector<T> inorder();
    vector<T> rangeSearch(Key key_begin, Key key_end);

private:
    void insert_and_balance(long& pos_node, T record, fstream& file);
    void balance(fstream& file, long pos_node);
    int BalancingFactor(long pos_node, fstream& file);
    long rightRotate(fstream& file, long y_pos);
    long leftRotate(fstream& file, long x_pos);
    int height(long pos_node, fstream& file);
    void updateHeight(long pos_node, fstream& file);
    void inorder(long pos_node, vector<T>& result, fstream& file);
    void rangeSearch(long pos_node, int start, int end, vector<T>& results, fstream& file);
    long deleteNode(long pos_node, Key key, fstream& file);
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
void AVLFile<Key, T>::insert(T record) {
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
        this->pos_root = 0;
    } else {
        insert_and_balance(this->pos_root, record, file);
    }

    file.close();
}

template<typename Key, typename T>
void AVLFile<Key, T>::delete_by_key(Key key) {
    fstream file(this->filename, ios::in | ios::out | ios::binary);
    if (this->pos_root != -1) {
        this->pos_root = deleteNode(this->pos_root, key, file);
    } else {
        cout << "No hay elementos registrados" << endl;
    }
    file.close();
}

template<typename Key, typename T>
vector<T> AVLFile<Key, T>::inorder() {
    fstream file(this->filename, ios::in | ios::binary);
    vector<T> result;
    if (this->pos_root != -1) {
        inorder(this->pos_root, result, file);
    } else {
        cout << "No hay elementos registrados" << endl;
    }
    file.close();
    return result;
}

template<typename Key, typename T>
vector<T> AVLFile<Key, T>::rangeSearch(Key key_start, Key key_end) {
    fstream file(this->filename, ios::in | ios::binary);
    vector<T> results;
    if (this->pos_root != -1) {
        rangeSearch(this->pos_root, key_start, key_end, results, file);
    } else {
        cout << "No hay elementos registrados" << endl;
    }
    file.close();
    return results;
}

template<typename Key, typename T>
void AVLFile<Key, T>::insert_and_balance(long& pos_node, T record, fstream& file) {
    if (pos_node == -1) {
        file.seekp(0, ios::end);
        pos_node = file.tellp() / sizeof(T);
        file.write((char*)&record, sizeof(T));
        return;
    }

    T node;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&node, sizeof(T));

    if (record.key < node.key) {
        insert_and_balance(node.left, record, file);
    } else if (record.key > node.key) {
        insert_and_balance(node.right, record, file);
    } else {
        cout << "El nodo con código " << record.key << " ya existe." << endl;
        return;
    }

    file.seekp(pos_node * sizeof(T));
    file.write((char*)&node, sizeof(T));
    balance(file, pos_node);
}

template<typename Key, typename T>
void AVLFile<Key, T>::balance(fstream& file, long pos_node) {
    if (pos_node == -1) return;

    T node;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&node, sizeof(T));

    updateHeight(pos_node, file);
    int balanceFactor = BalancingFactor(pos_node, file);

    if (balanceFactor > 1) {
        if (BalancingFactor(node.left, file) < 0) {
            node.left = leftRotate(file, node.left);
        }
        pos_node = rightRotate(file, pos_node);
    } else if (balanceFactor < -1) {
        if (BalancingFactor(node.right, file) > 0) {
            node.right = rightRotate(file, node.right);
        }
        pos_node = leftRotate(file, pos_node);
    }

    file.seekp(pos_node * sizeof(T));
    file.write((char*)&node, sizeof(T));
}

template<typename Key, typename T>
int AVLFile<Key, T>::BalancingFactor(long pos_node, fstream& file) {
    if (pos_node == -1) return 0;

    T record;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&record, sizeof(T));

    return height(record.left, file) - height(record.right, file);
}

template<typename Key, typename T>
long AVLFile<Key, T>::rightRotate(fstream& file, long y_pos) {
    T y, x;
    file.seekg(y_pos * sizeof(T));
    file.read((char*)&y, sizeof(T));
    file.seekg(y.left * sizeof(T));
    file.read((char*)&x, sizeof(T));

    long T2 = x.right; 

    x.right = y_pos; 
    y.left = T2; 

    file.seekp(y_pos * sizeof(T));
    file.write((char*)&y, sizeof(T));
    file.seekp(x.left * sizeof(T)); 
    file.write((char*)&x, sizeof(T));

    return x.left; 
}

template<typename Key, typename T>
long AVLFile<Key, T>::leftRotate(fstream& file, long x_pos) {
    T x, y;
    file.seekg(x_pos * sizeof(T));
    file.read((char*)&x, sizeof(T));
    file.seekg(x.right * sizeof(T));
    file.read((char*)&y, sizeof(T));

    long T2 = y.left; 

    y.left = x_pos;
    x.right = T2; 

    file.seekp(x_pos * sizeof(T));
    file.write((char*)&x, sizeof(T));
    file.seekp(y.left * sizeof(T)); 
    file.write((char*)&y, sizeof(T));

    return y.left; 
}

template<typename Key, typename T>
int AVLFile<Key, T>::height(long pos_node, fstream& file) {
    if (pos_node == -1) return 0;

    T record;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&record, sizeof(T));
    return record.height;
}

template<typename Key, typename T>
void AVLFile<Key, T>::updateHeight(long pos_node, fstream& file) {
    if (pos_node == -1) return;

    T record;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&record, sizeof(T));
    record.height = 1 + max(height(record.left, file), height(record.right, file));

    file.seekp(pos_node * sizeof(T));
    file.write((char*)&record, sizeof(T));
}

template<typename Key, typename T>
void AVLFile<Key, T>::inorder(long pos_node, vector<T>& result, fstream& file) {
    if (pos_node == -1) return;

    T record;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&record, sizeof(T));
    inorder(record.left, result, file);
    result.push_back(record);
    inorder(record.right, result, file);
}

template<typename Key, typename T>
void AVLFile<Key, T>::rangeSearch(long pos_node, int start, int end, vector<T>& results, fstream& file) {
    if (pos_node == -1) return;

    T record;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&record, sizeof(T));

    if (record.key > start) {
        rangeSearch(record.left, start, end, results, file);
    }

    if (record.key >= start && record.key <= end) {
        results.push_back(record);
    }

    if (record.key < end) {
        rangeSearch(record.right, start, end, results, file);
    }
}

template<typename Key, typename T>
long AVLFile<Key, T>::deleteNode(long pos_node, Key key, fstream& file) {
    if (pos_node == -1) {
        return pos_node; // Nodo no encontrado
    }

    T node;
    file.seekg(pos_node * sizeof(T));
    file.read((char*)&node, sizeof(T));

    if (key < node.key) {
        node.left = deleteNode(node.left, key, file);
    } else if (key > node.key) {
        node.right = deleteNode(node.right, key, file);
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
            if (node.left != -1) {
                successor_pos = node.left; 
            } else {
                break; 
            }
        }

        T successor;
        file.seekg(successor_pos * sizeof(T));
        file.read((char*)&successor, sizeof(T));
        long tmp_right = node.right;
        std::memcpy(&node, &successor, sizeof(T));
        node.right = tmp_right;
        node.right = deleteNode(node.right, successor.key, file);
    }

    // Actualizar y equilibrar
    file.seekp(pos_node * sizeof(T));
    file.write((char*)&node, sizeof(T));
    balance(file, pos_node);

    return pos_node;
}

template<typename Key, typename T>
T AVLFile<Key, T>::search_by_key(Key key) {
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

