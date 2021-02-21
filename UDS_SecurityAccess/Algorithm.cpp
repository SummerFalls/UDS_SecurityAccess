#include "Algorithm.h"

static void GenKeyFromSeed(const unsigned char *iSeedArray,
                           unsigned short iSeedArraySize,
                           unsigned char *iKeyArray,
                           unsigned short *iKeyArraySize,
                           const unsigned int iVariant,
                           const unsigned int iSecurityLevel)
{
    switch (iSecurityLevel)
    {
    case LEVEL_EXTEND:
        // Add security access function here.
        for (unsigned short i = 0; i < iSeedArraySize; ++i)
        {
            iKeyArray[i] = iSeedArray[i] + 1;
        }
        *iKeyArraySize = iSeedArraySize;
        break;
    case LEVEL_PROGRAM:
        // Add security access function here.
        break;
    case LEVEL_EOL:
        // Add security access function here.
        break;
    default:
        *((unsigned int *)iKeyArray) = 0xEEEEEEEEU;
        break;
    }
}

#pragma region ZLG ZCANPRO
SA_API int ZLGKey(const unsigned char *iSeedArray,
                  unsigned short iSeedArraySize,
                  unsigned int iSecurityLevel,
                  const char *iVariant,
                  unsigned char *iKeyArray,
                  unsigned short *iKeyArraySize)
{
    if (0 == iSeedArray || 0 == iKeyArray) return -1;

    GenKeyFromSeed(iSeedArray, iSeedArraySize, iKeyArray, iKeyArraySize, *iVariant, iSecurityLevel);

    return 0;
}
#pragma endregion

#pragma region CANoe
SA_API VKeyGenResultEx GenerateKeyEx(const unsigned char *iSeedArray,   /* Array for the seed [in] */
                                     unsigned short iSeedArraySize,     /* Length of the array for the seed [in] */
                                     const unsigned int iSecurityLevel, /* Security level [in] */
                                     const char *iVariant,              /* Name of the active variant [in] */
                                     unsigned char *ioKeyArray,         /* Array for the key [in, out] */
                                     unsigned int iKeyArraySize,        /* Maximum length of the array for the key [in] */
                                     unsigned int &oSize)               /* Length of the key [out] */
{
    unsigned int Variant = 0;
    unsigned int *ipKeyArraySize;

    if (iSeedArraySize > iKeyArraySize)
        return KGRE_BufferToSmall;

    if (iVariant[0] == '0')
    {
        Variant = 0;
    }

    ipKeyArraySize = &oSize;

    GenKeyFromSeed(iSeedArray, iSeedArraySize, ioKeyArray, (unsigned short *)ipKeyArraySize, Variant, iSecurityLevel);

    return KGRE_Ok;
}
#pragma endregion

#pragma region ETS
SA_API BOOL WINAPIV Seed2Key(ULONG VendorCode,
                             BYTE *Seed,
                             UINT SeedSize,
                             BYTE *Key,
                             UINT *KeySize,
                             USHORT SAAlg)
{
    GenKeyFromSeed(Seed, SeedSize, Key, (unsigned short *)KeySize, VendorCode, SAAlg);

    return true;
}
#pragma endregion

#pragma region Vehicle Spy3
SA_API bool __stdcall ES_CalculateKeyFromSeed(unsigned char ucSecurityLevel,
                                              int iSeedKeySize,
                                              int iExtraBytesSize,
                                              unsigned char *pucSeed,
                                              unsigned char *pucKey,
                                              unsigned char *pucExtraBytes)
{
    unsigned int Variant = 0;
    unsigned int iKeyArraySize;

    GenKeyFromSeed(pucSeed, iSeedKeySize, pucKey, (unsigned short *)&iKeyArraySize, Variant, ucSecurityLevel);

    return true;
}
#pragma endregion
