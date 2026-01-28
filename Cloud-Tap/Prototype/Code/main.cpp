#include <iostream>
#include "CloudTAPMini.h"

using namespace std;

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================

int main() {
    cout << "============================================\n";
    cout << "  CLOUD TAP MINI\n";
    cout << "  Tiered Access Platform\n";
    cout << "  Enterprise Network Simulator\n";
    cout << "============================================\n";
    cout << "\nInitializing network systems...\n";
    cout << "Loading device inventory...\n";
    cout << "Building network topology...\n";
    cout << "Starting DHCP services...\n";
    cout << "\nPress Enter to start...";
    cin.get();
    
    CloudTAPMini system;
    system.showMenu();
    
    return 0;
}