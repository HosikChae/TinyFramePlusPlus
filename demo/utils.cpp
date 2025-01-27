//
// Created by MightyPork on 2017/10/15.
//

#include "utils.hpp"
#include <cstdio>

namespace TinyFrame_n{

// helper func for testing
void dumpFrame(const uint8_t *buff, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++) {
        printf("%3u \033[94m%02X\033[0m", buff[i], buff[i]);
        if (buff[i] >= 0x20 && buff[i] < 127) {
            printf(" %c", buff[i]);
        }
        else {
            printf(" \033[31m.\033[0m");
        }
        printf("\n");
    }
    printf("--- end of frame ---\n\n");
}

void dumpFrameInfo(Msg *msg)
{
    printf("\033[33mFrame info\n"
               "  type: 0x%Xh\n"
               "  data: \"%.*s\"\n"
               "   len: 0x%X\n"
               "    id: 0x%Xh\033[0m\n\n",
           (int)msg->type, (int)msg->len, msg->data, (int)msg->len, (int)msg->frame_id);
}
} // TinyFrame_n