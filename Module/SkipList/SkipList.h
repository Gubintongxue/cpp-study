
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : SkipList.h
*   Last Modified : 2019-10-12 15:51
*   Describe      :
*
*******************************************************/

#ifndef  _SKIPLIST_H
#define  _SKIPLIST_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>

#define MAX_LEVEL 16   // 根据数据量设置

class SkipList {
private:
    struct Node {
        int key;
        int value;
        Node* next[1];
    };

public:
    SkipList() :
        level(0), 
        count(0) {
        srand(time(nullptr));
        nul = newNodeWithLevel(0);
        nul->key = INT_MAX;     // 有序

        header = newNodeWithLevel(MAX_LEVEL);
        for(int i = 0; i< MAX_LEVEL; ++i) {
            header->next[i] = nul;
        }
    }

    ~SkipList() {
        Node* p = header;
        Node* t;
        while(p != nul) {
            t = p->next[0];
            free(p);
            p = t;
        }
        free(p);
    }

    size_t size() { return count; }
    int getLevel() { return level; }

    bool search(const int& key, int& value) {
        Node* p = header;
        for(int i = level; i >= 0; --i) {
            while(p->next[i]->key < key)
                p = p->next[i];
        }   // 遍历到level = 0
        p = p->next[0]; // 下一个

        if(p->key == key) {
            value = p->value;
            return true;
        }
        else {
            return false;
        }
    }

    void insert(const int& key, const int& value) {
        Node* update[MAX_LEVEL]; // 更新位置
        Node* p = header;

        for(int i = level; i >= 0; --i) {
            while(p->next[i]->key < key)
                p = p->next[i];
            update[i] = p;
        }
        p = p->next[0];

        if(p->key == key) {
            p->value = value;  // 直接替换
        }
        else {
            int level = RandomLevel();
            if(level > this->level) {
                level = ++this->level;
                update[level] = header;
            }

            Node* newNode = newNodeWithLevel(level);
            newNode->key = key;
            newNode->value = value;

            for(int i = level; i >= 0; --i) {
                p = update[i];
                newNode->next[i] = p->next[i];
                p->next[i] = newNode;
            }
            count++;
        }
    }

    void remove(const int& key) {
        Node* update[MAX_LEVEL];
        Node* p = header;

        for(int i = level; i >= 0; --i) {
            while(p->next[i]->key < key)
                p = p->next[i];
            update[i] = p;
        }
        p = p->next[0];

        if(p->key == key) {
            for(int i = 0; i <= level; ++i) {
                if(update[i]->next[i] != p)
                    break;  // 跳过
                update[i]->next[i] = p->next[i];
            }
            free(p);

            while(level > 0 && header->next[level] == nul)
                --level;

            count--;
        }
    }

private:
    Node* newNodeWithLevel(int level) {
        int size = sizeof(Node) + level * sizeof(Node*);
        printf("%d\n", size);
        Node* node = (Node*)malloc(size);
        assert(node != nullptr);
        return node;
    }

    // 产生1到MAX_LEVEL-1的随机数
    int RandomLevel() {
        return rand() % (MAX_LEVEL-2) + 1;
    }

private:
    Node* header;        // 跳表第一个节点
    Node* nul;          // 跳表最后一个节点
    int level;
    size_t count;
};

#endif // _SKIPLIST_H

