
#ifndef VTOP_PROCESS_H
#define VTOP_PROCESS_H

#include <sys/sysctl.h>
#include <pwd.h>
#include <assert.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <string>
#include <ctime>
#include "utils.h"

typedef struct kinfo_proc kinfo_proc;


static std::string get_boottime(){
    char buffer[32];
    struct timeval boottime;
    size_t len = sizeof(boottime);
    sysctlbyname("kern.boottime", &boottime, &len, NULL, 0);
    std::time_t time = (std::time_t) boottime.tv_sec;
    std::tm* ptm = std::localtime(&time);
    std::strftime(buffer, 32, "%a, %d/%m/%Y %H:%M:%S ", ptm);
    return std::string(buffer);
}

static std::string get_model_info(){
    char model[300];
    size_t len = sizeof(model);
    sysctlbyname("hw.model", &model, &len, NULL, 0);
    return std::string(model);
}

static std::string get_cpu_id(){
    char id[300];
    size_t len = sizeof(id);
    sysctlbyname("machdep.cpu.brand_string", &id, &len, NULL, 0);
    return std::string(id);
}

static std::string get_physical_memory() {
    uint64_t memory;
    size_t len = sizeof(memory);
    sysctlbyname("hw.memsize", &memory, &len, NULL, 0);
    return pretty_bytes(memory);
}



static int get_bsd_process_list(kinfo_proc **procList, size_t *procCount) {
    int err;
    kinfo_proc* result;
    bool done;
    static const int name[] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };

    size_t length;


    *procCount = 0;

    result = NULL;
    done = false;
    do {
        assert(result == NULL);


        length = 0;
        err = sysctl( (int *) name, (sizeof(name) / sizeof(*name)) - 1,
                      NULL, &length,
                      NULL, 0);
        if (err == -1) {
            err = errno;
        }



        if (err == 0) {
            result = (kinfo_proc *) malloc(length);
            if (result == NULL) {
                err = ENOMEM;
            }
        }



        if (err == 0) {
            err = sysctl( (int *) name, (sizeof(name) / sizeof(*name)) - 1,
                          result, &length,
                          NULL, 0);
            if (err == -1) {
                err = errno;
            }
            if (err == 0) {
                done = true;
            } else if (err == ENOMEM) {
                assert(result != NULL);
                free(result);
                result = NULL;
                err = 0;
            }
        }
    }
    while (err == 0 && ! done);

    if (err != 0 && result != NULL) {
        free(result);
        result = NULL;
    }
    *procList = result;
    if (err == 0) {
        *procCount = length / sizeof(kinfo_proc);
    }

    assert( (err == 0) == (*procList != NULL) );

    return err;
}


#endif //VTOP_PROCESS_H
