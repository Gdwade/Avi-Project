#include <stdint.h>

typedef struct{
    double bin_value_x;
    double bin_value_y;
    int size;
    double *point_array;
    //POINT ARRAY ONLY HOLDS Z points
}point_bin;



union header_template{

    char bytes[227];

    //Needs to have a packed attribute because the file format has no padding
    struct __attribute__ ((__packed__)){
        char file_signature[4];
        unsigned short file_source_id;
        unsigned short global_encoding;
        uint32_t GUID_data_1;
        unsigned short GUID_data_2;
        unsigned short GUID_data_3;
        unsigned char GUID_data_4[8];
        unsigned char version_major;
        unsigned char version_minor;
        char system_identifier[32];
        char generating_software[32];
        unsigned short file_creation_day;
        unsigned short file_creation_year;
        unsigned short header_size;
        uint32_t offset_to_point_data;
        uint32_t number_variable_length_records;
        unsigned char point_data_record_format;
        unsigned short point_data_record_length;
        uint32_t legacy_number_of_point_records; //TRY WITH ME FIRST, normal num points seems messed up
        uint32_t legacy_number_of_points_by_return[5];
        double x_scale_factor;
        double y_scale_factor;
        double z_scale_factor;
        double x_offset;
        double y_offset;
        double z_offset;
        double max_x;
        double min_x;
        double max_y;
        double min_y;
        double max_z;
        double min_z;
        uint64_t start_of_waveform_data_packet_record;
        uint64_t start_of_First_extended_variable_length_record;
        uint32_t number_of_extended_variable_length_records;
        uint64_t number_of_point_records;
        uint64_t number_of_points_by_return;
    };
};

union variable_length_record_header_template{
    char bytes[62];//54+8(For the char*)
    struct __attribute__ ((__packed__)){
        unsigned short reserved;
        char user_id[16];
        unsigned short record_id;
        unsigned short record_length_after_header;
        char description[32];
        char* data;
    };
    
};

typedef union data_point_format_0{
        char bytes[20];
        struct {
            uint32_t x;
            uint32_t y;
            uint32_t z;
            unsigned short intensity;
            char return_info; 
            // This includes: Return number (bits 0-2), number of returns per pulse (3-5 bits)
            // Scan direction flag (bit 6), Edge of flight line (bit 7)
            unsigned char classification_0;
            signed char scan_angle_rank_0; // value ranges from -90 to +90
            unsigned char user_data_0;
            unsigned short point_source_id_0;
        }__attribute__ ((__packed__));
    }data_point_format_0;

typedef union data_point_format_1{
        char bytes[28];
        struct{
            uint32_t x;
            uint32_t y;
            uint32_t z;
            unsigned short intensity;
            char return_info; 
            // This includes: Return number (bits 0-2), number of returns per pulse (3-5 bits)
            // Scan direction flag (bit 6), Edge of flight line (bit 7)
            unsigned char classification;
            signed char scan_angle_rank; // value ranges from -90 to +90
            unsigned char user_data;
            unsigned short point_source_id;
            double gps_time;
        }__attribute__ ((__packed__));
        
    }data_point_format_1;

typedef union data_point_format_2{
        char bytes[26];
        struct{
            uint32_t x;
            uint32_t y;
            uint32_t z;
            unsigned short intensity;
            char return_info; 
            // This includes: Return number (bits 0-2), number of returns per pulse (3-5 bits)
            // Scan direction flag (bit 6), Edge of flight line (bit 7)
            unsigned char classification;
            signed char scan_angle_rank; // value ranges from -90 to +90
            unsigned char user_data;
            unsigned short point_source_id;
            unsigned short red;
            unsigned short green;
            unsigned short blue;
        }__attribute__ ((__packed__));
    }data_point_format_2;

typedef union data_point_format_3{
        char bytes[34];
        struct{
            uint32_t x;
            uint32_t y;
            uint32_t z;
            unsigned short intensity;
            char return_info; 
            // This includes: Return number (bits 0-2), number of returns per pulse (3-5 bits)
            // Scan direction flag (bit 6), Edge of flight line (bit 7)
            unsigned char classification;
            signed char scan_angle_rank; // value ranges from -90 to +90
            unsigned char user_data;
            unsigned short point_source_id;
            double gps_time;
            unsigned short red;
            unsigned short green;
            unsigned short blue;
        }__attribute__ ((__packed__));
    }data_point_format_3;

typedef union data_point_format_4{
        char bytes[57];
        struct{
            uint32_t x;
            uint32_t y;
            uint32_t z;
            unsigned short intensity;
            char return_info; 
            // This includes: Return number (bits 0-2), number of returns per pulse (3-5 bits)
            // Scan direction flag (bit 6), Edge of flight line (bit 7)
            unsigned char classification;
            signed char scan_angle_rank; // value ranges from -90 to +90
            unsigned char user_data;
            unsigned short point_source_id;
            double gps_time;
            unsigned char wave_packet_descriptor_index;
            uint64_t byte_offset_to_waveform_data;
            uint16_t waveform_packet_size; //in bytes
            float return_point_waveform_location;
            float parametric_dx;
            float parametric_dy;
            float parametric_dz;
        }__attribute__ ((__packed__));
    }data_point_format_4;

typedef union data_point_format_5{
        char bytes[63];
        struct{
            uint32_t x;
            uint32_t y;
            uint32_t z;
            unsigned short intensity;
            char return_info; 
            // This includes: Return number (bits 0-2), number of returns per pulse (3-5 bits)
            // Scan direction flag (bit 6), Edge of flight line (bit 7)
            unsigned char classification;
            signed char scan_angle_rank; // value ranges from -90 to +90
            unsigned char user_data;
            unsigned short point_source_id;
            double gps_time;
            unsigned short red;
            unsigned short green;
            unsigned short blue;
            unsigned char wave_packet_descriptor_index;
            uint64_t byte_offset_to_waveform_data;
            uint16_t waveform_packet_size; //in bytes
            float return_point_waveform_location;
            float parametric_dx;
            float parametric_dy;
            float parametric_dz;
        }__attribute__ ((__packed__));
    }data_point_format_5;

typedef union data_point_format_6{
        char bytes[30];
        struct{
            uint32_t x;
            uint32_t y;
            uint32_t z;
            unsigned short intensity;
            char return_info;
            //CONTAINS: Return number (bits 0-3) and number of returns per pulse (bits 4-7)

            char scan_info; 
            //CONTAINS: Classification flags (bits 0-3), Scanner channel (bits 4-5),
            //Scan dirrection flag (bit 6), edge of flight line (nit 7)

            unsigned char classification;
            unsigned char user_data;
            unsigned short scan_angle;
            unsigned short point_source_id;
            double gps_time;
        }__attribute__ ((__packed__));
    }data_point_format_6;

typedef union data_point_format_7{
        char bytes[36];
        struct{
            uint32_t x;
            uint32_t y;
            uint32_t z;
            unsigned short intensity;
            char return_info;
            //CONTAINS: Return number (bits 0-3) and number of returns per pulse (bits 4-7)

            char scan_info; 
            //CONTAINS: Classification flags (bits 0-3), Scanner channel (bits 4-5),
            //Scan dirrection flag (bit 6), edge of flight line (nit 7)

            unsigned char classification;
            unsigned char user_data;
            unsigned short scan_angle;
            unsigned short point_source_id;
            double gps_time;
            unsigned short red;
            unsigned short green;
            unsigned short blue;
        }__attribute__ ((__packed__));
    }data_point_format_7;

typedef union data_point_format_8{
        char bytes[38];
        struct{
            uint32_t x;
            uint32_t y;
            uint32_t z;
            unsigned short intensity;
           char return_info;
           //CONTAINS: Return number (bits 0-3) and number of returns per pulse (bits 4-7)

           char scan_info; 
           //CONTAINS: Classification flags (bits 0-3), Scanner channel (bits 4-5),
        //Scan dirrection flag (bit 6), edge of flight line (nit 7)

           unsigned char classification;
           unsigned char user_data;
           unsigned short scan_angle;
           unsigned short point_source_id;
           double gps_time;
           unsigned short red;
           unsigned short green;
           unsigned short blue;
           unsigned short nir;
        }__attribute__ ((__packed__));
    }data_point_format_8;

typedef union data_point_format_9{
        char bytes[59];
        struct{
            uint32_t x;
            uint32_t y;
            uint32_t z;
            unsigned short intensity;
            char return_info;
            //CONTAINS: Return number (bits 0-3) and number of returns per pulse (bits 4-7)

            char scan_info; 
            //CONTAINS: Classification flags (bits 0-3), Scanner channel (bits 4-5),
            //Scan dirrection flag (bit 6), edge of flight line (nit 7)

            unsigned char classification;
            unsigned char user_data;
            unsigned short scan_angle;
            unsigned short point_source_id;
            double gps_time;
            unsigned char wave_packet_descriptor_index;
            uint64_t byte_offset_to_waveform_data;
            uint16_t waveform_packet_size; //in bytes
            float return_point_waveform_location;
            float parametric_dx;
            float parametric_dy;
            float parametric_dz; 
        }__attribute__ ((__packed__));
    }data_point_format_9;
 

typedef union data_point_format_10{
        char bytes[67];
        struct
        {
            uint32_t x;
            uint32_t y;
            uint32_t z;
            unsigned short intensity;
            char return_info;
            //CONTAINS: Return number (bits 0-3) and number of returns per pulse (bits 4-7)

            char scan_info; 
            //CONTAINS: Classification flags (bits 0-3), Scanner channel (bits 4-5),
            //Scan dirrection flag (bit 6), edge of flight line (nit 7)

            unsigned char classification;
            unsigned char user_data;
            unsigned short scan_angle;
            unsigned short point_source_id;
            double gps_time;
            unsigned short red;
            unsigned short green;
            unsigned short blue;
            unsigned short nir;
            unsigned char wave_packet_descriptor_index;
            uint64_t byte_offset_to_waveform_data;
            uint16_t waveform_packet_size; //in bytes
            float return_point_waveform_location;
            float parametric_dx;
            float parametric_dy;
            float parametric_dz; 
        }__attribute__ ((__packed__));
    }data_point_format_10;
