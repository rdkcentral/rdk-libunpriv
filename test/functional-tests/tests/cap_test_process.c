#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/capability.h>
#include "cap.h"

int main(int argc, char **argv)
{
    cap_user user = {0};

    char *proc = strrchr(argv[0], '/');
    user.user_name = proc ? proc + 1 : argv[0];

    printf("L2: starting as UID=%d, proc=%s\n", getuid(), user.user_name);

    int ret = drop_root_caps(&user);
    if (ret != 0) {
        fprintf(stderr,
                "ERROR: drop_root_caps failed (ret=%d, errno=%d)\n",
                ret, errno);
        return 2;
    }

    cap_t caps = cap_get_proc();
    if (!caps) {
        perror("cap_get_proc");
        return 3;
    }

    char *text = cap_to_text(caps, NULL);
    if (text) {
        printf("CAPABILITIES:%s\n", text);
        cap_free(text);
    }

    cap_free(caps);
    return 0;
}
