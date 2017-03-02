#include <iostream>

#include "network/tracker-connector.hh"

int main(void)
{
    TrackerConnector::TrackerConnector tc("www.google.com");

    std::cout << "connecting : " << tc.connect() << "\n";
    std::cout << "lol\n";
    return 0;
}