#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include "processListenMessage.h"

#define MSG_MAX_LEN 256

asmlinkage long sys_listenMessage(const char __user *user_msg) {
    char kbuf[MSG_MAX_LEN];
    long ret;

    // Copia a mensagem do usuário para o kernel de forma segura
    ret = strncpy_from_user(kbuf, user_msg, MSG_MAX_LEN);
    if (ret < 0 || ret == MSG_MAX_LEN)
        return -EFAULT;

    // Imprime a mensagem no log do kernel
    printk(KERN_INFO "Syscall listenMessage called with message: %s\n", kbuf);

    return 0; // sucesso
}
