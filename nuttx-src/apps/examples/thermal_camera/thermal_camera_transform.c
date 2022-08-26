#include "thermal_camera.h"

#ifdef CONFIG_CAMERA_LEPTON

extern struct temp_info_s *g_temp_image;
uint8_t rgb_image_buffer[LEPTON_FRAME_SIZE];

struct image_s rgb_image =
    {
        FT80X_FORMAT_RGB565,    /* format */
        LEPTON_FRAME_WIDTH,     /* width */
        LEPTON_FRAME_HEIGHT,    /* height */
        2 * LEPTON_FRAME_WIDTH, /* stride */
        0,                      /* offset */
        &rgb_image_buffer       /* data */
};

struct image_s *g_rgb_image = &rgb_image;

// Convert relative heat to heatmap representation
uint16_t heatmap_pixel(float heat)
{
    float red = -1.31888739 * heat * heat + 2.06663929 * heat + 0.145087;
    float green = -3.25995399 * heat * heat * heat + 5.50277848 * heat * heat + -1.46251282 * heat + 0.16182935;
    float blue = 7.6402748 * heat * heat * heat + -9.6954779 * heat * heat + 2.58820769 * heat + 0.43490329;
    return (((uint16_t)(red * 31) & 31) << 11) + (((uint16_t)(green * 63) & 63) << 5) + ((uint16_t)(blue * 31) & 31);
}

uint16_t get_temp(int32_t idx)
{
    uint16_t pixel = ((uint16_t *)(g_temp_image->temp_buffer))[idx];
    return ((pixel & 0xff) << 8) + ((pixel >> 8) & 0xFF);
}

static int32_t version;
int32_t *rgb_version = &version;

extern int32_t *temp_version;

int transform_image(int argc, char *argv[])
{
    uint16_t *intermediate_buffer = malloc(sizeof(u_int8_t) * LEPTON_FRAME_SIZE);
    if (intermediate_buffer == NULL)
    {
        return EXIT_FAILURE;
    }
    int32_t last = -1;
    for (;;)
    {
        if (last == *temp_version)
        {
            usleep(1000);
            continue;
        }
        else
        {
            last = *temp_version;
        }
        uint16_t *src = (uint16_t *)(g_temp_image->temp_buffer);
        uint16_t *dst = intermediate_buffer;
        uint16_t min = UINT16_MAX;
        int32_t min_idx = -1;
        uint16_t max = 0;
        int32_t max_idx = -1;
        for (int i = 0; i < LEPTON_FRAME_SIZE / 2; i++)
        {
            dst[i] = get_temp(i);
            if (dst[i] < min)
            {
                min_idx = i;
                min = dst[i];
            }

            if (dst[i] > max)
            {
                max_idx = i;
                max = dst[i];
            }
        }

        g_temp_image->min = min;
        g_temp_image->min_idx = min_idx;

        g_temp_image->max = max;
        g_temp_image->max_idx = max_idx;

        float range = (float)(max - min);
        caminfo("min = %x\n", min);
        // printf("Temp: min %d, max %d, range: %d\n", min, max, (int)range);

        src = intermediate_buffer;
        dst = (uint16_t *)(g_rgb_image->buffer);

        for (int i = 0; i < LEPTON_FRAME_SIZE / 2; i++)
        {
            float heat = (float)(src[i] - min) / range;
            dst[i] = heatmap_pixel(heat);
        }
        (*rgb_version)++;
        usleep(1000);
    }
    // unreachable
    free(intermediate_buffer);
    return EXIT_FAILURE;
}

#endif  /*CONFIG_CAMERA_LEPTON*/