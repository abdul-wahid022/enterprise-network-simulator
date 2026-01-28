#ifndef CONNECTION_STACK_H
#define CONNECTION_STACK_H

#include <iostream>
#include <string>
#include <stack>
using namespace std;

// ============================================================================
// STACK FOR CONNECTION HISTORY
// ============================================================================

class ConnectionStack {
private:
    stack<string> history;
    int maxSize;
    
public:
    ConnectionStack(int size = 20) : maxSize(size) {}
    
    void push(string connection) {
        if ((int)history.size() >= maxSize) {
            // Remove oldest (bottom of stack)
            stack<string> temp;
            while ((int)history.size() > 1) {
                temp.push(history.top());
                history.pop();
            }
            history.pop(); // Remove oldest
            while (!temp.empty()) {
                history.push(temp.top());
                temp.pop();
            }
        }
        history.push(connection);
    }
    
    void display() {
        cout << "\n=== CONNECTION HISTORY ===\n";
        stack<string> temp = history;
        int count = 1;
        while (!temp.empty()) {
            cout << count++ << ". " << temp.top() << "\n";
            temp.pop();
        }
    }
};

#endif // CONNECTION_STACK_H