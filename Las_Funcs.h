#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Las_structs.h"

char *add_null_char(char *first_letter, int length);
void print_header(union header_template *header);
point_bin *generate_bins(int size_pixels_x, int size_pixels_y, union header_template *header);
point_bin *fill_bins(point_bin *bins, FILE *file, union header_template *header, int image_size_x, int image_size_y);

point_bin *fill_bins(point_bin *bins, FILE *file, union header_template *header, int image_size_x, int image_size_y)
{
    // Returns 1 if successful, 0 if it fails somehow. IDK if thats even possible
    // write points dirrectly into bins prevents need to hold data points for so long.
    fseek(file, header->offset_to_point_data, SEEK_SET);
    // set file pointer to right spot
    double x_scale = header->x_scale_factor;
    double y_scale = header->y_scale_factor;
    double z_scale = header->z_scale_factor;

    double x_offset = header->x_offset;
    double y_offset = header->y_offset;
    double z_offset = header->z_offset;

    double step_x = (header->max_x - header->min_x) / image_size_x;
    double step_y = (header->max_y - header->min_y) / image_size_y;

    if (header->point_data_record_format == 3)
    {
        int format_length = 34;
        int last_percent = -1;
        for (int i = 0; i < header->legacy_number_of_point_records; i++)
        {
            // printing out a progress percentage:
            float i_float = i;
            int percentage = ((i_float / header->legacy_number_of_point_records) * 100) / 10;
            // divide by 10 at the end so that it only prints every +10
            if (percentage != last_percent)
            {
                printf("%d%% of points placed\n", percentage * 10);
                last_percent = percentage;
            }
            data_point_format_3 temp_point;
            // TODO: write code for more formats than just data_point_format_3
            fread(temp_point.bytes, 1, format_length, file);

            temp_point.x = (temp_point.x * x_scale) + x_offset;
            temp_point.y = (temp_point.y * y_scale) + y_offset;
            temp_point.z = (temp_point.z * z_scale) + z_offset;
            int index = 0;
            // the stupid solution is fine for now, but we need a MUCH faster version cause this takes too mich time. What ever we do is N*0(find bin). In this case, that probubly makes it 0(N^2)
            // I can probubly do something like a dictionary.

            int distance_x = (header->max_x - temp_point.x) / step_x;
            int distance_y = (header->max_y - temp_point.y) / step_y;
            index = (distance_x) + (distance_y * image_size_y);
            if (index > image_size_x * image_size_y)
            {
                if (distance_x >= image_size_x)
                {
                    index -= 1;
                }
                if (distance_y >= image_size_y)
                {
                    index -= image_size_y;
                }
            }

            point_bin *bin = &bins[index];
            bin->size += 1;
            bin->point_array = (double *)realloc(bin->point_array, 8 + 8 * bin->size);
            bin->point_array[bin->size] = temp_point.z;
        }
        return bins;
    }
    return NULL;
}

point_bin *generate_bins(int size_pixels_x, int size_pixels_y, union header_template *header)
{
    double delta_x = header->max_x - header->min_x;
    double delta_y = header->max_y - header->min_y;

    double step_size_x = delta_x / size_pixels_x;
    double step_size_y = delta_y / size_pixels_y;
    point_bin *bin_max = (point_bin *)malloc(size_pixels_x * size_pixels_y * sizeof(point_bin));
    // Create an array of bins, each bin has an x value, a y value, an array of points (z), and the arrays size.

    double current_x = header->min_x + step_size_x;
    double current_y = header->min_y + step_size_y;
    int counter = 0;
    // generates column by column
    for (int i = 0; i < size_pixels_x; i++)
    {
        point_bin temp;
        temp.bin_value_x = current_x;
        for (int i2 = 0; i2 < size_pixels_y - 1; i2++)
        {
            temp.bin_value_y = current_y;
            temp.point_array = (double *)malloc(8);
            temp.size = 0;
            bin_max[counter] = temp;

            current_y += step_size_y;
            counter++;
        }
        current_x += step_size_x;
        current_y = header->min_y;
        counter++;
    }
    // printf("%d\n", counter);
    return bin_max;
}

void print_header(union header_template *header)
{
    char *file_sig = add_null_char(header->file_signature, 4);
    printf("file signature: %s\n", file_sig); // FREE ME
    free(file_sig);

    printf("Source id: %d\n", header->file_source_id);
    printf("version major: %d\n", header->version_major);
    printf("version minor: %d\n", header->version_minor);

    char *syst_id = add_null_char(header->system_identifier, 32);
    printf("System id: %s\n", syst_id); // FREE ME
    free(syst_id);

    char *gen_software = add_null_char(header->generating_software, 32);
    printf("Generating Software: %s\n", gen_software); // FREE ME
    free(gen_software);

    printf("Creation day: %hd\n", header->file_creation_day);
    printf("Creation year: %hd\n", header->file_creation_year);
    printf("Header size: %hd\n", header->header_size);
    printf("Offset to point data: %d\n", header->offset_to_point_data);
    printf("Number variable length records: %x\n", header->number_variable_length_records);
    printf("Point data record format: %x\n", header->point_data_record_format);
    printf("Point data record length: %x\n", header->point_data_record_length);
    printf("Legacy number of point records: %d\n", header->legacy_number_of_point_records);
    printf("Legacy number of points by return is an array that Ill check later\n");
    printf("X scale factor %f\n", header->x_scale_factor);
    printf("Y scale factor %f\n", header->y_scale_factor);
    printf("Z scale factor %f\n", header->z_scale_factor);
    printf("X offset %f\n", header->x_offset);
    printf("Y offset %f\n", header->y_offset);
    printf("Z offset %f\n", header->z_offset);

    printf("Max x: %f\n", header->max_x);
    printf("Min x: %f\n", header->min_x);
    printf("DELTA X: %f\n", header->max_x - header->min_x);
    printf("Max y: %f\n", header->max_y);
    printf("Min y: %f\n", header->min_y);
    printf("DELTA Y: %f\n", header->max_y - header->min_y);

    printf("Max z: %f\n", header->max_z);
    printf("Min z: %f\n", header->min_z);
    printf("Start of waveform data packet record: %llu\n", header->start_of_waveform_data_packet_record);
    printf("Start of first extended variable length records: %llu\n", header->start_of_First_extended_variable_length_record);
    printf("Number of extended variable length records: %d\n", header->number_of_extended_variable_length_records);
    printf("Number of point records: : %llu\n", header->number_of_point_records);
    printf("Number of points by return: %llu\n", header->number_of_points_by_return);
}

char *add_null_char(char *first_letter, int length)
{
    // Function takes a pointer to the first char in a string, and its length (literal number of characters).
    // It then copies all of the chars in the string into a new chunck of memory that
    char *newWord = (char *)malloc(length);
    for (int i = 0; i < length; i++)
    {
        newWord[i] = first_letter[i];
    }
    newWord[length] = '\0';
    return newWord;
    // At some point we have to remember to free the memory
}