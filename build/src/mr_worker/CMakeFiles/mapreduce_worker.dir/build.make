# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/zhangxu/D/mapreduce

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/zhangxu/D/mapreduce/build

# Include any dependencies generated for this target.
include src/mr_worker/CMakeFiles/mapreduce_worker.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/mr_worker/CMakeFiles/mapreduce_worker.dir/compiler_depend.make

# Include the progress variables for this target.
include src/mr_worker/CMakeFiles/mapreduce_worker.dir/progress.make

# Include the compile flags for this target's objects.
include src/mr_worker/CMakeFiles/mapreduce_worker.dir/flags.make

src/mr_worker/CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.o: src/mr_worker/CMakeFiles/mapreduce_worker.dir/flags.make
src/mr_worker/CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.o: /Users/zhangxu/D/mapreduce/src/mr_worker/mr_worker.cpp
src/mr_worker/CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.o: src/mr_worker/CMakeFiles/mapreduce_worker.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhangxu/D/mapreduce/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/mr_worker/CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.o"
	cd /Users/zhangxu/D/mapreduce/build/src/mr_worker && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/mr_worker/CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.o -MF CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.o.d -o CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.o -c /Users/zhangxu/D/mapreduce/src/mr_worker/mr_worker.cpp

src/mr_worker/CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.i"
	cd /Users/zhangxu/D/mapreduce/build/src/mr_worker && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zhangxu/D/mapreduce/src/mr_worker/mr_worker.cpp > CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.i

src/mr_worker/CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.s"
	cd /Users/zhangxu/D/mapreduce/build/src/mr_worker && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zhangxu/D/mapreduce/src/mr_worker/mr_worker.cpp -o CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.s

src/mr_worker/CMakeFiles/mapreduce_worker.dir/worker.cpp.o: src/mr_worker/CMakeFiles/mapreduce_worker.dir/flags.make
src/mr_worker/CMakeFiles/mapreduce_worker.dir/worker.cpp.o: /Users/zhangxu/D/mapreduce/src/mr_worker/worker.cpp
src/mr_worker/CMakeFiles/mapreduce_worker.dir/worker.cpp.o: src/mr_worker/CMakeFiles/mapreduce_worker.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhangxu/D/mapreduce/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/mr_worker/CMakeFiles/mapreduce_worker.dir/worker.cpp.o"
	cd /Users/zhangxu/D/mapreduce/build/src/mr_worker && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/mr_worker/CMakeFiles/mapreduce_worker.dir/worker.cpp.o -MF CMakeFiles/mapreduce_worker.dir/worker.cpp.o.d -o CMakeFiles/mapreduce_worker.dir/worker.cpp.o -c /Users/zhangxu/D/mapreduce/src/mr_worker/worker.cpp

src/mr_worker/CMakeFiles/mapreduce_worker.dir/worker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mapreduce_worker.dir/worker.cpp.i"
	cd /Users/zhangxu/D/mapreduce/build/src/mr_worker && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zhangxu/D/mapreduce/src/mr_worker/worker.cpp > CMakeFiles/mapreduce_worker.dir/worker.cpp.i

src/mr_worker/CMakeFiles/mapreduce_worker.dir/worker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mapreduce_worker.dir/worker.cpp.s"
	cd /Users/zhangxu/D/mapreduce/build/src/mr_worker && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zhangxu/D/mapreduce/src/mr_worker/worker.cpp -o CMakeFiles/mapreduce_worker.dir/worker.cpp.s

# Object files for target mapreduce_worker
mapreduce_worker_OBJECTS = \
"CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.o" \
"CMakeFiles/mapreduce_worker.dir/worker.cpp.o"

# External object files for target mapreduce_worker
mapreduce_worker_EXTERNAL_OBJECTS =

/Users/zhangxu/D/mapreduce/lib/libmapreduce_worker.a: src/mr_worker/CMakeFiles/mapreduce_worker.dir/mr_worker.cpp.o
/Users/zhangxu/D/mapreduce/lib/libmapreduce_worker.a: src/mr_worker/CMakeFiles/mapreduce_worker.dir/worker.cpp.o
/Users/zhangxu/D/mapreduce/lib/libmapreduce_worker.a: src/mr_worker/CMakeFiles/mapreduce_worker.dir/build.make
/Users/zhangxu/D/mapreduce/lib/libmapreduce_worker.a: src/mr_worker/CMakeFiles/mapreduce_worker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/zhangxu/D/mapreduce/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library /Users/zhangxu/D/mapreduce/lib/libmapreduce_worker.a"
	cd /Users/zhangxu/D/mapreduce/build/src/mr_worker && $(CMAKE_COMMAND) -P CMakeFiles/mapreduce_worker.dir/cmake_clean_target.cmake
	cd /Users/zhangxu/D/mapreduce/build/src/mr_worker && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mapreduce_worker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/mr_worker/CMakeFiles/mapreduce_worker.dir/build: /Users/zhangxu/D/mapreduce/lib/libmapreduce_worker.a
.PHONY : src/mr_worker/CMakeFiles/mapreduce_worker.dir/build

src/mr_worker/CMakeFiles/mapreduce_worker.dir/clean:
	cd /Users/zhangxu/D/mapreduce/build/src/mr_worker && $(CMAKE_COMMAND) -P CMakeFiles/mapreduce_worker.dir/cmake_clean.cmake
.PHONY : src/mr_worker/CMakeFiles/mapreduce_worker.dir/clean

src/mr_worker/CMakeFiles/mapreduce_worker.dir/depend:
	cd /Users/zhangxu/D/mapreduce/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/zhangxu/D/mapreduce /Users/zhangxu/D/mapreduce/src/mr_worker /Users/zhangxu/D/mapreduce/build /Users/zhangxu/D/mapreduce/build/src/mr_worker /Users/zhangxu/D/mapreduce/build/src/mr_worker/CMakeFiles/mapreduce_worker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/mr_worker/CMakeFiles/mapreduce_worker.dir/depend

