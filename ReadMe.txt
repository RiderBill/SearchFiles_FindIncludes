2016-09-27
There are two projects: SearchFiles1 and FindAllIncludesRecursive.

I started working on a large project (over a million lines of code).
I wanted to be able to use MSVS to navigate around
individual projects, but the include files were scattered over hundreds of
sudirectories, and of course, include files include other include files.
It would have been a major effort to find all the directly and indirectly
referenced include files manually.  I probably could have found them using
grep in a script, but I had been teaching myself C# and Java, so I wrote
the SearchFiles and FindAllIncludes projects to solve my problem, while
tuning up on C++ and MSVS.

I wrote the projects expecting to run them under both Windows and Linux, and
possibly Unix. I only tested it on Windows machines. One obvious (and ugly)
artifact of that intent is the ConditionalIncludes.hpp file.

There is no GUI; both projects are command line and file I/O.

I wasn't using Git when I started writing the codes. The suffix digits
on the some filenames are leftovers from a pre-git version.

========= SearchFiles1 ========= 
The program constructs the directory tree and prints it, finds all files
in the tree with the requested extensions , searches the files to collect
a list (a vector--might have been better to use a set container, composed of
arrays of three strings, to eliminate redundancy) of all the lines in the
files with the search pattern (#include), along with the associated filenames
and paths. It then searches for include files specified in the #include
statements.

Example command statement for SearchFiles1 (from the SearchFiles1/Run directory):
SearchFiles1.exe "../../SearchFiles1wGit"  ".c,   .cpp, .h, .hpp"  #include

Note that this project finds files referenced by any source file in
the directory tree. That is, it finds files not necessarily required for any
specific project.

Four files are output:
DirectoryTree.csv
CodeHeaderList.csv
ReferencedFilesIncode.csv
ReferencedFilesNotIncode.csv

========= FindAllIncludesRecursive ========= 
The program constructs the directory tree(s) and prints them,
creates a std::set (so unique) of include files contained in the directory
tree(s), then searches recursively (I'm assuming--I've slept a few times since
I wrote this) for include files referenced by the target file and files
referenced by  referenced files.

Example command statement for FindAllIncludesRecursive (from the FindAllIncludesRecursive/Run directory):
FindAllIncludesRecursive.exe "../../../SearchFiles1wGit/SourceCode/FindAllIncludesRecursive.cpp"  "../../../SearchFiles1wGit"

The actual execution statement included other top level directories, including
gcc and g++ directories.

Note that this project finds include files referenced directly and indirectly
by a single source file. The logical next step would have been to modify it
to find files referenced by a set of .cpp files, but I got the answers I
needed, so I stopped.

Three files are output:
DirectoryTrees.csv
foundPaths.csv
incMap.csv