#ifndef DHCP_QUEUE_H
#define DHCP_QUEUE_H

#include <iostream>
#include <string>
#include <queue>
#include <set>
#include <cstdio>
using namespace std;

// ============================================================================
// QUEUE FOR DHCP POOL MANAGEMENT
// ============================================================================

class DHCPQueue {
private:
    queue<string> availableIPs;
    set<string> usedIPs;
    string subnet;
    
    string toString(int num) {
        char buf[10];
        sprintf(buf, "%d", num);
        return string(buf);
    }
    
public:
    DHCPQueue(string sub, int start, int end) : subnet(sub) {
        for (int i = start; i <= end; i++) {
            availableIPs.push(subnet + "." + toString(i));
        }
    }
    
    string assignIP() {
        if (availableIPs.empty()) {
            return "";
        }
        string ip = availableIPs.front();
        availableIPs.pop();
        usedIPs.insert(ip);
        return ip;
    }
    
    void releaseIP(string ip) {
        if (usedIPs.find(ip) != usedIPs.end()) {
            usedIPs.erase(ip);
            availableIPs.push(ip);
        }
    }
    
    void displayStatus() {
        cout << "\n=== DHCP POOL STATUS ===\n";
        cout << "Subnet: " << subnet << "\n";
        cout << "Available IPs: " << availableIPs.size() << "\n";
        cout << "Used IPs: " << usedIPs.size() << "\n";
    }
};

#endif // DHCP_QUEUE_H