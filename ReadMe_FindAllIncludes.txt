I started working on a large project (over a million lines of code).
I wanted to be able to use MSVS to navigate around
individual projects, but the include files were scattered over hundreds of
sudirectories, and of course, include files include other include files.
It would have been a major effort to find all the required include files
manually.  I probably could have found them using grep, but I had been
teaching myself Java (didn't get far), and C#, so I wrote the FindAllIncludes
and SearchFiles projects to solve my problem, while refreshing myself
on C++ and MSVS.

I wrote the project expecting to run it under both Windows and Linux, and
possibly Unix. I only tested it on Windows machines. One obvious (and ugly)
artifact of that intent is the ConditionalIncludes.hpp file.

Nothing is GUI in the project this project.

BTW, I was just starting to use Git. The suffix digits on the filenames are
left over from an older version of the project.

FindAllIncludesRecursive constructs the directory tree(s) and prints them,
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