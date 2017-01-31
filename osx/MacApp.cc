#ifdef __APPLE__

#include "MacApp.hh"

void setResourcePath(void) {
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if(!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)) {
        std::cout << "Error getting relative paths, using fallback\n";
    }
    else {
        chdir(path);
    }
    CFRelease(resourcesURL);
}
#endif
