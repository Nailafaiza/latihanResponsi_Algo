#include <iostream>
#include <string>
using namespace std;

// Struktur data Pelaku
struct Pelaku {
    string nama;
    string posisi;
    double nominal;
    Pelaku* left;
    Pelaku* right;

    Pelaku(string n, string p, double d) {
        nama = n;
        posisi = p;
        nominal = d;
        left = right = nullptr;
    }
};

// Struktur Aksi untuk undo
struct Aksi {
    string jenis; // "tambah" atau "hapus"
    Pelaku data;
};

// Stack Node
struct StackNode {
    Aksi data;
    StackNode* next;
};

// Stack buatan
struct Stack {
    StackNode* top;

    Stack() : top(nullptr) {}

    bool isEmpty() {
        return top == nullptr;
    }

    void push(Aksi aksi) {
        StackNode* newNode = new StackNode{ aksi, top };
        top = newNode;
    }

    Aksi pop() {
        if (isEmpty()) {
            cout << "Stack kosong.\n";
            return { "", {"", "", 0} };
        }
        StackNode* temp = top;
        Aksi result = temp->data;
        top = top->next;
        delete temp;
        return result;
    }
};

// Linked list node
struct NodeList {
    Pelaku* data;
    NodeList* next;
    NodeList(Pelaku* d) : data(d), next(nullptr) {}
};

// Queue Node
struct QueueNode {
    Pelaku* data;
    QueueNode* next;
};

// Queue buatan
struct Queue {
    QueueNode* front;
    QueueNode* rear;

    Queue() : front(nullptr), rear(nullptr) {}

    bool isEmpty() {
        return front == nullptr;
    }

    void enqueue(Pelaku* d) {
        QueueNode* newNode = new QueueNode{ d, nullptr };
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    Pelaku* dequeue() {
        if (isEmpty()) return nullptr;
        QueueNode* temp = front;
        Pelaku* result = temp->data;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;
        return result;
    }
};

// Fungsi tambah ke BST
Pelaku* tambahPelaku(Pelaku* root, Pelaku* node) {
    if (root == nullptr)
        return node;
    if (node->nama < root->nama)
        root->left = tambahPelaku(root->left, node);
    else
        root->right = tambahPelaku(root->right, node);
    return root;
}

// Inorder traversal (A-Z)
void inorderTampil(Pelaku* root) {
    if (root != nullptr) {
        inorderTampil(root->left);
        cout << "Nama: " << root->nama << ", Posisi: " << root->posisi << ", Nominal: " << root->nominal << endl;
        inorderTampil(root->right);
    }
}

// Level order traversal (pakai queue)
void levelOrderTampil(Pelaku* root) {
    if (!root) return;
    Queue q;
    q.enqueue(root);

    cout << "\nLevel Order Traversal:\n";
    while (!q.isEmpty()) {
        Pelaku* current = q.dequeue();
        cout << "Nama: " << current->nama << ", Posisi: " << current->posisi << ", Nominal: " << current->nominal << endl;

        if (current->left) q.enqueue(current->left);
        if (current->right) q.enqueue(current->right);
    }
}

// Tambah ke linked list
void tambahLinkedList(NodeList*& head, Pelaku* p) {
    NodeList* baru = new NodeList(p);
    if (head == nullptr) {
        head = baru;
    } else {
        NodeList* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = baru;
    }
}

// Tampilkan linked list
void tampilLinkedList(NodeList* head) {
    cout << "\nData berdasarkan urutan penambahan:\n";
    NodeList* temp = head;
    while (temp != nullptr) {
        cout << "Nama: " << temp->data->nama << ", Posisi: " << temp->data->posisi << ", Nominal: " << temp->data->nominal << endl;
        temp = temp->next;
    }
}

// Cari node terkecil di BST
Pelaku* cariMin(Pelaku* root) {
    while (root->left != nullptr)
        root = root->left;
    return root;
}

// Hapus data berdasarkan nama + simpan ke undoStack
Pelaku* hapusPelaku(Pelaku* root, string nama, Stack& undoStack) {
    if (root == nullptr) return nullptr;
    if (nama < root->nama)
        root->left = hapusPelaku(root->left, nama, undoStack);
    else if (nama > root->nama)
        root->right = hapusPelaku(root->right, nama, undoStack);
    else {
        Pelaku* backup = new Pelaku(root->nama, root->posisi, root->nominal);
        undoStack.push({ "hapus", *backup });

        if (root->left == nullptr && root->right == nullptr) {
            delete root;
            return nullptr;
        } else if (root->left == nullptr) {
            Pelaku* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == nullptr) {
            Pelaku* temp = root->left;
            delete root;
            return temp;
        } else {
            Pelaku* minRight = cariMin(root->right);
            root->nama = minRight->nama;
            root->posisi = minRight->posisi;
            root->nominal = minRight->nominal;
            root->right = hapusPelaku(root->right, minRight->nama, undoStack);
        }
    }
    return root;
}

// Undo aksi terakhir
Pelaku* undoTerakhir(Pelaku* root, Stack& undoStack) {
    if (undoStack.isEmpty()) {
        cout << "Tidak ada aksi yang bisa di-undo.\n";
        return root;
    }
    Aksi lastAction = undoStack.pop();

    if (lastAction.jenis == "tambah") {
        root = hapusPelaku(root, lastAction.data.nama, undoStack);
        cout << "Aksi tambah terakhir di-undo.\n";
    } else if (lastAction.jenis == "hapus") {
        root = tambahPelaku(root, new Pelaku(lastAction.data.nama, lastAction.data.posisi, lastAction.data.nominal));
        cout << "Aksi hapus terakhir di-undo.\n";
    }
    return root;
}

// MAIN PROGRAM
int main() {
    Pelaku* root = nullptr;
    NodeList* listHead = nullptr;
    Stack undoStack;
    int pilihan;

    do {
        cout << "\n========================================================\n";
        cout << "|                      MENU                            |\n";
        cout << "========================================================\n";
        cout << "| 1. Tambah data                                        |\n";
        cout << "| 2. Tampilkan data (Inorder Traversal)                 |\n";
        cout << "| 3. Hapus data                                         |\n";
        cout << "| 4. Undo                                               |\n";
        cout << "| 5. Tampilkan urutan penambahan (Linked List)          |\n";
        cout << "| 6. Tampilkan Level Order (Queue)                      |\n";
        cout << "| 0. Keluar                                             |\n";
        cout << "========================================================\n";
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
        case 0:
            cout << "Keluar...\n";
            break;
        case 1: {
            string nama, posisi;
            double nominal;
            cout << "Nama: ";
            getline(cin, nama);
            cout << "Posisi: ";
            getline(cin, posisi);
            cout << "Nominal: ";
            cin >> nominal;
            cin.ignore();

            Pelaku* baru = new Pelaku(nama, posisi, nominal);
            root = tambahPelaku(root, baru);
            tambahLinkedList(listHead, baru);
            undoStack.push({ "tambah", *baru });

            cout << "Data berhasil ditambahkan.\n";
            break;
        }
        case 2:
            cout << "\nDaftar Pelaku (Inorder A-Z):\n";
            inorderTampil(root);
            break;
        case 3: {
            string nama;
            cout << "Masukkan nama yang akan dihapus: ";
            getline(cin, nama);
            root = hapusPelaku(root, nama, undoStack);
            cout << "Data pelaku dihapus (jika ditemukan).\n";
            break;
        }
        case 4:
            root = undoTerakhir(root, undoStack);
            break;
        case 5:
            tampilLinkedList(listHead);
            break;
        case 6:
            levelOrderTampil(root);
            break;
        default:
            cout << "Pilihan tidak valid!\n";
            break;
        }
    } while (pilihan != 0);

    return 0;
}