//THIS PROGRAM WILL ONLY WORK AS 32-bit
//by tuukkas 2025

#include <fstream>
#include <string>
#include <vector>
#include <new>
#include <map>
#include <iomanip>

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
        // mhdr points at the start of the on-disk VBRDataHeaderValue (16-byte
        // aligned). Stored as a real host pointer so x64 builds don't lose the
        // high 32 bits. Inline layout follows it: palette (4 * paletteSize),
        // const-channel-map runs (2 * ceil(mConstChanMapSize/8)), then aligned
        // up to 16, the channel-info table (16 bytes per channel).
        const uint8_t* mhdr;
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
        // Construction & teardown.
        VBRDecompressor();                      // Zero the entire data struct (recomp.116:39527).
        // (no explicit destructor; allocations from Init() are tracked outside)

        // Channel descriptor build (call between Begin and End).
        void BeginAddingChannels();             // recomp.116:37873
        void EndAddingChannels();               // recomp.116:37887
        void AddChannel(uint32_t typeId,        // recomp.116:37897
                        uint32_t arg2,
                        uint16_t offset,
                        uint16_t stride);

        // Per-instance IDCT table init + scratch allocation. `numBones` controls
        // working-buffer size. Standard 8-point IDCT formula populates the
        // coefficient tables; replace if you need bit-exact match to the recomp.
        void Init(uint32_t numBones);

        // Frame-level operations (already implemented further down the file).
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

    // Skeleton-free VBR extractor. Walks `animData->mPartsOffsets[]` in the file
    // directly and decodes every part it finds, without going through BatchFetch's
    // partID-vs-skeleton matching. Each part's SQTs land in their own slice of
    // sqtBuffer, so you get the file's full content even when no skeleton matches.
    // Returns the number of SQTs written. `maxBones` bounds the output buffer.
    uint32_t ExtractAllVBRParts(const DataHeader* hdr,
                                uint32_t frameIndex,
                                ANIM_CODEC::VBRDecompressor* decompressor,
                                SQTData* sqtBuffer,
                                uint32_t maxBones);
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

    // Copy `mDataSize` floats from src to target. The previous switch had
    // multiple fall-through bugs (case 3 leaking into case 4 -> OOB read of
    // newData[3]). The recomp's PutData (sk83_na_m_recomp.116.cpp:rex_PutData_*)
    // is just a memcpy of size floats, with optional 1/2/3/4 fast paths -- the
    // simple loop below produces the same result.
    for (size_t i = 0; i < mDataSize; ++i) {
        targetData[i] = newData[i];
    }
}

//============================================================================
// Anim:: helpers (small support funcs the recomp uses internally).
//============================================================================
namespace Anim {
    // The original allocator is a tagged pool allocator. For our standalone tool
    // a tagged malloc is enough; the leak across reloads is acceptable since the
    // process is short-lived and the buffers are < a few MB total.
    inline void* UserAnimCodecAlloc(uint32_t size, const char* tag) {
        (void)tag;  // tag is debug-only; ignore for now
        if (size == 0) return nullptr;
        return std::malloc(size);
    }
}

//============================================================================
// VBRDecompressor lifecycle: ctor + Begin/End/AddChannel + Init.
// Ports of recomp.116.cpp around offsets 37873 / 37887 / 37897 / 39527 / 39939.
//============================================================================

ANIM_CODEC::VBRDecompressor::VBRDecompressor() {
    // Recomp at sk83_na_m_recomp.116.cpp:39527 individually zeros each
    // mWorkBuffers[]/mQuantTableInfo[] field that matters; doing a full memset
    // of the data parent is functionally equivalent (Init() and InitPerAnim()
    // overwrite the tables that need real values).
    std::memset(static_cast<VBRDecompressorData*>(this), 0, sizeof(VBRDecompressorData));
}

void ANIM_CODEC::VBRDecompressor::BeginAddingChannels() {
    mCanAddChannel    = 1;
    mNumChannelBlocks = 0;
}

void ANIM_CODEC::VBRDecompressor::EndAddingChannels() {
    mCanAddChannel = 0;
}

void ANIM_CODEC::VBRDecompressor::AddChannel(uint32_t typeId,
                                             uint32_t arg2,
                                             uint16_t offset,
                                             uint16_t stride) {
    if (!mCanAddChannel) {
        std::cout << "AddChannel called outside Begin/End block\n";
        return;
    }
    if (mNumChannelBlocks + 1 >= 4) {
        std::cout << "AddChannel: max 4 channel blocks\n";
        return;
    }

    // Recomp stores into mChannelBlocks[numCh] (12 bytes per slot). The slot
    // address is computed as `this + numCh * 12` because mChannelBlocks is the
    // very first member of VBRDecompressorData.
    VBRChannelTypeBlockInfoData& slot = mChannelBlocks[mNumChannelBlocks];
    slot.mOffset   = offset;     // (recomp r28) byte offset within the per-frame data
    slot.mStride   = stride;     // (recomp r27) total stride per joint
    slot.mData     = arg2;       // (recomp r29) -- always 0 in current callers
    slot.mDataSize = (uint8_t)typeId;  // (recomp r30) channel type id (3 or 4 in callers)
    mNumChannelBlocks++;
}

// Standard 8-point IDCT cosine basis. The recomp uses a more elaborate setup
// (sk83_na_m_recomp.116.cpp:37423 InitIDctTablesInv) with several preloaded
// constants -- if VBR output is consistently scaled wrong, replacing this
// formula with a faithful port of that function is the next step.
void ANIM_CODEC::VBRDecompressor::Init(uint32_t numBones) {
    if (mCanAddChannel) {
        std::cout << "Init called before EndAddingChannels()\n";
    }

    // ---- Working-buffer sizes ----
    // Total bytes per frame = sum over channel blocks of (mDataSize * numBones).
    uint32_t perFrameBytes = 0;
    for (uint8_t i = 0; i < mNumChannelBlocks; ++i) {
        perFrameBytes += mChannelBlocks[i].mDataSize * numBones;
    }
    // Round up to vector-friendly sizes. The recomp uses tighter math but this
    // is generous-enough scratch for a working tool.
    const uint32_t scratchInts   = (perFrameBytes + 0xF) & ~0xFu;        // ints
    const uint32_t scratchFloats = ((perFrameBytes * 4u) + 0xFFu) & ~0xFFu;  // floats

    // ---- Allocate per-thread work-buffer scratch (6 threads). ----
    for (int t = 0; t < 6; ++t) {
        DataPerDecompress& wb = mWorkBuffers[t];

        wb.mUncompressedBlkDataScratch =
            (int*)Anim::UserAnimCodecAlloc(scratchInts * sizeof(int),
                                           "VBRDecompressor::UncompressedBlkData");
        wb.mNormalizedData =
            (float*)Anim::UserAnimCodecAlloc(scratchFloats * sizeof(float),
                                             "VBRDecompressor::NormalizedData");
        wb.mConstData =
            (float*)Anim::UserAnimCodecAlloc(scratchFloats * sizeof(float),
                                             "VBRDecompressor::ConstData");
        wb.mIsConstantChannel =
            (bool*)Anim::UserAnimCodecAlloc(perFrameBytes,
                                            "VBRDecompressor::IsConstantChannel");

        // Zero-init the scratch buffers. The recomp's allocator does this
        // implicitly; our malloc-based UserAnimCodecAlloc does not, and
        // leftover memory shows up as nonzero IDCT output for clips with
        // mFrameOffset == 0 (where the bit decoder is skipped and
        // DecompressFrameBlock reads stale ints from scratch).
        if (wb.mUncompressedBlkDataScratch)
            std::memset(wb.mUncompressedBlkDataScratch, 0, scratchInts * sizeof(int));
        if (wb.mNormalizedData)
            std::memset(wb.mNormalizedData, 0, scratchFloats * sizeof(float));
        if (wb.mConstData)
            std::memset(wb.mConstData, 0, scratchFloats * sizeof(float));
        if (wb.mIsConstantChannel)
            std::memset(wb.mIsConstantChannel, 0, perFrameBytes);
    }

    // ---- IDCT coefficient tables (port of ANIM_CODEC::InitIDctTablesInv,
    //      sk82_na_zd.xex.c:3897841 / sk83 recomp.116:37423) ----
    //
    // For each row k in 0..7, column n in 0..7:
    //   mIdctCoefTables[k][n] = cos((k + 0.5) * n * pi/8) * 0.25
    // Then mIdctCoefTables[k][0] is halved (cos(0)*0.25 = 0.25 -> 0.125), so
    // the DC coefficient gets weight 1/8 (matches the (1/N) factor in the
    // standard 8-point inverse DCT-II), and the non-DC columns get 2/N = 0.25
    // applied to the standard cos((k+0.5)*n*pi/N) basis.
    //
    // Note: row index is the *output sample* (= frameinBlk in
    // DecompressFrameBlock), column index is the *frequency bin*. Our previous
    // formula had row/column swapped, which gave a non-constant first column
    // and was the main reason VBR per-frame output was scaled ~1000x off.
    constexpr float kPi = 3.14159265358979323846f;
    for (int k = 0; k < 8; ++k) {
        for (int n = 0; n < 8; ++n) {
            mIdctCoefTables[k][n] =
                std::cos((((float)k + 0.5f) * (float)n * kPi) / 8.0f) * 0.25f;
        }
        mIdctCoefTables[k][0] *= 0.5f;  // DC weight: 0.25 * 0.5 = 0.125 = 1/N
    }

    // ---- Initial quantize tables (port of the SIMD log loop in recomp's
    //      VBRDecompressor::Init at sk82_na_zd.xex.c:3899181-3899282) ----
    //
    // The recomp computes 8 "base" values via an XMVectorLog-based loop on
    // inputs (i + 2) for i in 0..7, then propagates them across 256 channel
    // rows scaled by (1 + ch * 0.2):
    //
    //   base[i] = ln(i + 2) / 32768                  // log loop output
    //   v31[ch] = 1.0 + ch * 0.2                      // per-row scale
    //   mInitialQuantizeTables[ch][i] = base[i] * v31[ch]
    //
    // The ln/32768 produces values in the ~2e-5..7e-5 range, so initial[i] is
    // tiny -- it acts as a small frequency-dependent perturbation on top of
    // the dominant `scale * 0.5` bias term in DecompressFrameBlock's
    // quantTable[i] = initial[i] * scaleSquared + scale * 0.5 formula.
    //
    // Note: XMVectorLog uses a 5th-order polynomial approximation with
    // specific C0/C1 constants; we substitute std::log() / 32768 here, which
    // is exact rather than approximate -- match should be within float epsilon
    // for our needs.
    float base[8];
    for (int i = 0; i < 8; ++i) {
        base[i] = std::log(static_cast<float>(i + 2)) / 32768.0f;
    }
    for (int ch = 0; ch < 256; ++ch) {
        const float v31 = 1.0f + (float)ch * 0.2f;
        for (int i = 0; i < 8; ++i) {
            mInitialQuantizeTables[ch][i] = base[i] * v31;
        }
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

// Read a big-endian 32-bit float stored at `p`.
static inline float ReadBEFloat(const uint8_t* p) {
    uint32_t bits = Convert(*reinterpret_cast<const uint32_t*>(p));
    float f;
    std::memcpy(&f, &bits, sizeof(f));
    return f;
}

// Compute the inline palette base and the 16-byte-aligned channel-info table
// pointer that lives after a per-frame "map" area. Mirrors the recomp's
// mhdr-wrapper fields f4 (palette base) and f8 (channel info base).
//
// Per recomp (sk83_na_m_recomp.116.cpp:39752-39772) the inline layout is:
//   hdr+0..15          : VBRDataHeaderValue
//   hdr+16..16+4*paletteSize : constant palette (BE floats)
//   then 2 * ceil(mNumFrames / 8) bytes of frame-map data
//   aligned up to 16   : channel-info table (16 bytes per channel)
//
// Note: the size of the "frame map" area uses mNumFrames (header offset 2),
// NOT mConstChanMapSize (offset 0). Both happen to give the same result for a
// 1-frame pose, but diverge for real animations -- a previous offset-0 read
// put channelInfoBase 16 bytes too early on multi-frame clips.
static inline void ComputeVBRInline(
    const uint8_t* hdr,
    const uint8_t*& paletteBase,
    const uint8_t*& channelInfoBase)
{
    uint8_t  paletteSize = hdr[13];
    uint16_t numFrames   = Convert(*reinterpret_cast<const uint16_t*>(hdr + 2));
    paletteBase = hdr + 16;
    uint32_t paletteBytes = 4u * paletteSize;
    uint32_t mapBytes     = 2u * ((numFrames + 7u) / 8u);
    uintptr_t aligned     = (reinterpret_cast<uintptr_t>(paletteBase + paletteBytes + mapBytes) + 0xF)
                            & ~uintptr_t(0xF);
    channelInfoBase = reinterpret_cast<const uint8_t*>(aligned);
}

// VBRDecompressor::DecompressFrameBlock
//
// Faithful port of the recomp at sk83_na_m_recomp.116.cpp:38088
// (and the IDA decomp at sk83_na_m.xex.c:3565075). The original is heavy VMX
// SIMD; this port reproduces the math scalar-style. Reference points:
//
//  Setup:
//   - threadBuf base   = this + threadId*2052
//   - per-channel flag = threadBuf[9648 + ch*8]   (8 bytes per ch: [0]=flag, [4]=tableIdx)
//   - tableCounter     = threadBuf[11696]  (incremented as quant tables get built)
//   - IDCT coefs       = mIdctCoefTables[(frameinBlk*32)&0x1FE0 .. +32]  (8 floats)
//   - quantTables base = this + 8496 (32 bytes per (threadId*4 + tableIdx))
//   - mInitialQuant    = this + 304 (32 bytes per channel)
//
//  Math:
//   - scale         = max(|dctMin|, |dctMax|)               (recomp uses vandc + vmaxfp)
//   - scaleSquared  = scale * scale
//   - quantTable[i] = mInitialQuantizeTables[ch][i] * scaleSquared + scale * 0.5
//                                                          (recomp: vmaddcfp v63, v12, v63, v0
//                                                           where v0 = scale * vhalf)
//   - coef[i]       = quantTable[i] * idctCoef[i]
//   - per output    = sum_{k=0..7}(input[base+k] * coef[k]) + 0.5  (the +0.5 is the recomp's vhalf)
//   - per output   *= chRange = chMax - chMin  ; += chMin       (range remap, vmaddcfp v125,v0,v123,v124)
//
//  Per-channel chMin/chMax come from the inline channel-info table that
//  ComputeVBRInline locates from the VBRDataHeaderValue (16-byte aligned, just
//  after the palette and constant-channel map). All multi-byte fields on disk
//  are big-endian, so floats need Convert() on read.
//
//  Inner loop iterates ((end - start + 3) >> 2) chunks; each chunk consumes
//  32 ints of input (8 ints per output slot * 4 slots) and produces 4 floats
//  of output.
//
void ANIM_CODEC::VBRDecompressor::DecompressFrameBlock(
    uint8_t frameinBlk,
    ANIM_CODEC::DataPerDecompress* pWorkBuffer,
    uint8_t threadId)
{
    using namespace ANIM_CODEC;

    // mhdr is a direct pointer to the on-disk VBRDataHeaderValue. The
    // channel-info table lives inline (16-byte aligned, after palette + map);
    // ComputeVBRInline returns its base.
    const uint8_t* hdrBytes = pWorkBuffer->mhdr;
    if (!hdrBytes) {
        std::cerr << "[DecompressFrameBlock] mhdr is null\n";
        return;
    }
    const VBRDataHeaderValue* dataValue =
        reinterpret_cast<const VBRDataHeaderValue*>(hdrBytes);
    const uint8_t* paletteBase     = nullptr;
    const uint8_t* channelInfoBaseB = nullptr;
    ComputeVBRInline(hdrBytes, paletteBase, channelInfoBaseB);
    const VBRChannelTypeHdr* chTypeInfo =
        reinterpret_cast<const VBRChannelTypeHdr*>(channelInfoBaseB);

    int*           uncompressed     = pWorkBuffer->mUncompressedBlkDataScratch;
    if (!uncompressed) {
        std::cout << "UncompressedBlkData != 0 (vbrdecompressor.cpp:365)" << std::endl;
        return;
    }

    const uint8_t* compressed       = pWorkBuffer->mCompressedData;        // first numChannels bytes are channel indices
    float*         normalized       = pWorkBuffer->mNormalizedData;
    // mNumChannels is a single byte -- no byte-swap.
    const uint32_t numChannels      = dataValue->mNumChannels;
    const int      frameOffsetAln   = (int)((pWorkBuffer->mFrameOffset + 3u) & ~3u);

    // Per-thread scratch state
    uint8_t* threadBuf       = reinterpret_cast<uint8_t*>(this) + (uint32_t)threadId * 2052u;
    uint8_t* flagBase        = threadBuf + 9648;          // 8-byte entry per channel
    uint32_t& tableCounter   = *reinterpret_cast<uint32_t*>(threadBuf + 11696);

    // 8 IDCT coefficients for this frame block (lives in mIdctCoefTables, 32B per block)
    const float* idctCoef =
        reinterpret_cast<const float*>(reinterpret_cast<uint8_t*>(mIdctCoefTables)
                                       + ((frameinBlk * 32u) & 0x1FE0u));

    //--------------------------------------------------------
    // Reset per-channel flags & quant-table-build counter
    //--------------------------------------------------------
    for (uint32_t i = 0; i < numChannels; ++i) {
        uint8_t ch = compressed[i];
        flagBase[ch * 8] = 0;
    }
    tableCounter = 0;

    //--------------------------------------------------------
    // Global scale: max(|dctMin|, |dctMax|), squared.
    // mDctMin/mDctMax are big-endian floats on disk -- byte-swap on read.
    //--------------------------------------------------------
    const float dctMin        = Convert(dataValue->mDctMin);
    const float dctMax        = Convert(dataValue->mDctMax);
    const float scale         = std::max(std::fabs(dctMin), std::fabs(dctMax));
    const float scaleSquared  = scale * scale;
    // Rexglue's recomp uses two DIFFERENT constants that should both be 0.5:
    //   - `quantBiasFactor` (multiplied with scale to build the quant-table
    //     bias term) is BROKEN -- it loads 1/32768 instead of 0.5. This
    //     bug only matters during quant-table construction.
    //   - `vhalf` (added to each per-bone dot product before range-remap)
    //     is correct at 0.5.
    // Matching rexglue here means using 1/32768 for the bias factor so
    // AnimTest's extraction reproduces the recompiled runtime's behavior
    // bit-for-bit (which is the data the user actually sees in their
    // rexglue-launched game). The original Xbox 360 build used 0.5 for
    // both; if a user needs that, restore the `0.5f` below.
    const float quantBiasFactor = 1.0f / 32768.0f;
    const float vhalf           = 0.5f;
    const float quantBias       = scale * quantBiasFactor;

    // [debug] dump the first few animated DFB calls with mFrameOffset >= 8
    // (the diverging records have mFrameOffset 8 or 11; the mFrameOffset=3
    // matching records previously dominated this trace).
    // DFB diagnostic stays off by default (cluttered the normal extraction
    // path's CSV output). Toggle to true to dump per-channel quantTable/coef
    // when debugging IDCT scaling issues.
    constexpr bool kDFBVerbose = false;
    static int s_dfbTraced = 0;
    const bool dfbTrace = kDFBVerbose && (s_dfbTraced < 3 && pWorkBuffer->mFrameOffset >= 8);
    if (dfbTrace) {
        ++s_dfbTraced;
        std::cerr << "[DFB trace #" << s_dfbTraced << "] frameinBlk=" << (int)frameinBlk
                  << " numChannels=" << numChannels
                  << " mFrameOffset=" << pWorkBuffer->mFrameOffset
                  << " frameOffsetAln=" << frameOffsetAln << "\n";
        std::cerr << "  headerDump[0..15] (BE on disk):";
        for (int i = 0; i < 16; ++i) {
            std::cerr << ' ' << std::hex << std::setw(2) << std::setfill('0')
                      << (unsigned)hdrBytes[i];
        }
        std::cerr << std::dec << std::setfill(' ') << "\n";
        std::cerr << "  dctMin=" << dctMin
                  << " dctMax=" << dctMax
                  << " scale=" << scale
                  << " scaleSquared=" << scaleSquared
                  << " quantBias=" << quantBias << "\n";
        std::cerr << "  idctCoef[8] =";
        for (int i = 0; i < 8; ++i) std::cerr << ' ' << idctCoef[i];
        std::cerr << "\n";
        std::cerr << "  channelInfoBase offset from hdr = "
                  << (channelInfoBaseB - hdrBytes) << "\n";
        std::cerr << "  channelInfo[0..2] entries (raw 16 bytes each):\n";
        for (int e = 0; e < std::min<int>(3, (int)numChannels); ++e) {
            std::cerr << "    [ch=" << e << "]";
            for (int i = 0; i < 16; ++i) {
                std::cerr << ' ' << std::hex << std::setw(2) << std::setfill('0')
                          << (unsigned)channelInfoBaseB[e * 16 + i];
            }
            std::cerr << std::dec << std::setfill(' ') << "\n";
        }
    }

    //--------------------------------------------------------
    // Per-channel processing
    //--------------------------------------------------------
    for (uint32_t ch = 0; ch < numChannels; ++ch) {
        const uint16_t start = pWorkBuffer->mChannelBlockStarts[ch];
        const uint16_t end   = pWorkBuffer->mChannelBlockEnds[ch];
        if (start == end) continue;

        const uint8_t channelIndex = compressed[ch];

        // Per-channel range used for remapping the final output
        // (recomp masks ch by 0xFF since it's <<4 then masks with 0xFF0).
        // Min/Max are big-endian floats on disk -- byte-swap on read.
        const VBRChannelTypeHdr& chHdr = chTypeInfo[ch & 0xFFu];
        const float chMin   = Convert(chHdr.Min);
        const float chMax   = Convert(chHdr.Max);
        const float chRange = chMax - chMin;

        // Locate the quant-table slot for (threadId, table-index-from-flag)
        uint8_t*  flagEntry = flagBase + channelIndex * 8;
        uint32_t  flagIdx   = *reinterpret_cast<uint32_t*>(flagEntry + 4);
        float*    quantTable = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this)
                                + (((uint32_t)threadId * 4u + flagIdx) << 5) + 8496);

        // First time we encounter this channel this frame block: build its quant table
        if (!flagEntry[0]) {
            flagEntry[0] = 1;
            const uint32_t newIdx = tableCounter++;

            // mInitialQuantizeTables[channelIndex] = 8 floats at this+304+ch*32
            const float* initial = mInitialQuantizeTables[channelIndex];

            // Recompute table address with the freshly-allocated index (recomp does this).
            float* qtab = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(this)
                            + (((uint32_t)threadId * 4u + newIdx) << 5) + 8496);

            for (int i = 0; i < 8; ++i) {
                qtab[i] = initial[i] * scaleSquared + quantBias;
            }
            quantTable = qtab;
        }

        // Combined transform coefficient = quantTable * idctCoef
        float coef[8];
        for (int i = 0; i < 8; ++i) {
            coef[i] = quantTable[i] * idctCoef[i];
        }

        if (dfbTrace) {
            std::cerr << "  ch=" << ch << " channelIndex=" << (int)channelIndex
                      << " start=" << start << " end=" << end
                      << " chMin=" << chMin << " chMax=" << chMax
                      << " chRange=" << chRange << "\n";
            std::cerr << "    quantTable=";
            for (int i = 0; i < 8; ++i) std::cerr << ' ' << quantTable[i];
            std::cerr << "\n    coef      =";
            for (int i = 0; i < 8; ++i) std::cerr << ' ' << coef[i];
            std::cerr << "\n";
            // Print scratch bytes the IDCT is about to read for the first
            // 16 ints (= bones 0 and 1's data) so we can verify they hold
            // what the bit decoder said it wrote (-3.0f, -1.0f, 192f, ...).
            const int* inp = uncompressed + (uint32_t)start * 8u;
            std::cerr << "    scratch[start..start+16] as int :";
            for (int i = 0; i < 16; ++i) std::cerr << ' ' << inp[i];
            std::cerr << "\n    scratch[start..start+16] as flt :";
            for (int i = 0; i < 16; ++i) {
                float v; std::memcpy(&v, &inp[i], 4);
                std::cerr << ' ' << v;
            }
            std::cerr << "\n";
        }

        //----------------------------------------------------
        // Inner transform loop: each chunk of 32 ints -> 4 floats
        //----------------------------------------------------
        const int* input  = uncompressed + (uint32_t)start * 8u;
        float*     output = normalized   + (uint32_t)frameinBlk * (uint32_t)frameOffsetAln + start;

        const int total      = (int)end - (int)start + 3;
        const int chunkCount = (total & ~3) >> 2;

        for (int chunk = 0; chunk < chunkCount; ++chunk) {
            // 4 dot products, one per 8-int slot
            float dotResult[4];
            for (int slot = 0; slot < 4; ++slot) {
                float sum = 0.0f;
                const int* row = input + slot * 8;
                for (int k = 0; k < 8; ++k) {
                    // scratch stores IEEE-754 float bit patterns in int32 slots
                    // (vcuxwfp output). Must reinterpret bits, NOT convert int→float.
                    float v;
                    std::memcpy(&v, &row[k], sizeof(v));
                    sum += v * coef[k];
                }
                dotResult[slot] = sum + vhalf;
            }

            // Range-remap and store. Tracing the recomp's vmrghw cascade
            // through SSA shows the final v0 (after three vmrghw stages on
            // replicated-scalar vectors) ends up in PPC order
            // (bone0, bone1, bone2, bone3); stvx128 stores it BE, so memory
            // gets (bone0, bone1, bone2, bone3) -- normal order, no
            // reversal needed.
            output[0] = dotResult[0] * chRange + chMin;
            output[1] = dotResult[1] * chRange + chMin;
            output[2] = dotResult[2] * chRange + chMin;
            output[3] = dotResult[3] * chRange + chMin;

            input  += 32;
            output += 4;
        }
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
void ANIM_CODEC::VBRDecompressor::Vector_UnPackFrameBlockAndDecompressOneFrame(
    const uint8_t* pCompressedData,
    ANIM_CODEC::DataPerDecompress* pWorkBuffer,
    uint8_t frameinBlk,
    uint8_t threadId)
{
    if (!pCompressedData || !pWorkBuffer || !pWorkBuffer->mUncompressedBlkDataScratch)
        return;

    const uint32_t boneCount = pWorkBuffer->mFrameOffset;
    if (!boneCount)
    {
        DecompressFrameBlock(frameinBlk, pWorkBuffer, threadId);
        return;
    }

    int32_t* outBuf =
        reinterpret_cast<int32_t*>(pWorkBuffer->mUncompressedBlkDataScratch);

    // -----------------------------------------------------------------------
    // Two independent bit streams, mirroring the PowerPC VMX implementation:
    //
    //  Stream A – selector (sel) stream (r17 / v14 in the REX code)
    //    32-bit shift register initialised from pCmp[1..4] (little-endian).
    //    Refilled 24 bits at a time (3 bytes) after every complete 8-nibble
    //    bone group when the remaining count has dropped to <= 8.
    //    One bit consumed per non-zero nibble slot.
    //
    //  Stream B – magnitude stream (r31 bitstream / lvx/vperm path)
    //    64-bit lazy-refill buffer.  Starts at pCmp + 1 + ceil(pCmp[0]/8).
    //    Refilled whenever fewer than 57 bits remain.
    //    Consumed only when the selector bit == 1:
    //      1 sign bit  (0 → negative output, 1 → positive output)
    //      N magnitude bits  where N = the nibble value
    //
    //  Float encoding (vcuxwfp + vor in the REX code):
    //    output_float = (float)(uint32_t)(mag)   [unsigned-int-to-float]
    //    if sign_bit_from_stream == 0 → set IEEE-754 sign bit (negative)
    //    The result is stored as its raw float bit-pattern in the int32 scratch.
    // -----------------------------------------------------------------------

    // --- Stream A (selector) init -------------------------------------------
    // pCmp[0]: number of bits in the leading selector-prefix (may be skipped).
    // selBuf is loaded from pCmp[1..4] as a little-endian 32-bit word.
    uint32_t selBuf =
        (uint32_t)pCompressedData[1]        |
        ((uint32_t)pCompressedData[2] << 8)  |
        ((uint32_t)pCompressedData[3] << 16) |
        ((uint32_t)pCompressedData[4] << 24);
    uint32_t selCount = 32;

    // selRefillPtr starts at byte index 5, matching the PPC initialisation:
    //   addi r11, r31, 1     ; r11 = pCmp+2  (mag stream start)
    //   addi r10, r11, 4     ; r10 = pCmp+6
    //   addi r18, r10, -1    ; r18 = pCmp+5  <- refill pointer
    // The refill lambda reads r18[1..3] = pCmp[6..8] on the first call, then
    // advances r18 by 3 so subsequent calls read pCmp[9..11], etc.
    const uint8_t* selRefillPtr = pCompressedData + 5;

    auto refillSel = [&]()
    {
        // Reads pCmp[ptr+1], pCmp[ptr+2], pCmp[ptr+3] as a 24-bit LE chunk
        // (matches the REX lbz / rlwimi sequence).
        uint32_t chunk =
            (uint32_t)selRefillPtr[1]        |
            ((uint32_t)selRefillPtr[2] << 8)  |
            ((uint32_t)selRefillPtr[3] << 16);
        selRefillPtr += 3;
        selBuf    = (selBuf | (chunk << selCount)) & 0xFFFFFFFFu;
        selCount += 24;
    };

    // --- Stream B (magnitude) init ------------------------------------------
    // Start byte: pCmp + 1 + ceil(pCmp[0] / 8)
    uint32_t selHeaderBits = pCompressedData[0];
    const uint8_t* magPtr  = pCompressedData + 1 + ((selHeaderBits + 7) >> 3);

    uint64_t magBuf   = 0;
    uint32_t magCount = 0;

    auto refillMag = [&]()
    {
        while (magCount <= 56)
        {
            magBuf   |= (uint64_t)(*magPtr++) << magCount;
            magCount += 8;
        }
    };

    // --- Ctrl word pointer --------------------------------------------------
    const uint8_t* ctrlPtr =
        pWorkBuffer->mCompressedHeaderData +
        pWorkBuffer->mNumBitsOffset;

    // --- Main decode loop ---------------------------------------------------
    for (uint32_t bone = 0; bone < boneCount; bone++)
    {
        // Ctrl word is stored big-endian on PPC.
        uint32_t ctrl =
            ((uint32_t)ctrlPtr[0] << 24) |
            ((uint32_t)ctrlPtr[1] << 16) |
            ((uint32_t)ctrlPtr[2] <<  8) |
             (uint32_t)ctrlPtr[3];
        ctrlPtr += 4;

        for (uint32_t k = 0; k < 8; k++)
        {
            uint32_t nb = (ctrl >> (k * 4)) & 0xFu;

            if (nb == 0)
            {
                // Zero nibble: no bits from either stream; output is zero.
                outBuf[bone * 8 + k] = 0;
                continue;
            }

            // Consume one selector bit from Stream A.
            uint32_t sel = selBuf & 1u;
            selBuf  >>= 1;
            selCount -= 1;

            if (sel == 1)
            {
                // Consume sign + nb magnitude bits from Stream B.
                refillMag();

                uint32_t sign_stream = (uint32_t)(magBuf & 1u);
                magBuf   >>= 1;
                magCount  -= 1;

                uint32_t mag = (uint32_t)(magBuf & ((1u << nb) - 1u));
                magBuf   >>= nb;
                magCount  -= nb;

                // vcuxwfp: reinterpret mag as unsigned int, convert to float.
                float fval = (float)mag;
                uint32_t fbits;
                std::memcpy(&fbits, &fval, sizeof(fbits));

                // sign_stream == 0 → negative (set IEEE-754 sign bit).
                if (sign_stream == 0)
                    fbits |= 0x80000000u;

                outBuf[bone * 8 + k] = (int32_t)fbits;
            }
            else
            {
                // Selector is 0: coefficient is zero.
                outBuf[bone * 8 + k] = 0;
            }
        }

        // After each complete bone (8 nibble slots): refill Stream A if low.
        if (selCount <= 8)
            refillSel();
    }

    DecompressFrameBlock(frameinBlk, pWorkBuffer, threadId);
}

void ANIM_CODEC::VBRDecompressor::DecompressFrame(
    const uint8_t* compressedData,
    uint32_t frame,
    uint32_t putLoc,
    uint8_t threadId)
{
    if (threadId >= 6) {
        std::cerr << "[VBR DecompressFrame] threadId out of range: " << (int)threadId << "\n";
        return;
    }

    DataPerDecompress& workBuffer = mWorkBuffers[threadId];
    if (!workBuffer.mhdr) {
        std::cerr << "[VBR DecompressFrame] mhdr not set up (call InitPerAnim first)\n";
        return;
    }

    // Per recomp (sk83_na_m_recomp.116.cpp:40605-40608): only mUsedTableOffset
    // and mCompressedData are reset here. mhdr / mCompressedHeaderData /
    // mFrameOffset / mNormalizedData / mConstData / mIsConstantChannel were
    // set by InitPerAnim and Init() and remain valid.
    workBuffer.mUsedTableOffset = 0;
    workBuffer.mCompressedData  = compressedData;

    // numChannels is byte 12 of the on-disk VBRDataHeaderValue. The bit stream
    // proper starts after the per-channel index prefix.
    const uint8_t numChannels = workBuffer.mhdr[12];
    const uint8_t frameBlock  = static_cast<uint8_t>(frame & 0x7);

    Vector_UnPackFrameBlockAndDecompressOneFrame(
        compressedData + numChannels,
        &workBuffer,
        frameBlock,
        threadId);

    // Distribute decompressed values via PutData (recomp:40618-40724). For
    // each channel block, walk (numEnt + numConst) joints; pull from
    // primaryData (mNormalizedData, advanced to this frame block's slot) for
    // animated joints and fallbackData (mConstData) for constant joints.
    const uint8_t* paletteBase     = nullptr;
    const uint8_t* channelInfoBase = nullptr;
    ComputeVBRInline(workBuffer.mhdr, paletteBase, channelInfoBase);

    const uint32_t alignedFrameSize = (workBuffer.mFrameOffset + 3u) & ~3u;
    const uint32_t blockByteOffset  = alignedFrameSize * frameBlock * 4u;

    const uint8_t* primaryB  = workBuffer.mNormalizedData
        ? reinterpret_cast<const uint8_t*>(workBuffer.mNormalizedData) + blockByteOffset
        : nullptr;
    const uint8_t* fallbackB = reinterpret_cast<const uint8_t*>(workBuffer.mConstData);
    const uint8_t* isConstPtr = reinterpret_cast<const uint8_t*>(workBuffer.mIsConstantChannel);

    // Per-clip diagnostic stays off by default (cluttered normal extraction
    // output). Toggle to true to dump channel layout / mConstData on frame 0
    // of each clip when investigating decode issues.
    constexpr bool kDecompressFrameVerbose = false;
    if (kDecompressFrameVerbose && frame == 0) {
        std::cerr << "[DecompressFrame trace] numChannels=" << (int)numChannels
                  << " mFrameOffset=" << workBuffer.mFrameOffset
                  << " putLoc=0x" << std::hex << putLoc << std::dec << "\n";
        const uint32_t channelInfoOff = (uint32_t)(channelInfoBase - workBuffer.mhdr);
        const uint32_t end = channelInfoOff + 64;  // 4 entries x 16 bytes
        std::cerr << "  hdr bytes [0.." << (end - 1) << "]: ";
        for (uint32_t b = 0; b < end; ++b) {
            std::cerr << std::hex << std::setw(2) << std::setfill('0')
                      << (int)workBuffer.mhdr[b] << ' ';
            if ((b & 0xF) == 0xF && b + 1 < end) std::cerr << "\n                     ";
        }
        std::cerr << std::dec << std::setfill(' ') << "\n";
        std::cerr << "  paletteBase offset=" << (paletteBase - workBuffer.mhdr)
                  << " (paletteSize=" << (int)workBuffer.mhdr[13] << ")"
                  << " channelInfoBase offset=" << channelInfoOff << "\n";
        for (uint8_t ch = 0; ch < numChannels; ++ch) {
            const uint8_t* entry = channelInfoBase + (uint32_t)ch * 16u;
            uint16_t nEnt   = Convert(*reinterpret_cast<const uint16_t*>(entry + 0));
            uint16_t nConst = Convert(*reinterpret_cast<const uint16_t*>(entry + 2));
            uint8_t  sz     = entry[12];
            std::cerr << "  ch=" << (int)ch
                      << " numEnt=" << nEnt << " numConst=" << nConst
                      << " size=" << (int)sz
                      << " (block.mDataSize=" << (int)mChannelBlocks[ch].mDataSize
                      << " mOffset=0x" << std::hex << mChannelBlocks[ch].mOffset
                      << " mStride=0x" << mChannelBlocks[ch].mStride << std::dec << ")\n";
        }
        // mCompressedHeaderData (palette indices + map runs)
        std::cerr << "  mCompressedHeaderData[0..15]: ";
        for (int b = 0; b < 16; ++b) {
            std::cerr << std::hex << std::setw(2) << std::setfill('0')
                      << (int)workBuffer.mCompressedHeaderData[b] << ' ';
        }
        std::cerr << std::dec << std::setfill(' ') << "\n";
        // mConstData (decoded constant float values, expected ~10 floats for our t-pose)
        std::cerr << "  mConstData[0..9]: ";
        for (int i = 0; i < 10; ++i) {
            std::cerr << workBuffer.mConstData[i] << ' ';
        }
        std::cerr << "\n";
        // mIsConstantChannel (1 byte per joint -- expected layout per channel)
        std::cerr << "  mIsConstantChannel[0..15]: ";
        const uint8_t* icc = reinterpret_cast<const uint8_t*>(workBuffer.mIsConstantChannel);
        for (int i = 0; i < 16; ++i) {
            std::cerr << (int)icc[i] << ' ';
        }
        std::cerr << "\n";
    }

    for (uint8_t ch = 0; ch < numChannels; ++ch) {
        const uint8_t* entry = channelInfoBase + (uint32_t)ch * 16u;
        uint16_t numEntPerFrm      = Convert(*reinterpret_cast<const uint16_t*>(entry + 0));
        uint16_t numConstEntPerFrm = Convert(*reinterpret_cast<const uint16_t*>(entry + 2));
        uint8_t  size              = entry[12];
        uint16_t total             = static_cast<uint16_t>(numEntPerFrm + numConstEntPerFrm);
        if (total == 0) continue;

        // size is float-count per joint; advance is in bytes. Per the recomp,
        // mIsConstantChannel / fallback / primary pointers all advance GLOBALLY
        // across channels (not reset per-channel).
        const uint32_t advanceBytes = (uint32_t)size * 4u;

        for (uint16_t j = 0; j < total; ++j) {
            const bool joint_is_const = isConstPtr && (*isConstPtr++ != 0);
            const float* src = reinterpret_cast<const float*>(joint_is_const ? fallbackB : primaryB);
            mChannelBlocks[ch].PutData(frame, j, src, putLoc);
            if (joint_is_const) fallbackB += advanceBytes;
            else if (primaryB)  primaryB  += advanceBytes;
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
    // mhdr is a direct pointer to the on-disk VBRDataHeaderValue (16-byte
    // aligned). Layout immediately after it (inline):
    //   [+16]  constant palette (4 * mConstantPaletteSize big-endian floats)
    //   [...]  constant-channel-map runs (2 * ceil(mConstChanMapSize/8) bytes)
    //   (aligned to 16) channel-info table (16 bytes per channel)
    const uint8_t* hdr = pWorkBuffer->mhdr;
    if (!hdr) {
        std::cerr << "[UnPackHeaderBits] mhdr is null\n";
        return nullptr;
    }

    // Single-byte fields don't need byte-swap; multi-byte ones do.
    uint8_t  numChannels  = hdr[12];   // mNumChannels
    uint16_t constMapSize = Convert(*reinterpret_cast<const uint16_t*>(hdr + 0));

    const uint8_t* paletteBase     = nullptr;
    const uint8_t* channelInfoBase = nullptr;
    ComputeVBRInline(hdr, paletteBase, channelInfoBase);

    const uint8_t* in            = pWorkBuffer->mCompressedHeaderData;
    float*         constDataOut  = pWorkBuffer->mConstData;
    uint8_t*       mapPtr        = reinterpret_cast<uint8_t*>(pWorkBuffer->mIsConstantChannel);

    // Per-channel constant-entry decode. Each channel-info entry is 16 bytes:
    //   [0]  u16 NumEntPerFrm
    //   [2]  u16 NumConstEntPerFrm
    //   [4]  f32 Min
    //   [8]  f32 Max
    //   [12] u8  Size
    for (uint8_t ch = 0; ch < numChannels; ++ch) {
        const uint8_t* entry = channelInfoBase + (uint32_t)ch * 16u;
        uint16_t numConstEntPerFrm = Convert(*reinterpret_cast<const uint16_t*>(entry + 2));
        float    min   = ReadBEFloat(entry + 4);
        float    max   = ReadBEFloat(entry + 8);
        uint8_t  size  = entry[12];
        float    range = max - min;

        int total = (int)size * (int)numConstEntPerFrm;
        for (int i = 0; i < total; ++i) {
            uint8_t paletteIdx = *in++;
            float   scale      = ReadBEFloat(paletteBase + (uint32_t)paletteIdx * 4u);
            *constDataOut++    = scale * range + min;
        }
    }

    // Constant-channel map decode (alternating-run encoding). The recomp
    // initializes the run value to 0, writes `count` copies, then toggles.
    {
        uint8_t curVal = 0;
        for (uint16_t i = 0; i < constMapSize; ++i) {
            uint8_t count = *in++;
            if (count) {
                std::memset(mapPtr, curVal, count);
                mapPtr += count;
            }
            curVal ^= 1;
        }
        if (constMapSize == 0 && numChannels > 0) {
            std::memset(mapPtr, 0, numChannels);
        }
    }

    // Recomp returns: bytesConsumed + mFrameOffset*4 (stored as
    // mMemoryHeaderSize by the InitPerAnim caller).
    uint32_t bytesConsumed   = static_cast<uint32_t>(in - pWorkBuffer->mCompressedHeaderData);
    pWorkBuffer->mNumBitsOffset = bytesConsumed;
    uint32_t frameOffsetBytes = (pWorkBuffer->mFrameOffset * 4u) & 0x7FFFFFFCu;
    return reinterpret_cast<const uint8_t*>(
        static_cast<uintptr_t>(bytesConsumed + frameOffsetBytes));
}

uint32_t ANIM_CODEC::VBRDecompressor::InitPerAnim(
    const uint8_t* compressedHeaderData,
    const uint8_t* headerData,        // pointer to on-disk VBRDataHeaderValue (16-byte aligned)
    uint8_t threadId)
{
    if ((reinterpret_cast<uintptr_t>(headerData) & 0xF) != 0) {
        std::cerr << "[VBR InitPerAnim] header not 16-byte aligned: 0x"
                  << std::hex << reinterpret_cast<uintptr_t>(headerData) << std::dec << "\n";
    }
    if (threadId >= 6) {
        std::cerr << "[VBR InitPerAnim] threadId out of range: " << (int)threadId << "\n";
        return 0;
    }

    DataPerDecompress& workBuffer = mWorkBuffers[threadId];
    workBuffer.mCompressedHeaderData = compressedHeaderData;

    // Cache: mhdr stores the VBRDataHeaderValue pointer directly. The recomp
    // uses a small wrapper struct here, but its fields f4/f8 are derivable
    // from mhdr (palette base = hdr+16; channel-info base = aligned ptr after
    // palette + map), so we just recompute them where needed.
    if (workBuffer.mhdr != headerData) {
        workBuffer.mhdr = headerData;
    }

    // numChannels is a single byte at offset 12 -- no swap.
    uint8_t numChannelBlocks = headerData[12];
    if (numChannelBlocks != this->mNumChannelBlocks) {
        std::cerr << "[VBR InitPerAnim] channel-block count mismatch: file says "
                  << (int)numChannelBlocks << ", set up "
                  << (int)this->mNumChannelBlocks << "\n";
    }

    const uint8_t* paletteBase     = nullptr;
    const uint8_t* channelInfoBase = nullptr;
    ComputeVBRInline(headerData, paletteBase, channelInfoBase);

    // Walk the channel-info table to fill mChannelBlockStarts/Ends and
    // mNumConstantChannels. Each entry is 16 bytes; layout matches the recomp
    // (NumEntPerFrm@0, NumConstEntPerFrm@2, Min@4, Max@8, Size@12).
    uint32_t totalConstChannels = 0;
    uint16_t lastEnd = 0;
    for (uint8_t i = 0; i < numChannelBlocks && i < 4; ++i) {
        const uint8_t* entry = channelInfoBase + (uint32_t)i * 16u;
        uint8_t  dataSize = entry[12];
        uint16_t numEnt   = Convert(*reinterpret_cast<const uint16_t*>(entry + 0));
        uint16_t numConst = Convert(*reinterpret_cast<const uint16_t*>(entry + 2));

        workBuffer.mChannelBlockStarts[i] = (i == 0) ? 0 : lastEnd;
        lastEnd = static_cast<uint16_t>(workBuffer.mChannelBlockStarts[i] + numEnt * dataSize);
        workBuffer.mChannelBlockEnds[i] = lastEnd;
        totalConstChannels += static_cast<uint32_t>(numConst) * dataSize;

        if (this->mChannelBlocks[i].mDataSize != dataSize) {
            std::cerr << "[VBR InitPerAnim] mChannelBlocks[" << (int)i
                      << "].mDataSize mismatch (file " << (int)dataSize
                      << " vs setup " << (int)this->mChannelBlocks[i].mDataSize << ")\n";
        }
    }
    workBuffer.mNumConstantChannels = totalConstChannels;
    workBuffer.mFrameOffset         = lastEnd;

    uintptr_t result = reinterpret_cast<uintptr_t>(this->UnPackHeaderBits(&workBuffer));
    workBuffer.mMemoryHeaderSize = static_cast<uint32_t>(result);
    return static_cast<uint32_t>(result);
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
    // Toggle to true to log entry / per-part header info when debugging the
    // extraction pipeline. Off by default so normal CSV runs aren't noisy.
    constexpr bool kExtractVerbose = false;
    if (kExtractVerbose) {
        static int s_calls = 0;
        if (s_calls < 3) {
            std::cerr << "[Scalar_ExtractPackedNVBR] entry, extractBuffSize=" << extractBuffSize
                      << " decompressor=0x" << std::hex << (uintptr_t)decompressor << std::dec << "\n";
            s_calls++;
        }
    }
    if (extractBuffSize == 0) {
        return;
    }

    static SQTData prevFrame;  // Store previous trajectory frame for continuity

    for (uint32_t i = 0; i < extractBuffSize; ++i) {
        ExtractType& extract = extractBuffer[i];

        AnimationPart* part = reinterpret_cast<AnimationPart*>(extract.header);
        if (kExtractVerbose) {
            static int s_iTrace = 0;
            if (s_iTrace < 4) {
                std::cerr << "  [Scalar_ExtractPackedNVBR loop] i=" << i
                          << " header=0x" << std::hex << (uintptr_t)extract.header
                          << " sqtBuff=0x" << (uintptr_t)extract.sqtBuff << std::dec
                          << " data[0]=" << extract.data[0]
                          << " data[1]=" << extract.data[1]
                          << " channelAnim=" << extract.channelAnim
                          << " totalBones=" << extract.totalBones << "\n";
                s_iTrace++;
            }
        }
        if (!part) continue;

        // All offset/header fields in AnimationPart are stored big-endian in
        // the file. Match the RD path which calls Convert() on each.
        // Per Skate 2 decomp (sk82_na_zd.xex.c:3417208), `compressedHeaderData`
        // (1st arg to InitPerAnim) and `compressedData` (1st arg to
        // DecompressFrame) are the SAME pointer: part + part->mCompressedData.
        // The previous `basePtr + 0xC` was reading palette-index bytes from
        // the mCompressedData *field's* raw bytes (the BE offset value),
        // producing garbage decoded constants in mConstData.
        const uint8_t* basePtr = reinterpret_cast<const uint8_t*>(part);
        const uint8_t* compressedData   = basePtr + Convert(part->mCompressedData);
        const uint8_t* compressedHeader = compressedData;
        uint32_t channelCount = ((Convert(part->mHeader) >> 6) & 0x3F) + 1;

        uint16_t currFrame = extract.data[0];
        uint16_t nextFrame = extract.data[1];

        // Offset of the decompression-header (ptr2) is stored at part+8 as a
        // big-endian uint32 relative to part.
        uint32_t offset2 = Convert(*reinterpret_cast<const uint32_t*>(basePtr + 8));
        const uint8_t* ptr2 = basePtr + offset2;

        // Initialize decompressor with compressed header and decompression header ptr.
        // InitPerAnim stores `mMemoryHeaderSize = bytesConsumed + mFrameOffset*4`
        // in the work buffer -- the byte offset within `compressedData` where
        // the per-frame bit stream begins (just past const palette indices /
        // run-encoded channel-map).
        uint32_t memHeaderSize = decompressor->InitPerAnim(compressedHeader, ptr2, 0);

        SQTData* outputBuffer = reinterpret_cast<SQTData*>(extract.sqtBuff);

        // Per recomp (sk82_na_zd.xex.c:3417235-3417249): the compressedData
        // pointer passed to DecompressFrame is advanced PER FRAME BLOCK by the
        // sum of preceding mFrameBlockSizes. mFrameBlockSizes lives inline at
        // `ptr2 + 16 + 4*paletteSize` -- u16 BE entries, one per frame block
        // (block count = ceil(mNumFrames/8)).
        const uint8_t* perFrameStream = compressedData + memHeaderSize;
        const uint32_t frameBlockIdx  = (uint32_t)nextFrame >> 3;
        if (frameBlockIdx > 0) {
            const uint8_t  paletteSize     = ptr2[13];
            const uint8_t* mFrameBlockSizes = ptr2 + 16 + 4u * paletteSize;
            uint32_t advance = 0;
            for (uint32_t b = 0; b < frameBlockIdx; ++b) {
                advance += Convert(*reinterpret_cast<const uint16_t*>(
                    mFrameBlockSizes + (size_t)b * 2));
            }
            perFrameStream += advance;
        }

        // Decompress only the next frame (no interpolation). putLoc is the
        // base address PutData adds to (see PutData: targetData = mStride*joint
        // + mOffset + putLoc). Pass the per-frame-block pointer so the bit
        // decoder reads the right block's data.
        decompressor->DecompressFrame(perFrameStream, nextFrame,
                                      reinterpret_cast<uint32_t>(outputBuffer), 0);

        // Per recomp (sk83_na_m_recomp.97.cpp:41155-41184): the loop writes
        // floats from `part + 0x10` to bone[j].mTrans.W (offset 44 within each
        // SQTData, i.e. mTrans Vector4's W lane), NOT mScale.Z. The previous
        // code was clobbering mScale.Z with these "trans-W" values, producing
        // garbage like sz=-nan on bone 0. The values themselves are BE floats
        // on disk -> Convert() on read.
        const float* transWValues = reinterpret_cast<const float*>(basePtr + 0x10);

        for (uint32_t j = 0; j < channelCount; ++j) {
            float w = extract.channelAnim ? Convert(transWValues[j]) : extract.deltaT[0];
            outputBuffer[j].mTrans.SetW(w);
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

uint32_t Andale::ExtractAllVBRParts(const DataHeader* hdr,
                                    uint32_t frameIndex,
                                    ANIM_CODEC::VBRDecompressor* decompressor,
                                    SQTData* sqtBuffer,
                                    uint32_t maxBones)
{
    if (!hdr || !decompressor || !sqtBuffer || maxBones == 0) return 0;

    const uint32_t* alignedData = reinterpret_cast<const uint32_t*>(
        (reinterpret_cast<uintptr_t>(&hdr[1].mGUID) + 3) & ~uintptr_t(0xF));
    const AnimationData* animData = reinterpret_cast<const AnimationData*>(alignedData);

    uint32_t bonesWritten = 0;

    // The on-disk parts table is contiguous and entries that aren't real have
    // an offset of 0 (or an offset that points back at the header itself).
    // We scan a generous upper bound and stop on a clearly-invalid entry.
    constexpr uint32_t kMaxPartsScan = 64;

    for (uint32_t i = 0; i < kMaxPartsScan && bonesWritten < maxBones; ++i) {
        // Each entry is BE-stored: high byte = partID, low 24 bits = animPart
        // offset relative to alignedData. We don't care about partID here --
        // skeleton-free extraction means we take every part with a valid offset.
        uint32_t entry = Convert(animData->mPartsOffsets[i]);
        uint32_t partOff = entry & 0xFFFFFFu;
        if (partOff == 0) break;  // sentinel / end of table

        const AnimationPart* part = reinterpret_cast<const AnimationPart*>(
            reinterpret_cast<const uint8_t*>(alignedData) + partOff);

        // Sanity: a real AnimationPart's mHeader has the upper 20 bits used
        // for sizing fields. If those are all zero the offset is bogus.
        uint32_t hdrWord = Convert(part->mHeader);
        if ((hdrWord & 0xFFFFF000u) == 0) break;

        // Set up an extract that points at *this* part with the output slice
        // starting at sqtBuffer[bonesWritten].
        ExtractType extract = {};
        extract.header     = reinterpret_cast<uint32_t>(part);
        extract.sqtBuff    = reinterpret_cast<uint32_t>(sqtBuffer + bonesWritten);
        extract.totalBones = static_cast<uint16_t>(maxBones - bonesWritten);
        extract.data[0]    = static_cast<uint16_t>(frameIndex);
        extract.data[1]    = static_cast<uint16_t>(frameIndex);
        extract.codecType  = static_cast<uint8_t>(codec_type::CONV_FORMAT);
        extract.deltaQ     = rw::math::vpu::Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
        extract.deltaT[0]  = extract.deltaT[1] = extract.deltaT[2] = 0.0f;

        // Decode this part. Existing Scalar_ExtractPackedNVBR handles
        // InitPerAnim / DecompressFrame / per-frame-block advance / PutData.
        Scalar_ExtractPackedNVBR(&extract, 1, decompressor);

        // Determine how many SQTs this part actually wrote so we can advance
        // the cursor. Channel-info table sits at ptr2 + 48 (per ComputeVBRInline
        // for typical pose layout) but the *actual* offset depends on
        // paletteSize and mNumFrames. We re-derive it.
        const uint8_t* basePtr = reinterpret_cast<const uint8_t*>(part);
        uint32_t hdrOff = Convert(*reinterpret_cast<const uint32_t*>(basePtr + 8));
        const uint8_t* ptr2 = basePtr + hdrOff;

        const uint8_t* paletteBase = nullptr;
        const uint8_t* channelInfoBase = nullptr;
        ComputeVBRInline(ptr2, paletteBase, channelInfoBase);

        uint8_t numChannels = ptr2[12];
        uint16_t partTotal = 0;  // bones written by this part = max(numEnt + numConst) per channel
        for (uint8_t ch = 0; ch < numChannels; ++ch) {
            const uint8_t* entryC = channelInfoBase + (uint32_t)ch * 16u;
            uint16_t numEnt   = Convert(*reinterpret_cast<const uint16_t*>(entryC + 0));
            uint16_t numConst = Convert(*reinterpret_cast<const uint16_t*>(entryC + 2));
            uint16_t total = static_cast<uint16_t>(numEnt + numConst);
            if (total > partTotal) partTotal = total;
        }
        if (partTotal == 0) partTotal = 1;  // at least one bone landed via PutData

        bonesWritten += partTotal;
    }

    return bonesWritten;
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
            // sqtBuff is the BASE of the main output buffer (same for every
            // extract slot); current.sqtOffset is the slot's per-bone-group
            // start. Write index is base + slot offset + bone-within-slot.
            SQTData& outData = output[current.sqtOffset + i];

            uint32_t wordIndex = i >> 5;
            uint32_t bitMask = 1 << (i & 0x1F);
            if (Convert(bitmask[wordIndex + 0]) & bitMask) {
                rw::math::vpu::Vector3 scale{};
                float* floatPtr = reinterpret_cast<float*>(dataPointer);
                scale.SetX(Convert(floatPtr[0]));
                scale.SetY(Convert(floatPtr[1]));
                scale.SetZ(Convert(floatPtr[2]));
                dataPointer += 3 * sizeof(float);
                outData.mScale = scale;
            }
            else {
                outData.mScale = rw::math::vpu::Vector3{ 1.0f, 1.0f, 1.0f };
            }
            if (Convert(bitmask[wordIndex + 2]) & bitMask) {
                rw::math::vpu::Quaternion quat{};
                if (CompressedQ) {
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
                outData.mQuat = quat;
            }
            else {
                outData.mQuat = rw::math::vpu::Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
            }
            if (Convert(bitmask[wordIndex + 4]) & bitMask) {
                rw::math::vpu::Vector4 trans{};
                if (CompressedT) {
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
                    trans.SetW(0.0f);  // file only stores 3 components
                    dataPointer += 3 * sizeof(float);
                }
                outData.mTrans = trans;
            }
            else {
                outData.mTrans = rw::math::vpu::Vector4{ 0.0f, 0.0f, 0.0f, 0.0f };
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
    // mBitField & 0x3F gives the part count for clips that match this
    // skeleton. For clips designed for a different skeleton (whose part
    // entries are all-zero in this rig), the field's low 6 bits read as 0,
    // matchedParts >= 0 breaks the loop, and BatchFetch correctly returns
    // an empty extract (header=0) -- same outcome as the recomp.
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

    {
        static int s_called = 0;
        if (s_called < 1) {
            std::cerr << "[BatchFetch trace] numParts=" << numParts
                      << " totalParts=" << totalParts
                      << " bitField=0x" << std::hex << bitField << std::dec
                      << " nbFrames=" << nbFrames << " fps=" << fps << "\n";
            const uint8_t* ad = reinterpret_cast<const uint8_t*>(alignedData);
            std::cerr << "  alignedData[0..63]: ";
            for (int b = 0; b < 64; ++b) {
                std::cerr << std::hex << std::setw(2) << std::setfill('0')
                          << (int)ad[b] << ' ';
                if ((b & 0xF) == 0xF && b + 1 < 64) std::cerr << "\n                      ";
            }
            std::cerr << std::dec << std::setfill(' ') << "\n";
            std::cerr << "  partIDs[0..7] (current = +56): ";
            for (int k = 0; k < 8; ++k) {
                std::cerr << (int)partIDs[k * 4] << ' ';
            }
            std::cerr << "\n  partIDs[0..7] (recomp = +4): ";
            for (int k = 0; k < 8; ++k) {
                std::cerr << (int)ad[4 + k * 4] << ' ';
            }
            std::cerr << "\n  skel boneIDs sample: ";
            const uint16_t* bp = boneIDs;
            for (int k = 0; k < (int)std::min<uint32_t>(numParts, 8); ++k) {
                std::cerr << Convert(bp[0]) << ' ';
                bp += 0x12;
            }
            std::cerr << "\n";
            s_called++;
        }
    }

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
        // VBR codec setup. Allocates a VBRDecompressor, registers the three
        // channel blocks (translation, rotation, scale -- channel-types 3/4/3),
        // and runs Init() to size scratch + populate IDCT/quant tables.
        // Original sizeof = 0x55D0 (= 21968) bytes, which matches our
        // VBRDecompressorData layout.
        ANIM_CODEC::VBRDecompressor* v = (ANIM_CODEC::VBRDecompressor*)Anim::UserAnimCodecAlloc(
            sizeof(ANIM_CODEC::VBRDecompressor), "AnimCodec::VBRDecompressor");
        if (v) {
            // Placement-new so the C++ constructor runs.
            new (v) ANIM_CODEC::VBRDecompressor();
        }
        this->mDecompressor = reinterpret_cast<uint32_t>(v);

        if (v) {
            v->BeginAddingChannels();
            v->AddChannel(3u, 0, 0x00u, 0x30u);
            v->AddChannel(4u, 0, 0x10u, 0x30u);
            v->AddChannel(3u, 0, 0x20u, 0x30u);
            v->EndAddingChannels();
            v->Init(Convert(skel->mNbBones));
        }
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

    {
        static int s_calls = 0;
        if (s_calls < 3) {
            std::cerr << "[Fetch(frame)] frameIndex=" << frameIndex
                      << " fetchedCount=" << fetchedCount
                      << " codec=" << (int)this->mCodecType << "\n";
            s_calls++;
        }
    }

    codec_type codec = static_cast<codec_type>(this->mCodecType);

    if (codec != codec_type::RAW) {
        if (codec == codec_type::CONV_FORMAT) {
            Scalar_ExtractPackedNVBR(extractBuffer, fetchedCount, reinterpret_cast<ANIM_CODEC::VBRDecompressor*>(this->mDecompressor));
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
    codec_type codec = static_cast<codec_type>(this->mCodecType);

    if (codec != codec_type::RAW) {
        if (codec == codec_type::CONV_FORMAT) {
            Scalar_ExtractPackedNVBR(extractBuffer, fetchedCount, reinterpret_cast<ANIM_CODEC::VBRDecompressor*>(this->mDecompressor));
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

    // If the file couldn't be opened, SetDBContent leaves mHierarchy = 0.
    // Return a zeroed FetchSys so the caller can detect failure via
    // fs.mDataBaseManager / fs.mSkel rather than crashing here.
    if (db[0].mHierarchy == 0) {
        Andale::FetchSys fs(dbManager, nullptr, 0, true, Andale::codec_type::RAW);
        fs.mSkel = 0;  // mark as load-failed
        return fs;
    }

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
    if (!hdr) {
        std::cout << "loadAnimation: no clip for GUID 0x" << std::hex << animGUID << std::dec
                  << " (try loadPose if this is a pose, or check the DB index)\n";
        return;
    }

    const uint32_t* alignedData = reinterpret_cast<const uint32_t*>(
        (reinterpret_cast<uintptr_t>(&hdr[1].mGUID) + 3) & ~0xF);

    const AnimationData* animData = reinterpret_cast<const AnimationData*>(alignedData);

    float fps = Convert(animData->mFPS);
    float nbFrames = Convert(animData->mNbFrames);
    float baseSpeed = Convert(animData->mBaseSpeed);
    uint32_t attributesOffset = Convert(animData->mAttributesOffset);
    uint32_t blendPosePartsOffsetsOffset = Convert(animData->mBlendPosePartsOffsetsOffset);
    // mBitField & 0x3F gives the part count for clips that match this
    // skeleton. For clips designed for a different skeleton (whose part
    // entries are all-zero in this rig), the field's low 6 bits read as 0,
    // matchedParts >= 0 breaks the loop, and BatchFetch correctly returns
    // an empty extract (header=0) -- same outcome as the recomp.
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

//============================================================================
// VBR ground-truth trace reader.
//
// Reads C:/Users/tuukk/Desktop/vbr_trace.bin (or arbitrary path) produced by
// the rexglue project's VBR hooks and prints / diffs the recorded calls.
//
// Trace file format:
//   header   : char[8] "VBRTRACE", u32 version=1
//   per call : char[4] "REC0", u32 kind, u32 idx, u32 payloadSize, bytes payload
// Kinds: 1=InitPerAnim 2=UnPackHeaderBits 3=Vector_UnPack 4=DecompressFrameBlock 5=DecompressFrame
//============================================================================
namespace vbr_compare {

struct InitPerAnimPayload {
    uint32_t this_ptr;
    uint32_t compressedHeaderData;
    uint32_t headerData;
    uint32_t threadId;
    uint32_t result;
    uint32_t pad;
    uint8_t  workBuffer_pre[64];
    uint8_t  workBuffer_post[64];
    uint8_t  mhdrWrapper_pre[24];
    uint8_t  mhdrWrapper_post[24];
    uint8_t  headerDump[256];
    // Increased from 64 to 256: with mNumBitsOffset up to ~200 and
    // boneCount*4 up to ~88 bytes, 64 was too small for the ctrl word
    // region (e.g. mNumBitsOffset=45 + 11 bones needs 89 bytes).
    uint8_t  cmpHdrDump[256];
};
struct UnPackHeaderBitsPayload {
    uint32_t this_ptr;
    uint32_t pWorkBuffer;
    uint32_t result;
    uint32_t pad;
    uint8_t  workBuffer_pre[64];
    uint8_t  workBuffer_post[64];
    uint8_t  mhdrWrapper[24];
    uint32_t mConstDataPtr;
    uint32_t mIsConstantPtr;
    uint32_t mCompressedHeaderDataPtr;
    uint32_t pad2;
    uint8_t  mCompressedHeaderDump[128];
    uint8_t  mConstData_post[256];
    uint8_t  mIsConstantChannel_post[64];
};
struct VectorUnPackPayload {
    uint32_t this_ptr;
    uint32_t pCompressedData;
    uint32_t pWorkBuffer;
    uint32_t frameinBlk;
    uint32_t threadId;
    uint32_t scratchPtr;
    uint32_t normalizedPtr;
    uint32_t pad;
    uint8_t  workBuffer_pre[64];
    uint8_t  workBuffer_post[64];
    uint8_t  mhdrWrapper[24];
    uint8_t  cmpDataDump[256];
    uint8_t  uncompressedScratch_pre[1024];   // stale state from prior calls
    uint8_t  uncompressedScratch_post[1024];
    uint8_t  normalizedData_pre[1024];        // stale state from prior calls
    uint8_t  normalizedData_post[1024];
};
struct DecompressFrameBlockPayload {
    uint32_t this_ptr;
    uint32_t frameinBlk;
    uint32_t pWorkBuffer;
    uint32_t threadId;
    uint32_t scratchPtr;
    uint32_t normalizedPtr;
    uint8_t  workBuffer_pre[64];
    uint8_t  mhdrWrapper[24];
    uint8_t  uncompressedScratch_pre[1024];
    uint8_t  normalizedData_post[1024];
};
struct DecompressFramePayload {
    uint32_t this_ptr;
    uint32_t compressedData;
    uint32_t frame;
    uint32_t putLoc;
    uint32_t threadId;
    uint32_t pad;
    uint8_t  workBuffer_pre[64];
    uint8_t  workBuffer_post[64];
    uint8_t  mhdrWrapper[24];
    uint8_t  cmpDataDump[256];
    uint8_t  putLoc_pre[36 * 48];
    uint8_t  putLoc_post[36 * 48];
};

static const char* kindName(uint32_t k) {
    switch (k) {
        case 1: return "InitPerAnim";
        case 2: return "UnPackHeaderBits";
        case 3: return "Vector_UnPack";
        case 4: return "DecompressFrameBlock";
        case 5: return "DecompressFrame";
        default: return "?";
    }
}

// Pretty-print a chunk of bytes, 16 per line, hex.
static void dumpBytes(const char* label, const uint8_t* p, size_t n, size_t maxLines = 4) {
    std::cout << "  " << label << ":";
    size_t lines = (n + 15) / 16;
    if (lines > maxLines) lines = maxLines;
    for (size_t row = 0; row < lines; ++row) {
        std::cout << "\n   ";
        size_t base_off = row * 16;
        for (size_t col = 0; col < 16 && base_off + col < n; ++col) {
            std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0')
                      << (int)p[base_off + col];
        }
    }
    std::cout << std::dec << std::setfill(' ') << "\n";
}

// Big-endian -> host float (recomp memory is PPC-native BE; we run on x86 LE).
static inline float beFloat(const uint8_t* p) {
    uint32_t u = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
                 ((uint32_t)p[2] << 8)  | (uint32_t)p[3];
    float f;
    std::memcpy(&f, &u, 4);
    return f;
}
static inline uint32_t beU32(const uint8_t* p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8)  | (uint32_t)p[3];
}

// Dump every DecompressFrame record's putLoc_post as a CSV file with one row
// per (record, bone). Layout matches AnimTest's CSV writer so users can diff
// directly against an AnimTest export.
static int exportFramesCsv(const char* tracePath, const char* csvPath) {
    std::ifstream in(tracePath, std::ios::binary);
    if (!in) {
        std::cout << "Cannot open " << tracePath << "\n";
        return 1;
    }
    char magic[8];
    in.read(magic, 8);
    if (std::memcmp(magic, "VBRTRACE", 8) != 0) {
        std::cout << "Not a VBRTRACE file\n";
        return 1;
    }
    uint32_t version = 0;
    in.read(reinterpret_cast<char*>(&version), 4);

    std::ofstream out(csvPath);
    if (!out) {
        std::cout << "Cannot open " << csvPath << " for writing\n";
        return 1;
    }
    out << "rec,frame,putLoc,bone,sx,sy,sz,sw,qx,qy,qz,qw,tx,ty,tz,tw\n";
    out << std::fixed;
    out.precision(6);

    int written = 0;
    while (in.peek() != EOF) {
        char marker[4];
        in.read(marker, 4);
        if (in.gcount() != 4) break;
        uint32_t kind = 0, idx = 0, sz = 0;
        in.read(reinterpret_cast<char*>(&kind), 4);
        in.read(reinterpret_cast<char*>(&idx),  4);
        in.read(reinterpret_cast<char*>(&sz),   4);
        std::vector<uint8_t> payload(sz);
        in.read(reinterpret_cast<char*>(payload.data()), sz);
        if (kind != 5 || sz < sizeof(DecompressFramePayload)) continue;

        auto* p = reinterpret_cast<const DecompressFramePayload*>(payload.data());
        // 36 bones x 48 bytes per SQTData.
        for (int bone = 0; bone < 36; ++bone) {
            const uint8_t* sqt = p->putLoc_post + bone * 48;
            // Layout per SQTData: mScale Vector3 (16 bytes incl pad),
            // mQuat Quaternion (16), mTrans Vector4 (16). Floats are BE.
            float sx = beFloat(sqt + 0);
            float sy = beFloat(sqt + 4);
            float sz = beFloat(sqt + 8);
            float sw = beFloat(sqt + 12);
            float qx = beFloat(sqt + 16);
            float qy = beFloat(sqt + 20);
            float qz = beFloat(sqt + 24);
            float qw = beFloat(sqt + 28);
            float tx = beFloat(sqt + 32);
            float ty = beFloat(sqt + 36);
            float tz = beFloat(sqt + 40);
            float tw = beFloat(sqt + 44);
            out << idx << ',' << p->frame << ",0x" << std::hex << p->putLoc
                << std::dec << ',' << bone
                << ',' << sx << ',' << sy << ',' << sz << ',' << sw
                << ',' << qx << ',' << qy << ',' << qz << ',' << qw
                << ',' << tx << ',' << ty << ',' << tz << ',' << tw
                << '\n';
        }
        written++;
    }
    std::cout << "Wrote " << csvPath << " (" << written << " DecompressFrame records, "
              << (written * 36) << " rows)\n";
    return 0;
}

// Per-record comparator: replays each trace record's inputs through AnimTest's
// VBR decoder and reports divergences against the recomp's recorded outputs.
//
// Note: our decoder uses workBuffer.mhdr as a direct VBRDataHeaderValue ptr,
// while the recomp uses a wrapper struct (mhdr -> { mData, mPalette, mChInfo,
// mFrameBlockSizes, flag }). We can't byte-compare workBuffer.mhdr; instead we
// compare the FUNCTIONAL outputs: mNumBitsOffset, mFrameOffset,
// mNumConstantChannels, mChannelBlockStarts/Ends, mConstData[], and
// mIsConstantChannel[].
static int diff(const char* tracePath) {
    std::ifstream in(tracePath, std::ios::binary);
    if (!in) {
        std::cout << "Cannot open " << tracePath << "\n";
        return 1;
    }
    char magic[8];
    in.read(magic, 8);
    if (std::memcmp(magic, "VBRTRACE", 8) != 0) {
        std::cout << "Not a VBRTRACE file\n";
        return 1;
    }
    uint32_t version = 0;
    in.read(reinterpret_cast<char*>(&version), 4);

    // Build a fresh decompressor matching the game's setup: SQT layout
    // (size 3, 4, 3 -- scale/quat/trans) at offsets 0/16/32 with stride 48.
    // Allocate enough scratch for ~256 bones.
    constexpr uint32_t kScratchBones = 256;
    auto* dec = new ANIM_CODEC::VBRDecompressor();
    dec->BeginAddingChannels();
    dec->AddChannel(3u, 0, 0x00u, 0x30u);
    dec->AddChannel(4u, 0, 0x10u, 0x30u);
    dec->AddChannel(3u, 0, 0x20u, 0x30u);
    dec->EndAddingChannels();
    dec->Init(kScratchBones);

    int recordsTotal = 0, recordsCompared = 0, recordsMatched = 0;
    int firstMismatchPrinted = 0;

    while (in.peek() != EOF) {
        char marker[4];
        in.read(marker, 4);
        if (in.gcount() != 4) break;
        uint32_t kind = 0, idx = 0, sz = 0;
        in.read(reinterpret_cast<char*>(&kind), 4);
        in.read(reinterpret_cast<char*>(&idx),  4);
        in.read(reinterpret_cast<char*>(&sz),   4);
        std::vector<uint8_t> payload(sz);
        in.read(reinterpret_cast<char*>(payload.data()), sz);
        recordsTotal++;
        if (kind != 1 || sz < sizeof(InitPerAnimPayload)) continue;

        auto* p = reinterpret_cast<const InitPerAnimPayload*>(payload.data());
        recordsCompared++;

        // Replay: copy the recorded headerData bytes (256) and cmpHdrDump
        // bytes (256) into local buffers and run our InitPerAnim against them.
        // We use threadId 0 regardless of the recorded threadId since each
        // call starts from a clean workBuffer.
        alignas(16) uint8_t localHeader[256];
        std::memcpy(localHeader, p->headerDump, sizeof(localHeader));

        // Allocate enough room for the 256-byte cmpHdrDump.  UnPackHeaderBits
        // can read past it if mConstChanMapSize is very large; beyond that we
        // read zeros -- acceptable for short clips.
        alignas(16) uint8_t localCmpHdr[256] = {};
        std::memcpy(localCmpHdr, p->cmpHdrDump, sizeof(p->cmpHdrDump));

        uint32_t ourResult = dec->InitPerAnim(localCmpHdr, localHeader, 0);

        // Read recomp's expected outputs (BE-stored u32s in workBuffer_post).
        // Layout offsets we care about (per recomp's PPC struct):
        //   +16  mNumBitsOffset
        //   +40  mNumConstantChannels
        //   +44  mFrameOffset
        //   +48..55 mChannelBlockStarts[4]  (u16 BE each)
        //   +56..63 mChannelBlockEnds[4]    (u16 BE each)
        uint32_t expNumBitsOffset       = beU32(p->workBuffer_post + 16);
        uint32_t expNumConstantChannels = beU32(p->workBuffer_post + 40);
        uint32_t expFrameOffset         = beU32(p->workBuffer_post + 44);
        uint16_t expStarts[4];
        uint16_t expEnds[4];
        for (int k = 0; k < 4; ++k) {
            expStarts[k] = ((uint16_t)p->workBuffer_post[48 + k*2] << 8)
                         |  (uint16_t)p->workBuffer_post[48 + k*2 + 1];
            expEnds[k]   = ((uint16_t)p->workBuffer_post[56 + k*2] << 8)
                         |  (uint16_t)p->workBuffer_post[56 + k*2 + 1];
        }

        ANIM_CODEC::DataPerDecompress& wb = dec->mWorkBuffers[0];

        bool mismatch = false;
        if (wb.mNumBitsOffset != expNumBitsOffset)             mismatch = true;
        if (wb.mNumConstantChannels != expNumConstantChannels) mismatch = true;
        if (wb.mFrameOffset != expFrameOffset)                 mismatch = true;
        for (int k = 0; k < 4; ++k) {
            if (wb.mChannelBlockStarts[k] != expStarts[k]) mismatch = true;
            if (wb.mChannelBlockEnds[k]   != expEnds[k])   mismatch = true;
        }

        // (mConstData diff happens in the UnPackHeaderBits comparator -- the
        // InitPerAnim payload doesn't carry the decoded const buffer; only the
        // workBuffer fields it writes are checked here.)

        if (!mismatch) {
            recordsMatched++;
            continue;
        }
        if (firstMismatchPrinted >= 5) continue;
        firstMismatchPrinted++;

        std::cout << "\n[#" << idx << "] InitPerAnim DIVERGES\n";
        std::cout << "  hdr=0x" << std::hex << p->headerData
                  << " cmpHdr=0x" << p->compressedHeaderData
                  << " tid=" << std::dec << p->threadId
                  << " (replayed at threadId=0)\n";
        std::cout << "  field           recomp        ours\n";
        std::cout << "  mNumBitsOffset  " << expNumBitsOffset
                  << "             " << wb.mNumBitsOffset
                  << ((wb.mNumBitsOffset == expNumBitsOffset) ? " ✓" : " ✗") << "\n";
        std::cout << "  mNumConstantCh  " << expNumConstantChannels
                  << "             " << wb.mNumConstantChannels
                  << ((wb.mNumConstantChannels == expNumConstantChannels) ? " ✓" : " ✗") << "\n";
        std::cout << "  mFrameOffset    " << expFrameOffset
                  << "             " << wb.mFrameOffset
                  << ((wb.mFrameOffset == expFrameOffset) ? " ✓" : " ✗") << "\n";
        for (int k = 0; k < 4; ++k) {
            std::cout << "  blockStart[" << k << "]   " << expStarts[k]
                      << "            " << wb.mChannelBlockStarts[k]
                      << ((wb.mChannelBlockStarts[k] == expStarts[k]) ? " ✓" : " ✗") << "\n";
        }
        for (int k = 0; k < 4; ++k) {
            std::cout << "  blockEnd[" << k << "]     " << expEnds[k]
                      << "            " << wb.mChannelBlockEnds[k]
                      << ((wb.mChannelBlockEnds[k] == expEnds[k]) ? " ✓" : " ✗") << "\n";
        }
    }

    std::cout << "\n=== InitPerAnim diff summary ===\n";
    std::cout << "  total records  : " << recordsTotal    << "\n";
    std::cout << "  InitPerAnim    : " << recordsCompared << "\n";
    std::cout << "  matched        : " << recordsMatched  << "\n";
    std::cout << "  diverged       : " << (recordsCompared - recordsMatched) << "\n";

    // ------------------------------------------------------------------
    // Pass 2: UnPackHeaderBits diff. Walks the trace again, replays each
    // record's inputs, and compares mConstData / mIsConstantChannel /
    // mNumBitsOffset (return value).
    // ------------------------------------------------------------------
    in.clear();
    in.seekg(12);  // skip header
    int upRecords = 0, upMatched = 0, upPrinted = 0;
    int constMismatchCount = 0, isConstMismatchCount = 0, resultMismatchCount = 0;

    while (in.peek() != EOF) {
        char marker[4];
        in.read(marker, 4);
        if (in.gcount() != 4) break;
        uint32_t kind = 0, idx = 0, sz = 0;
        in.read(reinterpret_cast<char*>(&kind), 4);
        in.read(reinterpret_cast<char*>(&idx),  4);
        in.read(reinterpret_cast<char*>(&sz),   4);
        std::vector<uint8_t> payload(sz);
        in.read(reinterpret_cast<char*>(payload.data()), sz);
        if (kind != 2 || sz < sizeof(UnPackHeaderBitsPayload)) continue;
        upRecords++;

        auto* p = reinterpret_cast<const UnPackHeaderBitsPayload*>(payload.data());

        // Reconstruct the inputs UnPackHeaderBits saw:
        //   - workBuffer.mhdr -> wrapper struct -> headerData (from
        //     mhdrWrapper field0 = headerData ptr; we already have those
        //     bytes via the InitPerAnim record by index, but UnPackHeaderBits
        //     itself doesn't carry them. Instead we can derive: in our
        //     simplified scheme mhdr IS the VBRDataHeaderValue ptr, so we
        //     need to materialize the headerData bytes the wrapper points at.
        //     We don't have those in this record -- BUT we have the same
        //     palette/channelInfo data accessible via the workBuffer_pre's
        //     mhdr wrapper which lives in foreign memory. To keep the
        //     comparator self-contained we re-run InitPerAnim from the
        //     PAIRED InitPerAnim record (index = idx + 1 in the trace).
        //
        //   - mCompressedHeaderData bytes (the immutable input we DO have).
        //
        // Strategy: skip records where we can't pair to an InitPerAnim. For
        // this trace pattern (UnPackHeaderBits at record idx, paired
        // InitPerAnim at idx+1), we do a sidecar lookup.

        // --- find paired InitPerAnim (the very next record by index) ---
        std::streampos savedPos = in.tellg();
        // The paired InitPerAnim follows immediately; peek next record.
        char m2[4]; uint32_t k2 = 0, i2 = 0, s2 = 0;
        in.read(m2, 4);
        in.read(reinterpret_cast<char*>(&k2), 4);
        in.read(reinterpret_cast<char*>(&i2), 4);
        in.read(reinterpret_cast<char*>(&s2), 4);
        if (std::memcmp(m2, "REC0", 4) != 0 || k2 != 1 || s2 < sizeof(InitPerAnimPayload)) {
            // Not paired -- restore and skip.
            in.clear();
            in.seekg(savedPos);
            continue;
        }
        std::vector<uint8_t> ipPayload(s2);
        in.read(reinterpret_cast<char*>(ipPayload.data()), s2);
        auto* ip = reinterpret_cast<const InitPerAnimPayload*>(ipPayload.data());

        // Now we have headerDump from the paired InitPerAnim. Replay:
        alignas(16) uint8_t localHeader[256];
        std::memcpy(localHeader, ip->headerDump, sizeof(localHeader));
        alignas(16) uint8_t localCmpHdr[256] = {};
        std::memcpy(localCmpHdr, p->mCompressedHeaderDump, sizeof(p->mCompressedHeaderDump));

        uint32_t ourResult = dec->InitPerAnim(localCmpHdr, localHeader, 0);

        ANIM_CODEC::DataPerDecompress& wb = dec->mWorkBuffers[0];

        // Compare result (= mNumBitsOffset + mFrameOffset*4).
        bool resultOK = (ourResult == p->result);
        if (!resultOK) resultMismatchCount++;

        // Compute the ACTUALLY-WRITTEN ranges -- both decoders leave memory
        // past these untouched, so any diff there reflects unrelated heap
        // state from prior calls and is not a real bug.

        const uint8_t* hdr = ip->headerDump;
        uint16_t hdr_mConstChanMapSize = ((uint16_t)hdr[0] << 8) | hdr[1];
        uint8_t  hdr_mNumChannels      = hdr[12];
        uint8_t  hdr_mPaletteSize      = hdr[13];

        // mConstData valid count = sum(size*numConst) across channels.
        uint32_t constFloatsWritten = 0;
        // mIsConstantChannel valid count = sum of run-counts in cmpHdr.
        uint32_t icBytesWritten = 0;
        {
            // Re-derive channelInfoBase the same way ComputeVBRInline does
            // (palette + frame-map area, aligned to 16).
            uint16_t nf = ((uint16_t)hdr[2] << 8) | hdr[3];
            uint32_t mapBytes = 2u * ((nf + 7u) / 8u);
            uintptr_t alignedRaw = (16u + 4u*hdr_mPaletteSize + mapBytes + 0xFu)
                                   & ~uintptr_t(0xF);
            for (uint8_t ch = 0; ch < hdr_mNumChannels; ++ch) {
                const uint8_t* entry = hdr + alignedRaw + (uint32_t)ch * 16u;
                uint16_t numConst = ((uint16_t)entry[2] << 8) | entry[3];
                uint8_t  size     = entry[12];
                constFloatsWritten += (uint32_t)size * numConst;
            }
            // Run-bytes start where palette indices end (which equals
            // constFloatsWritten -- one byte per palette index).
            uint32_t runStart = constFloatsWritten;
            for (uint16_t k = 0; k < hdr_mConstChanMapSize; ++k) {
                if (runStart + k >= sizeof(p->mCompressedHeaderDump)) break;
                icBytesWritten += p->mCompressedHeaderDump[runStart + k];
            }
        }

        // Compare mConstData ONLY in the written range.
        bool constOK = true;
        int firstConstDiff = -1;
        uint32_t cLimit = constFloatsWritten;
        if (cLimit > 64) cLimit = 64;
        for (uint32_t i = 0; i < cLimit; ++i) {
            float exp = beFloat(p->mConstData_post + i * 4);
            float ours = wb.mConstData[i];
            uint32_t expU, oursU;
            std::memcpy(&expU,  &exp,  4);
            std::memcpy(&oursU, &ours, 4);
            if (expU != oursU) {
                constOK = false;
                if (firstConstDiff < 0) firstConstDiff = (int)i;
            }
        }
        if (!constOK) constMismatchCount++;

        // Compare mIsConstantChannel ONLY in the written range.
        const uint8_t* ourIc = reinterpret_cast<const uint8_t*>(wb.mIsConstantChannel);
        uint32_t icLimit = icBytesWritten;
        if (icLimit > 64) icLimit = 64;
        bool icMeaningfulOK = true;
        int firstIcDiff = -1;
        for (uint32_t i = 0; i < icLimit; ++i) {
            if (ourIc[i] != p->mIsConstantChannel_post[i]) {
                icMeaningfulOK = false;
                if (firstIcDiff < 0) firstIcDiff = (int)i;
                break;
            }
        }
        if (!icMeaningfulOK) isConstMismatchCount++;

        bool allOK = resultOK && constOK && icMeaningfulOK;
        if (allOK) { upMatched++; continue; }
        if (upPrinted >= 5) continue;
        upPrinted++;

        std::cout << "\n[#" << idx << "] UnPackHeaderBits DIVERGES";
        if (!resultOK)         std::cout << " [result]";
        if (!constOK)          std::cout << " [mConstData]";
        if (!icMeaningfulOK)   std::cout << " [mIsConstantChannel]";
        std::cout << "\n  result: recomp=" << p->result
                  << " ours=" << ourResult
                  << ((resultOK) ? " ✓" : " ✗") << "\n";
        if (!constOK) {
            std::cout << "  mConstData first diff at index " << firstConstDiff << ":\n";
            std::cout << "    recomp:";
            for (int i = firstConstDiff; i < firstConstDiff + 8 && i < 64; ++i) {
                std::cout << ' ' << beFloat(p->mConstData_post + i * 4);
            }
            std::cout << "\n    ours  :";
            for (int i = firstConstDiff; i < firstConstDiff + 8 && i < 64; ++i) {
                std::cout << ' ' << wb.mConstData[i];
            }
            std::cout << "\n";
        }
        if (!icMeaningfulOK) {
            std::cout << "  mIsConstantChannel diff (limit=" << icLimit
                      << ", first diff=" << firstIcDiff << "):\n";
            std::cout << "    recomp:";
            for (uint32_t i = 0; i < icLimit && i < 32; ++i)
                std::cout << ' ' << (int)p->mIsConstantChannel_post[i];
            std::cout << "\n    ours  :";
            for (uint32_t i = 0; i < icLimit && i < 32; ++i)
                std::cout << ' ' << (int)ourIc[i];
            std::cout << "\n";
            // Dump key inputs so we can see if our channel-info / map-size
            // reading matches the file.
            const uint8_t* hdr = ip->headerDump;
            uint16_t hdr_mConstChanMapSize = ((uint16_t)hdr[0] << 8) | hdr[1];
            uint16_t hdr_mNumFrames        = ((uint16_t)hdr[2] << 8) | hdr[3];
            uint8_t  hdr_mNumChannels      = hdr[12];
            uint8_t  hdr_mPaletteSize      = hdr[13];
            std::cout << "  hdr[0..15]:";
            for (int b = 0; b < 16; ++b)
                std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0') << (int)hdr[b];
            std::cout << std::dec << std::setfill(' ')
                      << "    (mConstChanMapSize=" << hdr_mConstChanMapSize
                      << " mNumFrames=" << hdr_mNumFrames
                      << " mNumChannels=" << (int)hdr_mNumChannels
                      << " mPaletteSize=" << (int)hdr_mPaletteSize << ")\n";
            std::cout << "  cmpHdr[0..31]:";
            for (int b = 0; b < 32; ++b)
                std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0')
                          << (int)p->mCompressedHeaderDump[b];
            std::cout << std::dec << std::setfill(' ') << "\n";
            // Print our-side's interpretation of where palette ends + run bytes start.
            uint32_t paletteIdxBytes = 0;
            for (uint8_t ch = 0; ch < hdr_mNumChannels; ++ch) {
                // ChannelInfo table starts after VBRDataHeaderValue+palette+frame-map
                // (aligned to 16). Re-derive same way as ComputeVBRInline.
                uint8_t  ps = hdr[13];
                uint16_t nf = ((uint16_t)hdr[2] << 8) | hdr[3];
                uint32_t mapBytes = 2u * ((nf + 7u) / 8u);
                uintptr_t alignedRaw = (16u + 4u*ps + mapBytes + 0xFu) & ~uintptr_t(0xF);
                const uint8_t* entry = hdr + alignedRaw + (uint32_t)ch * 16u;
                uint16_t numConst = ((uint16_t)entry[2] << 8) | entry[3];
                uint8_t  size     = entry[12];
                paletteIdxBytes += (uint32_t)size * numConst;
            }
            std::cout << "  expected palette-indices=" << paletteIdxBytes
                      << " run-bytes-start=" << paletteIdxBytes
                      << " run-bytes:";
            for (uint32_t k = 0; k < hdr_mConstChanMapSize && paletteIdxBytes + k < 128; ++k) {
                std::cout << ' ' << (int)p->mCompressedHeaderDump[paletteIdxBytes + k];
            }
            std::cout << "\n";
        }
    }

    std::cout << "\n=== UnPackHeaderBits diff summary ===\n";
    std::cout << "  records             : " << upRecords  << "\n";
    std::cout << "  matched             : " << upMatched  << "\n";
    std::cout << "  result mismatches   : " << resultMismatchCount << "\n";
    std::cout << "  mConstData mismatches: " << constMismatchCount << "\n";
    std::cout << "  mIsConst mismatches : " << isConstMismatchCount << "\n";

    // ------------------------------------------------------------------
    // Pass 3: Vector_UnPack diff.
    // ------------------------------------------------------------------
    std::cout << "\n[Vector_UnPack diff] starting pass 3...\n";
    std::cout.flush();
    in.clear();
    in.seekg(12);
    struct RecordRef { uint32_t kind; std::streampos pos; uint32_t size; };
    std::vector<RecordRef> recIndex;
    while (in.peek() != EOF) {
        std::streampos recStart = in.tellg();
        char m[4];
        in.read(m, 4);
        if (in.gcount() != 4) break;
        uint32_t k = 0, i = 0, s = 0;
        in.read(reinterpret_cast<char*>(&k), 4);
        in.read(reinterpret_cast<char*>(&i), 4);
        in.read(reinterpret_cast<char*>(&s), 4);
        recIndex.push_back({k, recStart, s});
        in.seekg(s, std::ios::cur);
    }

    auto loadRecord = [&](size_t idx, std::vector<uint8_t>& out) -> bool {
        if (idx >= recIndex.size()) return false;
        in.clear();
        in.seekg(recIndex[idx].pos);
        // Skip the 16-byte record header (REC0/kind/idx/size).
        in.seekg(16, std::ios::cur);
        out.resize(recIndex[idx].size);
        in.read(reinterpret_cast<char*>(out.data()), recIndex[idx].size);
        return true;
    };

    int vupRecords = 0, vupMatched = 0, vupPrinted = 0;
    int vupScratchMismatches = 0, vupNormMismatches = 0;

    int vupAnimated = 0;  // count records where mFrameOffset > 0 (bit decoder fired)

    // Helper: read the leading u32 (`this_ptr`) of a record's payload, since
    // both InitPerAnim and Vector_UnPack put it as their first field.
    auto readThisPtr = [&](size_t idx) -> uint32_t {
        in.clear();
        in.seekg(recIndex[idx].pos);
        in.seekg(16, std::ios::cur);   // skip REC0/kind/idx/size header
        uint32_t v = 0;
        in.read(reinterpret_cast<char*>(&v), 4);
        return v;
    };

    for (size_t recIdx = 0; recIdx < recIndex.size(); ++recIdx) {
        if (recIndex[recIdx].kind != 3) continue;
        // Find paired InitPerAnim: most recent kind=1 record before this one
        // **whose this_ptr matches**. Multiple decompressor instances may be
        // interleaved, so picking purely by recency would mis-pair clips.
        uint32_t targetThis = readThisPtr(recIdx);
        size_t pairedIp = (size_t)-1;
        for (size_t j = recIdx; j-- > 0;) {
            if (recIndex[j].kind == 1 && readThisPtr(j) == targetThis) {
                pairedIp = j;
                break;
            }
        }
        if (pairedIp == (size_t)-1) continue;

        std::vector<uint8_t> vupBuf, ipBuf;
        if (!loadRecord(recIdx,   vupBuf)) continue;
        if (!loadRecord(pairedIp, ipBuf))  continue;
        if (vupBuf.size() < sizeof(VectorUnPackPayload)) continue;
        if (ipBuf.size()  < sizeof(InitPerAnimPayload))  continue;

        auto* p  = reinterpret_cast<const VectorUnPackPayload*>(vupBuf.data());
        auto* ip = reinterpret_cast<const InitPerAnimPayload*>(ipBuf.data());
        vupRecords++;

        // Re-init our decoder against this clip. Use static large buffers
        // (zero-padded) so the control-nibble stream read past the captured
        // 256-byte cmpHdrDump returns 0 instead of crashing.
        alignas(16) static uint8_t localHeader[1024];
        alignas(16) static uint8_t localCmpHdr[8192];
        std::memset(localHeader, 0, sizeof(localHeader));
        std::memset(localCmpHdr, 0, sizeof(localCmpHdr));
        std::memcpy(localHeader, ip->headerDump, sizeof(ip->headerDump));
        std::memcpy(localCmpHdr, ip->cmpHdrDump, sizeof(ip->cmpHdrDump));
        dec->InitPerAnim(localCmpHdr, localHeader, 0);

        // Replay Vector_UnPack with the recorded compressedData bytes.
        // Use a large zero-padded buffer so reads past the captured 256
        // bytes don't crash; values won't match the recomp past that point
        // but no segfault and the early bytes still diff meaningfully.
        alignas(16) static uint8_t localCmpData[8192];
        std::memset(localCmpData, 0, sizeof(localCmpData));
        std::memcpy(localCmpData, p->cmpDataDump, sizeof(p->cmpDataDump));

        ANIM_CODEC::DataPerDecompress& wb = dec->mWorkBuffers[0];
        // Vector_UnPack -> DecompressFrameBlock reads workBuffer.mCompressedData
        // for its channel-index prefix loop. The recomp's DecompressFrame sets
        // that field before calling Vector_UnPack; our replay bypasses
        // DecompressFrame, so we provide a small zero-filled stand-in. The
        // first N bytes (= numChannels channel indices) read as 0 -- not the
        // game's real channel-index mapping, but the bit-decoder output we
        // care about doesn't depend on it.
        static uint8_t s_dummyCmpData[64] = {0};
        wb.mCompressedData = s_dummyCmpData;

        // Seed scratch + normalized buffers from the captured pre-state so
        // we faithfully reproduce the recomp's stateful behavior across calls.
        // The recomp doesn't zero these between Vector_UnPack invocations;
        // when mFrameOffset==0 the bit decoder is skipped entirely and any
        // stale values from prior animated calls remain. Without this seed,
        // mFrameOffset==0 records that read non-zero scratch in the recomp
        // would diverge against our fresh-zero state.
        if (wb.mUncompressedBlkDataScratch) {
            for (int i = 0; i < 256; ++i) {
                wb.mUncompressedBlkDataScratch[i] =
                    (int32_t)beU32(p->uncompressedScratch_pre + i * 4);
            }
        }
        if (wb.mNormalizedData) {
            for (int i = 0; i < 256; ++i) {
                wb.mNormalizedData[i] = beFloat(p->normalizedData_pre + i * 4);
            }
        }

        // Vector_UnPack expects pCompressedData *already advanced* past the
        // numChannels-byte channel-index prefix, so caller normally feeds
        // `compressedData + numChannels`. Our trace captured that adjusted
        // pointer; we mirror by simply passing localCmpData directly.
        dec->Vector_UnPackFrameBlockAndDecompressOneFrame(
            localCmpData, &wb,
            (uint8_t)(p->frameinBlk & 0xFF),
            0 /* threadId */);
        if (wb.mFrameOffset > 0) vupAnimated++;

        // Compare uncompressedScratch_post: 256 ints (1024 bytes), recomp
        // bytes are BE u32s, ours are host-LE u32s.
        bool scratchOK = true;
        int firstScratchDiff = -1;
        if (wb.mUncompressedBlkDataScratch) {
            for (int i = 0; i < 256; ++i) {
                int32_t exp = (int32_t)beU32(p->uncompressedScratch_post + i * 4);
                int32_t ours = wb.mUncompressedBlkDataScratch[i];
                if (exp != ours) {
                    scratchOK = false;
                    if (firstScratchDiff < 0) firstScratchDiff = i;
                }
            }
        }
        if (!scratchOK) vupScratchMismatches++;

        // Compare normalizedData_post: 256 floats, recomp BE bit-patterns.
        bool normOK = true;
        int firstNormDiff = -1;
        if (wb.mNormalizedData) {
            for (int i = 0; i < 256; ++i) {
                float exp = beFloat(p->normalizedData_post + i * 4);
                float ours = wb.mNormalizedData[i];
                uint32_t expU, oursU;
                std::memcpy(&expU, &exp, 4);
                std::memcpy(&oursU, &ours, 4);
                if (expU != oursU) {
                    normOK = false;
                    if (firstNormDiff < 0) firstNormDiff = i;
                }
            }
        }
        if (!normOK) vupNormMismatches++;

        if (scratchOK && normOK) { vupMatched++; continue; }
        if (vupPrinted >= 3) continue;
        vupPrinted++;

        std::cout << "\n[#" << recIndex[recIdx].pos << "/rec" << recIdx
                  << "] Vector_UnPack DIVERGES";
        if (!scratchOK) std::cout << " [uncompressedScratch]";
        if (!normOK)    std::cout << " [normalizedData]";
        std::cout << "\n  inputs: pCmp=0x" << std::hex << p->pCompressedData
                  << " pWB=0x" << p->pWorkBuffer
                  << " frameinBlk=" << std::dec << p->frameinBlk
                  << " mFrameOffset(ours)=" << wb.mFrameOffset << "\n";
        // Dump the recomp's mhdrWrapper bytes (= 24 bytes at wrapper struct).
        std::cout << "  mhdrWrapper[24] (recomp): " << std::hex << std::setfill('0');
        for (int i = 0; i < 24; ++i) {
            std::cout << ' ' << std::setw(2) << (unsigned)p->mhdrWrapper[i];
        }
        std::cout << std::setfill(' ') << std::dec << "\n";
        // Print scratch_post side-by-side for recomp vs ours, slot 0..15.
        // If diff says scratch matches but recomp's dot product behaves
        // differently from ours, we need to verify the captured BYTES
        // really are what we think they are.
        std::cout << "  scratch[0..15] recomp(as int): ";
        for (int i = 0; i < 16; ++i) {
            int32_t exp = (int32_t)beU32(p->uncompressedScratch_post + i * 4);
            std::cout << ' ' << exp;
        }
        std::cout << "\n  scratch[0..15] recomp(as flt): ";
        for (int i = 0; i < 16; ++i) {
            float f = beFloat(p->uncompressedScratch_post + i * 4);
            std::cout << ' ' << f;
        }
        std::cout << "\n  scratch[0..15] ours  (as int): ";
        if (wb.mUncompressedBlkDataScratch) {
            for (int i = 0; i < 16; ++i) {
                std::cout << ' ' << wb.mUncompressedBlkDataScratch[i];
            }
        }
        std::cout << "\n  scratch[0..15] ours  (as flt): ";
        if (wb.mUncompressedBlkDataScratch) {
            for (int i = 0; i < 16; ++i) {
                float f;
                std::memcpy(&f, &wb.mUncompressedBlkDataScratch[i], 4);
                std::cout << ' ' << f;
            }
        }
        std::cout << "\n";
        // Dump the first 64 bytes of the captured compressed data so we can
        // see exactly what bit-stream input the bit decoder is being fed.
        // Byte 0 is the leading-skip count; bytes 1..N are the bit stream.
        std::cout << "  cmpData[0..63]:" << std::hex << std::setfill('0');
        for (int i = 0; i < 64 && i < (int)sizeof(p->cmpDataDump); ++i) {
            if (i == 32) std::cout << "\n                ";
            std::cout << ' ' << std::setw(2) << (unsigned)p->cmpDataDump[i];
        }
        std::cout << std::setfill(' ') << std::dec << "\n";
        // Also print the start of mCompressedHeaderData (control nibble
        // stream). The work buffer's mNumBitsOffset tells where ctrl bytes
        // begin within mCompressedHeaderData.
        std::cout << "  cmpHdr[0..63]:" << std::hex << std::setfill('0');
        for (int i = 0; i < (int)sizeof(ip->cmpHdrDump); ++i) {
            if (i == 32) std::cout << "\n               ";
            std::cout << ' ' << std::setw(2) << (unsigned)ip->cmpHdrDump[i];
        }
        std::cout << std::setfill(' ') << std::dec
                  << "\n  mNumBitsOffset=" << wb.mNumBitsOffset
                  << " (ctrl bytes start at cmpHdr[" << wb.mNumBitsOffset << "])\n";
        // Print the first few ctrl words and what we'd interpret as nibbles.
        if (wb.mNumBitsOffset < 60u && wb.mCompressedHeaderData) {
            const uint8_t* ctrlBase = wb.mCompressedHeaderData + wb.mNumBitsOffset;
            std::cout << "  first ctrl words (BE):";
            for (int e = 0; e < std::min<int>(4, (int)wb.mFrameOffset); ++e) {
                uint32_t cw = ((uint32_t)ctrlBase[e*4+0] << 24)
                            | ((uint32_t)ctrlBase[e*4+1] << 16)
                            | ((uint32_t)ctrlBase[e*4+2] << 8)
                            |  (uint32_t)ctrlBase[e*4+3];
                std::cout << " 0x" << std::hex << std::setw(8) << std::setfill('0')
                          << cw << std::dec << std::setfill(' ');
            }
            std::cout << "\n";
        }
        if (!scratchOK) {
            std::cout << "  uncompressedScratch first diff at int " << firstScratchDiff << ":\n";
            std::cout << "    recomp:";
            for (int i = firstScratchDiff; i < firstScratchDiff + 8 && i < 256; ++i) {
                int32_t v = (int32_t)beU32(p->uncompressedScratch_post + i * 4);
                std::cout << ' ' << v;
            }
            std::cout << "\n    ours  :";
            for (int i = firstScratchDiff; i < firstScratchDiff + 8 && i < 256; ++i) {
                std::cout << ' ' << wb.mUncompressedBlkDataScratch[i];
            }
            std::cout << "\n";
        }
        if (!normOK) {
            std::cout << "  normalizedData first diff at float " << firstNormDiff << ":\n";
            std::cout << "    recomp:";
            for (int i = firstNormDiff; i < firstNormDiff + 8 && i < 256; ++i) {
                std::cout << ' ' << beFloat(p->normalizedData_post + i * 4);
            }
            std::cout << "\n    ours  :";
            for (int i = firstNormDiff; i < firstNormDiff + 8 && i < 256; ++i) {
                std::cout << ' ' << wb.mNormalizedData[i];
            }
            std::cout << "\n";
        }
    }

    std::cout << "\n=== Vector_UnPack diff summary ===\n";
    std::cout << "  records                       : " << vupRecords << "\n";
    std::cout << "  matched                       : " << vupMatched << "\n";
    std::cout << "  with bit decoder firing       : " << vupAnimated
              << " (mFrameOffset > 0)\n";
    std::cout << "  scratch (bit-decode) mismatch : " << vupScratchMismatches << "\n";
    std::cout << "  normalized (IDCT)    mismatch : " << vupNormMismatches    << "\n";
    if (vupAnimated == 0) {
        std::cout << "  NOTE: trace contains no animated clips (all mFrameOffset=0).\n"
                  << "        The bit decoder is unverified -- capture a trace from\n"
                  << "        gameplay where characters animate to exercise it.\n";
    }

    delete dec;
    return 0;
}

static int run(const char* tracePath) {
    std::ifstream in(tracePath, std::ios::binary);
    if (!in) {
        std::cout << "Cannot open " << tracePath << "\n";
        return 1;
    }
    char magic[8];
    in.read(magic, 8);
    if (std::memcmp(magic, "VBRTRACE", 8) != 0) {
        std::cout << "Not a VBRTRACE file\n";
        return 1;
    }
    uint32_t version = 0;
    in.read(reinterpret_cast<char*>(&version), 4);
    std::cout << "Trace version " << version << ", path=" << tracePath << "\n\n";

    // Tally per kind.
    int counts[6] = {0};
    int total = 0;

    while (in.peek() != EOF) {
        char marker[4];
        in.read(marker, 4);
        if (in.gcount() != 4) break;
        if (std::memcmp(marker, "REC0", 4) != 0) {
            std::cout << "Bad marker, stopping\n";
            break;
        }
        uint32_t kind = 0, idx = 0, sz = 0;
        in.read(reinterpret_cast<char*>(&kind), 4);
        in.read(reinterpret_cast<char*>(&idx),  4);
        in.read(reinterpret_cast<char*>(&sz),   4);
        std::vector<uint8_t> payload(sz);
        in.read(reinterpret_cast<char*>(payload.data()), sz);
        if (kind >= 1 && kind <= 5) counts[kind]++;
        total++;

        if (total <= 10 || total % 10 == 0) {
            std::cout << "[#" << idx << "] " << kindName(kind)
                      << " (kind=" << kind << ", " << sz << "B)";
            if (kind == 1 && sz >= sizeof(InitPerAnimPayload)) {
                auto* p = reinterpret_cast<const InitPerAnimPayload*>(payload.data());
                std::cout << "  this=0x" << std::hex << p->this_ptr
                          << " hdr=0x" << p->headerData
                          << " cmpHdr=0x" << p->compressedHeaderData
                          << " tid=" << std::dec << p->threadId
                          << " result=" << p->result;
            } else if (kind == 5 && sz >= sizeof(DecompressFramePayload)) {
                auto* p = reinterpret_cast<const DecompressFramePayload*>(payload.data());
                std::cout << "  this=0x" << std::hex << p->this_ptr
                          << " cmpData=0x" << p->compressedData
                          << " frame=" << std::dec << p->frame
                          << " putLoc=0x" << std::hex << p->putLoc
                          << " tid=" << std::dec << p->threadId;
            } else if (kind == 3 && sz >= sizeof(VectorUnPackPayload)) {
                auto* p = reinterpret_cast<const VectorUnPackPayload*>(payload.data());
                std::cout << "  pCmp=0x" << std::hex << p->pCompressedData
                          << " pWB=0x" << p->pWorkBuffer
                          << " frameinBlk=" << std::dec << p->frameinBlk
                          << " tid=" << p->threadId;
            }
            std::cout << "\n";
        }
    }

    std::cout << "\n=== summary ===\n";
    std::cout << "  total records       : " << total << "\n";
    for (int k = 1; k <= 5; ++k) {
        std::cout << "  " << kindName(k) << " (" << k << "): " << counts[k] << "\n";
    }
    std::cout << "\n";

    // Print one fully detailed example of each kind.
    in.clear();
    in.seekg(12);  // skip header
    bool shown[6] = {false};
    while (in.peek() != EOF) {
        char marker[4];
        in.read(marker, 4);
        if (in.gcount() != 4) break;
        uint32_t kind = 0, idx = 0, sz = 0;
        in.read(reinterpret_cast<char*>(&kind), 4);
        in.read(reinterpret_cast<char*>(&idx),  4);
        in.read(reinterpret_cast<char*>(&sz),   4);
        std::vector<uint8_t> payload(sz);
        in.read(reinterpret_cast<char*>(payload.data()), sz);
        if (kind < 1 || kind > 5 || shown[kind]) continue;
        shown[kind] = true;

        std::cout << "\n--- first " << kindName(kind) << " (record #" << idx << ") ---\n";
        if (kind == 1) {
            auto* p = reinterpret_cast<const InitPerAnimPayload*>(payload.data());
            std::cout << "  args: this=0x" << std::hex << p->this_ptr
                      << " cmpHdr=0x" << p->compressedHeaderData
                      << " hdr=0x" << p->headerData
                      << " tid=" << std::dec << p->threadId
                      << " -> result=0x" << std::hex << p->result << std::dec << "\n";
            dumpBytes("workBuffer_pre",   p->workBuffer_pre,   sizeof(p->workBuffer_pre));
            dumpBytes("workBuffer_post",  p->workBuffer_post,  sizeof(p->workBuffer_post));
            dumpBytes("mhdrWrapper_pre",  p->mhdrWrapper_pre,  sizeof(p->mhdrWrapper_pre), 2);
            dumpBytes("mhdrWrapper_post", p->mhdrWrapper_post, sizeof(p->mhdrWrapper_post), 2);
            dumpBytes("headerDump",       p->headerDump,       sizeof(p->headerDump), 8);
            dumpBytes("cmpHdrDump",       p->cmpHdrDump,       sizeof(p->cmpHdrDump));
        } else if (kind == 2) {
            auto* p = reinterpret_cast<const UnPackHeaderBitsPayload*>(payload.data());
            std::cout << "  args: this=0x" << std::hex << p->this_ptr
                      << " pWB=0x" << p->pWorkBuffer
                      << " -> result=0x" << p->result << std::dec
                      << "  ptrs: mConstData=0x" << std::hex << p->mConstDataPtr
                      << " mIsConst=0x" << p->mIsConstantPtr
                      << " cmpHdr=0x" << p->mCompressedHeaderDataPtr << std::dec << "\n";
            dumpBytes("workBuffer_pre",  p->workBuffer_pre,  sizeof(p->workBuffer_pre));
            dumpBytes("workBuffer_post", p->workBuffer_post, sizeof(p->workBuffer_post));
            dumpBytes("mhdrWrapper",     p->mhdrWrapper,     sizeof(p->mhdrWrapper), 2);
            dumpBytes("cmpHdrDump",      p->mCompressedHeaderDump, sizeof(p->mCompressedHeaderDump), 4);
            dumpBytes("mConstData_post", p->mConstData_post, sizeof(p->mConstData_post), 4);
            dumpBytes("mIsConst_post",   p->mIsConstantChannel_post, sizeof(p->mIsConstantChannel_post), 2);
        } else if (kind == 3) {
            auto* p = reinterpret_cast<const VectorUnPackPayload*>(payload.data());
            std::cout << "  args: this=0x" << std::hex << p->this_ptr
                      << " pCmp=0x" << p->pCompressedData
                      << " pWB=0x" << p->pWorkBuffer
                      << " frameinBlk=" << std::dec << p->frameinBlk
                      << " tid=" << p->threadId << "\n";
            dumpBytes("workBuffer_pre",      p->workBuffer_pre,      sizeof(p->workBuffer_pre));
            dumpBytes("mhdrWrapper",         p->mhdrWrapper,         sizeof(p->mhdrWrapper), 2);
            dumpBytes("cmpDataDump",         p->cmpDataDump,         sizeof(p->cmpDataDump), 4);
            dumpBytes("uncompressed_post",   p->uncompressedScratch_post, sizeof(p->uncompressedScratch_post), 4);
            dumpBytes("normalized_post",     p->normalizedData_post, sizeof(p->normalizedData_post), 4);
        } else if (kind == 4) {
            auto* p = reinterpret_cast<const DecompressFrameBlockPayload*>(payload.data());
            std::cout << "  args: this=0x" << std::hex << p->this_ptr
                      << " frameinBlk=" << std::dec << p->frameinBlk
                      << " pWB=0x" << std::hex << p->pWorkBuffer
                      << " tid=" << std::dec << p->threadId << "\n";
            dumpBytes("workBuffer_pre",       p->workBuffer_pre, sizeof(p->workBuffer_pre));
            dumpBytes("uncompressed_pre",     p->uncompressedScratch_pre, sizeof(p->uncompressedScratch_pre), 4);
            dumpBytes("normalized_post",      p->normalizedData_post, sizeof(p->normalizedData_post), 4);
        } else if (kind == 5) {
            auto* p = reinterpret_cast<const DecompressFramePayload*>(payload.data());
            std::cout << "  args: this=0x" << std::hex << p->this_ptr
                      << " cmpData=0x" << p->compressedData
                      << " frame=" << std::dec << p->frame
                      << " putLoc=0x" << std::hex << p->putLoc
                      << " tid=" << std::dec << p->threadId << "\n";
            dumpBytes("workBuffer_pre",  p->workBuffer_pre,  sizeof(p->workBuffer_pre));
            dumpBytes("workBuffer_post", p->workBuffer_post, sizeof(p->workBuffer_post));
            dumpBytes("cmpDataDump",     p->cmpDataDump,     sizeof(p->cmpDataDump), 4);
            dumpBytes("putLoc_pre",      p->putLoc_pre,      sizeof(p->putLoc_pre), 4);
            dumpBytes("putLoc_post",     p->putLoc_post,     sizeof(p->putLoc_post), 4);
        }
    }

    return 0;
}

} // namespace vbr_compare

//testing
int main(int argc, char** argv) {
    // CLI: --trace <path> reads the rexglue VBR trace and prints a summary.
    if (argc >= 2 && std::string(argv[1]) == "--trace") {
        const char* path = (argc >= 3)
            ? argv[2]
            : "C:/Users/tuukk/Desktop/vbr_trace.bin";
        return vbr_compare::run(path);
    }
    // CLI: --trace-csv [tracePath] [csvPath]
    // Converts every DecompressFrame record's putLoc_post to a per-bone CSV
    // matching AnimTest's CSV format, so you can diff ground truth against
    // AnimTest's own output.
    if (argc >= 2 && std::string(argv[1]) == "--trace-csv") {
        const char* tracePath = (argc >= 3) ? argv[2] : "C:/Users/tuukk/Desktop/vbr_trace.bin";
        const char* csvPath   = (argc >= 4) ? argv[3] : "C:/Users/tuukk/Desktop/vbr_trace_decompress.csv";
        return vbr_compare::exportFramesCsv(tracePath, csvPath);
    }
    // CLI: --trace-diff [tracePath]
    // Replays each InitPerAnim record through our local decoder and reports
    // any field-level divergence vs the recomp's recorded outputs.
    if (argc >= 2 && std::string(argv[1]) == "--trace-diff") {
        const char* path = (argc >= 3) ? argv[2] : "C:/Users/tuukk/Desktop/vbr_trace.bin";
        return vbr_compare::diff(path);
    }
    static const char* kDefaultDBs[] = {
        "G:/Skate 3 (USA, Asia) (En,Fr,Es)/PS3_GAME/USRDIR/data/big/data abin/ALL ABINS/SkaterLeftHandPres.abin",
        "G:/Skate 3 (USA, Asia) (En,Fr,Es)/PS3_GAME/USRDIR/data/big/data abin/ALL ABINS/animationDB.abin",
        "G:/Skate 3 (USA, Asia) (En,Fr,Es)/PS3_GAME/USRDIR/data/big/data abin/ALL ABINS/cac_female_throwarms.abin",
        "G:/Skate 3 (USA, Asia) (En,Fr,Es)/PS3_GAME/USRDIR/data/big/data abin/ALL ABINS/cac_viewer_male.abin",
        "G:/Skate 2 (USA) (En,Fr)/PS3_GAME/USRDIR/data/big/data/anim/OnBoard.abin",
    };

    // 1. Pick database: command-line arg overrides; otherwise show menu of defaults.
    std::string dbFilename;
    if (argc >= 2) {
        dbFilename = argv[1];
    } else {
        std::cout << "Select a database:\n";
        for (size_t i = 0; i < sizeof(kDefaultDBs) / sizeof(kDefaultDBs[0]); ++i) {
            std::cout << "  [" << i << "] " << kDefaultDBs[i] << "\n";
        }
        std::cout << "  [c] custom path\n> ";
        std::string line;
        if (!std::getline(std::cin, line)) return 0;
        if (line == "c" || line == "C") {
            std::cout << "Path: ";
            if (!std::getline(std::cin, dbFilename)) return 0;
        } else {
            size_t idx = 0;
            try { idx = std::stoul(line); } catch (...) { std::cout << "Invalid selection.\n"; return 1; }
            if (idx >= sizeof(kDefaultDBs) / sizeof(kDefaultDBs[0])) { std::cout << "Out of range.\n"; return 1; }
            dbFilename = kDefaultDBs[idx];
        }
    }

    // 2. Load database.
    std::cout << "\nLoading " << dbFilename << "...\n";
    Andale::FetchSys fs = loadAnimationDataBase(dbFilename.c_str());
    if (fs.mSkel == 0) {
        std::cout << "Failed to load database (file not found / unreadable).\n"
                  << "Check the path -- the bundled defaults assume a Skate 3/2 install layout.\n";
        return 1;
    }
    auto* dbManager = reinterpret_cast<Andale::DataBaseManager*>(fs.mDataBaseManager);
    const int mDBIndex = 0;
    auto* db = reinterpret_cast<Andale::DataBase*>(dbManager->mDataBases[mDBIndex]);
    if (!db) {
        std::cout << "Database loaded but mDataBases[0] is null.\n";
        return 1;
    }

    // 3. Build a flat list of (kind, intra-index, name, codec).
    // mCodecCompactName is stored already in the form GetCodecType reads --
    // i.e. the high byte is the first char of the tag. Do NOT byte-swap here:
    // the user's GetCodecType uses the raw field, and matching that is what
    // determines whether the FetchSys codec lines up at fetch time.
    auto codecTagToString = [](uint32_t compactName) -> std::string {
        char s[5] = {};
        s[0] = (char)((compactName >> 24) & 0xFF);
        s[1] = (char)((compactName >> 16) & 0xFF);
        s[2] = (char)((compactName >> 8) & 0xFF);
        s[3] = (char)((compactName >> 0) & 0xFF);
        for (int i = 0; i < 4; ++i) if (s[i] && (s[i] < 0x20 || s[i] >= 0x7F)) s[i] = ' ';
        std::string out = s;
        // Trim trailing spaces.
        while (!out.empty() && out.back() == ' ') out.pop_back();
        return out;
    };

    struct Entry {
        bool        isPose;   // true = pose lookup, false = clip lookup
        uint32_t    index;    // index within mPoses or mClips
        std::string name;
        std::string codec;    // "RD" / "VBR" / etc., from header's mCodecCompactName
        uint32_t    rawCodec; // raw field value (for hex display when codec is empty)
    };
    std::vector<Entry> entries;

    {
        uint32_t i = 0;
        for (auto it = db->mPoses.begin(); it != db->mPoses.end(); ++it, ++i) {
            const Andale::DataHeader* h = it->second;
            uint32_t raw = h ? h->mCodecCompactName : 0;
            entries.push_back({ true, i, it->first.to_string(), codecTagToString(raw), raw });
        }
    }
    {
        uint32_t i = 0;
        for (auto it = db->mClips.begin(); it != db->mClips.end(); ++it, ++i) {
            const Andale::DataHeader* h = it->second;
            uint32_t raw = h ? h->mCodecCompactName : 0;
            entries.push_back({ false, i, it->first.to_string(), codecTagToString(raw), raw });
        }
    }

    if (entries.empty()) {
        std::cout << "Database has no poses or clips.\n";
        return 1;
    }

    // Codec summary -- helps spot whether a database actually contains the
    // codec you want to test before scrolling through hundreds of entries.
    {
        std::map<uint32_t, int> codecCounts;
        for (auto& e : entries) codecCounts[e.rawCodec]++;
        std::cout << "\nCodec summary in this DB:\n";
        for (auto& kv : codecCounts) {
            uint32_t raw = kv.first;
            char tag[5] = {};
            tag[0] = (char)((raw >> 24) & 0xFF);
            tag[1] = (char)((raw >> 16) & 0xFF);
            tag[2] = (char)((raw >>  8) & 0xFF);
            tag[3] = (char)((raw >>  0) & 0xFF);
            for (int i = 0; i < 4; ++i) if (tag[i] && (tag[i] < 0x20 || tag[i] >= 0x7F)) tag[i] = '.';
            int t = fs.GetCodecType(raw);
            const char* name =
                (t == 0) ? "RAW (RD)" :
                (t == 1) ? "VBR (CONV_FORMAT)" :
                (t == 2) ? "Unknown / unimplemented" : "?";
            std::cout << "  raw=0x" << std::hex << std::setw(8) << std::setfill('0') << raw
                      << std::dec << std::setfill(' ')
                      << "  tag=\"" << tag << "\"  -> " << name
                      << "  (" << kv.second << " entries)\n";
        }
    }

    // 4. Selection loop: list, prompt, load, repeat.
    // Use FetchSys::GetCodecType for routing -- it's the source of truth that
    // BatchFetch checks against, so matching it guarantees no codec-mismatch
    // warning at load time.
    auto codecForEntry = [&fs](uint32_t rawCodec) -> Andale::codec_type {
        switch (fs.GetCodecType(rawCodec)) {
            case 0: return Andale::codec_type::RAW;
            case 1: return Andale::codec_type::CONV_FORMAT;
            default: return Andale::codec_type::RAW;  // safe fallback
        }
    };

    while (true) {
        std::cout << "\nEntries in database  (P=pose, C=clip,  codec)\n";
        for (size_t i = 0; i < entries.size(); ++i) {
            std::cout << "  [" << i << "] "
                      << (entries[i].isPose ? "(P) " : "(C) ");
            if (entries[i].codec.empty()) {
                // Show raw hex when the tag has no printable chars
                std::cout << "[0x" << std::hex << entries[i].rawCodec << std::dec << "] ";
            } else {
                std::cout << "[" << entries[i].codec << "] ";
            }
            std::cout << entries[i].name << "\n";
        }
        std::cout << "\nSelect [0-" << (entries.size() - 1) << "] (q to quit): ";

        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line == "q" || line == "Q" || line.empty()) break;

        size_t sel = 0;
        try { sel = std::stoul(line); } catch (...) { std::cout << "Invalid input.\n"; continue; }
        if (sel >= entries.size()) { std::cout << "Out of range.\n"; continue; }

        const Entry& e = entries[sel];
        const uint32_t guid = (e.index << 5) | (mDBIndex & 0x1F);
        const Andale::codec_type wantCodec = codecForEntry(e.rawCodec);

        // Rebuild fs with the codec this entry expects, if it differs. We keep
        // the same dbManager and skeleton; the previous fs.mDecompressor (if
        // any) leaks, which is acceptable for a short-lived test tool.
        if (static_cast<Andale::codec_type>(fs.mCodecType) != wantCodec) {
            auto* dbManager_ = reinterpret_cast<Andale::DataBaseManager*>(fs.mDataBaseManager);
            auto* skel_     = reinterpret_cast<const Andale::HierarchyData*>(fs.mSkel);
            new (&fs) Andale::FetchSys(dbManager_, skel_, 0, true, wantCodec);
            const char* codecName =
                (wantCodec == Andale::codec_type::RAW)         ? "RAW (RD)" :
                (wantCodec == Andale::codec_type::CONV_FORMAT) ? "VBR" : "?";
            std::cout << "(switched FetchSys codec to " << codecName << ")\n";
        }

        std::cout << "\n--- Loading " << (e.isPose ? "pose" : "clip")
                  << " \"" << e.name << "\" [" << e.codec << "]"
                  << " (GUID 0x" << std::hex << guid << std::dec << ") ---\n";

        // Get bone count so we can size the output buffer properly.
        auto* skel_ = reinterpret_cast<const Andale::HierarchyData*>(fs.mSkel);
        const uint32_t nbBones = skel_ ? Convert(skel_->mNbBones) : 0;

        // The pipeline (BatchFetch -> ExtractPackedN) writes Andale::SQTData
        // entries, NOT AnimCmdSys::SQTExt. SQTData has a Vector3 mScale
        // (potentially 12 bytes if not SIMD-padded), so the stride differs
        // from SQTExt (which uses Vector4 mScale = 16 bytes). Allocate as
        // SQTData[] so the buffer matches what Fetch actually writes, and
        // alias it to SQTExt* for the existing API call.
        auto* output = new xvectoralloc::XVectorPtr<AnimCmdSys::SQTExt>;
        Andale::SQTData* rawSQT = (nbBones > 0)
            ? new Andale::SQTData[nbBones]
            : new Andale::SQTData[0];
        // Force zero-init: Vector3/Vector4/Quaternion are trivial POD-like
        // types with no zeroing default ctor, so `new SQTData[N]` leaves
        // the buffer full of heap garbage. memset gives us a known baseline
        // so any value the pipeline doesn't explicitly write reads as 0.
        if (nbBones > 0) std::memset(rawSQT, 0, nbBones * sizeof(Andale::SQTData));
        output->mVector = reinterpret_cast<AnimCmdSys::SQTExt*>(rawSQT);

        // Per-frame snapshot in SQTData layout (matches what Fetch writes).
        std::vector<Andale::SQTData> allFrames;  // [frameIdx * nbBones + bone]
        uint32_t framesCaptured = 0;

        // For VBR codec, use the skeleton-free extractor so we get every part
        // the file encodes, not just whatever happened to match this rig.
        const bool useSkeletonFreeVBR =
            (fs.mCodecType == (uint32_t)Andale::codec_type::CONV_FORMAT);

        if (e.isPose) {
            if (useSkeletonFreeVBR) {
                const Andale::DataHeader* hdr =
                    reinterpret_cast<Andale::DataBaseManager*>(fs.mDataBaseManager)->GetPoseHeader(guid);
                if (hdr) {
                    auto* decompressor = reinterpret_cast<ANIM_CODEC::VBRDecompressor*>(fs.mDecompressor);
                    uint32_t written = Andale::ExtractAllVBRParts(hdr, 0, decompressor, rawSQT, nbBones);
                    std::cout << "  ExtractAllVBRParts wrote " << written << " bones\n";
                }
            } else {
                fs.loadPose(guid, output);
            }
            framesCaptured = 1;
            allFrames.assign(rawSQT, rawSQT + nbBones);
        } else {
            // Pull nbFrames from the clip header. Mirrors loadAnimation()'s setup.
            const Andale::DataHeader* hdr =
                reinterpret_cast<Andale::DataBaseManager*>(fs.mDataBaseManager)->GetClipHeader(guid);
            uint32_t nbFrames = 0;
            if (hdr) {
                const uint32_t* alignedData = reinterpret_cast<const uint32_t*>(
                    (reinterpret_cast<uintptr_t>(&hdr[1].mGUID) + 3) & ~0xF);
                const Andale::AnimationData* animData =
                    reinterpret_cast<const Andale::AnimationData*>(alignedData);
                nbFrames = (uint32_t)Convert(animData->mNbFrames);
            }
            std::cout << "  nbFrames=" << nbFrames << " nbBones=" << nbBones << "\n";

            allFrames.reserve((size_t)nbFrames * nbBones);
            for (uint32_t f = 0; f < nbFrames && nbBones > 0; ++f) {
                // Zero between frames so bones not written by this frame's
                // fetch don't carry stale values from the previous frame.
                std::memset(rawSQT, 0, nbBones * sizeof(Andale::SQTData));
                if (useSkeletonFreeVBR && hdr) {
                    auto* decompressor = reinterpret_cast<ANIM_CODEC::VBRDecompressor*>(fs.mDecompressor);
                    Andale::ExtractAllVBRParts(hdr, f, decompressor, rawSQT, nbBones);
                } else {
                    fs.GetSQTs(guid, f, output);
                }
                allFrames.insert(allFrames.end(), rawSQT, rawSQT + nbBones);
                framesCaptured++;
            }
        }

        std::cout << "--- Done (" << framesCaptured << " frame"
                  << (framesCaptured == 1 ? "" : "s") << " captured) ---\n";

        // Offer CSV export. mScale is Vector3 (3 floats), mQuat is Quaternion
        // (4 floats: x,y,z,w), mTrans is Vector4 (4 floats: x,y,z,w).
        if (framesCaptured > 0 && nbBones > 0) {
            std::cout << "Export to CSV? (filename, or empty to skip): ";
            std::string outpath;
            if (std::getline(std::cin, outpath) && !outpath.empty()) {
                std::ofstream out(outpath);
                if (!out) {
                    std::cout << "Failed to open " << outpath << " for writing.\n";
                } else {
                    out << "frame,bone,sx,sy,sz,qx,qy,qz,qw,tx,ty,tz,tw\n";
                    out << std::fixed;
                    out.precision(6);
                    for (uint32_t f = 0; f < framesCaptured; ++f) {
                        for (uint32_t b = 0; b < nbBones; ++b) {
                            const Andale::SQTData& s = allFrames[(size_t)f * nbBones + b];
                            out << f << ',' << b
                                << ',' << s.mScale.GetX() << ',' << s.mScale.GetY() << ',' << s.mScale.GetZ()
                                << ',' << s.mQuat.GetX()  << ',' << s.mQuat.GetY()  << ',' << s.mQuat.GetZ()  << ',' << s.mQuat.GetW()
                                << ',' << s.mTrans.GetX() << ',' << s.mTrans.GetY() << ',' << s.mTrans.GetZ() << ',' << s.mTrans.GetW()
                                << '\n';
                        }
                    }
                    std::cout << "Wrote " << outpath << " ("
                              << (size_t)framesCaptured * nbBones << " rows)\n";
                }
            }
        }

        delete[] rawSQT;
        delete output;
    }

    return 0;
}
