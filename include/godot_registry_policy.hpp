#ifndef GDSENSE_GODOT_REGISTRY_POLICY_HPP
#define GDSENSE_GODOT_REGISTRY_POLICY_HPP

#include <stdint.h>
#include <cstddef>

namespace godot {

struct GodotRegistryPolicy
{
    using EngineIdType = int32_t;

    struct Hasher {
        std::size_t operator()(const EngineIdType& id) const;
    };

    static inline EngineIdType NextDeviceId = 0;

    EngineIdType AllocEngineDevice();
    void DispatchNewGamepad(EngineIdType id);
    void DisconnectDevice(EngineIdType id);
};

}

#endif