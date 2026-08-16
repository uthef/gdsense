#ifndef GDSENSE_SONY_CONTROLLER_MANAGER_HPP
#define GDSENSE_SONY_CONTROLLER_MANAGER_HPP

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_registry_policy.hpp>

#include "GCore/Templates/TBasicDeviceRegistry.h"

namespace godot {

using DeviceRegistry = GamepadCore::TBasicDeviceRegistry<GodotRegistryPolicy>;

class SonyControllerManager : public Node {
    GDCLASS(SonyControllerManager, Node)

    std::unique_ptr<DeviceRegistry> _registry;
    static SonyControllerManager *_primary_instance;
    
    static void _bind_methods();

    IGamepadBase *get_connected_gamepad(int32_t id);

public:
    SonyControllerManager();

    static SonyControllerManager *get_primary_instance();

    void _ready() override;
    void _process(double delta) override;
    void _exit_tree() override;
    float get_battery_level(int32_t device_id);
    bool set_trigger_resistance(int32_t device_id, int32_t start_zones, int32_t strength, int32_t hand);
    bool stop_trigger(int32_t device_id, int32_t hand);
    bool set_vibration(int32_t device_id, int32_t left, int32_t right);
    bool set_microphone_led(int32_t device_id, int32_t state);
};

}

#endif