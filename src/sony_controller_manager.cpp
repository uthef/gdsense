#include <sony_controller_manager.hpp>
using namespace godot;

SonyControllerManager *SonyControllerManager::_primary_instance = nullptr;

void SonyControllerManager::_bind_methods() {
    ADD_SIGNAL(MethodInfo("gamepad_connected", PropertyInfo(Variant::INT, "id")));
    ADD_SIGNAL(MethodInfo("gamepad_disconnected", PropertyInfo(Variant::INT, "id")));

    ClassDB::bind_integer_constant("SonyControllerManager", "Hand", "BOTH", (GDExtensionInt)EDSGamepadHand::AnyHand);
    ClassDB::bind_integer_constant("SonyControllerManager", "Hand", "LEFT", (GDExtensionInt)EDSGamepadHand::Left);
    ClassDB::bind_integer_constant("SonyControllerManager", "Hand", "RIGHT", (GDExtensionInt)EDSGamepadHand::Right);

    ClassDB::bind_integer_constant("SonyControllerManager", "MicLed", "OFF", (GDExtensionInt)EDSMic::MicOff);
    ClassDB::bind_integer_constant("SonyControllerManager", "MicLed", "ON", (GDExtensionInt)EDSMic::MicOn);
    ClassDB::bind_integer_constant("SonyControllerManager", "MicLed", "PULSE", (GDExtensionInt)EDSMic::Pulse);

    ClassDB::bind_method(D_METHOD("set_trigger_resistance", "device_id", "start_zones", "strength", "hand"), &SonyControllerManager::set_trigger_resistance);
    ClassDB::bind_method(D_METHOD("stop_trigger", "device_id",  "hand"), &SonyControllerManager::stop_trigger);
    ClassDB::bind_method(D_METHOD("set_vibration", "device_id",  "left", "right"), &SonyControllerManager::set_vibration);
    ClassDB::bind_method(D_METHOD("set_microphone_led", "device_id", "state"), &SonyControllerManager::set_microphone_led);
    ClassDB::bind_method(D_METHOD("get_battery_level", "device_id"), &SonyControllerManager::get_battery_level);
}

SonyControllerManager::SonyControllerManager() {
    if (_primary_instance)
        return;

    _registry = std::make_unique<DeviceRegistry>();
    _primary_instance = this;
}

void SonyControllerManager::_exit_tree() {
    if (_primary_instance == this) {
        _primary_instance = nullptr;

        godot::print_line("[GDSense] The primary controller manager instance exited the tree");
    }
}

SonyControllerManager *SonyControllerManager::get_primary_instance() {
    return _primary_instance;
}

void SonyControllerManager::_ready() {
    if (_primary_instance != this)
        return;
    
    godot::print_line("[GDSense] The primary controller manager is ready");
}

void SonyControllerManager::_process(double delta) {
    if (_primary_instance != this)
        return;

    _registry->PlugAndPlay(delta);
}

float SonyControllerManager::get_battery_level(int32_t device_id) {
    if (auto *gamepad = get_connected_gamepad(device_id)) {
        gamepad->UpdateInput((float)1.0 / 60.0);
        return gamepad->GetBattery();
    }

    return -1.0;
}

bool SonyControllerManager::set_trigger_resistance(int32_t device_id, int32_t start_zones, int32_t strength, int32_t hand) {
    if (start_zones > 255 || start_zones < 0 || strength < 0 || strength > 255 || hand < 0 || hand > 2)
        return false;

    if (auto *gamepad = get_connected_gamepad(device_id)) {
        if (!gamepad->GetIGamepadTrigger())
            return false;

        gamepad->GetIGamepadTrigger()->SetResistance((uint8_t)start_zones, (uint8_t)strength, (EDSGamepadHand)hand);
        gamepad->UpdateOutput();

        return true;
    }

    return false;
}

bool SonyControllerManager::stop_trigger(int32_t device_id, int32_t hand) {
    if (hand < 0 || hand > 2)
        return false;

    if (auto *gamepad = get_connected_gamepad(device_id)) {
        if (!gamepad->GetIGamepadTrigger())
            return false;

        gamepad->GetIGamepadTrigger()->StopTrigger((EDSGamepadHand)hand);
        gamepad->UpdateOutput();
        return true;
    }

    return false;
}

bool SonyControllerManager::set_vibration(int32_t device_id, int32_t left, int32_t right) {
    if (left < 0 || left > 255 || right < 0 || right > 255)
        return false;

    if (auto *gamepad = get_connected_gamepad(device_id)) {
        if (!gamepad->GetIGamepadRumbles())
            return false;

        gamepad->GetIGamepadRumbles()->SetVibration(left, right);
        gamepad->UpdateOutput();
        return true;
    }

    return false;
}

bool SonyControllerManager::set_microphone_led(int32_t device_id, int32_t state) {
    if (state < 0 || state > 2)
        return false;

    if (auto *gamepad = get_connected_gamepad(device_id)) {
        if (!gamepad->GetIGamepadLightbar())
            return false;

        gamepad->GetIGamepadLightbar()->SetMicrophoneLed((EDSMic)state);
        gamepad->UpdateOutput();

        return true;
    }

    return false;
}

IGamepadBase *SonyControllerManager::get_connected_gamepad(int32_t id) {
    if (_primary_instance != this)
        return nullptr;

    auto *gamepad = _registry->GetLibrary(id);

    if (!gamepad || !gamepad->IsConnected()) {
        return nullptr;
    }

    return gamepad;
}
