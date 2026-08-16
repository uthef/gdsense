#ifndef GDSENSE_INIT_HPP
#define GDSENSE_INIT_HPP

void initialize_gdsense_module(godot::ModuleInitializationLevel p_level);
void deinitialize_gdsense_module(godot::ModuleInitializationLevel p_level);

extern "C" {

GDExtensionBool GDE_EXPORT gdsense_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization);

}

#endif