# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Alexey\git\mashgraph\task4\task

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Alexey\git\mashgraph\task4\task

# Include any dependencies generated for this target.
include CMakeFiles/main.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.dir/flags.make

CMakeFiles/main.dir/glad.c.obj: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/glad.c.obj: CMakeFiles/main.dir/includes_C.rsp
CMakeFiles/main.dir/glad.c.obj: glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Alexey\git\mashgraph\task4\task\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/main.dir/glad.c.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\gcc.exe  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\main.dir\glad.c.obj   -c C:\Users\Alexey\git\mashgraph\task4\task\glad.c

CMakeFiles/main.dir/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/glad.c.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\gcc.exe  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Alexey\git\mashgraph\task4\task\glad.c > CMakeFiles\main.dir\glad.c.i

CMakeFiles/main.dir/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/glad.c.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\gcc.exe  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Alexey\git\mashgraph\task4\task\glad.c -o CMakeFiles\main.dir\glad.c.s

CMakeFiles/main.dir/glad.c.obj.requires:

.PHONY : CMakeFiles/main.dir/glad.c.obj.requires

CMakeFiles/main.dir/glad.c.obj.provides: CMakeFiles/main.dir/glad.c.obj.requires
	$(MAKE) -f CMakeFiles\main.dir\build.make CMakeFiles/main.dir/glad.c.obj.provides.build
.PHONY : CMakeFiles/main.dir/glad.c.obj.provides

CMakeFiles/main.dir/glad.c.obj.provides.build: CMakeFiles/main.dir/glad.c.obj


CMakeFiles/main.dir/main.cpp.obj: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/main.cpp.obj: CMakeFiles/main.dir/includes_CXX.rsp
CMakeFiles/main.dir/main.cpp.obj: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Alexey\git\mashgraph\task4\task\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/main.dir/main.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\main.dir\main.cpp.obj -c C:\Users\Alexey\git\mashgraph\task4\task\main.cpp

CMakeFiles/main.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/main.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Alexey\git\mashgraph\task4\task\main.cpp > CMakeFiles\main.dir\main.cpp.i

CMakeFiles/main.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/main.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Alexey\git\mashgraph\task4\task\main.cpp -o CMakeFiles\main.dir\main.cpp.s

CMakeFiles/main.dir/main.cpp.obj.requires:

.PHONY : CMakeFiles/main.dir/main.cpp.obj.requires

CMakeFiles/main.dir/main.cpp.obj.provides: CMakeFiles/main.dir/main.cpp.obj.requires
	$(MAKE) -f CMakeFiles\main.dir\build.make CMakeFiles/main.dir/main.cpp.obj.provides.build
.PHONY : CMakeFiles/main.dir/main.cpp.obj.provides

CMakeFiles/main.dir/main.cpp.obj.provides.build: CMakeFiles/main.dir/main.cpp.obj


CMakeFiles/main.dir/Camera.cpp.obj: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/Camera.cpp.obj: CMakeFiles/main.dir/includes_CXX.rsp
CMakeFiles/main.dir/Camera.cpp.obj: Camera.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Alexey\git\mashgraph\task4\task\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/main.dir/Camera.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\main.dir\Camera.cpp.obj -c C:\Users\Alexey\git\mashgraph\task4\task\Camera.cpp

CMakeFiles/main.dir/Camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/Camera.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Alexey\git\mashgraph\task4\task\Camera.cpp > CMakeFiles\main.dir\Camera.cpp.i

CMakeFiles/main.dir/Camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/Camera.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Alexey\git\mashgraph\task4\task\Camera.cpp -o CMakeFiles\main.dir\Camera.cpp.s

CMakeFiles/main.dir/Camera.cpp.obj.requires:

.PHONY : CMakeFiles/main.dir/Camera.cpp.obj.requires

CMakeFiles/main.dir/Camera.cpp.obj.provides: CMakeFiles/main.dir/Camera.cpp.obj.requires
	$(MAKE) -f CMakeFiles\main.dir\build.make CMakeFiles/main.dir/Camera.cpp.obj.provides.build
.PHONY : CMakeFiles/main.dir/Camera.cpp.obj.provides

CMakeFiles/main.dir/Camera.cpp.obj.provides.build: CMakeFiles/main.dir/Camera.cpp.obj


CMakeFiles/main.dir/ShaderProgram.cpp.obj: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/ShaderProgram.cpp.obj: CMakeFiles/main.dir/includes_CXX.rsp
CMakeFiles/main.dir/ShaderProgram.cpp.obj: ShaderProgram.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Alexey\git\mashgraph\task4\task\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/main.dir/ShaderProgram.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\main.dir\ShaderProgram.cpp.obj -c C:\Users\Alexey\git\mashgraph\task4\task\ShaderProgram.cpp

CMakeFiles/main.dir/ShaderProgram.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/ShaderProgram.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Alexey\git\mashgraph\task4\task\ShaderProgram.cpp > CMakeFiles\main.dir\ShaderProgram.cpp.i

CMakeFiles/main.dir/ShaderProgram.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/ShaderProgram.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Alexey\git\mashgraph\task4\task\ShaderProgram.cpp -o CMakeFiles\main.dir\ShaderProgram.cpp.s

CMakeFiles/main.dir/ShaderProgram.cpp.obj.requires:

.PHONY : CMakeFiles/main.dir/ShaderProgram.cpp.obj.requires

CMakeFiles/main.dir/ShaderProgram.cpp.obj.provides: CMakeFiles/main.dir/ShaderProgram.cpp.obj.requires
	$(MAKE) -f CMakeFiles\main.dir\build.make CMakeFiles/main.dir/ShaderProgram.cpp.obj.provides.build
.PHONY : CMakeFiles/main.dir/ShaderProgram.cpp.obj.provides

CMakeFiles/main.dir/ShaderProgram.cpp.obj.provides.build: CMakeFiles/main.dir/ShaderProgram.cpp.obj


CMakeFiles/main.dir/ini.c.obj: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/ini.c.obj: CMakeFiles/main.dir/includes_C.rsp
CMakeFiles/main.dir/ini.c.obj: ini.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Alexey\git\mashgraph\task4\task\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/main.dir/ini.c.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\gcc.exe  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\main.dir\ini.c.obj   -c C:\Users\Alexey\git\mashgraph\task4\task\ini.c

CMakeFiles/main.dir/ini.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/ini.c.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\gcc.exe  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Alexey\git\mashgraph\task4\task\ini.c > CMakeFiles\main.dir\ini.c.i

CMakeFiles/main.dir/ini.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/ini.c.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\gcc.exe  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Alexey\git\mashgraph\task4\task\ini.c -o CMakeFiles\main.dir\ini.c.s

CMakeFiles/main.dir/ini.c.obj.requires:

.PHONY : CMakeFiles/main.dir/ini.c.obj.requires

CMakeFiles/main.dir/ini.c.obj.provides: CMakeFiles/main.dir/ini.c.obj.requires
	$(MAKE) -f CMakeFiles\main.dir\build.make CMakeFiles/main.dir/ini.c.obj.provides.build
.PHONY : CMakeFiles/main.dir/ini.c.obj.provides

CMakeFiles/main.dir/ini.c.obj.provides.build: CMakeFiles/main.dir/ini.c.obj


CMakeFiles/main.dir/INIReader.cpp.obj: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/INIReader.cpp.obj: CMakeFiles/main.dir/includes_CXX.rsp
CMakeFiles/main.dir/INIReader.cpp.obj: INIReader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Alexey\git\mashgraph\task4\task\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/main.dir/INIReader.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\main.dir\INIReader.cpp.obj -c C:\Users\Alexey\git\mashgraph\task4\task\INIReader.cpp

CMakeFiles/main.dir/INIReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/INIReader.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Alexey\git\mashgraph\task4\task\INIReader.cpp > CMakeFiles\main.dir\INIReader.cpp.i

CMakeFiles/main.dir/INIReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/INIReader.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Alexey\git\mashgraph\task4\task\INIReader.cpp -o CMakeFiles\main.dir\INIReader.cpp.s

CMakeFiles/main.dir/INIReader.cpp.obj.requires:

.PHONY : CMakeFiles/main.dir/INIReader.cpp.obj.requires

CMakeFiles/main.dir/INIReader.cpp.obj.provides: CMakeFiles/main.dir/INIReader.cpp.obj.requires
	$(MAKE) -f CMakeFiles\main.dir\build.make CMakeFiles/main.dir/INIReader.cpp.obj.provides.build
.PHONY : CMakeFiles/main.dir/INIReader.cpp.obj.provides

CMakeFiles/main.dir/INIReader.cpp.obj.provides.build: CMakeFiles/main.dir/INIReader.cpp.obj


# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/glad.c.obj" \
"CMakeFiles/main.dir/main.cpp.obj" \
"CMakeFiles/main.dir/Camera.cpp.obj" \
"CMakeFiles/main.dir/ShaderProgram.cpp.obj" \
"CMakeFiles/main.dir/ini.c.obj" \
"CMakeFiles/main.dir/INIReader.cpp.obj"

# External object files for target main
main_EXTERNAL_OBJECTS =

main.exe: CMakeFiles/main.dir/glad.c.obj
main.exe: CMakeFiles/main.dir/main.cpp.obj
main.exe: CMakeFiles/main.dir/Camera.cpp.obj
main.exe: CMakeFiles/main.dir/ShaderProgram.cpp.obj
main.exe: CMakeFiles/main.dir/ini.c.obj
main.exe: CMakeFiles/main.dir/INIReader.cpp.obj
main.exe: CMakeFiles/main.dir/build.make
main.exe: CMakeFiles/main.dir/linklibs.rsp
main.exe: CMakeFiles/main.dir/objects1.rsp
main.exe: CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Alexey\git\mashgraph\task4\task\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable main.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\main.dir\link.txt --verbose=$(VERBOSE)
	"C:\Program Files\CMake\bin\cmake.exe" -E copy_directory C:/Users/Alexey/git/mashgraph/task4/task/shaders C:/Users/Alexey/git/mashgraph/task4/task
	"C:\Program Files\CMake\bin\cmake.exe" -E copy_directory C:/Users/Alexey/git/mashgraph/task4/task/dependencies/bin C:/Users/Alexey/git/mashgraph/task4/task

# Rule to build all files generated by this target.
CMakeFiles/main.dir/build: main.exe

.PHONY : CMakeFiles/main.dir/build

CMakeFiles/main.dir/requires: CMakeFiles/main.dir/glad.c.obj.requires
CMakeFiles/main.dir/requires: CMakeFiles/main.dir/main.cpp.obj.requires
CMakeFiles/main.dir/requires: CMakeFiles/main.dir/Camera.cpp.obj.requires
CMakeFiles/main.dir/requires: CMakeFiles/main.dir/ShaderProgram.cpp.obj.requires
CMakeFiles/main.dir/requires: CMakeFiles/main.dir/ini.c.obj.requires
CMakeFiles/main.dir/requires: CMakeFiles/main.dir/INIReader.cpp.obj.requires

.PHONY : CMakeFiles/main.dir/requires

CMakeFiles/main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\main.dir\cmake_clean.cmake
.PHONY : CMakeFiles/main.dir/clean

CMakeFiles/main.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Alexey\git\mashgraph\task4\task C:\Users\Alexey\git\mashgraph\task4\task C:\Users\Alexey\git\mashgraph\task4\task C:\Users\Alexey\git\mashgraph\task4\task C:\Users\Alexey\git\mashgraph\task4\task\CMakeFiles\main.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main.dir/depend
