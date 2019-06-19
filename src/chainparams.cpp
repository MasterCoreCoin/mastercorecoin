// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2019 The MasterCoreCoin Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "bignum.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (     0, uint256("0x0000097b575ab70b0eb8ae6c5ded0fa5e271a2e5fd3c35d3224d96e48ae0c4b6"))
    (  1000, uint256("0x0000017403e1e3da894b523e7efb27a7060da0cad0d4a1d24647bd24dd056ece"))
    (  2000, uint256("0x0000005f58812f17b99f5dfbc1349f2e656f7c4c655b5bac883e58ee51a7e049"))
    (  4000, uint256("0x000000bf41e29bf35c0ab6221a9d86a31425569c54afc0e42e2ab2d0a6e975f3"))
    (  8000, uint256("0x0000016763d88f65fb4c3ab9ce9b31f58a5e44ae65ef74b190dbd791d08ee9b1"))
    ( 10000, uint256("0x000000000028088c742c40ecfb3ddc359e36a1e9dc53768babc5a31d4449d8e9"))
    ( 15000, uint256("0x0000000000002aa7f82185f8f893b64ca859df449166be598d6f18e7040fe7b0"))
    ( 19000, uint256("0x00000000000179f33ab4261292e903a911407000ebbe674395b7190a7262d8eb"))
    ( 35301, uint256("0x9d51c6d46f9fa97fa269641666bf7985ffc1a9e4f2777574eb215e4b2b254acb"))
    ( 35401, uint256("0xf99ff888de96c77fd726bf7a613ee506d8df9475f3676501d83e7b4047042a73"))
    ( 45490, uint256("0x4568f68742ef5b374acd156354c2a493443228004beae6864c70066bf38b3fd9"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1560133073
    , // * UNIX timestamp of last checkpoint block
    60726,          // * total number of transactions between genesis and last checkpoint (the tx=... number in the SetBestChain debug.log lines)
    1440        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x0000097b575ab70b0eb8ae6c5ded0fa5e271a2e5fd3c35d3224d96e48ae0c4b6"));

static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1557320856, // * UNIX timestamp of last checkpoint block
    0,          // * total number of transactions between genesis and last checkpoint (the tx=... number in the SetBestChain debug.log lines)
    1440        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x0000097b575ab70b0eb8ae6c5ded0fa5e271a2e5fd3c35d3224d96e48ae0c4b6"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1557320856, // * UNIX timestamp of last checkpoint block
    0,          // * total number of transactions between genesis and last checkpoint (the tx=... number in the SetBestChain debug.log lines)
    1440        // * estimated number of transactions per day after checkpoint
};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID                      = CBaseChainParams::MAIN;
        strNetworkID                   = "main";

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0]             = 0x3c;
        pchMessageStart[1]             = 0xdc;
        pchMessageStart[2]             = 0x21;
        pchMessageStart[3]             = 0xcb;
        vAlertPubKey                   = ParseHex("04093f483bfefb589393158612f3e949905007bcad76e09e449f88bd5af83dd5a1eeedd5ae3aced724cbed84722c580357a863a7cf0e867dba34ffad053998802a");
        nDefaultPort                   = 29871;
        nSubsidyHalvingInterval        = 1050000;
        nMaxReorganizationDepth        = 100;
        nEnforceBlockUpgradeMajority   = 750;
        nRejectBlockOutdatedMajority   = 950;
        nToCheckBlockUpgradeMajority   = 1000;
        nMinerThreads                  = 0;

        bnProofOfWorkLimit             = ~uint256(0) >> 20;
        nTargetTimespan                =  1 * 60;
        nTargetSpacing                 =  1 * 60;  // MasterCoreCoin: 1 minute blocks during POW

        bnProofOfStakeLimit            = ~uint256(0) >> 20;
        nTargetTimespanPOS             = 40 * 60;
        nTargetSpacingPOS              =  1 * 60;  // MasterCoreCoin: 1 minute blocks during POS

        nMaturity                      = 5;
        nMasternodeCountDrift          = 20;
        nMaxMoneyOut                   = 30000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock                  = 35300;
        nModifierUpdateBlock           = 1;

        const char* pszTimestamp       = "By MasterCoreCoin Devs 2019";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig         = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue           = 0 * COIN;
        txNew.vout[0].scriptPubKey     = CScript() << ParseHex("04afc8b7ed7b0a825ded476c253713e16628a6ba19f08e43d9a3f6890aed1700fa9d4d8539dec50747b88e9ea6c6db3a5c12d4b2e64bf4fb658bf656e285184757") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock          = 0;
        genesis.hashMerkleRoot         = genesis.BuildMerkleTree();
        genesis.nVersion               = 1;
        genesis.nTime                  = 1557320856;
        genesis.nBits                  = 504365040;
        genesis.nNonce                 = 412962;

        hashGenesisBlock               = genesis.GetHash();
        assert(hashGenesisBlock        == uint256("0x0000097b575ab70b0eb8ae6c5ded0fa5e271a2e5fd3c35d3224d96e48ae0c4b6"));
        assert(genesis.hashMerkleRoot  == uint256("0xde80287f4a349b411fc5d59c409b280ff1f2a3132b54ffeca8267a54a43b6534"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,  50);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,  63);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1, 193);

        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >(); // MasterCoreCoin BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >(); // MasterCoreCoin BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x92)(0xf1).convert_to_container<std::vector<unsigned char> >(); // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md 109 	0x800092f1

        vFixedSeeds.clear();
        vSeeds.clear();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        vSeeds.push_back(CDNSSeedData("45.76.208.183", "45.76.208.183"));
        vSeeds.push_back(CDNSSeedData("198.13.38.119", "198.13.38.119"));
        vSeeds.push_back(CDNSSeedData("45.77.21.70", "45.77.21.70"));
        vSeeds.push_back(CDNSSeedData("45.32.39.247", "45.32.39.247"));

        fMiningRequiresPeers           = true;
        fAllowMinDifficultyBlocks      = false;
        fDefaultConsistencyChecks      = false;
        fRequireStandard               = true;
        fMineBlocksOnDemand            = false;
        fSkipProofOfWorkCheck          = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive     = false;

        nPoolMaxTransactions           = 3;
        strSporkKey                    = "045aabb21152a1f5c0cfb3a9fc67ca457c03804d241239db04347b72b319f348c88e224ba4afed80b01d16af91615aa3ab6ba82277ca7fd8cc95175df5079a49d1";
        strMasternodePoolDummyAddress  = "MQ7Y1bswueUMpbdrX69NbqU9cVyJHFy73H";
        nStartMasternodePayments       = 1525192183;

        nBudget_Fee_Confirmations      = 6;

        strTreasuryAddress             = "MHGrD2ua36ii4L73rvXiJZBJR6KrsZBZji";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID                      = CBaseChainParams::TESTNET;
        strNetworkID                   = "test";
        pchMessageStart[0]             = 0xa4;
        pchMessageStart[1]             = 0xc2;
        pchMessageStart[2]             = 0xcc;
        pchMessageStart[3]             = 0xbc;
        vAlertPubKey                   = ParseHex("04093f483bfefb589393158612f3e949905007bcad76e09e449f88bd5af83dd5a1eeedd5ae3aced724cbed84722c580357a863a7cf0e867dba34ffad053998802a");
        nDefaultPort                   = 29873;
        nEnforceBlockUpgradeMajority   = 51;
        nRejectBlockOutdatedMajority   = 75;
        nToCheckBlockUpgradeMajority   = 100;
        nMinerThreads                  = 0;

        bnProofOfWorkLimit             = ~uint256(0) >> 20;
        nTargetTimespan                =  1 * 60;
        nTargetSpacing                 =  1 * 60;  // MasterCoreCoin: 1 minute blocks during POW

        bnProofOfStakeLimit            = ~uint256(0) >> 20;
        nTargetTimespanPOS             =  40 * 60;
        nTargetSpacingPOS              =   1 * 60;  // MasterCoreCoin: 1 minute blocks during POS

        nLastPOWBlock                  = 525601;
        nMaturity                      = 5;
        nMasternodeCountDrift          = 4;
        nModifierUpdateBlock           = 1;
        nMaxMoneyOut                   = 30000000 * COIN;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime                  = 1557320856;
        genesis.nNonce                 = 412962;

        hashGenesisBlock               = genesis.GetHash();
        assert(hashGenesisBlock        == uint256("0x0000097b575ab70b0eb8ae6c5ded0fa5e271a2e5fd3c35d3224d96e48ae0c4b6"));
        assert(genesis.hashMerkleRoot  == uint256("0xde80287f4a349b411fc5d59c409b280ff1f2a3132b54ffeca8267a54a43b6534"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,  110);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 125);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1, 193);

        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >(); // MasterCoreCoin BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >(); // MasterCoreCoin BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();
        // Testnet MasterCoreCoin BIP44 coin type is '1' (All coin's testnet default)

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers           = true;
        fAllowMinDifficultyBlocks      = false;
        fDefaultConsistencyChecks      = false;
        fRequireStandard               = false;
        fMineBlocksOnDemand            = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions           = 2;
        strSporkKey                    = "045aabb21152a1f5c0cfb3a9fc67ca457c03804d241239db04347b72b319f348c88e224ba4afed80b01d16af91615aa3ab6ba82277ca7fd8cc95175df5079a49d1";
        strMasternodePoolDummyAddress  = "mP5DQxNJ41jaDqiRNJeJMjSF1auhfW9zMQ";
        nStartMasternodePayments       = genesis.nTime + 86400; // 24 hours after genesis
        nBudget_Fee_Confirmations      = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet

        strTreasuryAddress             = "mHk54sqsqmU4z2bxty1vak3iDAt1ApP15y";
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x20;
        pchMessageStart[1] = 0xee;
        pchMessageStart[2] = 0x32;
        pchMessageStart[3] = 0xbc;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // MasterCoreCoin: 1 day
        nTargetSpacing = 2 * 60;        // MasterCoreCoin: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1516926684;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 20542300;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 14034;
       // assert(hashGenesisBlock == uint256("0x0000097b575ab70b0eb8ae6c5ded0fa5e271a2e5fd3c35d3224d96e48ae0c4b6"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
