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

SearchFiles1 constructs the directory tree and prints it, finds all files
in the tree with the requested extensions , searches the files to collect
a list (a vector--might have been better to use a set container, composed of
arrays of three strings, to eliminate redundancy) of all the lines in the
files with the search pattern (#include), along with the associated filenames
and paths. It then searches for include files specified in the #include
statements.

Example command statement for SearchFiles1 (from the SearchFiles1/Run directory):
SearchFiles1.exe "../../SearchFiles1wGit"  ".c,   .cpp, .h, .hpp"  #include

Note that this project finds include files referenced by any source file in
the directory tree. That is, it finds files not required
for a specific project.

Four files are output:
DirectoryTree.csv
CodeHeaderList.csv
ReferencedFilesIncode.csv
ReferencedFilesNotIncode.csv