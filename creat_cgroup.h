#ifndef CREAT_CGROUP_H
#define CREAT_CGROUP_H

#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

#define CGROUP_FOLDER "/sys/fs/cgroup/pids/container/"
#define concat(a,b) (a"" b)

void write_rule(const char *path, const char *value) {
    int fd = open(path, O_WRONLY | O_CREAT | O_APPEND);
    printf("open: %s\n", strerror(errno));
    write(fd, value, strlen(value));
    printf("write: %s\n", strerror(errno));
    close(fd);
    printf("close: %s\n", strerror(errno));
}

void limitProcessCreation() {
    int res = mkdir(CGROUP_FOLDER, S_IRUSR | S_IWUSR);

    printf("mkdir code is %d\n", res);
    if (res == -1){
        printf("Error msg: %s\n", strerror(errno));
    }
    const char *pid = std::to_string(getpid()).c_str();
    std::string path = CGROUP_FOLDER;
    write_rule((path + "pids.max").c_str(), "5");
    write_rule((path + "notify_on_release").c_str(), "1");
    write_rule((path + "cgroup.procs").c_str(), pid);
}

#endif // CREAT_CGROUP_H