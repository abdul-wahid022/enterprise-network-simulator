#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include <iostream>
#include <string>
#include <queue>
#include <ctime>
using namespace std;

// ============================================================================
// PRIORITY QUEUE FOR EVENT LOGGING
// ============================================================================

struct LogEntry {
    int severity; // 0=INFO, 1=WARN, 2=ERROR, 3=CRITICAL
    string message;
    string timestamp;
    
    LogEntry(int s, string m, string t) : severity(s), message(m), timestamp(t) {}
    
    bool operator<(const LogEntry& other) const {
        return severity < other.severity; // Max heap
    }
};

class EventLogger {
private:
    priority_queue<LogEntry> logs;
    int maxSize;
    
public:
    EventLogger(int size = 50) : maxSize(size) {}
    
    void addLog(int severity, string message) {
        time_t now = time(0);
        char buf[80];
        strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&now));
        
        logs.push(LogEntry(severity, message, string(buf)));
        
        // Keep only recent logs
        if ((int)logs.size() > maxSize) {
            priority_queue<LogEntry> temp;
            for (int i = 0; i < maxSize; i++) {
                temp.push(logs.top());
                logs.pop();
            }
            logs = temp;
        }
    }
    
    void displayLogs(int count = 10) {
        cout << "\n=== RECENT SYSTEM EVENTS (Top " << count << ") ===\n";
        priority_queue<LogEntry> temp = logs;
        
        for (int i = 0; i < count && !temp.empty(); i++) {
            LogEntry entry = temp.top();
            temp.pop();
            
            string severityStr;
            switch(entry.severity) {
                case 0: severityStr = "[INFO]"; break;
                case 1: severityStr = "[WARN]"; break;
                case 2: severityStr = "[ERROR]"; break;
                case 3: severityStr = "[CRITICAL]"; break;
            }
            
            cout << entry.timestamp << " " << severityStr << " " << entry.message << "\n";
        }
    }
};

#endif // EVENT_LOGGER_H