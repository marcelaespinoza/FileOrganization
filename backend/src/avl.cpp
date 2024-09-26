#include "avl.h"

AVLFile::AVLFile(string filename) : filename(filename) {
    fstream file;
    file.open(this->filename, ios::in | ios::binary);
    if (!file.is_open()) {
        this->pos_root = -1;
    } else {
        this->pos_root = 0; // Si el archivo existe, inicializamos la raíz
    }
    file.close();
}

void AVLFile::insert(Record record) {
    fstream file;
    file.open(this->filename, ios::in | ios::out | ios::binary | ios::ate);

    if (!file.is_open()) {
        ofstream createFile(filename, ios::out | ios::binary);
        createFile.close();
        file.open(this->filename, ios::in | ios::out | ios::binary | ios::ate);
    }

    if (this->pos_root == -1) {
        file.seekp(0, ios::beg);
        file.write((char*)&record, sizeof(Record));
        this->pos_root = 0;
    } else {
        insert_and_balance(this->pos_root, record, file);
    }

    file.close();
}

void AVLFile::delete_by_key(int cod) {
    fstream file(this->filename, ios::in | ios::out | ios::binary);
    if (this->pos_root != -1) {
        this->pos_root = deleteNode(this->pos_root, cod, file);
    } else {
        cout << "No hay elementos registrados" << endl;
    }
    file.close();
}

vector<Record> AVLFile::inorder() {
    fstream file(this->filename, ios::in | ios::binary);
    vector<Record> result;
    if (this->pos_root != -1) {
        inorder(this->pos_root, result, file);
    } else {
        cout << "No hay elementos registrados" << endl;
    }
    file.close();
    return result;
}

vector<Record> AVLFile::rangeSearch(int start, int end) {
    fstream file(this->filename, ios::in | ios::binary);
    vector<Record> results;
    if (this->pos_root != -1) {
        rangeSearch(this->pos_root, start, end, results, file);
    } else {
        cout << "No hay elementos registrados" << endl;
    }
    file.close();
    return results;
}

void AVLFile::insert_and_balance(long& pos_node, Record record, fstream& file) {
    if (pos_node == -1) {
        file.seekp(0, ios::end);
        pos_node = file.tellp() / sizeof(Record);
        file.write((char*)&record, sizeof(Record));
        return;
    }

    Record node;
    file.seekg(pos_node * sizeof(Record));
    file.read((char*)&node, sizeof(Record));

    if (record.cod < node.cod) {
        insert_and_balance(node.left, record, file);
    } else if (record.cod > node.cod) {
        insert_and_balance(node.right, record, file);
    } else {
        cout << "El nodo con código " << record.cod << " ya existe." << endl;
        return;
    }

    file.seekp(pos_node * sizeof(Record));
    file.write((char*)&node, sizeof(Record));
    balance(file, pos_node);
}

void AVLFile::balance(fstream& file, long pos_node) {
    if (pos_node == -1) return;

    Record node;
    file.seekg(pos_node * sizeof(Record));
    file.read((char*)&node, sizeof(Record));

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

    file.seekp(pos_node * sizeof(Record));
    file.write((char*)&node, sizeof(Record));
}

int AVLFile::BalancingFactor(long pos_node, fstream& file) {
    if (pos_node == -1) return 0;

    Record record;
    file.seekg(pos_node * sizeof(Record));
    file.read((char*)&record, sizeof(Record));

    return height(record.left, file) - height(record.right, file);
}

long AVLFile::rightRotate(fstream& file, long y_pos) {
    Record y, x;
    file.seekg(y_pos * sizeof(Record));
    file.read((char*)&y, sizeof(Record));
    file.seekg(y.left * sizeof(Record));
    file.read((char*)&x, sizeof(Record));

    long T2 = x.right; 

    x.right = y_pos; 
    y.left = T2; 

    file.seekp(y_pos * sizeof(Record));
    file.write((char*)&y, sizeof(Record));
    file.seekp(x.left * sizeof(Record)); 
    file.write((char*)&x, sizeof(Record));

    return x.left; 
}

long AVLFile::leftRotate(fstream& file, long x_pos) {
    Record x, y;
    file.seekg(x_pos * sizeof(Record));
    file.read((char*)&x, sizeof(Record));
    file.seekg(x.right * sizeof(Record));
    file.read((char*)&y, sizeof(Record));

    long T2 = y.left; 

    y.left = x_pos;
    x.right = T2; 

    file.seekp(x_pos * sizeof(Record));
    file.write((char*)&x, sizeof(Record));
    file.seekp(y.left * sizeof(Record)); 
    file.write((char*)&y, sizeof(Record));

    return y.left; 
}

int AVLFile::height(long pos_node, fstream& file) {
    if (pos_node == -1) return 0;

    Record record;
    file.seekg(pos_node * sizeof(Record));
    file.read((char*)&record, sizeof(Record));
    return record.height;
}

void AVLFile::updateHeight(long pos_node, fstream& file) {
    if (pos_node == -1) return;

    Record record;
    file.seekg(pos_node * sizeof(Record));
    file.read((char*)&record, sizeof(Record));
    record.height = 1 + max(height(record.left, file), height(record.right, file));

    file.seekp(pos_node * sizeof(Record));
    file.write((char*)&record, sizeof(Record));
}

void AVLFile::inorder(long pos_node, vector<Record>& result, fstream& file) {
    if (pos_node == -1) return;

    Record record;
    file.seekg(pos_node * sizeof(Record));
    file.read((char*)&record, sizeof(Record));
    inorder(record.left, result, file);
    result.push_back(record);
    inorder(record.right, result, file);
}

void AVLFile::rangeSearch(long pos_node, int start, int end, vector<Record>& results, fstream& file) {
    if (pos_node == -1) return;

    Record record;
    file.seekg(pos_node * sizeof(Record));
    file.read((char*)&record, sizeof(Record));

    if (record.cod > start) {
        rangeSearch(record.left, start, end, results, file);
    }

    if (record.cod >= start && record.cod <= end) {
        results.push_back(record);
    }

    if (record.cod < end) {
        rangeSearch(record.right, start, end, results, file);
    }
}

long AVLFile::deleteNode(long pos_node, int cod, fstream& file) {
    if (pos_node == -1) {
        return pos_node; // Nodo no encontrado
    }

    Record node;
    file.seekg(pos_node * sizeof(Record));
    file.read((char*)&node, sizeof(Record));

    if (cod < node.cod) {
        node.left = deleteNode(node.left, cod, file);
    } else if (cod > node.cod) {
        node.right = deleteNode(node.right, cod, file);
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
            file.seekg(successor_pos * sizeof(Record));
            file.read((char*)&node, sizeof(Record));
            if (node.left != -1) {
                successor_pos = node.left; 
            } else {
                break; 
            }
        }

        Record successor;
        file.seekg(successor_pos * sizeof(Record));
        file.read((char*)&successor, sizeof(Record));

        node.cod = successor.cod;
        strncpy(node.nombre, successor.nombre, sizeof(node.nombre) - 1);
        node.nombre[sizeof(node.nombre) - 1] = '\0';
        node.ciclo = successor.ciclo;

        node.right = deleteNode(node.right, successor.cod, file);
    }

    // Actualizar y equilibrar
    file.seekp(pos_node * sizeof(Record));
    file.write((char*)&node, sizeof(Record));
    balance(file, pos_node);

    return pos_node;
}

Record AVLFile::search_by_key(int cod) {
    fstream file(this->filename, ios::in | ios::binary);
    if (this->pos_root == -1) {
        cout << "No hay elementos registrados" << endl;
        return Record(); 
    }

    long result_pos = this->pos_root;
    while (result_pos != -1) {
        Record node;
        file.seekg(result_pos * sizeof(Record));
        file.read((char*)&node, sizeof(Record));

        if (cod < node.cod) {
            result_pos = node.left; 
        } else if (cod > node.cod) {
            result_pos = node.right; 
        } else {
            file.close();
            return node; 
        }
    }

    cout << "Registro con código " << cod << " no encontrado." << endl;
    file.close();
    return Record(-1, "bad", -1); 
}


