#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <cstring>
#include <fstream>
#include "Vector.h"

// ����������ڵ�
struct BinTreeNode {
    char data;
    int frequency;
    BinTreeNode* left;
    BinTreeNode* right;

    // ���캯��
    BinTreeNode(char d, int f) : data(d), frequency(f), left(nullptr), right(nullptr) {}

    // �������ȶ��бȽϵ����������
    bool operator>(const BinTreeNode& other) const {
        return frequency > other.frequency;
    }
};

// �������ṹ
class BinTree {
public:
    BinTreeNode* root;

    BinTree(BinTreeNode* r) : root(r) {}
    ~BinTree() { destroy(root); }

private:
    void destroy(BinTreeNode* node) {
        if (node) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }
};

// Bitmap �ඨ��
class Bitmap {
private:
    unsigned char* M;
    size_t N, _sz;

    void init(size_t n) {
        M = new unsigned char[N = (n + 7) / 8];
        std::memset(M, 0, N);
        _sz = 0;
    }

public:
    Bitmap(size_t n = 8) { init(n); }
    ~Bitmap() { delete[] M; }

    void set(size_t k) {
        expand(k);
        _sz++;
        M[k >> 3] |= (0x80 >> (k & 0x07));
    }

    bool test(size_t k) {
        expand(k);
        return M[k >> 3] & (0x80 >> (k & 0x07));
    }

    void expand(size_t k) {
        if (k < 8 * N) return;
        size_t oldN = N;
        unsigned char* oldM = M;
        init(2 * k);
        std::memcpy(M, oldM, oldN);
        delete[] oldM;
    }
};

// Huffman ����ʵ��
class HuffTree {
public:
    BinTreeNode* root;
    std::map<char, std::string> huffCodeMap;

    HuffTree(const std::map<char, int>& frequencies) {
        auto comp = [](BinTreeNode* a, BinTreeNode* b) { return *a > *b; };
        std::priority_queue<BinTreeNode*, std::vector<BinTreeNode*>, decltype(comp)> pq(comp);

        // ��ÿ���ַ���Ƶ�ʴ������ڵ㣬���������ȶ���
        for (const auto& pair : frequencies) {
            pq.push(new BinTreeNode(pair.first, pair.second));
        }

        // ���� Huffman ��
        while (pq.size() > 1) {
            BinTreeNode* left = pq.top(); pq.pop();
            BinTreeNode* right = pq.top(); pq.pop();
            BinTreeNode* parent = new BinTreeNode('\0', left->frequency + right->frequency);
            parent->left = left;
            parent->right = right;
            pq.push(parent);
        }
        root = pq.top();
        generateHuffCode(root, "");
    }

    ~HuffTree() { destroy(root); }

    std::string encode(const std::string& input) {
        std::string encoded;
        for (char ch : input) {
            encoded += huffCodeMap[ch];
        }
        return encoded;
    }

private:
    void generateHuffCode(BinTreeNode* node, const std::string& code) {
        if (!node) return;
        if (node->data != '\0') {
            huffCodeMap[node->data] = code;
        }
        generateHuffCode(node->left, code + "0");
        generateHuffCode(node->right, code + "1");
    }

    void destroy(BinTreeNode* node) {
        if (node) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }
};

// ������������ Huffman ���������Ե��ʽ��б���
int main() {
    std::map<char, int> frequencies = {
        {'a', 5}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 7},
        {'f', 2}, {'g', 1}, {'h', 4}, {'i', 2}, {'j', 1},
        {'k', 1}, {'l', 2}, {'m', 2}, {'n', 3}, {'o', 2},
        {'p', 1}, {'q', 1}, {'r', 4}, {'s', 2}, {'t', 5},
        {'u', 1}, {'v', 1}, {'w', 1}, {'x', 1}, {'y', 1}, {'z', 1}
    };

    HuffTree huffTree(frequencies);
    std::string word = "dream";
    std::string encodedWord = huffTree.encode(word);

    std::cout << "Encoded word \"" << word << "\": " << encodedWord << std::endl;

    // ������������
    std::string anotherWord = "huffman";
    std::string encodedAnother = huffTree.encode(anotherWord);
    std::cout << "Encoded word \"" << anotherWord << "\": " << encodedAnother << std::endl;

    return 0;
}
