// TODO : put license here

#include "thermal_camera.h"

int main(int argc, FAR char *argv[])
{
    int ret;

    ret = boardctl(BOARDIOC_INIT, 0);

    ret = task_create("read_image", 100, 20 * 1024, read_image, NULL);
    ret = task_create("transform_image", 50, 20 * 1024, transform_image, NULL);
    ret = task_create("display", 50, 20 * 1024, display, NULL);
    //ret = task_create("usb_transfer", 50, 20 * 1024, usb_transfer, NULL);

    return EXIT_SUCCESS;
}

