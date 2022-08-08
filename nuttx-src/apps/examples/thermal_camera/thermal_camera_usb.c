#include "thermal_camera.h"

#define CDCACM_DEVNAME_FORMAT  "/dev/ttyACM%d"
#define CDCACM_DEVNAME_SIZE    16

int open_usb(void)
{
    struct boardioc_usbdev_ctrl_s ctrl;
    char devname[CDCACM_DEVNAME_SIZE];
    FAR void *handle;
    int ret;
    int fd;

    /* Initialize the USB serial driver */
    ctrl.usbdev = BOARDIOC_USBDEV_CDCACM;
    ctrl.action = BOARDIOC_USBDEV_CONNECT;
    ctrl.instance = CONFIG_EXAMPLES_USBDEV_INSTANCE;
    ctrl.handle = &handle;

    ret = boardctl(BOARDIOC_USBDEV_CONTROL, (uintptr_t)&ctrl);
    if (ret < 0)
    {
        fprintf(stderr,"ERROR %d: Failed to create the USB serial device.\n",
               -ret);
        return 1;
    }
    /* disable all tracable bits, can be done later */
    usbtrace_enable(0);

    snprintf(devname, CDCACM_DEVNAME_SIZE, CDCACM_DEVNAME_FORMAT, CONFIG_EXAMPLES_USBDEV_INSTANCE);
    fd = open(devname, O_WRONLY);
    if (fd < 0)
    {
        int errcode = errno;

         /* ENOTCONN means that the USB device is not yet connected */
        if (errcode == ENOTCONN)
        {
            fprintf(stderr,"ERROR %d: USB device %s is not connected.\n", errcode,devname);
        }
        else
        {
            fprintf(stderr,"ERROR %d: fail to open usb device.\n", errcode,devname);
        }
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