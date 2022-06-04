#include <vector>
#include <map>
#include "common_utils.h"
#include "client_selectable.h"

class CPlayer {
private:
    std::map<uint16_t,CSelectable> elements;
public:
    void addElement(unit_t type,State & desc);
    void addElement(building_t type,State & desc);
    void update(std::vector<State> & server_data);
};

