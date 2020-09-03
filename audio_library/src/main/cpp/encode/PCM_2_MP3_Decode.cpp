//
// Created by 阳坤 on 2020-05-06.
//


#include "PCM_2_MP3_Decode.h"

#define DEBUG_WRITE 0

PCM_2_MP3_Decode::PCM_2_MP3_Decode(Native2JavaCallback *native2JavaCallback) {
    this->mNative2JavaCallback = native2JavaCallback;

}

PCM_2_MP3_Decode::~PCM_2_MP3_Decode() {

}

/**
 * 初始化
 * @param mp3SavePath
 * @param sampleRate
 * @param channels
 * @param bit
 */
int PCM_2_MP3_Decode::init(const char *mp3SavePath, int sampleRate, int channels, uint64_t bitRate) {
    this->mp3SavePath = new char[strlen(mp3SavePath) + 1];
    if (DEBUG_WRITE)
        this->mp3File = fopen("sdcard/audioplay/mp3/test2.pcm", "wb");
    //初始化
    lameClient = lame_init();
    //设置采样率
    lame_set_in_samplerate(lameClient, sampleRate);
    lame_set_out_samplerate(lameClient, sampleRate);

    //设置通道
    lame_set_num_channels(lameClient, channels);

    //设置码率
    lame_set_brate(lameClient, bitRate);
    //第二个参数是质量设置，0~9 0质量最好
    lame_set_quality(lameClient, 2);

    //初始化参数
    lame_init_params(lameClient);

    return 1;


}

/**
 * 开始将 PCM 编码为 MP3
 * @param pcm
 * @param mp3SavePath
 * @param sampleRate
 * @param channels
 * @param bit
 */
int PCM_2_MP3_Decode::encode(uint8_t *pcm, jbyte *outmp3, int bufferSize) {
    if (!pcm || bufferSize <= 0)return -1;
    int ret = 0;
    if (lame_get_num_channels(lameClient) == 2) {
        ret = lame_encode_buffer_interleaved(lameClient, reinterpret_cast<short *>(pcm), bufferSize / 4,
                                             (u_char *) outmp3,
                                             (int) (bufferSize * 1.25) + 7500);
    } else {
        ret = lame_encode_buffer(lameClient, reinterpret_cast<const short *>(pcm), NULL, bufferSize / 2,
                                 (u_char *) outmp3,
                                 (int) (bufferSize * 1.25) + 7500);
    }

    if (DEBUG_WRITE)
        fwrite(outmp3, ret, 1, mp3File);

    return ret;
}

/**
 * 编码结束
 */
void PCM_2_MP3_Decode::destory() {
    if (mp3File) {
        fclose(mp3File);
        mp3File = NULL;
    }
    if (lameClient) {
        lame_close(lameClient);
    }

    if (mp3SavePath) {
        free(mp3SavePath);
        mp3SavePath = NULL;
    }

}


