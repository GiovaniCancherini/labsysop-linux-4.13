#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include "processSleep.h"

asmlinkage long sys_listSleepProcesses(char __user *buf, int size) {
    struct task_struct *proces;
    char kbuf[2048]; // buffer temporário no kernel
    int bufsz = 0;
    int ret;

    memset(kbuf, 0, sizeof(kbuf));

    for_each_process(proces) {
        if (proces->state == TASK_INTERRUPTIBLE || proces->state == TASK_UNINTERRUPTIBLE) {
            int written = snprintf(kbuf + bufsz, sizeof(kbuf) - bufsz,
                                   "PID: %d | Nome: %s | Estado: %ld\n",
                                   task_pid_nr(proces),
                                   proces->comm,
                                   (long)proces->state);
            if (written < 0)
                return -EFAULT;

            if (written >= (sizeof(kbuf) - bufsz))
                break; // sem espaço suficiente

            bufsz += written;
        }
    }

    if (bufsz > size)
        bufsz = size;

    ret = copy_to_user(buf, kbuf, bufsz);
    if (ret != 0)
        return -EFAULT;

    return bufsz;
}
