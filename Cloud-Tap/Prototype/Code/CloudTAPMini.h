#ifndef CLOUDTAP_MINI_H
#define CLOUDTAP_MINI_H

#include <iostream>
#include <string>
#include <vector>
#include "DeviceBST.h"
#include "NetworkGraph.h"
#include "EventLogger.h"
#include "ConnectionStack.h"
#include "DHCPQueue.h"

using namespace std;

// ============================================================================
// MAIN CLOUD TAP MINI SYSTEM
// ============================================================================

class CloudTAPMini {
private:
    DeviceBST deviceTree;
    NetworkGraph topology;
    EventLogger logger;
    ConnectionStack connHistory;
    DHCPQueue dhcpPool;
    
public:
    CloudTAPMini() : dhcpPool("10.10.10", 20, 100) {
        initializeNetwork();
    }
    
    void initializeNetwork() {
        // Add devices to BST
        deviceTree.addDevice("CORE-R1", "Core Router", "192.168.1.1", "Core");
        deviceTree.addDevice("FW-1", "Firewall", "192.168.100.1", "Security");
        deviceTree.addDevice("MGMT-SW1", "Management Switch", "10.10.10.254", "Management");
        deviceTree.addDevice("MGMT-SRV1", "DHCP/Email Server", "10.10.10.10", "Management");
        deviceTree.addDevice("MGMT-PC1", "Manager PC", "10.10.10.1", "Management");
        deviceTree.addDevice("IT-SW1", "IT Switch", "10.10.20.254", "IT");
        deviceTree.addDevice("IT-PC1", "IT Admin PC", "10.10.20.1", "IT");
        deviceTree.addDevice("SALES-SW1", "Sales Switch", "10.10.30.254", "Sales");
        deviceTree.addDevice("SALES-PC1", "Sales PC", "10.10.30.1", "Sales");
        
        // Build network topology (Graph)
        topology.addEdge("CORE-R1", "FW-1", 1);
        topology.addEdge("CORE-R1", "MGMT-SW1", 1);
        topology.addEdge("CORE-R1", "IT-SW1", 2);
        topology.addEdge("CORE-R1", "SALES-SW1", 2);
        topology.addEdge("MGMT-SW1", "MGMT-SRV1", 1);
        topology.addEdge("MGMT-SW1", "MGMT-PC1", 1);
        topology.addEdge("IT-SW1", "IT-PC1", 1);
        topology.addEdge("SALES-SW1", "SALES-PC1", 1);
        
        // Log initial events
        logger.addLog(0, "System initialized successfully");
        logger.addLog(0, "9 devices added to network");
        logger.addLog(1, "Firewall rules applied");
    }
    
    void addNewDevice() {
        cout << "\n=== ADD NEW DEVICE ===\n";
        
        string deviceId, deviceName, ipAddress, department;
        
        cout << "Enter Device ID (e.g., PC-5): ";
        cin.ignore();
        getline(cin, deviceId);
        
        // Check if device already exists
        BSTNode* existing = deviceTree.findDevice(deviceId);
        if (existing) {
            cout << "\nError: Device ID already exists!\n";
            logger.addLog(1, "Failed to add device: " + deviceId + " (duplicate ID)");
            return;
        }
        
        cout << "Enter Device Name (e.g., Workstation): ";
        getline(cin, deviceName);
        
        cout << "Enter IP Address (e.g., 10.10.10.50 or leave empty for DHCP): ";
        getline(cin, ipAddress);
        
        // Auto-assign IP if empty
        if (ipAddress.empty()) {
            ipAddress = dhcpPool.assignIP();
            if (ipAddress.empty()) {
                cout << "\nError: DHCP pool exhausted!\n";
                logger.addLog(2, "Failed to add device: DHCP pool exhausted");
                return;
            }
            cout << "Auto-assigned IP: " << ipAddress << "\n";
        }
        
        cout << "Enter Department (e.g., IT, Sales, Management): ";
        getline(cin, department);
        
        // Add device to BST
        deviceTree.addDevice(deviceId, deviceName, ipAddress, department);
        
        cout << "\nDevice added successfully!\n";
        cout << "Would you like to add network connections? (y/n): ";
        char choice;
        cin >> choice;
        
        if (choice == 'y' || choice == 'Y') {
            addDeviceConnections(deviceId);
        }
        
        logger.addLog(0, "New device added: " + deviceId + " (" + deviceName + ")");
        
        cout << "\n=== DEVICE ADDED ===\n";
        cout << "ID: " << deviceId << "\n";
        cout << "Name: " << deviceName << "\n";
        cout << "IP: " << ipAddress << "\n";
        cout << "Department: " << department << "\n";
    }
    
    void addDeviceConnections(string deviceId) {
        cout << "\n=== ADD NETWORK CONNECTIONS ===\n";
        cout << "Enter number of connections to add: ";
        int numConnections;
        cin >> numConnections;
        
        cin.ignore();
        for (int i = 0; i < numConnections; i++) {
            cout << "\nConnection " << (i+1) << ":\n";
            cout << "Connect to Device ID: ";
            string targetDevice;
            getline(cin, targetDevice);
            
            // Check if target device exists
            BSTNode* target = deviceTree.findDevice(targetDevice);
            if (!target) {
                cout << "Warning: Device " << targetDevice << " not found. Skipping...\n";
                continue;
            }
            
            cout << "Enter latency in ms (default 1): ";
            string latencyStr;
            getline(cin, latencyStr);
            int latency = latencyStr.empty() ? 1 : atoi(latencyStr.c_str());
            
            topology.addEdge(deviceId, targetDevice, latency);
            cout << "Connection added: " << deviceId << " <-> " << targetDevice << " (" << latency << "ms)\n";
            
            logger.addLog(0, "Connection added: " + deviceId + " <-> " + targetDevice);
        }
    }
    
    void searchDevice() {
        cout << "\n=== DEVICE SEARCH ===\n";
        cout << "Enter Device ID: ";
        string id;
        cin >> id;
        
        BSTNode* device = deviceTree.findDevice(id);
        if (device) {
            cout << "\nDevice Found!\n";
            cout << "  ID: " << device->deviceId << "\n";
            cout << "  Name: " << device->deviceName << "\n";
            cout << "  IP: " << device->ipAddress << "\n";
            cout << "  Department: " << device->department << "\n";
            cout << "  Status: " << (device->isOnline ? "ONLINE" : "OFFLINE") << "\n";
            
            logger.addLog(0, "Device search: " + id);
        } else {
            cout << "\nDevice not found!\n";
            logger.addLog(1, "Device search failed: " + id);
        }
    }
    
    void traceroute() {
        cout << "\n=== TRACEROUTE ===\n";
        cout << "Source Device: ";
        string src;
        cin >> src;
        cout << "Destination Device: ";
        string dst;
        cin >> dst;
        
        vector<string> path = topology.bfsPath(src, dst);
        
        if (path.empty()) {
            cout << "\nNo route found!\n";
            logger.addLog(2, "Traceroute failed: " + src + " -> " + dst);
        } else {
            cout << "\nPath found (" << path.size() << " hops):\n";
            for (size_t i = 0; i < path.size(); i++) {
                cout << "  Hop " << (i+1) << ": " << path[i] << "\n";
            }
            
            string conn = src + " -> " + dst;
            connHistory.push(conn);
            logger.addLog(0, "Traceroute: " + conn);
        }
    }
    
    void calculateLatency() {
        cout << "\n=== NETWORK LATENCY CALCULATOR ===\n";
        cout << "Source Device: ";
        string src;
        cin >> src;
        cout << "Destination Device: ";
        string dst;
        cin >> dst;
        
        int latency = topology.dijkstra(src, dst);
        
        if (latency == -1) {
            cout << "\nNo route found!\n";
        } else {
            cout << "\nOptimal path latency: " << latency << " ms\n";
            logger.addLog(0, "Latency check: " + src + " -> " + dst);
        }
    }
    
    void networkExploration() {
        cout << "\n=== NETWORK EXPLORATION ===\n";
        cout << "Start from Device: ";
        string start;
        cin >> start;
        
        set<string> visited;
        cout << "\nReachable Devices: ";
        topology.dfs(start, visited);
        cout << "END\n";
        
        logger.addLog(0, "Network exploration from " + start);
    }
    
    void dhcpManager() {
        cout << "\n=== DHCP MANAGER ===\n";
        cout << "1. Assign IP\n";
        cout << "2. Release IP\n";
        cout << "3. Show Status\n";
        cout << "Choice: ";
        int choice;
        cin >> choice;
        
        if (choice == 1) {
            string ip = dhcpPool.assignIP();
            if (ip.empty()) {
                cout << "\nDHCP pool exhausted!\n";
                logger.addLog(2, "DHCP pool exhausted");
            } else {
                cout << "\nAssigned IP: " << ip << "\n";
                logger.addLog(0, "IP assigned: " + ip);
            }
        } else if (choice == 2) {
            cout << "Enter IP to release: ";
            string ip;
            cin >> ip;
            dhcpPool.releaseIP(ip);
            cout << "\nIP released: " << ip << "\n";
            logger.addLog(0, "IP released: " + ip);
        } else if (choice == 3) {
            dhcpPool.displayStatus();
        }
    }
    
    void showMenu() {
        while (true) {
            cout << "\n";
            cout << "============================================\n";
            cout << "   CLOUD TAP MINI - Network Simulator\n";
            cout << "   Tiered Access Platform v1.0\n";
            cout << "============================================\n";
            cout << "1. Show All Devices\n";
            cout << "2. Search Device\n";
            cout << "3. Add New Device\n";
            cout << "4. Network Connections\n";
            cout << "5. Traceroute\n";
            cout << "6. Calculate Latency\n";
            cout << "7. Network Exploration\n";
            cout << "8. DHCP Manager\n";
            cout << "9. Connection History\n";
            cout << "10. System Event Logs\n";
            cout << "0. Exit\n";
            cout << "============================================\n";
            cout << "Choice: ";
            
            int choice;
            cin >> choice;
            
            switch(choice) {
                case 0:
                    cout << "\nShutting down Cloud TAP...\n";
                    logger.addLog(0, "System shutdown");
                    return;
                case 1:
                    deviceTree.displayAll();
                    break;
                case 2:
                    searchDevice();
                    break;
                case 3:
                    addNewDevice();
                    break;
                case 4:
                    topology.displayConnections();
                    break;
                case 5:
                    traceroute();
                    break;
                case 6:
                    calculateLatency();
                    break;
                case 7:
                    networkExploration();
                    break;
                case 8:
                    dhcpManager();
                    break;
                case 9:
                    connHistory.display();
                    break;
                case 10:
                    logger.displayLogs(15);
                    break;
                default:
                    cout << "\nInvalid choice!\n";
                    logger.addLog(1, "Invalid menu choice");
            }
            
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
};

#endif // CLOUDTAP_MINI_H