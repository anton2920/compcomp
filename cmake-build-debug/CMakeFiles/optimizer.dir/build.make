# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/anton/Downloads/clion-2019.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/anton/Downloads/clion-2019.2/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/anton/compcomp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/anton/compcomp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/optimizer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/optimizer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/optimizer.dir/flags.make

CMakeFiles/optimizer.dir/asm/optimize.cpp.o: CMakeFiles/optimizer.dir/flags.make
CMakeFiles/optimizer.dir/asm/optimize.cpp.o: ../asm/optimize.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/anton/compcomp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/optimizer.dir/asm/optimize.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/optimizer.dir/asm/optimize.cpp.o -c /home/anton/compcomp/asm/optimize.cpp

CMakeFiles/optimizer.dir/asm/optimize.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/optimizer.dir/asm/optimize.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/anton/compcomp/asm/optimize.cpp > CMakeFiles/optimizer.dir/asm/optimize.cpp.i

CMakeFiles/optimizer.dir/asm/optimize.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/optimizer.dir/asm/optimize.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/anton/compcomp/asm/optimize.cpp -o CMakeFiles/optimizer.dir/asm/optimize.cpp.s

CMakeFiles/optimizer.dir/asm/Lexer.cpp.o: CMakeFiles/optimizer.dir/flags.make
CMakeFiles/optimizer.dir/asm/Lexer.cpp.o: ../asm/Lexer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/anton/compcomp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/optimizer.dir/asm/Lexer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/optimizer.dir/asm/Lexer.cpp.o -c /home/anton/compcomp/asm/Lexer.cpp

CMakeFiles/optimizer.dir/asm/Lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/optimizer.dir/asm/Lexer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/anton/compcomp/asm/Lexer.cpp > CMakeFiles/optimizer.dir/asm/Lexer.cpp.i

CMakeFiles/optimizer.dir/asm/Lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/optimizer.dir/asm/Lexer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/anton/compcomp/asm/Lexer.cpp -o CMakeFiles/optimizer.dir/asm/Lexer.cpp.s

CMakeFiles/optimizer.dir/asm/Tokens.cpp.o: CMakeFiles/optimizer.dir/flags.make
CMakeFiles/optimizer.dir/asm/Tokens.cpp.o: ../asm/Tokens.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/anton/compcomp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/optimizer.dir/asm/Tokens.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/optimizer.dir/asm/Tokens.cpp.o -c /home/anton/compcomp/asm/Tokens.cpp

CMakeFiles/optimizer.dir/asm/Tokens.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/optimizer.dir/asm/Tokens.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/anton/compcomp/asm/Tokens.cpp > CMakeFiles/optimizer.dir/asm/Tokens.cpp.i

CMakeFiles/optimizer.dir/asm/Tokens.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/optimizer.dir/asm/Tokens.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/anton/compcomp/asm/Tokens.cpp -o CMakeFiles/optimizer.dir/asm/Tokens.cpp.s

# Object files for target optimizer
optimizer_OBJECTS = \
"CMakeFiles/optimizer.dir/asm/optimize.cpp.o" \
"CMakeFiles/optimizer.dir/asm/Lexer.cpp.o" \
"CMakeFiles/optimizer.dir/asm/Tokens.cpp.o"

# External object files for target optimizer
optimizer_EXTERNAL_OBJECTS =

optimizer: CMakeFiles/optimizer.dir/asm/optimize.cpp.o
optimizer: CMakeFiles/optimizer.dir/asm/Lexer.cpp.o
optimizer: CMakeFiles/optimizer.dir/asm/Tokens.cpp.o
optimizer: CMakeFiles/optimizer.dir/build.make
optimizer: CMakeFiles/optimizer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/anton/compcomp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable optimizer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/optimizer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/optimizer.dir/build: optimizer

.PHONY : CMakeFiles/optimizer.dir/build

CMakeFiles/optimizer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/optimizer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/optimizer.dir/clean

CMakeFiles/optimizer.dir/depend:
	cd /home/anton/compcomp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/anton/compcomp /home/anton/compcomp /home/anton/compcomp/cmake-build-debug /home/anton/compcomp/cmake-build-debug /home/anton/compcomp/cmake-build-debug/CMakeFiles/optimizer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/optimizer.dir/depend
