#ifndef DEVICE_BST_H
#define DEVICE_BST_H

#include <iostream>
#include <string>
using namespace std;

// ============================================================================
// BINARY SEARCH TREE FOR DEVICE MANAGEMENT
// ============================================================================

struct BSTNode {
    string deviceId;
    string deviceName;
    string ipAddress;
    string department;
    bool isOnline;
    BSTNode* left;
    BSTNode* right;
    
    BSTNode(string id, string name, string ip, string dept) {
        deviceId = id;
        deviceName = name;
        ipAddress = ip;
        department = dept;
        isOnline = true;
        left = NULL;
        right = NULL;
    }
};

class DeviceBST {
private:
    BSTNode* root;
    
    BSTNode* insert(BSTNode* node, string id, string name, string ip, string dept) {
        if (node == NULL) {
            return new BSTNode(id, name, ip, dept);
        }
        
        if (id < node->deviceId) {
            node->left = insert(node->left, id, name, ip, dept);
        } else {
            node->right = insert(node->right, id, name, ip, dept);
        }
        return node;
    }
    
    BSTNode* search(BSTNode* node, string id) {
        if (node == NULL || node->deviceId == id) {
            return node;
        }
        
        if (id < node->deviceId) {
            return search(node->left, id);
        }
        return search(node->right, id);
    }
    
    void inorder(BSTNode* node) {
        if (node == NULL) return;
        inorder(node->left);
        cout << "  [" << node->deviceId << "] " << node->deviceName 
             << " | IP: " << node->ipAddress 
             << " | Dept: " << node->department
             << " | Status: " << (node->isOnline ? "ONLINE" : "OFFLINE") << "\n";
        inorder(node->right);
    }
    
public:
    DeviceBST() { root = NULL; }
    
    void addDevice(string id, string name, string ip, string dept) {
        root = insert(root, id, name, ip, dept);
    }
    
    BSTNode* findDevice(string id) {
        return search(root, id);
    }
    
    void displayAll() {
        if (root == NULL) {
            cout << "No devices in network.\n";
            return;
        }
        cout << "\n=== ALL NETWORK DEVICES ===\n";
        inorder(root);
    }
};

#endif // DEVICE_BST_H