#include "thermal_camera.h"

#ifdef CONFIG_CAMERA_LEPTON

uint8_t packets[LEPTON_PACKETTED_FRAME_SIZE];
struct temp_info_s temp_image;
struct temp_info_s *g_temp_image = &temp_image;

int open_camera(void)
{
    caminfo("Opening /dev/lepton ...\n");
    int fd = open(CONFIG_EXAMPLES_LEPTON_DEVPATH, O_RDONLY);
    if (fd < 0)
    {
        int errcode = errno;
        fprintf(stderr,"ERROR %d: Failed to open %s.\n",errcode,
               CONFIG_EXAMPLES_LEPTON_DEVPATH);
        UNUSED(errcode);
        return EXIT_FAILURE;
    }
    return fd;
}

static int32_t version;
int32_t *temp_version = &version;

int read_image(int argc, char *argv[])
{
    int ret = EXIT_FAILURE;
    int discard_packets = 0;
    int i = 0;
    int fd = open_camera();
    while (ret >= 0)
    {

        // Read discard packets till a new image is available
        // caminfo("Reading discard packets...\n");
        ret = read(fd, packets, LEPTON_PACKET_SIZE);
        if (ret < 0)
        {
            fprintf(stderr, "Failed to read discard packet: errno = %d\n", errno);
        }
        else
        {
            // caminfo("done.\n");
            if ((packets[LEPTON_DISCARD_BYTE] & 0x0F) != 0x0F)
            {
                // if available, read the rest of the image
                ret = read(fd, packets + LEPTON_PACKET_SIZE, LEPTON_PACKETTED_FRAME_SIZE - LEPTON_PACKET_SIZE);
                if (ret < 0)
                {
                    fprintf(stderr, "Failed to read the rest of the image: errno = %d\n", errno);
                }
                // caminfo("done.\n Integrity verification...\n");
                // print_packets(packets);

                // check that all the image lines have been sent (ie last line id is 59 (Ox3B))
                int last_line_id_byte = LEPTON_PACKETTED_FRAME_SIZE - LEPTON_PACKET_SIZE + LEPTON_LINE_ID_BYTE;
                if (packets[last_line_id_byte] == 0x3B)
                {
                    // caminfo("done.\nCopying the data...\n");
                    for (int i = 0; i < LEPTON_FRAME_HEIGHT; i++)
                    {
                        memcpy(&(g_temp_image->temp_buffer[i * LEPTON_LINE_SIZE]), packets + i * LEPTON_PACKET_SIZE + 4, LEPTON_LINE_SIZE);
                    }
                    discard_packets = 0;
                    (*temp_version)++;
                    usleep(1000);
                    // caminfo("done.\n");
                }
                else
                {
                    // camwarn("Error found! Last id : %d\n", packets[last_line_id_byte]);
                    // TODO : need reync
                }
            }
            else
            {
                if (discard_packets > LEPTON_MAX_DISCARD_FRAMES)
                {
                    camwarn("Too many discarded packets!\n");
                    // TODO : need resync
                }
                else
                {
                    discard_packets++;
                }
            }
        }
    }
    // not supposed to be there unless an error occured
    close(fd);
    return EXIT_FAILURE;
}

// void print_packet(uint8_t *buffer)
// {
//     printf("    %02x         %02x  %04x  ||", buffer[0], buffer[1], *((uint16_t *)&(buffer[2])));
//     for (int j = 4; j < LEPTON_PACKET_SIZE; j += 2)
//     {
//         if (j - 4 > 0 && (j - 4) % 40 == 0)
//         {
//             printf("\n                         ||");
//         }
//         printf(" %04x", *((uint16_t *)&(buffer[j])));
//     }
//     printf("\n");
// }

// void print_packets(uint8_t *buffer)
// {
//     printf("FIRST/Discard  ID  CRC   ||  TRAM\n");
//     for (int i = 0; i < LEPTON_FRAME_HEIGHT; i++)
//     {
//         print_packet(buffer + i * LEPTON_PACKET_SIZE);
//     }
//     printf("\n");
//     printf("\n");
// }
#endif  /*CONFIG_CAMERA_LEPTON*/