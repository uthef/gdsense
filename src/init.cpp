#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/print_string.hpp>
#include "init.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <sony_controller_manager.hpp>

#ifdef _WIN32
    #include "Platform/windows/windows_hardware_policy.h"
    using platform_hardware = windows_platform::windows_hardware;
#else
    #include "Platform/linux/linux_hardware_policy.h"
    using platform_hardware = linux_platform::linux_hardware;
#endif

void initialize_gdsense_module(godot::ModuleInitializationLevel p_level) {
    if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    auto hw = std::make_unique<platform_hardware>();
    IPlatformHardware::SetInstance(std::move(hw));

    godot::ClassDB::register_class<godot::SonyControllerManager>();

    godot::print_line("[GDSense] The module has been initialized");
}

void deinitialize_gdsense_module(godot::ModuleInitializationLevel p_level) {
    if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    godot::print_line("[GDSense] The module has been deinitialized");
}

extern "C" {

GDExtensionBool GDE_EXPORT gdsense_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_object(p_get_proc_address, p_library, r_initialization);

    init_object.register_initializer(initialize_gdsense_module);
    init_object.register_terminator(deinitialize_gdsense_module);
    init_object.set_minimum_library_initialization_level(godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_object.init();
}

}