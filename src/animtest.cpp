//THIS PROGRAM WILL ONLY WORK AS 32-bit
//by tuukkas 2025

#include <fstream>
#include <string>
#include <vector>

#include <iostream>
#include <rwmath.h>
#include "endian.h"

#define EASTL_CUSTOM_FLOAT_CONSTANTS_REQUIRED 1
#define EASTL_DEBUG 0

#include <EASTL/vector_map.h>
#include <EASTL/vector.h>
#include <EASTL/utility.h>
#include <EASTL/functional.h>
#include <EASTL/allocator.h>

#include <cstdlib>

template <typename T>
T clamp(T val, T minVal, T maxVal) {
    return (val < minVal) ? minVal : (val > maxVal) ? maxVal : val;
}

template <typename T>
class SingletonHolder
{
public:
    // Returns a pointer to the singleton instance
    static T* Instance()
    {
        static T instance;  // Guaranteed to be lazy-initialized and thread-safe in C++11+
        return &instance;
    }

private:
    SingletonHolder() = delete;               // Prevent instantiation of the holder itself
    ~SingletonHolder() = delete;
    SingletonHolder(const SingletonHolder&) = delete;
    SingletonHolder& operator=(const SingletonHolder&) = delete;
};

//operators
void* operator new[](uint32_t size, const char* pFile, int line, uint32_t flags, const char* pName, int allocationCount) {
    (void)pFile; (void)line; (void)flags; (void)pName; (void)allocationCount;
    return std::malloc(size);
}

void* operator new[](uint32_t size, uint32_t alignment, uint32_t alignmentOffset, const char* pFile, int line, uint32_t flags, const char* pName, int allocationCount) {
    (void)alignment; (void)alignmentOffset; (void)pFile; (void)line; (void)flags; (void)pName; (void)allocationCount;
    return std::malloc(size);
}

void operator delete[](void* ptr, const char* pFile, int line, uint32_t flags, const char* pName, int allocationCount) noexcept {
    (void)pFile; (void)line; (void)flags; (void)pName; (void)allocationCount;
    std::free(ptr);
}

void operator delete[](void* ptr, uint32_t alignment, uint32_t alignmentOffset, const char* pFile, int line, uint32_t flags, const char* pName, int allocationCount) noexcept {
    (void)alignment; (void)alignmentOffset; (void)pFile; (void)line; (void)flags; (void)pName; (void)allocationCount;
    std::free(ptr);
}

namespace ANIM_CODEC {
    struct AnimCodecAPI {
        uint32_t __vftable; // offset
    };

    struct VBRUnitTest {
        uint32_t mNumFrames;
        uint32_t mNumJoints;
        uint32_t mOrigData;         // offset
        uint8_t mOutputBigEndian;
        uint8_t undefined1;
        uint8_t undefined2;
        uint8_t undefined3;
        uint32_t mHeaderData;       // offset
        uint32_t mCompressedData;   // offset
        uint32_t mVBRCompressor;    // offset
        uint32_t mVBRDecompressor;  // offset
    };

    struct VBRCompressorChannelTypeBlock{
        uint32_t __vftable; // offset
        uint32_t mOffsetStart;
        uint32_t mOffsetEnd;
        uint8_t mBlkid;
        uint8_t mSize;
        uint16_t mNumEntries;
        float mErrorTolerance;
        uint8_t mUseConstantChannels;
        uint8_t undefined1;
        uint8_t undefined2;
        uint8_t undefined3;
        float mConstChannelTolerance;
        uint32_t mhdr;      // offset
        uint32_t mdata;     // offset
        uint32_t mInitChanOffset;
        uint32_t mChanStride;
    };

    struct VBRCompressor {
        ANIM_CODEC::AnimCodecAPI baseclass_0;
        float mDctCoefTables[64];
        float mIdctCoefTables[64];
        float mQuantizeTables[2048];
        float mInitialQuantizeTables[2048];
        eastl::vector<ANIM_CODEC::VBRCompressorChannelTypeBlock*, eastl::allocator> mChannelBlocks;
        uint32_t mhdr; // offset
        uint8_t mCanAddChannels;
        uint8_t undefined1;
        uint8_t undefined2;
        uint8_t undefined3;
        uint32_t mFramesToNormalize;
        uint32_t mTotalChannelsPerFrame;
        uint32_t mFrameOffset;
        uint8_t mNumChannelBlks;
        uint8_t undefined4;
        uint8_t undefined5;
        uint8_t undefined6;
        uint32_t mNumConstFloats;
        uint8_t mStoredBits;
        uint8_t mBitBucket;
        uint8_t undefined7;
        uint8_t undefined8;
        uint32_t mNumberOfBits;
        uint32_t mMaxArraySize;
        uint32_t mCurCompressedDataSize;
        uint32_t mCompressedData;       // offset
        uint32_t mHeaderData;           // offset
        uint32_t mNumBits;              // offset
        uint32_t mNormalizedData;       // offset
        uint32_t mConstantChannel;      // offset
        uint32_t mConstantChannelData;  // offset
    };

    struct VBRChannelTypeBlockInfoData {
        uint16_t mOffset;
        uint16_t mStride;
        uint32_t mData; //offset
        uint8_t mDataSize;
        uint8_t mPadding0;
        uint16_t mPadding1;
    };
    class VBRChannelTypeBlockInfo : public VBRChannelTypeBlockInfoData {
    public:
        void PutData(uint32_t frame, uint32_t joint, const float* newData, uint32_t putLoc);
    };
    struct DataPerDecompress {
        uint32_t mhdr;                          // offset
        int *mUncompressedBlkDataScratch;   // offset
        uint32_t mMemoryHeaderSize;
        uint32_t mUsedTableOffset;
        uint32_t mNumBitsOffset;
        const uint8_t *mCompressedHeaderData;         // offset
        const uint8_t *mCompressedData;               // offset
        float *mNormalizedData;               // offset
        float *mConstData;                    // offset
        bool *mIsConstantChannel;            // offset
        uint32_t mNumConstantChannels;
        uint32_t mFrameOffset;
        uint16_t mChannelBlockStarts[4];
        uint16_t mChannelBlockEnds[4];
    };
    struct TableInfo {
        uint8_t isInited;
        uint8_t undefined1;
        uint8_t undefined2;
        uint8_t undefined3;
        uint32_t remap;
    };
    struct TableInfoArray {
        TableInfo tableArray[256];
        uint32_t usedTables;
    };
    struct VBRDecompressorData {
        VBRChannelTypeBlockInfo mChannelBlocks[4];
        float mIdctCoefTables[8][8];
        float mInitialQuantizeTables[256][8];
        float mQuantizeTables[6][4][8];
        DataPerDecompress mWorkBuffers[6];
        TableInfoArray mQuantTableInfo[6];
        uint8_t mCanAddChannel;
        uint8_t mNumChannelBlocks;
        uint16_t mMaxFrameSize;
        uint16_t mMaxTotalChannels;
        uint8_t undefined1;
        uint8_t undefined2;
    };
    struct VBRDataHeader {
        uint32_t mData;                     // offset
        uint32_t mConstantPaletteData;      // offset
        uint32_t mChannelTypeInfo;          // offset
        uint32_t mFrameBlockSizes;          // offset
        uint8_t mLoadedFromMemory;
        uint8_t undefined1;
        uint8_t undefined2;
        uint8_t undefined3;
        int GetChanBlockNumTotalEntPerFrm(int dataHeaderPtr, int channelIndex)
        {
            uint16_t* channelBlockEntry = reinterpret_cast<uint16_t*>( // Calculate pointer to the start of the channel block entry
                *reinterpret_cast<uint32_t*>(dataHeaderPtr + 8) + ((channelIndex * 16) & 0xFF0)
                );
            // Return the sum of the first two 16-bit values in this block
            return static_cast<int>(static_cast<uint16_t>(channelBlockEntry[0]) + static_cast<uint16_t>(channelBlockEntry[1]));
        }
    };
    struct VBRDataHeaderValue {
        uint16_t mConstChanMapSize;
        uint16_t mNumFrames;
        float mDctMin;
        float mDctMax;
        uint8_t mNumChannels;
        uint8_t mConstantPaletteSize;
        uint8_t undefined1;
        uint8_t undefined2;
    };
    struct VBRChannelTypeHdr {
        uint16_t NumEntPerFrm;
        uint16_t NumConstEntPerFrm;
        float Min;
        float Max;
        uint8_t Size;
        uint8_t undefined1;
        uint8_t undefined2;
        uint8_t undefined3;
    };
    class VBRDecompressor : public VBRDecompressorData {
    public:
        int DecompressTwoConsecFrames(int basePtr, unsigned int frameIndex1, unsigned int frameIndex2, int outputBuffer1, int outputBuffer2, int blockId);
        void DecompressFrameBlock(uint8_t frameBlockIdx, ANIM_CODEC::DataPerDecompress* pWorkBuffer, uint8_t threadId);
        void Vector_UnPackFrameBlockAndDecompressOneFrame(const uint8_t* pCompressedData, ANIM_CODEC::DataPerDecompress* pWorkBuffer, uint8_t frameinBlk, uint8_t threadId);
        void DecompressFrame(const uint8_t* compressedData, uint32_t frame, uint32_t putLoc, uint8_t threadId);
        const uint8_t* UnPackHeaderBits(DataPerDecompress* pWorkBuffer);
        uint32_t UnPackFrameBlockBits(int* a3, uint32_t* a4);
        uint32_t InitPerAnim(const uint8_t* compressedHeaderData, const uint8_t* headerData, uint8_t threadId);
    };
    namespace RawDiscretize {
        struct CompressionHeader { //RD header for each frame for each bone group
            uint32_t mMask[6]; //masks for scale, quat, trans (2 each) (tells which bones have data)
            uint16_t mFrameSize; //size of the data
            uint8_t mNbBonesM1; //number of bones
            uint8_t mCompressedT; //translation compressed true/false
            uint8_t mCompressedQ; //quaternion compressed true/false
            uint8_t undefined1;
            uint8_t undefined2;
            uint8_t undefined3;
        };
        struct tQuatAxisAngle{
            uint8_t data[6];
        };
        struct tQuatQuantize3{
            uint16_t data[3];
        };
        struct tQuatQuantize4{
            uint16_t data[4];
        };
        struct tTransCompressed{
            int16_t t[3]; // x, y, z
        };
        void TransCompress(const rw::math::vpu::Vector4& input, tTransCompressed& output, float range);
        void TransDecompress(const tTransCompressed& input, rw::math::vpu::Vector4& output, float range);
        void QuatCompress(rw::math::vpu::Quaternion* q, uint32_t* qcomp);
        void QuatDecompress(uint32_t qcomp, rw::math::vpu::Quaternion* q);
    } // namespace RawDiscretize
} // namespace ANIM_CODEC

namespace Util {

    constexpr uint32_t Seed = 79235168;
    constexpr const char* CharSet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_";

    inline static uint32_t encode6(const std::string& str) {
        if (str.length() > 6)
            throw std::invalid_argument("encode6 accepts strings of 6 or fewer characters.");

        uint32_t value = 0;
        uint32_t seed = Seed;

        for (char c : str) {
            int change = 0;

            if (c < 'a') {
                if (c > '9') {
                    if (c > 'Z') {
                        if (c != '_') throw std::invalid_argument(std::string("Invalid char: ") + c);
                        change = c - 58; // '_' -> 37
                    }
                    else {
                        if (c < 'A') throw std::invalid_argument("Invalid character");
                        change = c - 54; // 'A'-'Z' -> 11-36
                    }
                }
                else {
                    if (c < '0') throw std::invalid_argument("Invalid character");
                    change = c - 47; // '0'-'9' -> 1-10
                }
            }
            else {
                if (c > 'z') throw std::invalid_argument("Invalid character");
                change = c - 86; // 'a'-'z' -> 11-36
            }

            value += change * seed;
            seed /= 38;
        }

        value = Convert(value);
        return value;
    }

    inline static std::string decode6(uint32_t value) {
        value = Convert(value);
        std::string result;
        uint32_t seed = Seed;

        while (value > 0 && seed > 0) {
            uint32_t index = value / seed;
            if (index == 0 || index > 37)
                throw std::invalid_argument("Invalid encoded value");

            result += CharSet[index - 1];
            value %= seed;
            seed /= 38;
        }

        return result;
    }

    struct FastString6 {
        uint32_t mHead;

        FastString6() : mHead(0) {}
        explicit FastString6(const std::string& s) : mHead(encode6(s)) {}
        std::string to_string() const { return decode6(mHead); }
    };

    struct FastString12 {
        FastString6 mHead;
        FastString6 mTail;

        FastString12() = default;
        explicit FastString12(const std::string& s) {
            mHead = FastString6(s.substr(0, 6));
            mTail = FastString6(s.length() > 6 ? s.substr(6, 6) : "");
        }
        std::string to_string() const {
            return mHead.to_string() + mTail.to_string();
        }
    };

    struct FastString18 {
        FastString6 mHead;
        FastString12 mTail;

        FastString18() = default;
        explicit FastString18(const std::string& s) {
            mHead = FastString6(s.substr(0, 6));
            mTail = FastString12(s.length() > 6 ? s.substr(6, 12) : "");
        }
        std::string to_string() const {
            return mHead.to_string() + mTail.to_string();
        }
    };

    struct FastString24 {
        FastString6 mHead;
        FastString18 mTail;

        FastString24() = default;
        explicit FastString24(const std::string& s) {
            mHead = FastString6(s.substr(0, 6));
            mTail = FastString18(s.length() > 6 ? s.substr(6, 18) : "");
        }
        std::string to_string() const {
            return mHead.to_string() + mTail.to_string();
        }
    };

    struct FastString30 {
        FastString6 mHead;
        FastString24 mTail;

        FastString30() = default;
        explicit FastString30(const std::string& s) {
            mHead = FastString6(s.substr(0, 6));
            mTail = FastString24(s.length() > 6 ? s.substr(6, 24) : "");
        }
        std::string to_string() const {
            return mHead.to_string() + mTail.to_string();
        }
    };

    struct FastString36 {
        FastString6 mHead;
        FastString30 mTail;

        FastString36() = default;
        explicit FastString36(const std::string& s) {
            mHead = FastString6(s.substr(0, 6));
            mTail = FastString30(s.length() > 6 ? s.substr(6, 30) : "");
        }
        std::string to_string() const {
            return mHead.to_string() + mTail.to_string();
        }
    };
    template <size_t N> struct FastString; // Primary template declaration
    template <> struct FastString<6> : FastString6 { using FastString6::FastString6; };
    template <> struct FastString<12> : FastString12 { using FastString12::FastString12; };
    template <> struct FastString<18> : FastString18 { using FastString18::FastString18; };
    template <> struct FastString<24> : FastString24 { using FastString24::FastString24; };
    template <> struct FastString<30> : FastString30 { using FastString30::FastString30; };
    template <> struct FastString<36> : FastString36 { using FastString36::FastString36; };

    // Equality operator
    inline bool operator==(const FastString<36>& lhs, const FastString<36>& rhs) {
        return
            lhs.mHead.mHead == rhs.mHead.mHead &&
            lhs.mTail.mHead.mHead == rhs.mTail.mHead.mHead &&
            lhs.mTail.mTail.mHead.mHead == rhs.mTail.mTail.mHead.mHead &&
            lhs.mTail.mTail.mTail.mHead.mHead == rhs.mTail.mTail.mTail.mHead.mHead &&
            lhs.mTail.mTail.mTail.mTail.mHead.mHead == rhs.mTail.mTail.mTail.mTail.mHead.mHead &&
            lhs.mTail.mTail.mTail.mTail.mTail.mHead == rhs.mTail.mTail.mTail.mTail.mTail.mHead;
    }

    // Less-than operator (lexicographic comparison)
    inline bool operator<(const FastString<36>& lhs, const FastString<36>& rhs) {
        if (lhs.mHead.mHead != rhs.mHead.mHead) return lhs.mHead.mHead < rhs.mHead.mHead;
        if (lhs.mTail.mHead.mHead != rhs.mTail.mHead.mHead) return lhs.mTail.mHead.mHead < rhs.mTail.mHead.mHead;
        if (lhs.mTail.mTail.mHead.mHead != rhs.mTail.mTail.mHead.mHead) return lhs.mTail.mTail.mHead.mHead < rhs.mTail.mTail.mHead.mHead;
        if (lhs.mTail.mTail.mTail.mHead.mHead != rhs.mTail.mTail.mTail.mHead.mHead) return lhs.mTail.mTail.mTail.mHead.mHead < rhs.mTail.mTail.mTail.mHead.mHead;
        if (lhs.mTail.mTail.mTail.mTail.mHead.mHead != rhs.mTail.mTail.mTail.mTail.mHead.mHead) return lhs.mTail.mTail.mTail.mTail.mHead.mHead < rhs.mTail.mTail.mTail.mTail.mHead.mHead;
        return lhs.mTail.mTail.mTail.mTail.mTail.mHead < rhs.mTail.mTail.mTail.mTail.mTail.mHead;
    }
} // namespace Util

namespace xvectoralloc {
    template <typename T>
    struct XVectorPtr {
        T* mVector;
    };
}

namespace AnimCmdSys {
    struct SQTExt {
        rw::math::vpu::Vector4 mScale;
        rw::math::vpu::Quaternion mQuat;
        rw::math::vpu::Vector4 mTrans;
    };
    enum class ACSMode : uint32_t {
        IMMEDIATE = 0,
        CMDBUFFER = 1,
        CMDBUFFERNOEVAL = 2,
        ALL = 3,
    };
    enum class ACSFetchMode : uint32_t {
        FETCHFRAME = 0,
        FETCHBLEND = 1,
        ALL_FETCHES = 2,
    };
    enum class ACSJobCategory : uint32_t {
        JOB_CATEGORY_0 = 0,
        JOB_CATEGORY_1 = 1,
        JOB_CATEGORY_2 = 2,
        ALL_CATEGORIES = 3,
    };
    enum class TrajectoryUse : uint32_t {
        DoNotUseTrajectory = 0,
        UseTrajectoryWithAdjtoChildren = 1,
        UseTrajectoryWithoutAdjtoChildren = 2,
    };
    struct ACSParams {
        uint32_t MaxTotalSQTCommandsPerAnimEntity;//long ?
        uint32_t MaxFetchCommandsPerAnimEntity;//long ?
        uint32_t MaxSetReplaceNonRegisteredSQTCommandsPerAnimEntity;//long ?
        uint32_t MaxSetNonRegisteredMatrixCommandsPerAnimEntity;//long ?
        uint32_t MaxTotalMatrixCommandsPerAnimEntity;//long ?
        float AverageTotalSQTCommandsPerAnimEntity;//float ?
        float AverageFetchCommandsPerAnimEntity;//float ?
        float AverageSetReplaceNonRegisteredSQTCommandsPerAnimEntity;//float ?
        float AverageSetNonRegisteredMatrixCommandsPerAnimEntity;//float ?
        float AverageTotalMatrixCommandsPerAnimEntity;//float ?
        uint32_t MaxNBlendCommandsPerAnimEntity;//long ?
        uint32_t MaxRegisteredSQTBuffers;//long ?
        uint32_t MaxRegisteredMatrixBuffers;//long ?
        ACSMode mode;//long ?
        ACSFetchMode fetchmode;//long ?
        ACSJobCategory category;//long ?
    };
} // namespace AnimCmdSys

namespace Andale {
    namespace Animatable {
        enum class eTransType : uint8_t {
            SEQUENCE = 0,
            BLEND = 1,
            CHANNELBLEND = 2
        };
    };

    enum class eChannelPriority : uint8_t {
        eChannelPriority_Low = 0,
        eChannelPriority_Med = 1,
        eChannelPriority_High = 2
    };

    enum class codec_type : uint8_t {
        RAW = 0,
        CONV_FORMAT = 1,
        U1U1U1U1 = 2,
        NULL_COPY = 3
    };

    enum class HeaderTypes : int32_t {
        HT_RESERVED = 0,
        HT_DBINFO = 1,
        HT_ANIMATION = 2,
        HT_POSE = 3,
        HT_HIERARCHY = 4,
        HT_PHYSICSPARAMS = 5,
        HT_BLENDSPACE = 6,
        HT_PHASEBLEND = 7,
        HT_SWITCHNODE = 8,
        HT_SEQUENCENODE = 9,
        HT_SELECTORNODE = 0xA,
        HT_SELECTIONSPACE = 0xB,
        HT_MAXVALUE = -1
    };

    enum class LocomotionStageType : int32_t {
        E_ContactToPassRight = 0,
        E_PassToAirRight = 1,
        E_AirRight = 2,
        E_ContactToPassLeft = 3,
        E_PassToAirLeft = 4,
        E_AirLeft = 5,
        E_LAST = 0x10000000
    };

    struct DataHeader {
        uint32_t mSize;
        uint32_t mType;
        uint32_t mCodecCompactName;
        uint32_t mGUID;
        Util::FastString<36> mName;
    };

    struct DataBaseData {
        enum class DeletionPolicy : uint32_t {
            E_DELETE = 0,
            E_DONT_DELETE = 1,
        };
        uint8_t mDBIndex;
        uint8_t undefined1;
        uint8_t undefined2;
        uint8_t undefined3;
        uint32_t mHierarchy;            // offset
        uint32_t mBuffer;               // offset
        DeletionPolicy mDeletionPolicy;
        uint32_t mDatabaseManager;       // offset
        eastl::vector_map<
            Util::FastString<36>,
            Andale::DataHeader*,
            eastl::less<Util::FastString<36>>,
            eastl::allocator,
            eastl::vector<
            eastl::pair<Util::FastString<36>, Andale::DataHeader*>,
            eastl::allocator
            >
        > mAnimations;
        eastl::vector_map<
            Util::FastString<36>,
            Andale::DataHeader*,
            eastl::less<Util::FastString<36>>,
            eastl::allocator,
            eastl::vector<
            eastl::pair<Util::FastString<36>, Andale::DataHeader*>,
            eastl::allocator
            >
        > mClips;
        eastl::vector_map<
            Util::FastString<36>,
            Andale::DataHeader*,
            eastl::less<Util::FastString<36>>,
            eastl::allocator,
            eastl::vector<
            eastl::pair<Util::FastString<36>, Andale::DataHeader*>,
            eastl::allocator
            >
        > mPoses;
        eastl::vector_map<
            Util::FastString<36>,
            Andale::DataHeader*,
            eastl::less<Util::FastString<36>>,
            eastl::allocator,
            eastl::vector<
            eastl::pair<Util::FastString<36>, Andale::DataHeader*>,
            eastl::allocator
            >
        > mPhysicsParams;
        uint32_t mPartNameMapping[31];
    };

    class DataBaseManager;

    class DataBase : public DataBaseData {
    public:
        DataBase(DataBaseManager* dbm);
        void Clear();
        void UpdatePartNames();
        int SetDBContent(void* buffer, Andale::DataBase::DeletionPolicy p);
        bool SetDBContent(const char* DBFilename);
    };

    struct PoolStackElement {
        uint8_t mPoolAllocated;
        uint8_t undefined1;
        uint8_t undefined2;
        uint8_t undefined3;
    };

    struct Animation {
        PoolStackElement baseclass_0;
        uint8_t mWasEvaluated;
        uint8_t undefined1;
        uint8_t undefined2;
        uint8_t undefined3;
    };

    struct AnimationPart {
        uint32_t mHeader;
        uint32_t mCompressedDataSize;
        uint32_t mCompressionHeader;    // 0x00 - bitfield controlling bones, flags, sizes
        uint32_t mCompressedData;       // offset to SQT data
    };

    struct AnimationPartSk81 {
        uint32_t mMasks[6];
        uint32_t mBitField;
        uint32_t mSQTDataOffset;
    };

    struct AnimationData {
        rw::math::vpu::Vector3 mTrajTransDelta;
        rw::math::vpu::Quaternion mTrajQuatDelta;
        float mFPS;
        float mNbFrames;
        float mBaseSpeed;
        uint32_t mAttributesOffset;
        uint32_t mBlendPosePartsOffsetsOffset;
        uint32_t mBitField;
        uint32_t mPartsOffsets[1];
    };

    struct Pose {
        PoolStackElement baseclass_0;
        uint8_t mWasEvaluated;
        uint8_t undefined1;
        uint8_t undefined2;
        uint8_t undefined3;
    };

    struct PoseData {
        uint32_t mNbParts;
        uint32_t mPartsOffsets;
    };

    struct PoseKey {
        uint16_t mTimeInCurrentAnimation;
        uint16_t mFrameInReferenceAnimation;
    };

    struct ExtractType {
        rw::math::vpu::Quaternion deltaQ;   // Delta rotation quaternion — represents the rotational difference or adjustment.
        float deltaT[3];       // Delta translation — 3D vector (x, y, z) representing movement offset, like root motion delta.
        uint32_t header;        // pointer to AnimationPart (AnimationPart*)
        uint32_t sqtBuff;       // pointer to the SQT data buffer (SQTData*)
        uint32_t extraOut;
        uint8_t codecType;      // compression or encoding method used for the animation
        uint8_t doTraj;         // Boolean flag (0/1): whether trajectory calculation is needed.
        uint16_t channelAnim;
        uint16_t headersize;
        uint16_t currblend;
        uint16_t prevblend;
        uint16_t totalBones;    // number of bones
        uint16_t data[2];       // 0=primary animation frame 1=secondary animation frame
        uint16_t prev[2];
        uint16_t sqtSize;       // frame output size
        uint16_t sqtOffset;     // frame output offset
    };

    struct SQTData {
        rw::math::vpu::Vector3 mScale;
        rw::math::vpu::Quaternion mQuat;
        rw::math::vpu::Vector4 mTrans;
    };

    struct DataBaseManagerData {
        uint64_t mCRITICAL_SECTION[4]; //EA::Thread::Futex mAccessMutex !i'm not using this!
        uint32_t mMaxDatabases;
        uint32_t mDataBases[32]; // offset
        Util::FastString<36> mPartNames[31];
        uint32_t mNbPartNames;
    };

    class DataBaseManager : public DataBaseManagerData {
    public:
        DataBaseManager();
        int FindFreeSlot();
        const DataHeader* GetPoseHeader(uint32_t poseGUID);
        const DataHeader* GetClipHeader(uint32_t animGUID);
        void UpdateDatabase(DataBase* db);
        int GetPartNameIndex(const Util::FastString<36>* partName);
    };

    struct FetchSysData {
        uint32_t mSkel;                 // offset
        float mTruncationOffset;
        uint8_t mHasTrajectoryBone;
        uint8_t undefined1;
        uint8_t undefined2;
        uint8_t undefined3;
        uint32_t mMaxFetches;
        uint32_t mDataBaseManager;      // offset
        float mFetchTimeCmdError;
        uint32_t mJobCode;              // offset
        uint32_t mCodecType;            // enum Andale::codec_type
        uint32_t mDecompressor;         // offset
    };

    struct Mirror {
        Animation baseclass_0;
        uint32_t mHierarchy; // offset
        uint32_t mChild; // offset
        AnimCmdSys::TrajectoryUse mUseTraj; // enum AnimCmdSys::TrajectoryUse
    };

    struct Mirror_vtbl {
        uint32_t PoolStackElement_dtor_0; // offset
        uint32_t Reset;             // offset
        uint32_t AddTime;           // offset
        uint32_t Eval;              // offset
        uint32_t EvalBlendPose;     // offset
        uint32_t GetLength;         // offset
        uint32_t GetTime;           // offset
        uint32_t SetTime;           // offset
        uint32_t SetSpeed;          // offset
        uint32_t GetWeight;         // offset
        uint32_t Trim;              // offset
        uint32_t ResetPerFrameData; // offset
        uint32_t GetAttributes;     // offset
        uint32_t GetAttribute;      // offset
        uint32_t SetAttributes;     // offset
        uint32_t IsTrimmed;         // offset
    };

    struct Pose_vtbl {
        uint32_t PoolStackElement_dtor_0; // offset
        uint32_t Reset;             // offset
        uint32_t AddTime;           // offset
        uint32_t Eval;              // offset
        uint32_t EvalBlendPose;     // offset
        uint32_t GetLength;         // offset
        uint32_t GetTime;           // offset
        uint32_t SetTime;           // offset
        uint32_t SetSpeed;          // offset
        uint32_t GetWeight;         // offset
        uint32_t Trim;              // offset
        uint32_t ResetPerFrameData; // offset
        uint32_t GetAttributes;     // offset
        uint32_t GetAttribute;      // offset
        uint32_t SetAttributes;     // offset
        uint32_t DebugDump;         // offset
        uint32_t IsTrimmed;         // offset
    };

    struct Attribute {
        uint8_t mAttrBuffer[24];
        float mBeginTime;
        float mEndTime;
        Util::FastString<30> mName;
        uint8_t mFlags;
        uint8_t mType;
        uint8_t undefined1;
        uint8_t undefined2;
        uint32_t mSequenceID;
        uint8_t undefined3;
        uint8_t undefined4;
        uint8_t undefined5;
        uint8_t undefined6;
    };

    struct HierarchyPart {
        Util::FastString<36> mPartName;
        uint32_t mNbBones;
        uint32_t mPartOffset;
        uint32_t mIsAnimated;
        uint32_t mPartNameIndex;
    };

    struct HierarchyData {
        uint16_t mNbBones;
        uint16_t mHasTrajectory;
        uint32_t mNbParts;
        uint32_t mParents[1];
    };

    struct PhysicsParamBoneData{
        rw::math::vpu::Quaternion mParentOrientation;
        rw::math::vpu::Quaternion mJointOrientation;
        rw::math::vpu::Vector3 mSize;
        rw::math::vpu::Quaternion mOrientation;
        rw::math::vpu::Vector3 mPosition;
        float mTwistLimit;
        float mSwingLimit;
        float mMass;
        Util::FastString<30> mBoneName;
    };

    struct PhysicsParamData{
        uint32_t mNbBones;
        uint32_t mPadding[3];
        Andale::PhysicsParamBoneData mBones;
    };

    struct BlendPoseBuffer {
        Util::FastString<36> mPartName[6];
        uint32_t mIndex;
        uint32_t mPoolIndex;
        uint32_t mNbStreams;
        uint32_t mBuffer[3];          // offset
        uint32_t mTop[3];
    };

    struct SwitchNodeData {
        Util::FastString<30> mSwitchKey;
        uint32_t mChildrenOffset;
        uint32_t mAttrValuesOffset;
        uint32_t mNbChildren;
        uint32_t mPadding;
    };

    struct BlendSpaceData {
        uint32_t mNbAnimations;
        uint32_t mAnimationNamesOffset;
        uint32_t mNbVolumeSets;
        uint32_t mVolumeSetsOffset;
        uint32_t mMaxNbDimensions;
    };

    struct SelectorData {
        Util::FastString<36> mChildName;
        Util::FastString<30> mAttrName;
    };

    struct SelectorNodeData {
        uint32_t mNbChildren;
        Andale::SelectorData mChildren;
    };

    struct LocomotionStageData {
        Util::FastString<36> mName;
        uint32_t mLST; //Andale::LocomotionStageType
    };

    struct SequenceNodeData {
        uint16_t mNbChildren;
        uint8_t mIsLooping;
        uint8_t mWindow;
        Andale::LocomotionStageData mChildren;
    };

    struct PhaseBlendData {
        uint32_t mNbAttributes;
        uint32_t mAttributesOffset;
        uint32_t mNbChildren;
        uint32_t mPadding;
        Util::FastString<30> mBlendAttributeName;
        Util::FastString<36> mChildren;
    };

    struct Clip {
        Andale::Animation baseclass_0;
        float mDebugPrintOldTime;
        uint32_t mDebugPrintNumLoops;
        uint32_t mClipHeader; // offset
        float mSpeed;;
        float mLength;
        float mTime;
        float mOldTime;
        uint32_t mNumLoops;
        float mNbFrames;
        uint8_t mLoopingAnim;
        uint8_t undefined1;
        uint8_t undefined2;
        uint8_t undefined3;
        float mFps;
        float mBaseSpeed;
        uint8_t mTrimmed;
        uint8_t undefined4;
        uint8_t undefined5;
        uint8_t undefined6;
    };

    class FetchSys : public FetchSysData {
    public:
        FetchSys(Andale::DataBaseManager* dbm, const Andale::HierarchyData* skel, double truncationOffset, bool hasTrajectoryBone, Andale::codec_type codectype); //FetchSys(Andale::DataBaseManager* dbm, const Andale::HierarchyData* skel, void** pJobCode, uint32_t numChars, uint32_t maxFetchNodes, double truncationOffset, bool hasTrajectoryBone, Andale::codec_type codectype);
        void GetSQTsForPose(uint32_t poseGUID, xvectoralloc::XVectorPtr<AnimCmdSys::SQTExt>* output);
        void GetSQTs(uint32_t animGUID, uint32_t frameIndex, xvectoralloc::XVectorPtr<AnimCmdSys::SQTExt>* output);
        void loadPose(uint32_t animGUID, xvectoralloc::XVectorPtr<AnimCmdSys::SQTExt>* output);
        void loadAnimation(uint32_t animGUID, xvectoralloc::XVectorPtr<AnimCmdSys::SQTExt>* output);
        void Fetch(const Andale::DataHeader* hdr, xvectoralloc::XVectorPtr<SQTData>* output);
        void Fetch(const Andale::DataHeader* hdr, uint32_t frameIndex, xvectoralloc::XVectorPtr<SQTData>* output);

        void InitFetchSysForACS(uint32_t* MaxExtractRecords, uint32_t* ExtractRecordSize, uint32_t* MaxExtractsPerJob, uint32_t* MaxExtractsPerFetch, void** pJobCode, bool useJob, uint32_t decompressor);
        uint32_t FillExtractBuffers(bool isPose, uint32_t GUID, float prevTime, float currTime, uint32_t loops, bool blendFrames, xvectoralloc::XVectorPtr<AnimCmdSys::SQTExt>* output, uint32_t extraOutput, uint32_t extractBuffer, Andale::SQTData* optionalSQT, Andale::ExtractType* extractTypes);
        int GetCodecType(uint32_t codecId) const;
        int BatchFetch(const Andale::DataHeader* hdr, xvectoralloc::XVectorPtr<SQTData>* output, Andale::ExtractType* extractBuffer, int maxSize);
        int BatchFetch(const Andale::DataHeader* hdr, uint32_t frameIndex, xvectoralloc::XVectorPtr<SQTData>* output, Andale::ExtractType* extractBuffer, int maxSize);
    };

    void TrajectoryCalculation(Andale::SQTData* trajBone, bool channelAnim, double channelCoef, const rw::math::vpu::Quaternion* prevTrajQuat, const rw::math::vpu::Vector3* prevTrajTrans, const rw::math::vpu::Quaternion* deltaQ, const rw::math::vpu::Vector3* deltaT);
    void Scalar_ExtractPackedNVBR(ExtractType* ExtractBuffer, uint32_t ExtractBuffSize, ANIM_CODEC::VBRDecompressor* decompressor);
    void ExtractPackedN(ExtractType* ExtractBuffer, uint32_t ExtractBuffSize);
    void ExtractPackedNVBR(ExtractType* ExtractBuffer, uint32_t ExtractBuffSize, ANIM_CODEC::VBRDecompressor* decompressor);
}

void ANIM_CODEC::VBRChannelTypeBlockInfo::PutData(uint32_t frame, uint32_t joint, const float* newData, uint32_t putLoc) {
    // Pointer to the location where we will put the data
    float* targetData = nullptr;

    if (putLoc) {
        targetData = reinterpret_cast<float*>(mStride * joint + mOffset + putLoc);
    }
    else {
        if (!mData) {
            throw std::runtime_error("mData is null, assertion failed");
        }
        targetData = reinterpret_cast<float*>(mStride * joint + mData + mOffset);
    }

    // Process based on mDataSize
    switch (mDataSize) {
    case 1:
        *targetData = *newData;
        break;
    case 2:
        targetData[1] = newData[1];
    case 3:
        targetData[2] = newData[2];
    case 4:
        targetData[3] = newData[3];
        // Fall-through case for multi-dimensional data
    default:
        for (size_t i = 0; i < mDataSize; ++i) {
            targetData[i] = newData[i];
        }
        break;
    }
}
/*
void ANIM_CODEC::VBRDecompressor::DecompressFrameBlock(uint8_t frameinBlk, ANIM_CODEC::DataPerDecompress* pWorkBuffer, uint8_t threadId)
{
    uint32_t numChannels = *(uint8_t*)(pWorkBuffer->mIsConstantChannel + 0xC); // Read number of channels
    if (pWorkBuffer->mUncompressedBlkDataScratch == 0) {
        throw std::runtime_error("UncompressedBlkData != 0");
    }

    uint32_t usedTableOffset = pWorkBuffer->mUsedTableOffset;
    uint32_t uncompressedBase = pWorkBuffer->mUncompressedBlkDataScratch;
    uint32_t constDataBase = pWorkBuffer->mConstData;
    uint32_t normalizedDataBase = pWorkBuffer->mNormalizedData;
    uint32_t quantTableOffset = pWorkBuffer->mCompressedHeaderData;

    uint32_t numBitsOffset = pWorkBuffer->mNumBitsOffset;
    uint32_t frameOffset = pWorkBuffer->mFrameOffset;

    uint32_t usedTableSize = (numBitsOffset + 3) & 0xFFFFFFFC;
    uint32_t tableFlag = 0;

    std::vector<uint8_t> channelUsed(numChannels, 0);

    for (uint32_t i = 0; i < numChannels; ++i) {
        uint8_t ch = *((uint8_t*)(usedTableOffset + i));
        channelUsed[ch] = 0;
    }

    uint32_t initializedQuantTables = 0;

    for (uint32_t i = 0; i < numChannels; ++i) {
        uint8_t ch = *((uint8_t*)(usedTableOffset + i));
        uint16_t blockStart = pWorkBuffer->mChannelBlockStarts[ch];
        uint16_t blockEnd = pWorkBuffer->mChannelBlockEnds[ch];

        if (blockStart == blockEnd) continue;

        // Get decompression table info
        VBRChannelTypeBlockInfo* blockInfo = &mChannelBlocks[ch];
        uint32_t tableIndex = blockInfo->mData + 4 * frameinBlk;

        uint8_t* tableFlagPtr = (uint8_t*)(blockInfo->mData + 8 * ch + normalizedDataBase + 0x25B0);
        if (*tableFlagPtr == 0) {
            *tableFlagPtr = 1;
            ++initializedQuantTables;

            // Simulate quantization table building
            // Normally uses SIMD, here just scalar
            float scale = 1.0f; // dummy
            float table[64];
            for (int j = 0; j < 64; ++j) {
                table[j] = mInitialQuantizeTables[j] * scale;
            }

            // Save table
            for (int j = 0; j < 64; ++j) {
                mQuantizeTables[ch * 64 + j] = table[j];
            }
        }

        // Simulate transform and write results
        for (uint16_t j = blockStart; j < blockEnd; ++j) {
            float inputBlock[64]; // Normally from SIMD
            float quantizedBlock[64];

            // Load from source (uncompressed)
            for (int k = 0; k < 64; ++k) {
                inputBlock[k] = ((float*)uncompressedBase)[j * 64 + k];
            }

            // Quantize and transform
            for (int k = 0; k < 64; ++k) {
                quantizedBlock[k] = inputBlock[k] * mQuantizeTables[ch * 64 + k];
            }

            // Store to normalized data
            for (int k = 0; k < 64; ++k) {
                ((float*)normalizedDataBase)[(frameinBlk * usedTableSize + j) * 64 + k] = quantizedBlock[k];
            }
        }
    }

    // Store quantization metadata
    ((uint32_t*)(normalizedDataBase + 0x2DB0))[0] = initializedQuantTables;
}
*/

void ANIM_CODEC::VBRDecompressor::DecompressFrameBlock(
    uint8_t frameinBlk,
    ANIM_CODEC::DataPerDecompress* pWorkBuffer,
    uint8_t threadId)
{
    ANIM_CODEC::VBRDataHeader* mhdr = reinterpret_cast<ANIM_CODEC::VBRDataHeader*>(pWorkBuffer->mhdr);

    const uint8_t* compressed = pWorkBuffer->mCompressedData;
    int* scratch = pWorkBuffer->mUncompressedBlkDataScratch;
    float* output = pWorkBuffer->mNormalizedData;

    ANIM_CODEC::VBRDataHeaderValue* dataValue = reinterpret_cast<ANIM_CODEC::VBRDataHeaderValue*>(mhdr->mData);
    uint32_t numChannels = dataValue->mNumChannels;
    uint16_t* channelBlockEnds = pWorkBuffer->mChannelBlockEnds;

    int frameOffset = (pWorkBuffer->mFrameOffset + 3) & ~3;

    // thread-local decompression memory
    uint8_t* threadBuf = (uint8_t*)this + threadId * 2052;

    // IDCT coefficient tables
    float* idctCoefA =
        (float*)((uint8_t*)mIdctCoefTables + ((frameinBlk * 32) & 0x1FE0));

    float* idctCoefB = idctCoefA + 4;

    //------------------------------------------------------------
    // Reset per-channel flags
    //------------------------------------------------------------

    for (uint32_t i = 0; i < numChannels; i++)
    {
        uint8_t ch = compressed[i];
        threadBuf[9648 + (ch << 3)] = 0;
    }

    *(uint32_t*)(threadBuf + 11696) = 0;

    //------------------------------------------------------------
    // Precompute normalization scale
    //------------------------------------------------------------

    float dctMin = dataValue->mDctMin;
    float dctMax = dataValue->mDctMax;

    float scale = std::max(dctMin, dctMax);
    float scaleSquared = scale * scale;

    //------------------------------------------------------------
    // Channel processing
    //------------------------------------------------------------

    for (uint32_t ch = 0; ch < numChannels; ch++)
    {
        uint8_t channelIndex = compressed[ch];

        int start = (ch == 0) ? 0 : pWorkBuffer->mChannelBlockEnds[ch - 1];
        int end = pWorkBuffer->mChannelBlockEnds[ch];

        if (start == end)
        {
            channelBlockEnds++;
            continue;
        }

        //--------------------------------------------------------
        // locate quantization table
        //--------------------------------------------------------

        uint8_t* flagEntry = &threadBuf[9648 + channelIndex * 8];

        float* quantTable =
            mQuantizeTables[threadId][*(uint32_t*)(flagEntry + 4)];

        int* input = &scratch[start];

        //--------------------------------------------------------
        // build quantization table if needed
        //--------------------------------------------------------

        if (!flagEntry[0])
        {
            flagEntry[0] = 1;

            int idx = *(uint32_t*)(threadBuf + 11696);
            *(uint32_t*)(threadBuf + 11696) = idx + 1;

            float* initial = mInitialQuantizeTables[channelIndex];

            for (int i = 0; i < 8; i++)
            {
                quantTable[i] = initial[i] * scaleSquared;
            }
        }

        //--------------------------------------------------------
        // IDCT + dequantization
        //--------------------------------------------------------

        float coefA[4];
        float coefB[4];

        for (int i = 0; i < 4; i++)
        {
            coefA[i] = quantTable[i] * idctCoefA[i];
            coefB[i] = quantTable[i + 4] * idctCoefB[i];
        }

        //--------------------------------------------------------
        // output location
        //--------------------------------------------------------

        float* dst =
            &output[(uint8_t)frameinBlk * frameOffset + start];

        //--------------------------------------------------------
        // transform loop
        //--------------------------------------------------------

        int count = end - start;

        for (int i = 0; i < count; i++)
        {
            float accum = 0.0f;

            for (int k = 0; k < 4; k++)
            {
                accum += input[i * 4 + k] * coefA[k];
                accum += input[i * 4 + k + 4] * coefB[k];
            }

            dst[i] = accum;
        }

        channelBlockEnds++;
    }
}

/*
void ANIM_CODEC::VBRDecompressor::DecompressFrameBlock(uint8_t frameBlockIdx, ANIM_CODEC::DataPerDecompress* pWorkBuffer, uint8_t threadId)
{
    // Get basic information from work buffer
    const uint8_t numChannels = pWorkBuffer->mCompressedHeaderData[12];
    const uint32_t frameSizeAligned = (pWorkBuffer->mFrameOffset + 3) & ~3;

    // Get pointers to the various data tables
    float* quantTables = mQuantizeTables;
    float* initialQuantTables = mInitialQuantizeTables;
    float* idctTables = mIdctCoefTables;

    // Get pointer to this frame block's working area
    uint8_t* channelFlags = &mQuantTableInfo[frameBlockIdx].isInitialized;
    uint32_t& constantChannelCount = mQuantTableInfo[frameBlockIdx].constantChannelCount;

    // Validate required buffers
    if (!pWorkBuffer->mUncompressedBlkDataScratch) {
        std::cout << "Uncompressed block data buffer is null"s;
        return;
    }

    // Reset channel flags for this frame block
    for (uint32_t i = 0; i < numChannels; i++) {
        uint8_t channelIdx = pWorkBuffer->mCompressedData[i];
        channelFlags[channelIdx] = 0;
    }
    constantChannelCount = 0;

    // Process each channel in this frame block
    for (uint32_t ch = 0; ch < numChannels; ch++) {
        uint8_t channelIdx = pWorkBuffer->mCompressedData[ch];
        uint16_t blockStart = pWorkBuffer->mChannelBlockStarts[ch];
        uint16_t blockEnd = pWorkBuffer->mChannelBlockEnds[ch];

        if (blockStart == blockEnd) {
            continue; // Skip empty channel blocks
        }

        // Get this channel's quantization table info
        ChannelTableInfo& tableInfo = mQuantTableInfo[frameBlockIdx].channelTables[channelIdx];

        // Initialize quantization tables if this is the first time we're using them
        if (!tableInfo.isInitialized) {
            tableInfo.isInitialized = true;

            // Calculate new quantization tables from initial tables and IDCT coefficients
            const float* srcInitialTable = &initialQuantTables[channelIdx * 64];
            const float* srcIdctTable = &idctTables[channelIdx * 16];
            float* destTable = &quantTables[constantChannelCount * 64];

            // This would be matrix multiplication in SIMD, here done with regular loops
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    float sum = 0.0f;
                    for (int k = 0; k < 8; k++) {
                        sum += srcInitialTable[i * 8 + k] * srcIdctTable[k * 8 + j];
                    }
                    destTable[i * 8 + j] = sum;
                }
            }

            tableInfo.tableOffset = constantChannelCount;
            constantChannelCount++;
        }

        // Get the appropriate quantization table for this channel
        const float* currentQuantTable = &quantTables[tableInfo.tableOffset * 64];
        float* outputBuffer = pWorkBuffer->mNormalizedData + blockStart * 4;

        // Decompress each block in this channel
        for (uint32_t block = blockStart; block < blockEnd; block++) {
            // Get quantized coefficients (4 values per block)
            float coefficients[4];
            for (int i = 0; i < 4; i++) {
                coefficients[i] = currentQuantTable[block * 4 + i];
            }

            // Apply inverse DCT transform (simplified version)
            float transformed[4];
            for (int i = 0; i < 4; i++) {
                transformed[i] = 0.0f;
                for (int j = 0; j < 4; j++) {
                    transformed[i] += coefficients[j] * idctTables[j * 4 + i];
                }
            }

            // Store decompressed output
            for (int i = 0; i < 4; i++) {
                outputBuffer[block * 4 + i] = transformed[i];
            }
        }
    }
}
*/
void ANIM_CODEC::VBRDecompressor::Vector_UnPackFrameBlockAndDecompressOneFrame(const uint8_t* pCompressedData, ANIM_CODEC::DataPerDecompress* pWorkBuffer, uint8_t frameinBlk, uint8_t threadId) {
    if (!pWorkBuffer) {
        throw std::runtime_error("pWorkBuffer is null");
    }

    ANIM_CODEC::DataPerDecompress& workBuffer = *pWorkBuffer;
    uint8_t* compressedDataPointer = const_cast<uint8_t*>(pCompressedData);

    uint32_t frameOffset = workBuffer.mFrameOffset;
    if (!frameOffset) {
        throw std::runtime_error("Frame offset is zero");
    }

    uint8_t* headerData = reinterpret_cast<uint8_t*>(&workBuffer.mCompressedHeaderData);
    uint8_t* uncompressedData = reinterpret_cast<uint8_t*>(&workBuffer.mUncompressedBlkDataScratch);

    if (!uncompressedData) {
        throw std::runtime_error("Uncompressed data is null");
    }

    uint32_t bitsOffset = workBuffer.mNumBitsOffset;
    uint32_t dataBlockSize = 0x20;

    // Decompressor logic here (simplified for readability)
    uint32_t channelBlockStart = workBuffer.mChannelBlockStarts[frameinBlk];
    uint32_t channelBlockEnd = workBuffer.mChannelBlockEnds[frameinBlk];

    for (uint32_t i = 0; i < frameOffset; ++i) {
        uint32_t data = *(reinterpret_cast<uint32_t*>(compressedDataPointer + i * sizeof(uint32_t)));
        // Perform unpacking operations
        compressedDataPointer += sizeof(uint32_t);

        // Process decompression (simplified example logic)
        for (uint32_t j = channelBlockStart; j < channelBlockEnd; ++j) {
            // Unpacking logic for each channel (this is a simplified version of the byte-permutation and decompressing)
            uint8_t* targetChannelData = uncompressedData + j * dataBlockSize;
            std::memcpy(targetChannelData, &data, sizeof(data));
        }
    }

    //DecompressFrameBlock(frameinBlk, threadId, pWorkBuffer);
}

void ANIM_CODEC::VBRDecompressor::DecompressFrame(const uint8_t* compressedData, uint32_t frame, uint32_t putLoc, uint8_t threadId) {
    constexpr uint8_t MaxThreads = 6;
    if (threadId >= MaxThreads) {
        throw std::runtime_error(
            "VBRDecompressor::threadId exceeds number of parallel work buffers - increase number of work buffers"
        );
    }

    // Workbuffer and state pointer
    DataPerDecompress& workBuffer = mWorkBuffers[threadId];
    TableInfoArray& tableInfoArray = mQuantTableInfo[threadId];

    // Retrieve the quantization table pointer from the remap
    const TableInfo* tableInfo = &tableInfoArray.tableArray[0];
    uint32_t remapOffset = tableInfo->remap;
    const uint8_t* remapData = compressedData + remapOffset;

    // Setup internal state
    workBuffer.mCompressedHeaderData = &compressedData[remapData[0xC]];
    workBuffer.mCompressedData = compressedData;
    workBuffer.mNormalizedData = 0;
    workBuffer.mFrameOffset = 0;

    // Perform actual unpack and decompression for this frame
    Vector_UnPackFrameBlockAndDecompressOneFrame(
        &compressedData[remapData[0xC]],
        &workBuffer,
        frame & 0x7,
        threadId
    );

    uint32_t channelBlockCount = remapData[0xC];
    const float* fallbackData = reinterpret_cast<const float*>(
        ((workBuffer.mUsedTableOffset + 3) & ~0x3) * (frame & 0x7) + workBuffer.mNumBitsOffset
        );
    const float* primaryData = reinterpret_cast<const float*>(workBuffer.mNormalizedData);
    uint8_t* isConstantFlags = reinterpret_cast<uint8_t*>(workBuffer.mConstData);

    for (uint32_t iBlock = 0; iBlock < channelBlockCount; ++iBlock) {
        uint32_t blockOffset = ((0x10 * iBlock) & 0xFF0) + remapData[8];  // Possibly a base offset
        uint16_t start = *reinterpret_cast<const uint16_t*>(remapData + blockOffset);
        uint16_t count = *reinterpret_cast<const uint16_t*>(remapData + blockOffset + 2);
        uint8_t dataSize = remapData[blockOffset + 0xC];

        uint32_t totalChannels = static_cast<uint32_t>(start + count);
        if (totalChannels == 0)
            continue;

        size_t dataAdvance = (4 * dataSize) & 0x3FC;

        for (uint32_t ch = 0; ch < totalChannels; ++ch) {
            const float* source = isConstantFlags[ch] ? primaryData : fallbackData;

            mChannelBlocks->PutData(frame, ch, source, putLoc);

            if (isConstantFlags[ch]) {
                primaryData = reinterpret_cast<const float*>(
                    reinterpret_cast<const uint8_t*>(primaryData) + dataAdvance
                    );
            }
            else {
                fallbackData = reinterpret_cast<const float*>(
                    reinterpret_cast<const uint8_t*>(fallbackData) + dataAdvance
                    );
            }
        }
    }
}

uint32_t ANIM_CODEC::VBRDecompressor::UnPackFrameBlockBits(
    int* a3,
    uint32_t* a4)
{
    // Extract context values from the state pointer
    const uint32_t frameOffset = a4[0xB]; // Frame offset in the block
    const uint32_t* compressedData = reinterpret_cast<uint32_t*>(a4[5] + *a3);
    const uint32_t numChannels = *reinterpret_cast<uint8_t*>(*a4 + 0xC);
    int32_t* outputBuffer = reinterpret_cast<int32_t*>(a4[1]);

    // Update context with current position
    a4[3] = reinterpret_cast<uint32_t>(compressedData + numChannels);

    // Read initial 32-bit value from compressed data (big-endian)
    const uint32_t* dataPtr = compressedData + numChannels;
    uint32_t bitBuffer = (Convert(*dataPtr));

    // Skip the 4-byte header if there are frames to process
    if (frameOffset != 0) {
        dataPtr += 4;
    }

    uint32_t bitsAvailable = 32; // Tracks how many valid bits are in bitBuffer
    const uint8_t* controlPtr = reinterpret_cast<uint8_t*>(a4[4]);

    // Process each frame in the block
    for (uint32_t frame = 0; frame < frameOffset; ++frame) {
        int32_t* frameOutput = outputBuffer + frame;
        uint8_t controlShift = 0;

        // Process 8 channels per frame
        for (uint32_t channel = 0; channel < 8; ++channel) {
            int32_t value = 0;

            // Extract 4-bit control value
            uint32_t nibble = (*controlPtr >> (4 * (controlShift & 1))) & 0xF;
            controlShift++;

            if (nibble != 0) {
                if (bitBuffer & 1) {
                    // Extract variable-length value
                    uint32_t bitCount = nibble;
                    uint32_t mask = (1 << bitCount) - 1;

                    value = (bitBuffer >> 2) & mask;
                    if ((bitBuffer & 2) == 0) {
                        value = -value; // Apply sign
                    }

                    // Consume bits
                    bitBuffer >>= bitCount + 2;
                    bitsAvailable -= bitCount + 2;
                }
                else {
                    // Zero value
                    bitBuffer >>= 1;
                    bitsAvailable--;
                }

                // Refill bit buffer if running low
                if (bitsAvailable <= 16) {
                    uint32_t newBits = (dataPtr[1] << 8) | dataPtr[0];
                    bitBuffer |= newBits << bitsAvailable;
                    dataPtr += 2;
                    bitsAvailable += 16;
                }
            }

            // Store decompressed value with frame stride
            *frameOutput = value;
            frameOutput += frameOffset;

            // Advance control pointer every second channel
            if (channel & 1) {
                controlPtr++;
            }
        }

        // Move to next frame's base output position
        outputBuffer++;
    }

    return bitsAvailable;
}


const uint8_t* ANIM_CODEC::VBRDecompressor::UnPackHeaderBits(DataPerDecompress* pWorkBuffer) {
    // Aliases for convenience
    VBRDataHeader* hdr = reinterpret_cast<VBRDataHeader*>(pWorkBuffer->mhdr);
    const uint8_t* compressedHeaderData = reinterpret_cast<const uint8_t*>(pWorkBuffer->mCompressedHeaderData);
    float* constDataOut = reinterpret_cast<float*>(pWorkBuffer->mConstData);

    // Dereference the pointers to get internal structures
    auto* dataValue = reinterpret_cast<VBRDataHeaderValue*>(hdr->mData);
    auto* constantPalette = reinterpret_cast<const float*>(hdr->mConstantPaletteData);
    auto* typeHdrArray = reinterpret_cast<VBRChannelTypeHdr*>(hdr->mChannelTypeInfo);

    uint8_t* isConstantChannelMap = reinterpret_cast<uint8_t*>(pWorkBuffer->mIsConstantChannel);
    uint32_t numChannels = dataValue->mNumChannels;
    const uint8_t* in = compressedHeaderData;

    // Unpack constant channel values
    for (uint32_t ch = 0; ch < numChannels; ++ch) {
        auto& typeHdr = typeHdrArray[ch];
        float min = typeHdr.Min;
        float max = typeHdr.Max;
        float range = max - min;

        uint8_t size = typeHdr.Size;
        uint16_t constEntries = typeHdr.NumConstEntPerFrm;
        uint16_t entriesPerFrame = typeHdr.NumEntPerFrm;
        int totalEntries = size * constEntries;

        for (int i = 0; i < totalEntries; ++i, ++constDataOut, ++in) {
            uint8_t index = *in;
            float scale = constantPalette[index];
            *constDataOut = scale * range + min;
        }
    }

    // Decode constant channel map (alternating runs)
    uint16_t constMapSize = dataValue->mConstChanMapSize;
    bool currentVal = false;

    for (int i = 0; i < constMapSize; ++i, ++in) {
        uint8_t count = *in;
        std::memset(isConstantChannelMap, currentVal ? 1 : 0, count);
        isConstantChannelMap += count;
        currentVal = !currentVal;
    }

    // If constMapSize is 0, default all channels to 0
    if (constMapSize == 0 && numChannels > 0) {
        std::memset(isConstantChannelMap, 0, numChannels);
    }

    // Calculate offset
    const uint8_t* base = compressedHeaderData;
    uint32_t frameOffsetBytes = (pWorkBuffer->mFrameOffset * 4) & 0xFFFFFFFC;
    pWorkBuffer->mNumBitsOffset = static_cast<uint32_t>(in - base);
    return in + (frameOffsetBytes - pWorkBuffer->mNumBitsOffset);
}

uint32_t ANIM_CODEC::VBRDecompressor::InitPerAnim(const uint8_t* compressedHeaderData, const uint8_t* headerData, uint8_t threadId) {
    // Alignment check for header data (must be 16-byte aligned)
    if (reinterpret_cast<uint32_t>(headerData) % 16 != 0) {
        std::cout << "Header must be aligned on 16 bytes\n";
    }

    if (threadId >= 6) {
        std::cout << "VBRDecompressor::threadId exceeds number of parallel work buffers...\n";
    }

    // Per-thread data layout
    char* threadBase = reinterpret_cast<char*>(this) + threadId * 0x40;
    auto* workBuffer = reinterpret_cast<DataPerDecompress*>(threadBase + 0x2430);
    auto* headerInfo = reinterpret_cast<uint32_t*>(threadBase + 0x2430 + 0x240); // assumes offset 0x240 from DataPerDecompress
    headerInfo[5] = reinterpret_cast<uint32_t>(compressedHeaderData); // mCompressedHeaderData

    // Validate header is already parsed or parse it
    const uint8_t*& headerPtr = *reinterpret_cast<const uint8_t**>(headerInfo);
    if (headerPtr != headerData) {
        headerPtr = headerData;
        reinterpret_cast<const uint8_t*&>(headerInfo[1]) = headerData + 0x10;
        reinterpret_cast<uint8_t&>(headerInfo[4]) = 1;

        uint32_t offset = (headerData[0xD] << 2) | (headerData[0xD] >> (32 - 2));
        const uint8_t* channelInfoBase = headerData + 0x10 + offset;
        headerInfo[3] = reinterpret_cast<uint32_t>(channelInfoBase);

        // Compute the aligned offset to next section (normalized data)
        uint16_t channelMapSize = *reinterpret_cast<const uint16_t*>(headerData + 2);
        uint32_t sizeBytes = 2 * ((channelMapSize + 7) / 8);
        const uint8_t* normalizedPtr = &channelInfoBase[sizeBytes + 0xF];
        headerInfo[2] = reinterpret_cast<uint32_t>(reinterpret_cast<const void*>(
            reinterpret_cast<uint32_t>(normalizedPtr) & ~0xF));
    }

    // Validate number of channel blocks
    uint8_t numChannelBlocks = *reinterpret_cast<const uint8_t*>(headerPtr + 0xC);
    if (numChannelBlocks != this->mNumChannelBlocks) {
        std::cout << "Must have the same number of callbacks as channels\n";
    }

    // Compute block start/end and validate sizes
    uint32_t totalConstChannels = 0;
    for (uint32_t i = 0; i < numChannelBlocks; ++i) {
        int offset = (i * 0x10) & 0xFF0;
        uint8_t dataSize = *reinterpret_cast<const uint8_t*>(headerInfo[2] + offset + 0xC);

        uint16_t blockStart = (i == 0) ? 0 : workBuffer->mChannelBlockStarts[i + 3];
        workBuffer->mChannelBlockStarts[i] = blockStart;

        uint16_t entries = *reinterpret_cast<const uint16_t*>(headerInfo[2] + offset);
        workBuffer->mChannelBlockEnds[i] = entries * dataSize + blockStart;

        uint16_t blockConstEntries = *reinterpret_cast<const uint16_t*>(headerInfo[2] + offset + 2);
        totalConstChannels += blockConstEntries * dataSize;

        // Validate size
        if (this->mChannelBlocks[i].mDataSize != dataSize) {
            std::cout << "mChannelBlocks[curBlock].mDataSize==channelBlockSize\n";
        }
    }

    workBuffer->mNumConstantChannels = totalConstChannels;
    workBuffer->mFrameOffset = workBuffer->mChannelBlockStarts[numChannelBlocks + 3];

    // Zero out thread-local memory
    char* threadMem = reinterpret_cast<char*>(this) + 0x804 * threadId;
    *reinterpret_cast<uint32_t*>(threadMem + 0x25AC) = 0;
    memset(threadMem + 0x25B0, 0, 0x800);

    // Unpack remaining header bits and store memory header size
    uint32_t result = (uint32_t)this->UnPackHeaderBits(workBuffer);
    workBuffer->mMemoryHeaderSize = result;

    return result;
}

void Andale::TrajectoryCalculation(
    Andale::SQTData* trajBone,
    bool channelAnim,
    double channelCoef,
    const rw::math::vpu::Quaternion* prevTrajQuat,
    const rw::math::vpu::Vector3* prevTrajTrans,
    const rw::math::vpu::Quaternion* deltaQ,
    const rw::math::vpu::Vector3* deltaT
) {
    using namespace rw::math::vpu;

    // Extract prevTrajQuat components
    float pW = prevTrajQuat->GetW();
    float pX = prevTrajQuat->GetX();
    float pY = prevTrajQuat->GetY();
    float pZ = prevTrajQuat->GetZ();

    // Extract deltaQ components
    float dW = deltaQ->GetW();
    float dX = deltaQ->GetX();
    float dY = deltaQ->GetY();
    float dZ = deltaQ->GetZ();

    // Quaternion multiplication: updatedQuat = prevTrajQuat * deltaQ
    float uW = pW * dW - pX * dX - pY * dY - pZ * dZ;
    float uX = pW * dX + pX * dW + pY * dZ - pZ * dY;
    float uY = pW * dY - pX * dZ + pY * dW + pZ * dX;
    float uZ = pW * dZ + pX * dY - pY * dX + pZ * dW;

    // Normalize quaternion
    float length = std::sqrt(uW * uW + uX * uX + uY * uY + uZ * uZ);
    uW /= length;
    uX /= length;
    uY /= length;
    uZ /= length;

    // Set updated quaternion to trajBone
    trajBone->mQuat.SetW(uW);
    trajBone->mQuat.SetX(uX);
    trajBone->mQuat.SetY(uY);
    trajBone->mQuat.SetZ(uZ);

    // Rotate deltaT by prevTrajQuat: v' = q * v * q_conj
    // Quaternion conjugate of prevTrajQuat
    float cW = pW;
    float cX = -pX;
    float cY = -pY;
    float cZ = -pZ;

    // Convert deltaT to quaternion vQuat (w=0)
    float vX = deltaT->GetX();
    float vY = deltaT->GetY();
    float vZ = deltaT->GetZ();

    // q * vQuat
    float tW = pW * 0.0f - pX * vX - pY * vY - pZ * vZ;
    float tX = pW * vX + pX * 0.0f + pY * vZ - pZ * vY;
    float tY = pW * vY - pX * vZ + pY * 0.0f + pZ * vX;
    float tZ = pW * vZ + pX * vY - pY * vX + pZ * 0.0f;

    // temp * q_conj
    float rW = tW * cW - tX * cX - tY * cY - tZ * cZ;
    float rX = tW * cX + tX * cW + tY * cZ - tZ * cY;
    float rY = tW * cY - tX * cZ + tY * cW + tZ * cX;
    float rZ = tW * cZ + tX * cY - tY * cX + tZ * cW;

    // rotated delta vector (ignoring w)
    Vector3 rotatedDeltaT(rX, rY, rZ);

    // Update translation
    Vector3 updatedTrans = *prevTrajTrans + rotatedDeltaT;
    trajBone->mTrans = updatedTrans;

    // Optional override if channelAnim enabled
    if (channelAnim) {
        trajBone->mQuat = *prevTrajQuat;
        trajBone->mTrans = *prevTrajTrans;
    }
}

void Andale::ExtractPackedNVBR(ExtractType* extractBuffer, uint32_t extractBuffSize, ANIM_CODEC::VBRDecompressor* decompressor)
{
    if (!extractBuffer || extractBuffSize == 0 || !decompressor) {
        std::cerr << "[ExtractPackedNVBR] Invalid input parameters.\n";
        return;
    }

    for (uint32_t animIdx = 0; animIdx < extractBuffSize; ++animIdx) {
        ExtractType* animData = extractBuffer + animIdx;
        if (!animData) {
            std::cerr << "[ExtractPackedNVBR] Null animData pointer at index " << animIdx << "\n";
            continue;
        }

        if (!animData->sqtBuff) {
            std::cerr << "[ExtractPackedNVBR] sqtBuff is null at index " << animIdx << "\n";
            continue;
        }

        uint32_t* somePointer = reinterpret_cast<uint32_t*>(animData->header);
        if (!somePointer) {
            // No animation data: copy raw sqtBuff to extraOut if possible
            if (animData->extraOut) {
                // Assuming frameCount is meaningful here, else set a default safe size or skip
                uint32_t frameCount = animData->totalBones;
                if (frameCount > 0) {
                    memcpy(reinterpret_cast<void*>(animData->extraOut), reinterpret_cast<void*>(animData->sqtBuff), 0x2C * frameCount);
                }
                else {
                    std::cerr << "[ExtractPackedNVBR] Invalid frameCount (0) when copying raw data at index " << animIdx << "\n";
                }
            }
            continue;
        }

        uint16_t frameStart = animData->data[0];
        uint16_t frameEnd = animData->data[1];
        uint16_t frameCount = animData->totalBones;  // Make sure this is correct for your data!
        uint16_t frameIndex = animData->prev[0];     // example previous frame
        uint8_t hasScale = animData->codecType;      // assuming codecType as hasScale flag, adjust if needed

        // Validate frameCount
        if (frameCount == 0) {
            std::cerr << "[ExtractPackedNVBR] frameCount is zero at index " << animIdx << "\n";
            continue;
        }

        // Calculate decompressed output pointer safely
        if (animData->currblend >= frameCount) {
            std::cerr << "[ExtractPackedNVBR] currblend out of bounds at index " << animIdx << "\n";
            continue;
        }
        char* decompressedOutput = reinterpret_cast<char*>(animData->sqtBuff) + 0x2C * animData->currblend;

        // Extract frameBlockSize at offset: somePointer + 3 (careful with pointer arithmetic)
        int frameBlockSize = *(reinterpret_cast<int*>(reinterpret_cast<char*>(somePointer) + 3 * sizeof(uint32_t)));

        // Extract numFrames from bits of somePointer[0]
        int numFrames = ((*somePointer >> 6) & 0x3F) + 1;

        // Initialize decompressor per animation - adjust parameters as needed
        decompressor->InitPerAnim(
            reinterpret_cast<const uint8_t*>(somePointer) + frameBlockSize,
            reinterpret_cast<const uint8_t*>(somePointer),
            0 /* flags or threadId, adjust as needed */
        );

        if (frameStart == frameEnd) {
            // Decompress single frame for all frames
            decompressor->DecompressFrame(
                reinterpret_cast<const uint8_t*>(somePointer) + frameBlockSize,
                frameEnd,
                reinterpret_cast<uint32_t>(decompressedOutput),
                0 // threadId (example)
            );

            // Handle scaling if applicable
            if (hasScale) {
                // Scale data pointer: somePointer + 4 * sizeof(uint32_t) (adjust according to your data layout!)
                float* scaleData = reinterpret_cast<float*>(reinterpret_cast<char*>(somePointer) + 4 * sizeof(uint32_t));
                for (int i = 0; i < numFrames; ++i) {
                    float scaleVal = (animData->prev[1]) ? scaleData[i] : 0.0f;
                    // Offset 10 floats into decompressed output per frame: ensure decompressedOutput is large enough
                    float* targetScale = reinterpret_cast<float*>(decompressedOutput + i * 0x2C) + 10;
                    *targetScale = scaleVal;
                }
            }
        }
        else {
            std::cerr << "[ExtractPackedNVBR] frameStart != frameEnd not supported at index " << animIdx << "\n";
            continue;
        }

        // Trajectory calculation - only if hasScale flag is set (adjust as needed)
        if (hasScale) {
            rw::math::vpu::Vector3 pos = {
                *reinterpret_cast<float*>(reinterpret_cast<char*>(animData) + 0x10),
                *reinterpret_cast<float*>(reinterpret_cast<char*>(animData) + 0x14),
                *reinterpret_cast<float*>(reinterpret_cast<char*>(animData) + 0x18)
            };

            Andale::SQTData* trajBone = reinterpret_cast<Andale::SQTData*>(decompressedOutput);
            bool channelAnim = animData->prev[1] != 0;
            double channelCoef = 1.0; // adjust as needed

            rw::math::vpu::Quaternion prevTrajQuat;
            prevTrajQuat.SetW(1.0f); prevTrajQuat.SetX(0.0f); prevTrajQuat.SetY(0.0f); prevTrajQuat.SetZ(0.0f);

            rw::math::vpu::Quaternion deltaQ;
            deltaQ.SetW(1.0f); deltaQ.SetX(0.0f); deltaQ.SetY(0.0f); deltaQ.SetZ(0.0f);

            const rw::math::vpu::Vector3* prevTrajTrans = &pos;

            rw::math::vpu::Vector3 deltaT;
            deltaT.SetX(0.0f); deltaT.SetY(0.0f); deltaT.SetZ(0.0f);

            Andale::TrajectoryCalculation(
                trajBone,
                channelAnim,
                channelCoef,
                &prevTrajQuat,
                prevTrajTrans,
                &deltaQ,
                &deltaT
            );
        }

        // Copy decompressed data to output buffer if extraOut is valid
        if (animData->extraOut) {
            memcpy(reinterpret_cast<void*>(animData->extraOut), decompressedOutput, 0x2C * frameCount);
        }
    }
}

void Andale::Scalar_ExtractPackedNVBR(ExtractType* extractBuffer, uint32_t extractBuffSize, ANIM_CODEC::VBRDecompressor* decompressor)
{
    if (extractBuffSize == 0) {
        return;
    }

    static SQTData prevFrame;  // Store previous trajectory frame for continuity

    for (uint32_t i = 0; i < extractBuffSize; ++i) {
        ExtractType& extract = extractBuffer[i];

        AnimationPart* part = reinterpret_cast<AnimationPart*>(extract.header);
        if (!part) continue;

        const uint8_t* compressedHeader = reinterpret_cast<const uint8_t*>(part) + 0xC;
        const uint8_t* compressedData = reinterpret_cast<const uint8_t*>(part->mCompressedData);
        uint32_t channelCount = ((part->mHeader >> 6) & 0x3F) + 1;

        uint16_t currFrame = extract.data[0];
        uint16_t nextFrame = extract.data[1];

        // Offsets from part header
        uint32_t offset2 = *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(part) + 8);
        const uint8_t* ptr2 = reinterpret_cast<const uint8_t*>(part) + offset2;

        // Initialize decompressor with compressed header and decompression header ptr
        decompressor->InitPerAnim(compressedHeader, ptr2, 0);

        SQTData* outputBuffer = reinterpret_cast<SQTData*>(extract.sqtBuff);

        // Decompress only the next frame (no interpolation)
        decompressor->DecompressFrame(compressedData, nextFrame, reinterpret_cast<const uint32_t>(&outputBuffer), 0);

        // Pointer to scaleZ floats at part + 0x10
        const float* scaleZValues = reinterpret_cast<const float*>(reinterpret_cast<const uint8_t*>(part) + 0x10);

        for (uint32_t j = 0; j < channelCount; ++j) {
            outputBuffer[j].mScale.SetZ(extract.channelAnim ? scaleZValues[j] : extract.deltaT[0]);
        }

        if (extract.doTraj) {
            if (extract.currblend == 0) {
                prevFrame = outputBuffer[0];
                extract.data[0] = extract.prev[0];
                extract.data[1] = extract.prev[1];
                extract.channelAnim = extract.headersize;
            }
            else {
                rw::math::vpu::Vector3 oldPos(extract.deltaT[0], extract.deltaT[1], extract.deltaT[2]);
                rw::math::vpu::Vector3 newPos(
                    prevFrame.mScale.GetX(),
                    prevFrame.mScale.GetY(),
                    prevFrame.mScale.GetZ()
                );

                Andale::TrajectoryCalculation(
                    outputBuffer,
                    extract.channelAnim != 0,
                    static_cast<double>(extract.deltaQ.GetW()),
                    &prevFrame.mQuat,
                    &newPos,
                    &extract.deltaQ,
                    &oldPos
                );
            }
        }

        if (extract.extraOut) {
            memcpy(
                reinterpret_cast<void*>(extract.extraOut),
                reinterpret_cast<void*>(extract.sqtBuff),
                sizeof(SQTData) * extract.totalBones
            );
        }
    }
}

void ANIM_CODEC::RawDiscretize::TransDecompress(const tTransCompressed& input, rw::math::vpu::Vector4& output, float range)
{
    const float invScale = range / 32768.0f;

    // Decompress each int16 into float
    output.SetX(static_cast<float>(input.t[0]) * invScale);
    output.SetY(static_cast<float>(input.t[1]) * invScale);
    output.SetZ(static_cast<float>(input.t[2]) * invScale);
    output.SetW(0.0f); // W is unused
}

void ANIM_CODEC::RawDiscretize::TransCompress(const rw::math::vpu::Vector4& input, tTransCompressed& output, float range)
{
    const float maxInt16 = 32767.0f;
    const float scale = maxInt16 / range;

    output.t[0] = static_cast<int16_t>(std::lrintf(Clamp(input.GetX(), -range, range) * scale));
    output.t[1] = static_cast<int16_t>(std::lrintf(Clamp(input.GetY(), -range, range) * scale));
    output.t[2] = static_cast<int16_t>(std::lrintf(Clamp(input.GetZ(), -range, range) * scale));
}

void ANIM_CODEC::RawDiscretize::QuatDecompress(uint32_t qcomp, rw::math::vpu::Quaternion* q) {
    constexpr float PI = 3.14159265358979323846f;
    constexpr float TWO_PI = 2.0f * PI;
    
    // Extract bit fields
    unsigned int qa = (qcomp >> 21) & 0x7FF; // 11 bits
    unsigned int qb = (qcomp >> 11) & 0x3FF; // 10 bits
    unsigned int qc = qcomp & 0x7FF;         // 11 bits

    // Convert quantized angles/scales back to float
    float angleA = ((float)qa / 2047.0f) * TWO_PI - PI;
    float scale = (float)qb / 1023.0f;
    float angleB = ((float)qc / 2047.0f) * TWO_PI - PI;

    // Wrap angles in VecFloat (broadcast scalar)
    rw::math::vpu::VecFloat vecAngleA(angleA);
    rw::math::vpu::VecFloat vecAngleB(angleB);

    // Get sin and cos packed in Vector4: (sinA, cosA, sinB, cosB)
    float sinA = rw::math::VecFloat(sinf(vecAngleA.GetFloat()));
    float cosA = rw::math::VecFloat(cosf(vecAngleA.GetFloat()));
    float sinB = rw::math::VecFloat(sinf(vecAngleB.GetFloat()));
    float cosB = rw::math::VecFloat(cosf(vecAngleB.GetFloat()));

    // Construct quaternion components
    float x = sinB * scale;
    float y = cosB * scale;
    float z = sinA;
    float w = cosA;

    q->SetX(x);
    q->SetY(y);
    q->SetZ(z);
    q->SetW(w);
}

void ANIM_CODEC::RawDiscretize::QuatCompress(rw::math::vpu::Quaternion* q, uint32_t* qcomp) {
    constexpr float PI = 3.14159265358979323846f;
    constexpr float TWO_PI = 2.0f * PI;

    rw::math::vpu::Quaternion normQ = *q;
    //normQ.Normalize();

    // Step 2: Extract quaternion components
    float x = normQ.GetX();
    float y = normQ.GetY();
    float z = normQ.GetZ();
    float w = normQ.GetW();

    // Step 3: Recover scale and angles
    float scale = std::sqrt(x * x + y * y);         // Scale used during compression
    float angleA = std::atan2(z, w);                // Inverse of sinA / cosA
    float angleB = std::atan2(x, y);                // Inverse of sinB / cosB

    // Step 4: Clamp values for robustness
    angleA = std::fmod(angleA + PI, TWO_PI);        // Convert to [0, 2π]
    if (angleA < 0) angleA += TWO_PI;
    angleA -= PI;                                   // Convert to [-π, π]

    angleB = std::fmod(angleB + PI, TWO_PI);
    if (angleB < 0) angleB += TWO_PI;
    angleB -= PI;

    scale = clamp(scale, 0.0f, 1.0f);

    // Step 5: Quantize each component to integer
    uint32_t qa = static_cast<uint32_t>(((angleA + PI) / TWO_PI) * 2047.0f + 0.5f) & 0x7FF; // 11 bits
    uint32_t qb = static_cast<uint32_t>(scale * 1023.0f + 0.5f) & 0x3FF;                    // 10 bits
    uint32_t qc = static_cast<uint32_t>(((angleB + PI) / TWO_PI) * 2047.0f + 0.5f) & 0x7FF; // 11 bits

    // Step 6: Pack bits into 32-bit integer: [qa (11)] [qb (10)] [qc (11)]
    *qcomp = (qa << 21) | (qb << 11) | qc;
}

void NormalizeQuaternion(float* q) {
    float len2 = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
    if (len2 > 0.0f) {
        float invLen = 1.0f / std::sqrt(len2);
        for (int i = 0; i < 4; ++i)
            q[i] *= invLen;
    }
}

void ReconstructQuaternion(float* dst, const float* src, float blend) {
    for (int i = 0; i < 3; ++i)
        dst[i] = src[i] + src[i + 3] * blend;
    float dot = dst[0] * dst[0] + dst[1] * dst[1] + dst[2] * dst[2];
    dst[3] = std::sqrt(std::max(0.0f, 1.0f - dot));
    NormalizeQuaternion(dst);
}

inline void NormalizeQuaternion(rw::math::vpu::Quaternion& q) {
    float x = q.GetX();
    float y = q.GetY();
    float z = q.GetZ();
    float w = q.GetW();

    float lenSq = x * x + y * y + z * z + w * w;

    if (lenSq > 1e-6f) {
        float invLen = 1.0f / std::sqrt(lenSq);
        q.SetX(x * invLen);
        q.SetY(y * invLen);
        q.SetZ(z * invLen);
        q.SetW(w * invLen);
    }
    else {
        // Set to identity quaternion as fallback
        q.SetX(0.0f);
        q.SetY(0.0f);
        q.SetZ(0.0f);
        q.SetW(1.0f);
    }
}

void StoreQuaternion(rw::math::vpu::Quaternion* dst, const rw::math::vpu::Quaternion& src) {
    std::memcpy(dst, &src, sizeof(rw::math::vpu::Quaternion));
}

void Andale::ExtractPackedN(ExtractType* extractBuffer, uint32_t extractBufferSize) {
    if (!extractBuffer || extractBufferSize == 0) return;

    for (uint32_t bufferIndex = 0; bufferIndex < extractBufferSize; ++bufferIndex) { //current bone group
        ExtractType& current = extractBuffer[bufferIndex];

        AnimationPart* animPart = reinterpret_cast<AnimationPart*>(current.header);

        uint32_t compressionHeader = Convert(animPart->mCompressionHeader);
        ANIM_CODEC::RawDiscretize::CompressionHeader* compHeader = reinterpret_cast<ANIM_CODEC::RawDiscretize::CompressionHeader*>(current.header + compressionHeader);

        uint16_t frameSize = Convert(compHeader->mFrameSize);
        uint8_t bonesPerFrame = compHeader->mNbBonesM1;
        bool CompressedT = compHeader->mCompressedT;
        bool CompressedQ = compHeader->mCompressedQ;

        uint32_t channelCount = ((Convert(animPart->mHeader) >> 6) & 0x3F) + 1;
        uint8_t* basePtr = reinterpret_cast<uint8_t*>(current.header);
        uint8_t* compressedData = basePtr + Convert(animPart->mCompressedData);


        SQTData* output = reinterpret_cast<SQTData*>(current.sqtBuff);

        if (bonesPerFrame == 0) {
            float fallbackW = current.channelAnim ? 0.0f : 1.0f;
            rw::math::vpu::Quaternion identityQuat = { fallbackW, 0.0f, 0.0f, 0.0f };
            StoreQuaternion(&output->mQuat, identityQuat);
            continue;
        }

        uint32_t* bitmask = reinterpret_cast<uint32_t*>(compHeader->mMask);

        uint16_t frameIndex = current.currblend;
        if (frameIndex == 0xFFFF) {
            frameIndex = 0;  // Fallback to frame 0
        }

        uint8_t* dataPointer = compressedData + frameIndex * frameSize;

        for (uint32_t i = 0; i < channelCount; ++i) { //num bones used
            //SQTData& outData = output[bufferIndex * bonesPerFrame + i];
            uint32_t wordIndex = i >> 5;
            uint32_t bitMask = 1 << (i & 0x1F);
            if (Convert(bitmask[wordIndex + 0]) & bitMask) {
                //scale
                std::cout << "scale\n";
                std::cout << "test1: " << std::hex << compHeader->undefined1 << "\n";
                std::cout << "test2: " << std::hex << compHeader->undefined2 << "\n";
                std::cout << "test3: " << std::hex << compHeader->undefined3 << "\n";
                rw::math::vpu::Vector3 scale{};
                float* floatPtr = reinterpret_cast<float*>(dataPointer);
                scale.SetX(Convert(floatPtr[0]));
                scale.SetY(Convert(floatPtr[1]));
                scale.SetZ(Convert(floatPtr[2]));
                dataPointer += 3 * sizeof(float);
                //outData.mScale = scale;
                //std::cout << "Scale: " << outData.mScale.GetX() << " " << outData.mScale.GetY() << " " << outData.mScale.GetZ() << "\n";
            }
            else {
                //outData.mScale = rw::math::vpu::Vector3 { 1.0, 1.0, 1.0 };
            }
            if (Convert(bitmask[wordIndex + 2]) & bitMask) {
                rw::math::vpu::Quaternion quat{};
                if (CompressedQ) {
                    std::cout << "quat compressed\n";
                    uint32_t compressedQuat = *reinterpret_cast<uint32_t*>(dataPointer);
                    ANIM_CODEC::RawDiscretize::QuatDecompress(Convert(compressedQuat), &quat);
                    dataPointer += sizeof(uint32_t);
                }
                else {
                    float* floatPtr = reinterpret_cast<float*>(dataPointer);
                    quat.SetX(Convert(floatPtr[0]));
                    quat.SetY(Convert(floatPtr[1]));
                    quat.SetZ(Convert(floatPtr[2]));
                    quat.SetW(Convert(floatPtr[3]));
                    dataPointer += 4 * sizeof(float);
                }
                //outData.mQuat = quat;
                //std::cout << "Quat: " << outData.mQuat.GetX() << " " << outData.mQuat.GetY() << " " << outData.mQuat.GetZ() << " " << outData.mQuat.GetW() << "\n";
            }
            if (Convert(bitmask[wordIndex + 4]) & bitMask) {
                rw::math::vpu::Vector4 trans{};
                if (CompressedT) {
                    std::cout << "trans compressed\n";
                    uint16_t* shortPtr = reinterpret_cast<uint16_t*>(dataPointer);
                    ANIM_CODEC::RawDiscretize::tTransCompressed transc;
                    transc.t[0] = Convert(shortPtr[0]);
                    transc.t[1] = Convert(shortPtr[1]);
                    transc.t[2] = Convert(shortPtr[2]);
                    float range = static_cast<float>(shortPtr[3]);
                    ANIM_CODEC::RawDiscretize::TransDecompress(transc, trans, range);
                    dataPointer += 4 * sizeof(uint16_t);
                }
                else {
                    float* floatPtr = reinterpret_cast<float*>(dataPointer);
                    trans.SetX(Convert(floatPtr[0]));
                    trans.SetY(Convert(floatPtr[1]));
                    trans.SetZ(Convert(floatPtr[2]));
                    dataPointer += 3 * sizeof(float);
                }
                //outData.mTrans = trans;
                //std::cout << "Trans: " << outData.mTrans.GetX() << " " << outData.mTrans.GetY() << " " << outData.mTrans.GetZ() << " " << outData.mTrans.GetW() << "\n";
            }
        }

        if (current.doTraj) {
            static SQTData prevFrame;
            static bool prevInitialized = false;

            if (!prevInitialized || current.currblend == 0) {
                prevFrame = output[0];
                prevInitialized = true;

                current.data[0] = current.prev[0];
                current.data[1] = current.prev[1];
                current.channelAnim = current.headersize;
            }
            else {
                rw::math::vpu::Vector3 oldPos = { current.deltaT[0], current.deltaT[1], current.deltaT[2] };
                rw::math::vpu::Vector3 prevPos = {
                    prevFrame.mTrans.GetX(),
                    prevFrame.mTrans.GetY(),
                    prevFrame.mTrans.GetZ()
                };
                /*
                TrajectoryCalculation(
                    &output[0],
                    current.channelAnim != 0,
                    static_cast<double>(current.deltaQ.GetW()),
                    &prevFrame.mQuat,
                    &prevPos,
                    &current.deltaQ,
                    &oldPos
                );

                prevFrame = output[0];
                */
            }
        }
    }
}

const Andale::DataHeader* Andale::DataBaseManager::GetPoseHeader(uint32_t poseGUID)
{
    //Decodes a compact pose GUID to locate and return 
    //a specific poses metadata (DataHeader).
    uint16_t dbIndex = poseGUID & 0x1F;

    const Andale::DataBase* db = reinterpret_cast<const Andale::DataBase*>(this->mDataBases[dbIndex]);

    if (!db)
    {
        std::cout << "couldn't load database!" << "\n";
        return nullptr;
    }

    size_t index_pose = poseGUID >> 5;
    auto& poses = db->mPoses;

    if (index_pose >= static_cast<size_t>(poses.end() - poses.begin()))
    {
        return nullptr;
    }

    return poses.begin()[index_pose].second; // assuming the pair is <FastString, DataHeader*>
}


const Andale::DataHeader* Andale::DataBaseManager::GetClipHeader(uint32_t animGUID)
{
    //Decodes a compact anim GUID to locate and return 
    //a specific clips metadata (DataHeader).
    uint16_t dbIndex = animGUID & 0x1F;

    const Andale::DataBase* db = reinterpret_cast<const Andale::DataBase*>(this->mDataBases[dbIndex]);

    if (!db)
    {
        std::cout << "couldn't load database!" << "\n";
        return nullptr;
    }

    size_t index_clip = animGUID >> 5;
    auto& clips = db->mClips;

    if (index_clip >= static_cast<size_t>(clips.end() - clips.begin()))
    {
        return nullptr;
    }

    return clips.begin()[index_clip].second; // assuming the pair is <FastString, DataHeader*>
}

void Andale::FetchSys::InitFetchSysForACS(
    uint32_t* MaxExtractRecords,
    uint32_t* ExtractRecordSize,
    uint32_t* MaxExtractsPerJob,
    uint32_t* MaxExtractsPerFetch,
    void** pJobCode,
    bool useJob,
    uint32_t decompressor
) {
    //This function initializes several configuration parameters 
    //for the Fetch System when used with Animation Command System mode.
    *MaxExtractRecords = this->mMaxFetches * reinterpret_cast<const HierarchyData*>(this->mSkel)->mNbParts;
    *ExtractRecordSize = 0x40;
    *MaxExtractsPerJob = (static_cast<codec_type>(this->mCodecType) == codec_type::RAW) ? 0x200 : 0x100;
    *MaxExtractsPerFetch = reinterpret_cast<const HierarchyData*>(this->mSkel)->mNbParts;

    if (useJob && this->mJobCode) {
        *pJobCode = reinterpret_cast<void*>(this->mJobCode);
    }
}

uint32_t Andale::FetchSys::FillExtractBuffers(
    bool isPose,
    uint32_t GUID,
    float prevTime,
    float currTime,
    uint32_t loops,
    bool blendFrames,
    xvectoralloc::XVectorPtr<AnimCmdSys::SQTExt>* output,
    uint32_t extraOutput,
    uint32_t extractBuffer,
    Andale::SQTData* optionalSQT,
    Andale::ExtractType* extractTypes
) {
    //fills animation extraction buffers, either for a pose(static frame) or an animated clip, 
    //and returns how many results were extracted.
    Andale::DataBaseManager* dbManager = reinterpret_cast<Andale::DataBaseManager*>(this->mDataBaseManager);
    const Andale::DataHeader* header = nullptr;
    Andale::SQTData* sqtBuffer = nullptr;
    Andale::ExtractType* extractArray = extractTypes;
    xvectoralloc::XVectorPtr<Andale::SQTData> tempBuffer[2] = {};  // Used to hold buffer during fetch
    uint32_t result = 0;

    if (isPose) {
        // Pose-based extraction
        header = dbManager->GetPoseHeader(GUID);
        sqtBuffer = *reinterpret_cast<Andale::SQTData**>(extractBuffer); // reinterpret cast from int to ptr
        tempBuffer[0].mVector = sqtBuffer;

        result = this->BatchFetch(header, tempBuffer, extractArray, 0xFFFFFFFF);
    }
    else {
        // Animation-based extraction
        header = dbManager->GetClipHeader(GUID);
        sqtBuffer = *reinterpret_cast<Andale::SQTData**>(extractBuffer);
        tempBuffer[0].mVector = sqtBuffer;

        result = this->BatchFetch(
            header,
            currTime,
            reinterpret_cast<xvectoralloc::XVectorPtr<Andale::SQTData>*>(output),
            reinterpret_cast<Andale::ExtractType*>(extraOutput),
            reinterpret_cast<int>(tempBuffer)
        );
    }

    // Store optional output if provided
    if (optionalSQT) {
        extractArray[result - 1].extraOut = reinterpret_cast<uint32_t>(optionalSQT);
    }

    return result;
}

int Andale::FetchSys::GetCodecType(uint32_t codecId) const {
    char v3[5] = {};  // ensure null-terminated just in case

    //assuming big-endian
    v3[3] = static_cast<char>(codecId & 0xFF);         // BYTE0
    v3[2] = static_cast<char>((codecId >> 8) & 0xFF);  // BYTE1
    v3[1] = static_cast<char>((codecId >> 16) & 0xFF); // BYTE2
    v3[0] = static_cast<char>((codecId >> 24) & 0xFF); // BYTE3

    if (std::strncmp(v3, "RD", 2) == 0) {
        return 0;
    }
    else if (std::strncmp(v3, "VBR", 3) == 0) {
        return 1;
    }
    else {
        return 2;
    }
}

int Andale::FetchSys::BatchFetch(
    const Andale::DataHeader* hdr,
    xvectoralloc::XVectorPtr<Andale::SQTData>* output,
    Andale::ExtractType* extractBuffer,
    int maxSize)
{
    if (!output || !output->mVector) {
        std::cout << "output.GetPtr() != NULL\n";
    }

    if (GetCodecType(hdr->mCodecCompactName) != mCodecType) {
        std::cout << "Animation Codec Types does not match what is expected from fetchsys initialization\n";
    }

    const Andale::HierarchyData* skeleton = reinterpret_cast<Andale::HierarchyData*>(mSkel);
    const uint32_t* alignedData = reinterpret_cast<const uint32_t*>(
        (reinterpret_cast<uintptr_t>(&hdr[1].mGUID) + 3) & ~0xF);

    uint32_t totalParts = Convert(alignedData[0]);
    const uint8_t* partIDs = reinterpret_cast<const uint8_t*>(&alignedData[1]);

    const uint16_t numBones = Convert(skeleton->mNbBones);
    const uint32_t numParts = Convert(skeleton->mNbParts);
    const uint16_t* boneIDs = reinterpret_cast<const uint16_t*>(&skeleton->mParents[2 * numBones]) + 0x11;

    int extractedCount = 0;
    int offsetIndex = 0;
    uint32_t iteration = 0;
    uint32_t matchedParts = 0;

    Andale::ExtractType* extractSlot = extractBuffer;

    for (; iteration < numParts; ++iteration, boneIDs += 0x12, offsetIndex += 9)
    {
        if (matchedParts >= totalParts) break;

        uint16_t boneID = Convert(*boneIDs);
        uint32_t matchedIndex = 0;
        bool matchFound = false;

        if (boneID == partIDs[0]) {
            matchedIndex = matchedParts++;
            partIDs += 4;
            matchFound = true;
        }
        else {
            uint32_t probe = matchedParts + 1;
            const uint8_t* probePtr = partIDs + 4;
            while (probe < totalParts && !matchFound) {
                if (boneID == *probePtr) {
                    matchedIndex = probe;
                    matchFound = true;
                }
                ++probe;
                probePtr += 4;
            }
        }

        if (matchFound) {
            uint32_t animPartOffset = Convert(alignedData[matchedIndex + 1]) & 0xFFFFFF;
            const Andale::AnimationPart* animPart = reinterpret_cast<const Andale::AnimationPart*>(
                reinterpret_cast<const uint8_t*>(alignedData) + animPartOffset);

            uint32_t animHeader = Convert(animPart->mHeader);

            int sqtOffset = Convert(*reinterpret_cast<const int*>(
                reinterpret_cast<const char*>(&skeleton[3].mNbBones) + (_rotl(numBones, 3)) + offsetIndex * 4));

            int totalBoneCount = Convert(skeleton[2].mParents[2 * numBones + offsetIndex]);

            if ((animHeader & 0xFFFFF000) == 0) {
                std::cout << "_header->GetCompressionHeaderSize() > 0\n";
            }

            uint32_t headerSize = 4 * (((animHeader >> 6) & 0x3F) + 5) + (animHeader >> 12);

            if (headerSize <= 0x14) {
                std::cout << "this->headersize > 20\n";
            }

            extractSlot->header = reinterpret_cast<uint32_t>(animPart);
            extractSlot->headersize = headerSize;
            extractSlot->sqtBuff = reinterpret_cast<uint32_t>(output->mVector);
            extractSlot->codecType = mCodecType;
            extractSlot->extraOut = 0;
            extractSlot->currblend = 0xFFFF;
            extractSlot->prevblend = 0xFFFF;
            extractSlot->totalBones = totalBoneCount;
            extractSlot->doTraj = 0;
            extractSlot->channelAnim = 0;
            extractSlot->data[0] = 0;
            extractSlot->data[1] = 0;
            extractSlot->prev[0] = 0;
            extractSlot->prev[1] = 0;
            extractSlot->sqtSize = numBones;
            extractSlot->sqtOffset = sqtOffset;

            ++extractSlot;
            ++extractedCount;

            if (maxSize >= 0 && extractedCount >= maxSize) {
                std::cout << "Fetch extract buffer has exceed MaxSize\n";
            }
        }
    }

    if (extractedCount == 0) {
        extractBuffer->header = 0;
        extractBuffer->extraOut = 0;
        extractBuffer->currblend = 0xFFFF;
        extractBuffer->prevblend = 0xFFFF;
        extractBuffer->totalBones = numBones;
        extractBuffer->sqtBuff = reinterpret_cast<uint32_t>(output->mVector);
        extractBuffer->doTraj = 0;
        extractBuffer->channelAnim = 0;
        extractBuffer->data[0] = 0;
        extractBuffer->data[1] = 0;
        extractBuffer->prev[0] = 0;
        extractBuffer->prev[1] = 0;
        extractBuffer->sqtSize = numBones;
        extractBuffer->sqtOffset = 0;
        extractedCount = 1;
    }

    return extractedCount;
}

int Andale::FetchSys::BatchFetch(
    const Andale::DataHeader* hdr,
    uint32_t frameIndex,
    xvectoralloc::XVectorPtr<Andale::SQTData>* output,
    Andale::ExtractType* extractBuffer,
    int maxSize)
{
    if (!output || !output->mVector) {
        std::cout << "output.GetPtr() != NULL\n";
    }

    if (GetCodecType(hdr->mCodecCompactName) != mCodecType) {
        std::cout << "Animation Codec Types does not match what is expected from fetchsys initialization\n";
    }

    const Andale::HierarchyData* skeleton = reinterpret_cast<Andale::HierarchyData*>(mSkel);
    const uint32_t* alignedData = reinterpret_cast<const uint32_t*>(
        (reinterpret_cast<uintptr_t>(&hdr[1].mGUID) + 3) & ~0xF);

    const AnimationData* animData = reinterpret_cast<const AnimationData*>(alignedData);

    float fps = Convert(animData->mFPS);
    float nbFrames = Convert(animData->mNbFrames);
    float baseSpeed = Convert(animData->mBaseSpeed);
    uint32_t attributesOffset = Convert(animData->mAttributesOffset);
    uint32_t blendPosePartsOffsetsOffset = Convert(animData->mBlendPosePartsOffsetsOffset);
    uint32_t bitField = animData->mBitField;
    uint32_t totalParts = bitField & 0x3F;
    uint32_t partsOffsets = Convert(animData->mPartsOffsets[0]);

    if (fps <= 0.0f || nbFrames < 1.0f)
        return 0;

    const uint8_t* partIDs = reinterpret_cast<const uint8_t*>(&animData->mPartsOffsets[0]);

    const uint16_t numBones = Convert(skeleton->mNbBones);
    const uint32_t numParts = Convert(skeleton->mNbParts);
    const uint16_t* boneIDs = reinterpret_cast<const uint16_t*>(&skeleton->mParents[2 * numBones]) + 0x11;

    int extractedCount = 0;
    int offsetIndex = 0;
    uint32_t iteration = 0;
    uint32_t matchedParts = 0;

    Andale::ExtractType* extractSlot = extractBuffer;

    for (; iteration < numParts; ++iteration, boneIDs += 0x12, offsetIndex += 9)
    {
        if (matchedParts >= totalParts) break;

        uint16_t boneID = Convert(boneIDs[0]);
        uint32_t matchedIndex = 0;
        bool matchFound = false;

        if (boneID == partIDs[0]) {
            matchedIndex = matchedParts++;
            partIDs += 4;
            matchFound = true;
        }
        else {
            uint32_t probe = matchedParts + 1;
            const uint8_t* probePtr = partIDs + 4;
            while (probe < totalParts && !matchFound) {
                if (boneID == *probePtr) {
                    matchedIndex = probe;
                    matchFound = true;
                }
                ++probe;
                probePtr += 4;
            }
        }

        if (matchFound) {
            uint32_t animPartOffset = Convert(animData->mPartsOffsets[matchedIndex]) & 0xFFFFFF;
            const Andale::AnimationPart* animPart = reinterpret_cast<const Andale::AnimationPart*>(
                reinterpret_cast<const uint8_t*>(alignedData) + animPartOffset);

            uint32_t animHeader = Convert(animPart->mHeader);

            int sqtOffset = Convert(*reinterpret_cast<const int*>(
                reinterpret_cast<const char*>(&skeleton[3].mNbBones) + (_rotl(numBones, 3)) + offsetIndex * 4));

            int totalBoneCount = Convert(skeleton[2].mParents[2 * numBones + offsetIndex]);

            if ((animHeader & 0xFFFFF000) == 0) {
                std::cout << "_header->GetCompressionHeaderSize() > 0\n";
            }

            uint32_t headerSize = 4 * (((animHeader >> 6) & 0x3F) + 5) + (animHeader >> 12);

            if (headerSize <= 0x14) {
                std::cout << "this->headersize > 20\n";
            }

            extractSlot->header = reinterpret_cast<uint32_t>(animPart);
            extractSlot->headersize = headerSize;
            extractSlot->sqtBuff = reinterpret_cast<uint32_t>(output->mVector);
            extractSlot->codecType = mCodecType;
            extractSlot->extraOut = 0;
            extractSlot->prevblend = 0xFFFF;
            extractSlot->currblend = frameIndex;
            extractSlot->totalBones = totalBoneCount;
            extractSlot->channelAnim = 0;
            extractSlot->data[0] = 0;
            extractSlot->data[1] = 0;
            extractSlot->prev[0] = 0;
            extractSlot->prev[1] = 0;
            extractSlot->sqtSize = numBones;
            extractSlot->sqtOffset = sqtOffset;

            if (skeleton->mHasTrajectory) {
                extractSlot->deltaQ = Convert(animData->mTrajQuatDelta);
                rw::math::vpu::Vector3 traj = Convert(animData->mTrajTransDelta);
                extractSlot->deltaT[0] = traj.GetX();
                extractSlot->deltaT[1] = traj.GetY();
                extractSlot->deltaT[2] = traj.GetZ();
                extractSlot->doTraj = 1;
            }
            else {
                extractSlot->deltaQ = rw::math::vpu::Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
                extractSlot->deltaT[0] = extractSlot->deltaT[1] = extractSlot->deltaT[2] = 0.0f;
                extractSlot->doTraj = 0;
            }

            ++extractSlot;
            ++extractedCount;

            if (maxSize >= 0 && extractedCount >= maxSize) {
                std::cout << "Fetch extract buffer has exceed MaxSize\n";
            }
        }
    }

    if (extractedCount == 0) {
        Andale::ExtractType& extract = extractBuffer[0];
        extract = {};
        extract.header = 0;
        extract.sqtBuff = reinterpret_cast<uint32_t>(output->mVector);
        extract.currblend = 0xFFFF;
        extract.prevblend = 0xFFFF;
        extract.totalBones = Convert(skeleton->mNbBones);
        extract.sqtSize = Convert(skeleton->mNbBones);
        extract.sqtOffset = 0;
        extract.deltaQ = rw::math::vpu::Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
        extract.deltaT[0] = extract.deltaT[1] = extract.deltaT[2] = 0.0f;
        extract.doTraj = 0;
        extractedCount = 1;
    }

    return extractedCount;
}

int Andale::DataBaseManager::FindFreeSlot() {
    for (int i = 0; i < static_cast<int>(mMaxDatabases); ++i) {
        if (mDataBases[i] == 0) { // 0 means the slot is free
            return i;
        }
    }

    // No free slot found
    std::cout << "DataBaseManager has run out of DBIndexes\n";
    return -1; // Fallback return
}

Andale::DataBase::DataBase(DataBaseManager* dbm) {
    // Clear raw pointers and default values
    mBuffer = 0;
    mHierarchy = 0;
    mDeletionPolicy = DeletionPolicy::E_DELETE;

    // Initialize all animation-related containers
    mAnimations.clear();
    mAnimations.get_allocator().set_name("EASTL vector_map");

    mClips.clear();
    mClips.get_allocator().set_name("EASTL vector_map");

    mPoses.clear();
    mPoses.get_allocator().set_name("EASTL vector_map");

    mPhysicsParams.clear();
    mPhysicsParams.get_allocator().set_name("EASTL vector_map");

    // Save the manager reference
    mDatabaseManager = reinterpret_cast<uint32_t>(&dbm); // Depending on your type, maybe just = dbm;

    // Register this database in the manager
    // You must make sure these methods are implemented and thread-safe
    {
        // Lock mutex if necessary (mocked here, actual locking depends on your threading framework)
        // For now assuming single-threaded or external locking

        uint8_t index = dbm->FindFreeSlot();
        dbm->mDataBases[index] = reinterpret_cast<uint32_t>(this);  // Or just `this` if mDataBases is a pointer array

        mDBIndex = index;
    }

    // Optionally initialize mPartNameMapping
    std::fill(std::begin(mPartNameMapping), std::end(mPartNameMapping), 0);
}

Andale::DataBaseManager::DataBaseManager() {
    this->mMaxDatabases = 32;
    this->mNbPartNames = 0;

    // Zero out all part names
    for (int i = 0; i < 31; ++i) {
        mPartNames[i] = Util::FastString<36>();
    }

    // Initialize all database pointers to null
    for (int i = 0; i < static_cast<int>(mMaxDatabases); ++i) {
        mDataBases[i] = 0;
    }
}

Andale::FetchSys::FetchSys(Andale::DataBaseManager* dbm, const Andale::HierarchyData* skel, double truncationOffset, bool hasTrajectoryBone, Andale::codec_type codectype)
{
    ANIM_CODEC::VBRDecompressor* v33; // r3
    ANIM_CODEC::VBRDecompressor* v34; // r3

    this->mHasTrajectoryBone = hasTrajectoryBone;
    this->mTruncationOffset = truncationOffset;
    this->mSkel = reinterpret_cast<uint32_t>(skel);
    this->mDataBaseManager = reinterpret_cast<uint32_t>(dbm);
    this->mFetchTimeCmdError = 0.0;
    this->mMaxFetches = 1;//numChars * maxFetchNodes;
    this->mJobCode = 0;//reinterpret_cast<uint32_t>(pJobCode);
    this->mCodecType = static_cast<uint32_t>(codectype);
    if (codectype == codec_type::CONV_FORMAT)
    {
        std::cout << "VBR USED!!";
        /* TODO!
        v33 = (ANIM_CODEC::VBRDecompressor*)Anim::UserAnimCodecAlloc(0x55D0u, "AnimCodec::VBRDecompressor");
        if (v33)
            ANIM_CODEC::VBRDecompressor::VBRDecompressor(v33);
        else
            v34 = 0;
        this->mDecompressor = v34;
        ANIM_CODEC::VBRDecompressor::BeginAddingChannels(v34);
        ANIM_CODEC::VBRDecompressor::AddChannel(this->mDecompressor, 3u, 0, 0, 0x30u);
        ANIM_CODEC::VBRDecompressor::AddChannel(this->mDecompressor, 4u, 0, 0x10u, 0x30u);
        ANIM_CODEC::VBRDecompressor::AddChannel(this->mDecompressor, 3u, 0, 0x20u, 0x30u);
        ANIM_CODEC::VBRDecompressor::EndAddingChannels(this->mDecompressor);
        ANIM_CODEC::VBRDecompressor::Init(this->mDecompressor, skel->mNbBones);
        */
    }
    else
    {
        this->mDecompressor = 0;
    }
}

void Andale::FetchSys::Fetch(
    const Andale::DataHeader* hdr,
    uint32_t frameIndex,
    xvectoralloc::XVectorPtr<Andale::SQTData>* output)
{
    xvectoralloc::XVectorPtr<SQTData> tempOutput;
    ExtractType extractBuffer[2048];

    tempOutput.mVector = reinterpret_cast<SQTData*>(output->mVector);

    uint32_t fetchedCount = this->BatchFetch(hdr, frameIndex, &tempOutput, extractBuffer, 0x64);

    codec_type codec = static_cast<codec_type>(this->mCodecType);

    if (codec != codec_type::RAW) {
        if (codec == codec_type::CONV_FORMAT) {
            //Scalar_ExtractPackedNVBR(extractBuffer, fetchedCount, reinterpret_cast<ANIM_CODEC::VBRDecompressor*>(this->mDecompressor));
            //Vector_ExtractPackedNVBR(extractBuffer, fetchedCount, reinterpret_cast<ANIM_CODEC::VBRDecompressor*>(this->mDecompressor));
        }
        else {
            std::cout << "Unknown codec type\n";
        }
    }
    else {
        ExtractPackedN(extractBuffer, fetchedCount);
    }
}

void Andale::FetchSys::Fetch(
    const Andale::DataHeader* hdr,
    xvectoralloc::XVectorPtr<SQTData>* output)
{
    xvectoralloc::XVectorPtr<SQTData> tempOutput;
    ExtractType extractBuffer[36];  // temp buffer for extracted pose data

    tempOutput.mVector = reinterpret_cast<SQTData*>(output->mVector);

    int fetchedCount = this->BatchFetch(hdr, &tempOutput, extractBuffer, 0x64);
    std::cout << "fetchedCount: " << fetchedCount << "\n";
    codec_type codec = static_cast<codec_type>(this->mCodecType);

    if (codec != codec_type::RAW) {
        if (codec == codec_type::CONV_FORMAT) {
            //Scalar_ExtractPackedNVBR(extractBuffer, fetchedCount, reinterpret_cast<ANIM_CODEC::VBRDecompressor*>(this->mDecompressor));
            //Vector_ExtractPackedNVfBR(extractBuffer, fetchedCount, reinterpret_cast<ANIM_CODEC::VBRDecompressor*>(this->mDecompressor));
        }
        else {
            std::cout << "Unknown codec type\n";
        }
    }
    else {
        ExtractPackedN(extractBuffer, fetchedCount);
    }
}


void Andale::FetchSys::GetSQTsForPose(
    uint32_t poseGUID,
    xvectoralloc::XVectorPtr<AnimCmdSys::SQTExt>* output)
{
    const Andale::DataHeader* hdr = reinterpret_cast<DataBaseManager*>(mDataBaseManager)->GetPoseHeader(poseGUID);

    xvectoralloc::XVectorPtr<SQTData> sqtOutput;
    sqtOutput.mVector = reinterpret_cast<SQTData*>(output->mVector);

    this->Fetch(hdr, &sqtOutput);
}

void Andale::FetchSys::GetSQTs(
    uint32_t animGUID,
    uint32_t frameIndex,
    xvectoralloc::XVectorPtr<AnimCmdSys::SQTExt>* output)
{
    const DataHeader* hdr = reinterpret_cast<DataBaseManager*>(mDataBaseManager)->GetClipHeader(animGUID);

    xvectoralloc::XVectorPtr<SQTData> sqtOutput;
    sqtOutput.mVector = reinterpret_cast<SQTData*>(output->mVector);

    this->Fetch(hdr, frameIndex, &sqtOutput);
}

void Andale::DataBase::UpdatePartNames()
{
    // Get a pointer just past headers[1].mGUID, then align to 16-byte boundary
    uint32_t basePtr = reinterpret_cast<uint32_t>(&reinterpret_cast<Andale::DataHeader*>(mHierarchy)[1].mGUID);
    uint16_t* alignedPtr = reinterpret_cast<uint16_t*>((basePtr + 3) & ~static_cast<uint32_t>(0xF));

    uint32_t count = Convert(*(reinterpret_cast<uint32_t*>(alignedPtr) + 1)); // number of parts?
    if (count > 0)
    {
        int offset = 0;
        uint32_t* partNameMappingPtr = this->mPartNameMapping;
        // For each part, write its part name index into the hierarchy structure
        while (count-- > 0)
        {
            int partIndex = *partNameMappingPtr++;
            // __ROL4(*v1,3) means rotate-left 3 bits the first int pointed by v1
            int rotated = _rotl(Convert(*alignedPtr), 3) + offset;

            // Write partIndex as big endian uint16_t at offset + 0x2A bytes from v1 base
            *reinterpret_cast<uint16_t*>(reinterpret_cast<char*>(alignedPtr) + rotated + 0x2A) = Convert(static_cast<uint16_t>(partIndex));

            offset += 0x24; // move to next block
        }
    }

    // Update poses part names
    for (auto it = this->mPoses.begin(); it != this->mPoses.end(); ++it)
    {
        Andale::DataHeader* dataHeader = it->second;
        auto* dataPtr = reinterpret_cast<int*>(
            (reinterpret_cast<uint32_t>(&dataHeader[1].mGUID) + 3) & ~0xF);

        int innerCount = Convert(*dataPtr);
        if (innerCount > 0)
        {
            uint32_t* indices = reinterpret_cast<uint32_t*>(dataPtr + 1);

            while (innerCount-- > 0)
            {
                // Extract 24-bit offset from big-endian indices
                uint32_t rawIndexValue = Convert(*indices);
                int offset24 = rawIndexValue & 0xFFFFFF;

                // Read value at offset in dataPtr
                int value = Convert(*reinterpret_cast<int*>(reinterpret_cast<char*>(dataPtr) + offset24));

                // Compute DBIndex offset
                int idx = 4 * (((value >> 1) & 0x1F) + 0x19);

                // Replace bits [1..5] with DBIndex info
                int mask = 0x3E;
                int dbValue = *reinterpret_cast<int*>(reinterpret_cast<char*>(this) + idx);
                int newVal = ((2 * dbValue) & mask) | (value & ~mask);

                // Write back updated value
                *reinterpret_cast<int*>(reinterpret_cast<char*>(dataPtr) + offset24) = Convert(newVal);

                // Set upper byte to DBIndex, preserve offset
                rawIndexValue = (rawIndexValue & 0x00FFFFFF) | (dbValue << 24);
                *indices = Convert(rawIndexValue);

                ++indices;
            }
        }
    }

    // Update clips part names (similar logic to poses)
    for (auto it = this->mClips.begin(); it != this->mClips.end(); ++it)
    {
        Andale::DataHeader* dataHeader = it->second;

        // Align base pointer to 16 bytes after GUID
        uint32_t base = (reinterpret_cast<uintptr_t>(&dataHeader[1].mGUID) + 3) & ~0xF;

        // Extract totalParts (5 bits at [8..12] of word at offset +0x34)
        uint32_t count = (Convert(*reinterpret_cast<uint32_t*>(base + 0x34)) >> 8) & 0x1F;
        if (count > 0)
        {
            // Indices start at offset +0x38
            uint32_t* indices = reinterpret_cast<uint32_t*>(base + 0x38);

            while (count-- > 0)
            {
                uint32_t rawIndexValue = Convert(*indices);
                uint32_t offset24 = rawIndexValue & 0xFFFFFF;

                int* valuePtr = reinterpret_cast<int*>(base + offset24);
                int value = Convert(*valuePtr);

                int idx = 4 * (((value >> 1) & 0x1F) + 0x19);
                int dbValue = *reinterpret_cast<int*>(reinterpret_cast<char*>(this) + idx);

                int newVal = ((2 * dbValue) & 0x3E) | (value & ~0x3E);
                *valuePtr = Convert(newVal);

                // Update upper byte of the index with DB index
                rawIndexValue = (rawIndexValue & 0x00FFFFFF) | (dbValue << 24);
                *indices = Convert(rawIndexValue);

                ++indices;
            }
        }
    }
}

int Andale::DataBaseManager::GetPartNameIndex(const Util::FastString<36>* partName)
{
    for (int i = 0; i < static_cast<uint32_t>(Convert(mNbPartNames) + 1); ++i)
    {
        const Util::FastString<36>& stored = mPartNames[i];

        // Manual byte-wise comparison of stored and *partName
        if (std::memcmp(&stored, partName, sizeof(Util::FastString<36>)) == 0)
        {
            return i;
        }
    }

    // Not found
    return 0xFFFFFFFF;
}

void Andale::DataBaseManager::UpdateDatabase(DataBase* db)
{
    // Cast mHierarchy back to a pointer to Andale::DataHeader array
    Andale::DataHeader* headers = reinterpret_cast<Andale::DataHeader*>(db->mHierarchy);

    // Get a pointer just past headers[1].mGUID, then align to 16-byte boundary
    uint32_t basePtr = reinterpret_cast<uint32_t>(&headers[1].mGUID);

    uint32_t alignedPtr = (basePtr + 3) & ~0xF;  // Align up to next 16-byte boundary

    // The aligned pointer points to a structure like:
    // [uint32_t count][... part name GUIDs or indices ...]
    uint32_t* partData = reinterpret_cast<uint32_t*>(alignedPtr);

    // The count of part names is at offset 1 (index 1) from partData, per original code
    int partCount = static_cast<int>(Convert(partData[1]));

    if (partCount > 0)
    {
        uint32_t* mapping = db->mPartNameMapping;
        int offset = 0;

        // Loop over each part name in the block
        while ( partCount )
        {
            uint16_t subCount = Convert(*reinterpret_cast<uint16_t*>(partData));

            Util::FastString<36>* partNamePtr = reinterpret_cast<Util::FastString<36>*>(
                &partData[2 + 2 * subCount + offset]
                );

            // Look for this part name in the manager's list
            uint32_t currentNbPartNames = Convert(mNbPartNames);
            uint32_t index = GetPartNameIndex(partNamePtr);

            if (index == 0xFFFFFFFF) // Not found
            {
                if (currentNbPartNames >= 31) // Max allowed part names
                {
                    std::cout << "Andale::DataBaseManager: number of part names exceeded, game can't run further\n";
                    break;
                }

                // Insert new part name
                index = currentNbPartNames;
                mPartNames[index] = *partNamePtr;
                mNbPartNames = Convert(index + 1);
            }

            // Write the found or new index to the mapping
            *mapping = index;

            partCount--;
            mapping++;
            offset += 9; // Move to the next part name in the array (each 36 bytes = 9 uint32_t)
        }
    }
    db->UpdatePartNames();
}

void Andale::DataBase::Clear()
{
    auto* dbManager = reinterpret_cast<DataBaseManagerData*>(Convert(mDatabaseManager));
    int index = static_cast<int>(mDBIndex);

    // Slot bounds check
    if (index >= static_cast<int>(Convert(dbManager->mMaxDatabases)))
    {
        std::cout << "DataBaseManager asked to release invalid slot %d\n";
    }

    // Mark the slot as free
    dbManager->mDataBases[index] = 0;

    // Handle deletion policy
    if (mDeletionPolicy == DeletionPolicy::E_DELETE)
    {
        operator delete(reinterpret_cast<void*>(Convert(mBuffer)));
    }

    // Clear vector_maps by resetting end pointers to begin
    mAnimations.clear();
    mClips.clear();
    mPoses.clear();
    mPhysicsParams.clear();

    // Null out database manager reference
    mDatabaseManager = 0;
}

int Andale::DataBase::SetDBContent(void* buffer, Andale::DataBase::DeletionPolicy p) {
    char* ptr = static_cast<char*>(buffer);

    if (buffer) {
        if (mBuffer) {
            Clear();  // assumed to exist
        }

        mBuffer = Convert(reinterpret_cast<uint32_t>(ptr));

        bool shouldUpdateManager = true;
        mDeletionPolicy = p;

        if (Convert(reinterpret_cast<uint32_t*>(ptr)[1]) == 1) { //type is database
            if (Convert(reinterpret_cast<uint32_t*>(ptr)[3])) { //checks if database is set already
                shouldUpdateManager = false;
            }
            else {
                reinterpret_cast<uint32_t*>(ptr)[3] = Convert((uint32_t)1); //define database set
            }
            ptr = reinterpret_cast<char*>((reinterpret_cast<uint32_t>(ptr + 0x37) & ~0xF));
        }

        while (*reinterpret_cast<uint32_t*>(ptr)) {
            HeaderTypes type = static_cast<HeaderTypes>(Convert(reinterpret_cast<int32_t*>(ptr)[1]));
            Util::FastString<36>* name = reinterpret_cast<Util::FastString<36>*>(ptr + 0x10);
            Andale::DataHeader* header = reinterpret_cast<Andale::DataHeader*>(ptr);

            if (type == HeaderTypes::HT_ANIMATION)
            {
                mClips[*name] = header;
            }
            else if (type == HeaderTypes::HT_POSE)
            {
                mPoses[*name] = header;
            }
            else if (type == HeaderTypes::HT_HIERARCHY)
            {
                mHierarchy = reinterpret_cast<uint32_t>(header);
            }
            else if (type == HeaderTypes::HT_PHYSICSPARAMS)
            {
                mPhysicsParams[*name] = header;
            }
            else if (type == HeaderTypes::HT_SELECTORNODE)
            {
                mAnimations[*name] = header;
            }

            if ((*reinterpret_cast<uint32_t*>(ptr) & 0xF) != 0) {
                std::cout << "!(hdr->mSize & (sizeof(RwUInt128)-1))\n";
            }

            ptr += Convert(reinterpret_cast<uint32_t*>(ptr)[0]);
        }

        // int MAX_ANIMS_PER_DB = 4096; 2048 in Skate 2, but Skate 3 can have more than that in one database.
        
        // Update Pose GUIDs
        int index_pose = 0;
        for (auto it = mPoses.begin(); it != mPoses.end(); ++it) {
            const Util::FastString<36>& name = it->first;
            Andale::DataHeader* header_pose = it->second;

            header_pose->mGUID = (index_pose << 5) | (mDBIndex & 0x1F);
            index_pose++;
        }

        // Update Clip GUIDs
        int index_anim = 0;
        for (auto it = mClips.begin(); it != mClips.end(); ++it) {
            const Util::FastString<36>& name = it->first;
            Andale::DataHeader* header_anim = it->second;

            header_anim->mGUID = (index_anim << 5) | (mDBIndex & 0x1F);
            index_anim++;
        }

        if (shouldUpdateManager) {
            reinterpret_cast<Andale::DataBaseManager*>(mDatabaseManager)->UpdateDatabase(this);
        }

        return 1;
    }
    else {
        mDeletionPolicy = p;
        Clear();  // assumed to exist
        return 1;
    }
}

bool Andale::DataBase::SetDBContent(const char* DBFilename) {
    std::ifstream file(DBFilename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open file: " << DBFilename << "\n";
        return false;
    }

    std::streamsize size = file.tellg();
    if (size <= 0) {
        std::cerr << "Empty or invalid file size: " << DBFilename << "\n";
        return false;
    }

    file.seekg(0, std::ios::beg);

    // Allocate a temporary buffer to read the file (no alignment needed here)
    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        std::cerr << "Failed to read file: " << DBFilename << "\n";
        return false;
    }

    // Calculate aligned size: round up to multiple of 16
    size_t alignedSize = (size + 15) & ~static_cast<size_t>(0xF);

    // Allocate 16-byte aligned memory
    void* heapBuffer = _aligned_malloc(alignedSize, 2048);
    if (!heapBuffer) {
        std::cerr << "Failed to allocate aligned memory\n";
        return false;
    }

    // Copy file data to aligned buffer
    std::memcpy(heapBuffer, buffer.data(), size);

    // Zero out any extra bytes after file data (optional, for safety)
    if (alignedSize > static_cast<size_t>(size)) {
        std::memset(static_cast<char*>(heapBuffer) + size, 0, alignedSize - size);
    }

    bool result = SetDBContent(heapBuffer, Andale::DataBase::DeletionPolicy::E_DELETE);

    if (!result) {
        _aligned_free(heapBuffer);
    }

    return result;
}

Andale::FetchSys loadAnimationDataBase(const char* DBFilename)
{
    Andale::DataBaseManager* dbManager = new Andale::DataBaseManager();  // Or stack-allocate if safe

    Andale::DataBase* db = new Andale::DataBase(dbManager);

    db->mDatabaseManager = reinterpret_cast<uint32_t>(dbManager);
    db[0].SetDBContent(DBFilename);

    Andale::HierarchyData* skel = reinterpret_cast<Andale::HierarchyData*>(db[0].mHierarchy + 0x30);
    uint32_t* parentsPtr = &(skel->mNbParts) + 1;

    for (uint16_t i = 0; i < Convert(skel->mNbBones); ++i) {
        uint32_t parentValue = Convert(parentsPtr[i]);
        //std::cout << "Parent[" << i << "] = " << parentValue << "\n";
        skel->mParents[i] = parentValue;
    }

    Andale::FetchSys fs(
        dbManager,
        skel,
        0, //double truncationOffset
        true, //bool hasTrajectoryBone 
        Andale::codec_type::RAW); //Andale::codec_type codectype
    
    return fs;
}

void Andale::FetchSys::loadPose(uint32_t animGUID, xvectoralloc::XVectorPtr<AnimCmdSys::SQTExt>* output) {
    GetSQTsForPose(animGUID, output);
}

void Andale::FetchSys::loadAnimation(uint32_t animGUID, xvectoralloc::XVectorPtr<AnimCmdSys::SQTExt>* output) {
    const DataHeader* hdr = reinterpret_cast<DataBaseManager*>(mDataBaseManager)->GetClipHeader(animGUID);

    const uint32_t* alignedData = reinterpret_cast<const uint32_t*>(
        (reinterpret_cast<uintptr_t>(&hdr[1].mGUID) + 3) & ~0xF);

    const AnimationData* animData = reinterpret_cast<const AnimationData*>(alignedData);

    float fps = Convert(animData->mFPS);
    float nbFrames = Convert(animData->mNbFrames);
    float baseSpeed = Convert(animData->mBaseSpeed);
    uint32_t attributesOffset = Convert(animData->mAttributesOffset);
    uint32_t blendPosePartsOffsetsOffset = Convert(animData->mBlendPosePartsOffsetsOffset);
    uint32_t bitField = animData->mBitField;
    uint32_t totalParts = bitField & 0x3F;
    uint32_t partsOffsets = Convert(animData->mPartsOffsets[0]);

    for (uint16_t i = 0; i < nbFrames; ++i) {
        GetSQTs(animGUID, i, output);
    }
}

extern "C" {
    __declspec(dllexport) const char* load_database_and_get_names(const char* DBFilename) {

        Andale::FetchSys fs = loadAnimationDataBase(DBFilename);
        int mDBIndex = 0;
        Andale::DataBaseManager* dbManager = reinterpret_cast<Andale::DataBaseManager*>(fs.mDataBaseManager);
        Andale::DataBase* db = reinterpret_cast<Andale::DataBase*>(dbManager->mDataBases[mDBIndex]);

        static std::string names;

        int index_pose = 0;
        for (auto it = db->mPoses.begin(); it != db->mPoses.end(); ++it) {
            const Util::FastString<36>& name = it->first;
            names += name.to_string();
            if (std::next(it) != db->mPoses.end()) names += ",";
            index_pose++; 
        }

        names += "|";

        int index_anim = 0;
        for (auto it = db->mClips.begin(); it != db->mClips.end(); ++it) {
            const Util::FastString<36>& name = it->first;
            names += name.to_string();
            if (std::next(it) != db->mClips.end()) names += ",";
            index_anim++;
        }

        return names.c_str();
    }
}

//testing
int main() {
    const char* DBFilename = "C:/your/sample/animationdatabase.abin";

    Andale::FetchSys fs = loadAnimationDataBase(DBFilename);
    std::string names = load_database_and_get_names(DBFilename);
    std::cout << names;

    int mDBIndex = 0;
    int index_pose = 0;
    uint32_t animGUID = (index_pose << 5) | (mDBIndex & 0x1F);

    xvectoralloc::XVectorPtr<AnimCmdSys::SQTExt>* output = new xvectoralloc::XVectorPtr<AnimCmdSys::SQTExt>;
    output->mVector = new AnimCmdSys::SQTExt[0];

    fs.loadAnimation(animGUID, output);

    delete[] output->mVector;
    delete output;

    return 0;
}
