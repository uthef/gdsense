#include <godot_registry_policy.hpp>
#include <functional>
#include <sony_controller_manager.hpp>

using namespace godot;

std::size_t GodotRegistryPolicy::Hasher::operator ()(const EngineIdType& id) const {
    return std::hash<EngineIdType>{}(id);
}

GodotRegistryPolicy::EngineIdType GodotRegistryPolicy::AllocEngineDevice() {
     return ++NextDeviceId;
}

void GodotRegistryPolicy::DispatchNewGamepad(EngineIdType id) {
    auto *instance = godot::SonyControllerManager::get_primary_instance();

    if (instance)
        instance->emit_signal("gamepad_connected", id);
}

void GodotRegistryPolicy::DisconnectDevice(EngineIdType id) {
    auto *instance = godot::SonyControllerManager::get_primary_instance();

    if (instance)
        instance->emit_signal("gamepad_disconnected", id);
}