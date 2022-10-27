#ifndef WAVEFORMATENCODING_H
#define WAVEFORMATENCODING_H

enum WaveFormatEncoding {
    Unknown = 0,
    Pcm = 1,
    Adpcm = 2,
    IeeeFloat = 3,
    Vselp = 4,
    IbmCvsd = 5,
    ALaw = 6,
    MuLaw = 7,
    Dts = 8,
    Drm = 9,
    WmaVoice9 = 10,                             // 0x000A
    OkiAdpcm = 16,                              // 0x0010
    DviAdpcm = 17,                              // 0x0011
    ImaAdpcm = 17,                              // 0x0011
    MediaspaceAdpcm = 18,                       // 0x0012
    SierraAdpcm = 19,                           // 0x0013
    G723Adpcm = 20,                             // 0x0014
    DigiStd = 21,                               // 0x0015
    DigiFix = 22,                               // 0x0016
    DialogicOkiAdpcm = 23,                      // 0x0017
    MediaVisionAdpcm = 24,                      // 0x0018
    CUCodec = 25,                               // 0x0019
    YamahaAdpcm = 32,                           // 0x0020
    SonarC = 33,                                // 0x0021
    DspGroupTrueSpeech = 34,                    // 0x0022
    EchoSpeechCorporation1 = 35,                // 0x0023
    AudioFileAf36 = 36,                         // 0x0024
    Aptx = 37,                                  // 0x0025
    AudioFileAf10 = 38,                         // 0x0026
    Prosody1612 = 39,                           // 0x0027
    Lrc = 40,                                   // 0x0028
    DolbyAc2 = 48,                              // 0x0030
    Gsm610 = 49,                                // 0x0031
    MsnAudio = 50,                              // 0x0032
    AntexAdpcme = 51,                           // 0x0033
    ControlResVqlpc = 52,                       // 0x0034
    DigiReal = 53,                              // 0x0035
    DigiAdpcm = 54,                             // 0x0036
    ControlResCr10 = 55,                        // 0x0037
    WAVE_FORMAT_NMS_VBXADPCM = 56,              // 0x0038
    WAVE_FORMAT_CS_IMAADPCM = 57,               // 0x0039
    WAVE_FORMAT_ECHOSC3 = 58,                   // 0x003A
    WAVE_FORMAT_ROCKWELL_ADPCM = 59,            // 0x003B
    WAVE_FORMAT_ROCKWELL_DIGITALK = 60,         // 0x003C
    WAVE_FORMAT_XEBEC = 61,                     // 0x003D
    WAVE_FORMAT_G721_ADPCM = 64,                // 0x0040
    WAVE_FORMAT_G728_CELP = 65,                 // 0x0041
    WAVE_FORMAT_MSG723 = 66,                    // 0x0042
    Mpeg = 80,                                  // 0x0050
    WAVE_FORMAT_RT24 = 82,                      // 0x0052
    WAVE_FORMAT_PAC = 83,                       // 0x0053
    MpegLayer3 = 85,                            // 0x0055
    WAVE_FORMAT_LUCENT_G723 = 89,               // 0x0059
    WAVE_FORMAT_CIRRUS = 96,                    // 0x0060
    WAVE_FORMAT_ESPCM = 97,                     // 0x0061
    WAVE_FORMAT_VOXWARE = 98,                   // 0x0062
    WAVE_FORMAT_CANOPUS_ATRAC = 99,             // 0x0063
    WAVE_FORMAT_G726_ADPCM = 100,               // 0x0064
    WAVE_FORMAT_G722_ADPCM = 101,               // 0x0065
    WAVE_FORMAT_DSAT_DISPLAY = 103,             // 0x0067
    WAVE_FORMAT_VOXWARE_BYTE_ALIGNED = 105,     // 0x0069
    WAVE_FORMAT_VOXWARE_AC8 = 112,              // 0x0070
    WAVE_FORMAT_VOXWARE_AC10 = 113,             // 0x0071
    WAVE_FORMAT_VOXWARE_AC16 = 114,             // 0x0072
    WAVE_FORMAT_VOXWARE_AC20 = 115,             // 0x0073
    WAVE_FORMAT_VOXWARE_RT24 = 116,             // 0x0074
    WAVE_FORMAT_VOXWARE_RT29 = 117,             // 0x0075
    WAVE_FORMAT_VOXWARE_RT29HW = 118,           // 0x0076
    WAVE_FORMAT_VOXWARE_VR12 = 119,             // 0x0077
    WAVE_FORMAT_VOXWARE_VR18 = 120,             // 0x0078
    WAVE_FORMAT_VOXWARE_TQ40 = 121,             // 0x0079
    WAVE_FORMAT_SOFTSOUND = 128,                // 0x0080
    WAVE_FORMAT_VOXWARE_TQ60 = 129,             // 0x0081
    WAVE_FORMAT_MSRT24 = 130,                   // 0x0082
    WAVE_FORMAT_G729A = 131,                    // 0x0083
    WAVE_FORMAT_MVI_MVI2 = 132,                 // 0x0084
    WAVE_FORMAT_DF_G726 = 133,                  // 0x0085
    WAVE_FORMAT_DF_GSM610 = 134,                // 0x0086
    WAVE_FORMAT_ISIAUDIO = 136,                 // 0x0088
    WAVE_FORMAT_ONLIVE = 137,                   // 0x0089
    WAVE_FORMAT_SBC24 = 145,                    // 0x0091
    WAVE_FORMAT_DOLBY_AC3_SPDIF = 146,          // 0x0092
    WAVE_FORMAT_MEDIASONIC_G723 = 147,          // 0x0093
    WAVE_FORMAT_PROSODY_8KBPS = 148,            // 0x0094
    WAVE_FORMAT_ZYXEL_ADPCM = 151,              // 0x0097
    WAVE_FORMAT_PHILIPS_LPCBB = 152,            // 0x0098
    WAVE_FORMAT_PACKED = 153,                   // 0x0099
    WAVE_FORMAT_MALDEN_PHONYTALK = 160,         // 0x00A0
    Gsm = 161,                                  // 0x00A1
    G729 = 162,                                 // 0x00A2
    G723 = 163,                                 // 0x00A3
    Acelp = 164,                                // 0x00A4
    RawAac = 255,                               // 0x00FF
    WAVE_FORMAT_RHETOREX_ADPCM = 256,           // 0x0100
    WAVE_FORMAT_IRAT = 257,                     // 0x0101
    WAVE_FORMAT_VIVO_G723 = 273,                // 0x0111
    WAVE_FORMAT_VIVO_SIREN = 274,               // 0x0112
    WAVE_FORMAT_DIGITAL_G723 = 291,             // 0x0123
    WAVE_FORMAT_SANYO_LD_ADPCM = 293,           // 0x0125
    WAVE_FORMAT_SIPROLAB_ACEPLNET = 304,        // 0x0130
    WAVE_FORMAT_SIPROLAB_ACELP4800 = 305,       // 0x0131
    WAVE_FORMAT_SIPROLAB_ACELP8V3 = 306,        // 0x0132
    WAVE_FORMAT_SIPROLAB_G729 = 307,            // 0x0133
    WAVE_FORMAT_SIPROLAB_G729A = 308,           // 0x0134
    WAVE_FORMAT_SIPROLAB_KELVIN = 309,          // 0x0135
    WAVE_FORMAT_G726ADPCM = 320,                // 0x0140
    WAVE_FORMAT_QUALCOMM_PUREVOICE = 336,       // 0x0150
    WAVE_FORMAT_QUALCOMM_HALFRATE = 337,        // 0x0151
    WAVE_FORMAT_TUBGSM = 341,                   // 0x0155
    WAVE_FORMAT_MSAUDIO1 = 352,                 // 0x0160
    WindowsMediaAudio = 353,                    // 0x0161
    WindowsMediaAudioProfessional = 354,        // 0x0162
    WindowsMediaAudioLosseless = 355,           // 0x0163
    WindowsMediaAudioSpdif = 356,               // 0x0164
    WAVE_FORMAT_UNISYS_NAP_ADPCM = 368,         // 0x0170
    WAVE_FORMAT_UNISYS_NAP_ULAW = 369,          // 0x0171
    WAVE_FORMAT_UNISYS_NAP_ALAW = 370,          // 0x0172
    WAVE_FORMAT_UNISYS_NAP_16K = 371,           // 0x0173
    WAVE_FORMAT_CREATIVE_ADPCM = 512,           // 0x0200
    WAVE_FORMAT_CREATIVE_FASTSPEECH8 = 514,     // 0x0202
    WAVE_FORMAT_CREATIVE_FASTSPEECH10 = 515,    // 0x0203
    WAVE_FORMAT_UHER_ADPCM = 528,               // 0x0210
    WAVE_FORMAT_QUARTERDECK = 544,              // 0x0220
    WAVE_FORMAT_ILINK_VC = 560,                 // 0x0230
    WAVE_FORMAT_RAW_SPORT = 576,                // 0x0240
    WAVE_FORMAT_ESST_AC3 = 577,                 // 0x0241
    WAVE_FORMAT_IPI_HSX = 592,                  // 0x0250
    WAVE_FORMAT_IPI_RPELP = 593,                // 0x0251
    WAVE_FORMAT_CS2 = 608,                      // 0x0260
    WAVE_FORMAT_SONY_SCX = 624,                 // 0x0270
    WAVE_FORMAT_FM_TOWNS_SND = 768,             // 0x0300
    WAVE_FORMAT_BTV_DIGITAL = 1024,             // 0x0400
    WAVE_FORMAT_QDESIGN_MUSIC = 1104,           // 0x0450
    WAVE_FORMAT_VME_VMPCM = 1664,               // 0x0680
    WAVE_FORMAT_TPC = 1665,                     // 0x0681
    WAVE_FORMAT_OLIGSM = 4096,                  // 0x1000
    WAVE_FORMAT_OLIADPCM = 4097,                // 0x1001
    WAVE_FORMAT_OLICELP = 4098,                 // 0x1002
    WAVE_FORMAT_OLISBC = 4099,                  // 0x1003
    WAVE_FORMAT_OLIOPR = 4100,                  // 0x1004
    WAVE_FORMAT_LH_CODEC = 4352,                // 0x1100
    WAVE_FORMAT_NORRIS = 5120,                  // 0x1400
    WAVE_FORMAT_SOUNDSPACE_MUSICOMPRESS = 5376, // 0x1500
    MPEG_ADTS_AAC = 5632,                       // 0x1600
    MPEG_RAW_AAC = 5633,                        // 0x1601
    MPEG_LOAS = 5634,                           // 0x1602
    NOKIA_MPEG_ADTS_AAC = 5640,                 // 0x1608
    NOKIA_MPEG_RAW_AAC = 5641,                  // 0x1609
    VODAFONE_MPEG_ADTS_AAC = 5642,              // 0x160A
    VODAFONE_MPEG_RAW_AAC = 5643,               // 0x160B
    MPEG_HEAAC = 5648,                          // 0x1610
    WAVE_FORMAT_DVM = 8192,                     // 0x2000
    Vorbis1 = 26447,                            // 0x674F
    Vorbis2 = 26448,                            // 0x6750
    Vorbis3 = 26449,                            // 0x6751
    Vorbis1P = 26479,                           // 0x676F
    Vorbis2P = 26480,                           // 0x6770
    Vorbis3P = 26481,                           // 0x6771
    Extensible = 65534,                         // 0xFFFE
    WAVE_FORMAT_DEVELOPMENT = 65535,            // 0xFFFF
};

#endif // WAVEFORMATENCODING_H
