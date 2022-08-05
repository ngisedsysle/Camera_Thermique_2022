#include "thermal_camera.h"

int open_usb(void)
{
    struct boardioc_usbdev_ctrl_s ctrl;
    FAR void *handle;
    int ret;
    int fd;

    /* Initialize the USB serial driver */
    ctrl.usbdev = BOARDIOC_USBDEV_CDCACM;
    ctrl.action = BOARDIOC_USBDEV_CONNECT;
    ctrl.instance = 2;
    ctrl.handle = &handle;

    ret = boardctl(BOARDIOC_USBDEV_CONTROL, (uintptr_t)&ctrl);
    if (ret < 0)
    {
        printf("open_usb: ERROR: Failed to create the USB serial device: %d\n",
               -ret);
        return 1;
    }
    /* disable all tracable bits, can be done later */
    usbtrace_enable(0);

    fd = open(CONFIG_EXAMPLES_USB_DEVPATH, O_WRONLY);
    if (fd < 0)
    {
        int errcode = errno;
        camerr("ERROR: Failed to open %s: %d\n",
               CONFIG_EXAMPLES_USB_DEVPATH, errcode);
        UNUSED(errcode);
        return EXIT_FAILURE;
    }
    return fd;
}

int usb_transfer(int argc, char *argv[])
{
    int ret = EXIT_FAILURE;
    ssize_t nbytes;
    int fd = open_usb();

    while (ret >= 0)
    {
        // static char msg[] = "hello world\n";
        // nbytes = write(fd, msg, sizeof(msg));
        // if (nbytes < 0)
        // {
        //     fprintf(stderr,"ERROR: write failed: %d\n", errno);
        // }
    }
    close(fd);
    return EXIT_FAILURE;
}