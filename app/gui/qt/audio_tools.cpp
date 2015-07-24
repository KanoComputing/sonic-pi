#include <string>
#include <cstring>

#include <sndfile.h>

#include "audio_tools.h"

#define BUFFER_LEN    4096


int AudioTools::convert_wav_to_ogg(std::string src_path, std::string dst_path)
{
    /**
     * Convert a wav file to ogg/vorbis using the libsndfile Library
     */

    SF_INFO sf_info;
    std::memset(&sf_info, 0, sizeof(sf_info));

    SNDFILE * src_file = sf_open(src_path.c_str(), SFM_READ, &sf_info);

    if (!src_file)
        return -1;


    /**
     * Set the new params
     */

    sf_info.format = SF_FORMAT_OGG | SF_FORMAT_VORBIS;

    if (sf_format_check(&sf_info) == 0)
        return -4;


    /**
     * Open the output file
     */

    SNDFILE * dst_file = sf_open(dst_path.c_str(), SFM_WRITE, &sf_info);

    if (!dst_file)
        return -2;


    /**
     * Copy metadata
     */

    const char * metadata_string;

    for (int metadata_key = SF_STR_FIRST; metadata_key <= SF_STR_LAST; metadata_key++) {
        metadata_string = sf_get_string(src_file, metadata_key);

        if (metadata_string)
            sf_set_string(dst_file, metadata_key, metadata_string);
    }

    sf_set_string(dst_file, SF_STR_SOFTWARE, "Sonic Pi on Kano");


    /**
     * Write file
     */

    // Get the maximum signal for normalisation
    static double max;
    sf_command(src_file, SFC_CALC_SIGNAL_MAX, &max, sizeof(max));

    // Turn off normalisation for read values
    sf_command(src_file, SFC_SET_NORM_DOUBLE, NULL, SF_FALSE);

    // NB: 'frames' are synonymous with 'samples'
    const int max_buffer_frames = BUFFER_LEN / sf_info.channels;
    int read_frame_count = max_buffer_frames;
    double data[BUFFER_LEN];

    while (read_frame_count > 0) {
        read_frame_count = sf_readf_double(src_file, data, max_buffer_frames);

        // Normalise
        for (int k = 0; k < read_frame_count * sf_info.channels; k++) {
            data[k] /= max;
        }

        sf_writef_double(dst_file, data, read_frame_count);
    }

    sf_close(src_file);
    sf_close(dst_file);

    return 0;
}
