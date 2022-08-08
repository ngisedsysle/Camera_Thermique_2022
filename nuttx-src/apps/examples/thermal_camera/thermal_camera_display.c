#include "thermal_camera.h"

#ifdef CONFIG_LCD_FT80X_SPI

extern FAR struct image_s *g_rgb_image;
extern struct temp_info_s *g_temp_image;

struct
{
    struct ft80x_cmd32_s clearrgb;
    struct ft80x_cmd32_s clear;
    struct ft80x_cmd32_s colorrgb;
} setup_cmds;

struct
{
    struct ft80x_cmd32_s begin;
    struct ft80x_cmd32_s savecontext;
    struct ft80x_cmd32_s bitmaptransformA;
    struct ft80x_cmd32_s bitmaptransformE;
    struct ft80x_cmd32_s bitmapsource;
    struct ft80x_cmd32_s bitmaplayout;
    struct ft80x_cmd32_s bitmapsize;
    struct ft80x_cmd32_s vertex2f;
    struct ft80x_cmd32_s restorecontext;
    struct ft80x_cmd32_s end;
} bitmap_cmds;

struct
{
    struct ft80x_cmd32_s begin;
    struct ft80x_cmd32_s bitmapsource;
    struct ft80x_cmd32_s bitmaplayout;
    struct ft80x_cmd32_s bitmapsize;
    struct ft80x_cmd32_s vertex2f;
    struct ft80x_cmd32_s end;

    struct ft80x_cmd_number_s max;
    struct ft80x_cmd_number_s min;
} scale_cmds;

struct
{
    struct ft80x_cmd_number_s value;
    struct ft80x_cmd32_s begin;
    struct ft80x_cmd32_s linewidth;
    struct ft80x_cmd32_s west;
    struct ft80x_cmd32_s east;
    struct ft80x_cmd32_s north;
    struct ft80x_cmd32_s south;
    struct ft80x_cmd_text_s text;
} center_cmds;

struct
{
    struct ft80x_cmd_text_s text;
} unit_cmds;

void print_time()
{
    struct tm datetime;
    up_rtc_getdatetime(&datetime);
    caminfo("Time : %dH%d (%s), Date : %d/%d/%d\n", datetime.tm_hour, datetime.tm_min, datetime.tm_zone, datetime.tm_mday, datetime.tm_mon, datetime.tm_year);
}
int open_screen(void)
{
    lcdinfo("Opening /dev/ft801 ...\n");
    int fd = open(CONFIG_EXAMPLES_FT80X_DEVPATH,  O_CREAT | O_WRONLY);
    if (fd < 0)
    {
        int errcode = errno;
        fprintf(stderr, "ERROR: Failed to open %s: %d\n",
                CONFIG_EXAMPLES_FT80X_DEVPATH, errcode);
        UNUSED(errcode);
        return EXIT_FAILURE;
    }
    return fd;
}

u_int16_t *create_color_scale(size_t width, size_t height, size_t border)
{
    u_int16_t *buffer = malloc(height * width * sizeof(uint16_t));
    int i;
    float step = 1. / (float)(height - 2 * border - 1);
    float heat = 1.;
    for (i = 0; i < border; i++)
    {
        for (int j = 0; j < width; j++)
        {
            buffer[i * width + j] = 0;
        }
    }
    for (; i < height - border; i++)
    {
        int j;
        uint16_t pixel = heatmap_pixel(heat);
        heat -= step;

        for (j = 0; j < border; j++)
        {
            buffer[i * width + j] = 0;
        }

        for (; j < width - border; j++)
        {
            buffer[i * width + j] = pixel;
        }

        for (; j < width; j++)
        {
            buffer[i * width + j] = 0;
        }
    }

    for (; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            buffer[i * width + j] = 0;
        }
    }
    return buffer;
}

extern int32_t *rgb_version;

int display(int argc, char *argv[])
{
    caminfo("start display\n");
    FAR struct ft80x_dlbuffer_s *buffer;
    int ret = EXIT_FAILURE;
    int fd = open_screen();

    uint16_t *color_scale = create_color_scale(SCALE_COLOR_WIDTH, SCALE_COLOR_HEIGHT, SCALE_COLOR_BORDER);

    /* Allocate the display list buffer structure */
    buffer = (FAR struct ft80x_dlbuffer_s *)malloc(sizeof(struct ft80x_dlbuffer_s));
    if (buffer == NULL)
    {
        fprintf(stderr, "ERROR: Failed to allocate display list buffer\n");
        close(fd);
        return EXIT_FAILURE;
    }

    /* Make sure that the backlight off */
    ft80x_backlight_set(fd, 0);

    /* Copy the image into graphics ram for the color_scale */
    ret = ft80x_ramg_write(fd, 0, color_scale, SCALE_COLOR_SIZE);
    if (ret < 0)
    {
        fprintf(stderr, "ERROR: ft80x_ramg_write() failed: %d\n", ret);
        return ret;
    }

    int32_t last = -1;
    while (ret >= 0)
    {
        if (last == *rgb_version)
        {
            usleep(10);
            continue;
        }
        else
        {
            last = *rgb_version;
        }
        /* Copy the image into graphics ram for the rgb image */
        ret = ft80x_ramg_write(fd, SCALE_COLOR_SIZE, g_rgb_image->buffer, g_rgb_image->stride * g_rgb_image->height);
        if (ret < 0)
        {
            fprintf(stderr, "ERROR: ft80x_ramg_write() failed: %d\n", ret);
            return ret;
        }
        /* Create the display list */
        ret = ft80x_dl_start(fd, buffer, true);
        if (ret < 0)
        {
            fprintf(stderr, "ERROR: ft80x_dl_start failed: %d\n", ret);
            return ret;
        }
        /* Clear the display */

        setup_cmds.clearrgb.cmd = FT80X_CLEAR_COLOR_RGB(0, 0, 0);
        setup_cmds.clear.cmd = FT80X_CLEAR(1, 1, 1);
        setup_cmds.colorrgb.cmd = FT80X_COLOR_RGB(0xff, 0xff, 0xff);

        ret = ft80x_dl_data(fd, buffer, &setup_cmds, sizeof(setup_cmds));
        if (ret < 0)
        {
            fprintf(stderr, "ERROR: ft80x_dl_data failed: %d\n", ret);
            return ret;
        }

        /* Use the CMD_TEXT co-processor command to show the name of the next
         * example at the center of the display.
         */
        bitmap_cmds.begin.cmd = FT80X_BEGIN(FT80X_PRIM_BITMAPS);
        bitmap_cmds.savecontext.cmd = FT80X_SAVE_CONTEXT();
        bitmap_cmds.bitmapsource.cmd = FT80X_BITMAP_SOURCE(SCALE_COLOR_SIZE);
        bitmap_cmds.bitmaplayout.cmd = FT80X_BITMAP_LAYOUT(FT80X_FORMAT_RGB565, g_rgb_image->stride, g_rgb_image->height);
        bitmap_cmds.bitmaptransformA.cmd = FT80X_BITMAP_TRANSFORM_A(256 / 4);
        bitmap_cmds.bitmaptransformE.cmd = FT80X_BITMAP_TRANSFORM_E(256 / 4);
        bitmap_cmds.bitmapsize.cmd = FT80X_BITMAP_SIZE(FT80X_FILTER_BILINEAR, FT80X_WRAP_BORDER, FT80X_WRAP_BORDER, g_rgb_image->width * 4, g_rgb_image->height * 4);
        bitmap_cmds.vertex2f.cmd = FT80X_VERTEX2F(0, 0);
        bitmap_cmds.restorecontext.cmd = FT80X_RESTORE_CONTEXT();
        bitmap_cmds.end.cmd = FT80X_END();

        ret = ft80x_dl_data(fd, buffer, &bitmap_cmds, sizeof(bitmap_cmds));
        if (ret < 0)
        {
            fprintf(stderr, "ERROR: ft80x_dl_data failed: %d\n", ret);
            return ret;
        }

        int scale_posx = (FT80X_DISPLAY_WIDTH - SCALE_COLOR_WIDTH - SCALE_COLOR_DISPLAY_OFFSET);
        int scale_posy = (FT80X_DISPLAY_HEIGHT - SCALE_COLOR_HEIGHT) / 2;

        scale_cmds.begin.cmd = FT80X_BEGIN(FT80X_PRIM_BITMAPS);
        scale_cmds.bitmapsource.cmd = FT80X_BITMAP_SOURCE(0);
        scale_cmds.bitmaplayout.cmd = FT80X_BITMAP_LAYOUT(FT80X_FORMAT_RGB565, SCALE_COLOR_STRIDE, SCALE_COLOR_HEIGHT);
        scale_cmds.bitmapsize.cmd = FT80X_BITMAP_SIZE(FT80X_FILTER_NEAREST, FT80X_WRAP_BORDER, FT80X_WRAP_BORDER, SCALE_COLOR_WIDTH, SCALE_COLOR_HEIGHT);
        scale_cmds.vertex2f.cmd = FT80X_VERTEX2F(16 * scale_posx, 16 * scale_posy);
        scale_cmds.end.cmd = FT80X_END();

        scale_cmds.max.cmd = FT80X_CMD_NUMBER;
        scale_cmds.max.x = scale_posx;
        scale_cmds.max.y = scale_posy - 30;
        scale_cmds.max.font = 23;
        scale_cmds.max.options = 0;
        scale_cmds.max.n = g_temp_image->max * LEPTON_RAD_TEMPERATURE_SCALE_FACTOR;
#ifdef CONFIG_EXAMPLES_LEPTON_DEGREES_UNIT
        scale_cmds.max.n -= KELVIN_OFFSET;
#endif
        scale_cmds.min.cmd = FT80X_CMD_NUMBER;
        scale_cmds.min.x = scale_posx;
        scale_cmds.min.y = scale_posy + SCALE_COLOR_HEIGHT + 10;
        scale_cmds.min.font = 23;
        scale_cmds.min.options = 0;
        scale_cmds.min.n = g_temp_image->min * LEPTON_RAD_TEMPERATURE_SCALE_FACTOR;
#ifdef CONFIG_EXAMPLES_LEPTON_DEGREES_UNIT
        scale_cmds.min.n -= KELVIN_OFFSET;
#endif

        ret = ft80x_dl_data(fd, buffer, &scale_cmds, sizeof(scale_cmds));
        if (ret < 0)
        {
            fprintf(stderr, "ERROR: ft80x_dl_data failed: %d\n", ret);
            return ret;
        }
        int32_t line_center = LEPTON_FRAME_WIDTH / 2;
        int32_t middle_line = LEPTON_FRAME_HEIGHT / 2;
        int32_t middle_pixel = middle_line * LEPTON_FRAME_WIDTH + line_center;
        uint16_t center_tempK = get_temp(middle_pixel);

        center_cmds.value.cmd = FT80X_CMD_NUMBER;
        center_cmds.value.x = 75;
        center_cmds.value.y = FT80X_DISPLAY_HEIGHT - 30;
        center_cmds.value.font = 23;
        center_cmds.value.options = 0;
        center_cmds.value.n = center_tempK * LEPTON_RAD_TEMPERATURE_SCALE_FACTOR;
#ifdef CONFIG_EXAMPLES_LEPTON_DEGREES_UNIT
        center_cmds.value.n -= KELVIN_OFFSET;
#endif

        center_cmds.begin.cmd = FT80X_BEGIN(FT80X_PRIM_LINES);
        center_cmds.linewidth.cmd = FT80X_LINE_WIDTH(15);
        center_cmds.north.cmd = FT80X_VERTEX2F(16 * (FT80X_DISPLAY_WIDTH / 2), 16 * (FT80X_DISPLAY_HEIGHT / 2 - CENTER_CROSS_SIZE));
        center_cmds.south.cmd = FT80X_VERTEX2F(16 * (FT80X_DISPLAY_WIDTH / 2), 16 * (FT80X_DISPLAY_HEIGHT / 2 + CENTER_CROSS_SIZE));
        center_cmds.west.cmd = FT80X_VERTEX2F(16 * (FT80X_DISPLAY_WIDTH / 2 - CENTER_CROSS_SIZE), 16 * (FT80X_DISPLAY_HEIGHT / 2));
        center_cmds.east.cmd = FT80X_VERTEX2F(16 * (FT80X_DISPLAY_WIDTH / 2 + CENTER_CROSS_SIZE), 16 * (FT80X_DISPLAY_HEIGHT / 2));

        center_cmds.text.cmd = FT80X_CMD_TEXT;
        center_cmds.text.x = 5;
        center_cmds.text.y = FT80X_DISPLAY_HEIGHT - 30;
        center_cmds.text.font = 23;
        center_cmds.text.options = 0;

        ret = ft80x_dl_data(fd, buffer, &center_cmds, sizeof(center_cmds));
        if (ret < 0)
        {
            fprintf(stderr, "ERROR: ft80x_dl_data failed: %d\n", ret);
            return ret;
        }

        ret = ft80x_dl_string(fd, buffer, "Center : ");
        if (ret < 0)
        {
            fprintf(stderr, "ERROR: ft80x_dl_string failed: %d\n", ret);
            return ret;
        }
        unit_cmds.text.cmd = FT80X_CMD_TEXT;
        unit_cmds.text.x = 110;
        unit_cmds.text.y = FT80X_DISPLAY_HEIGHT - 30;
        unit_cmds.text.font = 23;
        unit_cmds.text.options = 0;

        ret = ft80x_dl_data(fd, buffer, &unit_cmds, sizeof(unit_cmds));
        if (ret < 0)
        {
            fprintf(stderr, "ERROR: ft80x_dl_data failed: %d\n", ret);
            return ret;
        }
#ifdef CONFIG_EXAMPLES_LEPTON_DEGREES_UNIT
        ret = ft80x_dl_string(fd, buffer, "C");
#else
        ret = ft80x_dl_string(fd, buffer, "K");
#endif
        if (ret < 0)
        {
            fprintf(stderr, "ERROR: ft80x_dl_string failed: %d\n", ret);
            return ret;
        }

        /* And terminate the display list */

        ret = ft80x_dl_end(fd, buffer);
        if (ret < 0)
        {
            fprintf(stderr, "ERROR: ft80x_dl_end failed: %d\n", ret);
            return ret;
        }
        ft80x_backlight_fade(fd, 10, 10);
        usleep(10);
    }
    free(buffer);
    close(fd);
    return ret;
}

#endif  /*CONFIG_LCD_FT80X_SPI*/