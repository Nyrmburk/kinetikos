#include <linux/limits.h>
#include <unistd.h>
#include <libgen.h>

#include "File.h"

std::string* originPath = NULL;

std::string& origin() {
    if (!originPath) {
        char result[PATH_MAX] = {};
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        if (count != -1) {
            originPath = new std::string(dirname(result));
        }
    }
    return *originPath;
}
