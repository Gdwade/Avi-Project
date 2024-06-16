#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Las_Funcs.h"
// TODO: Split this off into header files properly so that the entire thing isnt a shit show.

// Functions:
char *add_null_char(char *first_letter, int length);
void print_header(union header_template *header);
point_bin *generate_bins(int size_pixels_x, int size_pixels_y, union header_template *header);
point_bin *fill_bins(point_bin *bins, FILE *file, union header_template *header, int image_size_x, int image_size_y);
int generate_bmp(int x, int y, point_bin *bins);

#define RESOLUTION 2835
#define NO_COMPRESION 0
#define HEADER_SIZE 40

int main(int argc, char const *argv[])
{

    printf("\n### Welcome to the totally not janky .Las to bitmap converter ###\nPlease enter .las file name (max 1000 chars):");

    // Get File Name:
    char file_name[1000]; // There is definitly a better way to do this.
    // this feels horribly wasteful, there has to be a better way to do this.
    //  I guess I could malloc an emense amount of space, the realloc down to the propper size. still seems not great.
    scanf("%s", file_name);

    // Load File:
    union header_template file_header;
    FILE *LAS_file = fopen(file_name, "rb");
    if (LAS_file == NULL)
    {
        printf("\n*** Failed to read file: Check File Name *** \n");
        return -1;
    }
    fread(file_header.bytes, 1, 227, LAS_file);
    print_header(&file_header);

    // Load Variable length records:
    union variable_length_record_header_template vlrs[file_header.number_variable_length_records];
    for (int i = 0; i < file_header.number_variable_length_records; i++)
    {
        // printf("getting vlr %d\n", i);
        union variable_length_record_header_template temp_vlr;

        fread(temp_vlr.bytes, 1, 54, LAS_file); // load data into header
        unsigned short length_after_header = temp_vlr.record_length_after_header;
        char *vlr_data = malloc(length_after_header); // malloc space for data

        // printf("%d\n", length_after_header);
        char temp_data_array[length_after_header];         //
        fread(vlr_data, 1, length_after_header, LAS_file); // read in data

        temp_vlr.data = vlr_data; // put data in the struct
        vlrs[i] = temp_vlr;       // add the struct to the array
    };

    // Generate Bins:
    int image_size_x;
    int image_size_y;
    printf("Enter desired image width in pixels: ");
    scanf("%d", &image_size_x);

    printf("Enter desired image height in pixels: ");
    scanf("%d", &image_size_y);
    printf("Generating a %d by %d pixel image...\n", image_size_x, image_size_y);

    point_bin *bins = generate_bins(image_size_x, image_size_y, &file_header);
    // bin generation works!!!

    bins = fill_bins(bins, LAS_file, &file_header, image_size_x, image_size_y);

    int sum = 0;
    // Checks to see if all points are in a bin:
    for (int i = 0; i <= image_size_x * image_size_y; i++)
    {
        sum += bins[i].size;
    }
    if (sum == file_header.legacy_number_of_point_records)
    {
        printf("Bins filled!\n");
    }
    else
    {
        printf("Bin filling FAILED:\n%d points without bins", file_header.legacy_number_of_point_records - sum);
    }

    int generatei_bmp_flag = generate_bmp(image_size_x, image_size_y, bins);
    if (generatei_bmp_flag == 0)
    {
        printf("Image Generation Complete");
    }
    else
    {
        printf("Image Generation failed");
    }

    return 0;
}

int generate_bmp(int image_size_x, int image_size_y, point_bin *bins)
{
    // This aproach isnt great because it requires the creation of data set before writting it. This is reduntant as we could technically do it pixel by pixel when the average is taken.
    //  Investigate if its slow.

    // Data is coming in as point_bins!!!

    int amount_of_padding = (image_size_x % 4 == 0) ? 0 : image_size_x;
    unsigned int number_data_bytes = (3 * image_size_x * image_size_y) + image_size_y * amount_of_padding;

    unsigned int file_size = HEADER_SIZE + 14 + number_data_bytes;
    unsigned int reserved = 0x0000;
    unsigned int data_offset = HEADER_SIZE + 14;
    unsigned int header_size = HEADER_SIZE;
    unsigned int width = image_size_x;
    unsigned int height = image_size_y;

    unsigned short number_of_planes = 1;
    unsigned short bits_per_pixel = 24;

    unsigned int compression = NO_COMPRESION;
    unsigned int image_size = number_data_bytes;
    unsigned int horz_resolution = RESOLUTION;
    unsigned int vert_resolution = RESOLUTION;
    unsigned int colors_used = 0;
    unsigned int important_colors = 0;

    unsigned int header_part_one[6] = {file_size, reserved, data_offset, header_size, width, height};
    unsigned short header_part_two[2] = {number_of_planes, bits_per_pixel};
    unsigned int header_part_three[6] = {compression, image_size, horz_resolution, vert_resolution, colors_used, important_colors};

    char tag[2] = {'B', 'M'};
    FILE *file_ptr;
    file_ptr = fopen("BMP_Test_File.bmp", "wb");
    if (file_ptr == NULL)
    {
        return 1;
    }
    fwrite(tag, sizeof(char), 2, file_ptr);
    fwrite(header_part_one, 4, 6, file_ptr);
    fwrite(header_part_two, 2, 2, file_ptr);
    fwrite(header_part_three, 4, 6, file_ptr);
    char padding_array[] = {0, 0, 0, 0};
    // B,G,R format
    for (int i = 0; i < image_size_x * image_size_y * 3; i++)
    {
        // data[] does not exist yet. Have to take the point bins and work from there.
        // putc(data[i],file_ptr);

        if ((i + 1) % (image_size_x * 3) == 0 && i != 0)
        {
            fwrite(padding_array, 1, amount_of_padding, file_ptr);
        }
    }
    return 0;
}
