#ifndef RESOURCE_HH
#define RESOURCE_HH

#include "json.hpp"

/* For convenience. */
using json = nlohmann::json;

enum class ResourceType {
    SHADE,
    SUN,
    SHALLOW_WATER,
    MEDIUM_WATER,
    DEEP_WATER,
    SURFACE_WATER,
    UNUSED,
    UNUSED_,
    INSECT_POLLINATED_FLOWER,
    INSECT_POLLINATION,
    NITROGEN
};

struct Resource {
public:
    ResourceType type;
    double consumption;
    double affinity;
};

void from_json(const json &j, Resource &resource);
void to_json(json &j, const Resource &r);

#endif
