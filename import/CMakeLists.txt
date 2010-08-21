cmake_minimum_required(VERSION 2.6)

add_library(
	luabind STATIC
	
	luabind-0.9/src/class.cpp
	luabind-0.9/src/class_info.cpp
	luabind-0.9/src/class_registry.cpp
	luabind-0.9/src/class_rep.cpp
	luabind-0.9/src/create_class.cpp
	luabind-0.9/src/error.cpp
	luabind-0.9/src/exception_handler.cpp
	luabind-0.9/src/function.cpp
	luabind-0.9/src/inheritance.cpp
	luabind-0.9/src/link_compatibility.cpp
	luabind-0.9/src/object_rep.cpp
	luabind-0.9/src/open.cpp
	luabind-0.9/src/pcall.cpp
	luabind-0.9/src/scope.cpp
	luabind-0.9/src/stack_content_by_name.cpp
	luabind-0.9/src/weak_ref.cpp
	luabind-0.9/src/wrapper_base.cpp
)

include_directories(
	luabind-0.9
)

target_link_libraries(
	luabind
	${LUA_LIBRARIES}
)

set(LUABIND_LIBRARIES luabind)
set(LUABIND_INCLUDE_DIRS import/luabind-0.9)
