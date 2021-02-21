#pragma once
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "framework.h"

#define SA_API extern "C"

#define PROJECT_NAME    "UDS_SecurityAccess"
#define LEVEL_EXTEND    0x01
#define LEVEL_PROGRAM   0x11
#define LEVEL_EOL       0x61

// ZLGKey 和 GenerateKeyEx 这两个函数不能同时都导出，
// 否则 ZCANPRO 中调用此 DLL 在刷新阶段会报错，
// 请在 FunctionExport.def 中选择要导出到 DLL 的函数。

// For 'ZLG ZCANPRO'
SA_API int ZLGKey(const unsigned char *iSeedArray,  // seed数组
                  unsigned short iSeedArraySize,    // seed数组大小
                  unsigned int iSecurityLevel,      // 安全级别 1,3,5...
                  const char *iVariant,             // 其他数据, 可设置为空
                  unsigned char *iKeyArray,         // key数组, 空间由用户构造
                  unsigned short *iKeyArraySize);   // key数组大小, 函数返回时为key数组实际大小

// For 'CANoe'
enum VKeyGenResultEx
{
    KGRE_Ok = 0,
    KGRE_BufferToSmall = 1,
    KGRE_SecurityLevelInvalid = 2,
    KGRE_VariantInvalid = 3,
    KGRE_UnspecifiedError = 4
};

// The client has to provide a keyArray buffer and has to transfer this buffer -
// including its size - to the GenerateKey method. The method checks, if the size is
// sufficient. The client can discover the required size by examinig the service used
// transfer the key to the ECU.
// Returns false if the key could not be generated:
//  -> keyArraySize to small
//  -> generation for specified security level not possible
//  -> variant unknown
SA_API VKeyGenResultEx GenerateKeyEx(const unsigned char *iSeedArray,
                                     unsigned short iSeedArraySize,
                                     const unsigned int iSecurityLevel,
                                     const char *iVariant,
                                     unsigned char *ioKeyArray,
                                     unsigned int iKeyArraySize,
                                     unsigned int &oSize);

// For 'ETS'
SA_API BOOL WINAPIV Seed2Key(ULONG VendorCode,
                             BYTE *Seed,
                             UINT SeedSize,
                             BYTE *Key,
                             UINT *KeySize,
                             USHORT SAAlg);


// For 'Vehecle Spy3"
/**
* Generates an unlock key based on a given seed.
* @param ucSecurityLevel The security level requested that generated this seed. Usually the subfunction to the $27 service.
* @param iSeedKeySize Size of pucSeed and pucSeed in bytes.
* @param iExtraBytesSize Size of pucExtraBytes in bytes.
* @param pucSeed Pointer to a buffer containing the seed.
* @param pucKey Pointer to a buffer where the calculated key should be written to. This is already allocated.
* @param pucExtraByes Extra bytes which can be used as a general extra parameter (for algorithms that require more than seed as input).
* @return Return true if the key calculated successfully, otherwise false.
*/
SA_API bool __stdcall ES_CalculateKeyFromSeed(unsigned char ucSecurityLevel,
                                              int iSeedKeySize,
                                              int iExtraBytesSize,
                                              unsigned char *pucSeed,
                                              unsigned char *pucKey,
                                              unsigned char *pucExtraBytes);

#endif // ALGORITHM_H
