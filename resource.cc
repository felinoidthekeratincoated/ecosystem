#include "resource.hh"

/* For convenience. */
using json = nlohmann::json;

/* Load from a json file. */
void from_json(const json &j, Resource &resource) {
    int type = j["type"];
    resource.type = (ResourceType)type;
    resource.consumption = j["consumption"];
    resource.affinity = j["affinity"];
}

/* Save to a json file. */
void to_json(json &j, const Resource &r) {
    j["type"] = (int)r.type;
    j["consumption"] = r.consumption;
    j["affinity"] = r.affinity;
}
