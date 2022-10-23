#ifndef VERSION_H
#define VERSION_H

struct Version {
    int major;
    int minor;
    int build;
    int revision;
    Version() : major(0), minor(0), build(-1), revision(-1){};
    Version(int major, int minor, int build, int revision)
        : major(major), minor(minor), build(build), revision(revision){};
};

#endif // VERSION_H
