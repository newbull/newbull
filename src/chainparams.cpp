// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime = nTime;
    genesis.nBits = nBits;
    genesis.nNonce = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    // const char* pszTimestamp = "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks";
    const char* pszTimestamp = "The New York Times June 25, 2016 BREXIT SETS OFF A CASCADE OF AFTERSHOCKS";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        strNetworkID = "main";
        // consensus.nSubsidyHalvingInterval = 210000;
        //blocks will halving block out 50 to 25 etc., this means about 4 years.
        //cut in half every 700,800 blocks which will occur approximately every 4 years.
        consensus.nSubsidyHalvingInterval = 160000;
        // consensus.BIP34Height = 227931;
        // consensus.BIP34Hash = uint256S("0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8");
        // consensus.BIP65Height = 388381; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        // consensus.BIP66Height = 363725; // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        // consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        // consensus.nPowTargetSpacing = 10 * 60;
        // consensus.BIP34Height = 227931;
        // consensus.BIP34Hash = uint256S("0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8");
        consensus.BIP34Height = 321742;
        consensus.BIP34Hash = uint256S("0x000000026f514fc0b16480d2cf277f7c0e05f555e18ac58383e4bf23cc44a40c");
        consensus.BIP65Height = 324913; // 00000003c7165c9e4044f3d4c86328c59a989903fab8c6d9a8f292ac0495d4dd
        consensus.BIP66Height = 324913; // 00000003c7165c9e4044f3d4c86328c59a989903fab8c6d9a8f292ac0495d4dd
        // consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // bnProofOfWorkLimit = ~uint256(0) >> 32;
        // bnProofOfWorkLimit = ~uint256(0) >> 16;
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks. about change difficulty per 2016(60m/10m=6/h*24h=144/d*14d=2016) blocks.
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks. about change difficulty per 6720(60m/3m=20/h*24h=480/d*14d=6720) blocks.
        // consensus.nPowTargetSpacing = 10 * 60;
        consensus.nPowTargetSpacing = 3 * 60; // generate each block may use 3 minutes about.
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        // consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        // consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.nRuleChangeActivationThreshold = 950; // 95% of 2016
        consensus.nMinerConfirmationWindow = 1000;      // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999;   // December 31, 2008

        // // Deployment of BIP68, BIP112, and BIP113.
        // consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        // consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1462060800; // May 1st, 2016
        // consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // // Deployment of SegWit (BIP141, BIP143, and BIP147)
        // consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        // consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1479168000; // November 15th, 2016.
        // consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1510704000; // November 15th, 2017.

        // The best chain should have at least this much work.
        // consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000003f94d1ad391682fe038bf5");

        // By default assume that the signatures in ancestors of this block are valid.
        // consensus.defaultAssumeValid = uint256S("0x00000000000000000013176bf8d7dfeab4e1db31dc93bc311b436e82ab226b90"); //453354

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        // consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1601510400; // Oct 1, 2020
        // consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1604102400;   // Oct 31, 2020
        // consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1598918400; // Sep 1, 2020
        // // consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1600041600;   // Sep 14, 2020
        // consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1600300800; // Sep 17, 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1598918400; // Sep 1, 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1601510400;   // Oct 1, 2020

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        // consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1604188800; // November 1, 2020
        // consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1606694400;   // November 30, 2020
        // consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1600300800; // Sep 17, 2020
        // consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1601424000;   // Sep 30, 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1600300800; // Sep 17, 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1602892800;   // Oct 17, 2020

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000005b773be821f3a1"); //429009

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000000000000a100a32824095659cbcb577477e4a028134c826c71e3f9a4f1"); //429009

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        // pchMessageStart[0] = 0xf9;
        // pchMessageStart[1] = 0xbe;
        // pchMessageStart[2] = 0xb4;
        // pchMessageStart[3] = 0xd9;
        // nDefaultPort = 8333;
        pchMessageStart[0] = 0xe3;
        pchMessageStart[1] = 0xfc;
        pchMessageStart[2] = 0x9b;
        pchMessageStart[3] = 0xd9;
        nDefaultPort = 10101;
        nPruneAfterHeight = 100000;

        // genesis = CreateGenesisBlock(1231006505, 2083236893, 0x1d00ffff, 1, 50 * COIN);
        // consensus.hashGenesisBlock = genesis.GetHash();
        // assert(consensus.hashGenesisBlock == uint256S("0x000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"));
        // assert(genesis.hashMerkleRoot == uint256S("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

        genesis = CreateGenesisBlock(1466861400, 63342, 0x1f00ffff, 1, 1048576000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000529cb36b46a72792152bbab7ddcac642b9ee918ac3dd8f9862001511b0a1"));
        assert(genesis.hashMerkleRoot == uint256S("0x1c736e9abfb024cce3fd1e1390b75b526ae82f69104e30d32a81943ee534715b"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        // vSeeds.push_back(CDNSSeedData("bitcoin.sipa.be", "seed.bitcoin.sipa.be", true));                   // Pieter Wuille, only supports x1, x5, x9, and xd
        // vSeeds.push_back(CDNSSeedData("bluematt.me", "dnsseed.bluematt.me", true));                        // Matt Corallo, only supports x9
        // vSeeds.push_back(CDNSSeedData("dashjr.org", "dnsseed.bitcoin.dashjr.org"));                        // Luke Dashjr
        // vSeeds.push_back(CDNSSeedData("bitcoinstats.com", "seed.bitcoinstats.com", true));                 // Christian Decker, supports x1 - xf
        // vSeeds.push_back(CDNSSeedData("bitcoin.jonasschnelli.ch", "seed.bitcoin.jonasschnelli.ch", true)); // Jonas Schnelli, only supports x1, x5, x9, and xd

        // base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 0);
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 53); //53:N
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 5);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of(11111, uint256S("0x000011c00d7268c8659ed2d90684a721769d2c283008e14ac7f4776c4af71bc1"))(33333, uint256S("0x000000950aa0419636f644f7655bc9f722bf9083299efbb79f0be9791c0100ad"))(74000, uint256S("0x000000002520130b1414be16f30ac96537c08f89aaeb7e8f97e47318bf1f5a66"))(105000, uint256S("0x0000010190be97db7f402be8e8a6cf040ae02d431c7838021c601d81305997d3"))(134444, uint256S("0x0000005af85f4801c110a034d4da2bdee4d5f16aa62b2b52dc71de0953a401cc"))(168000, uint256S("0x00000180cde0e4a46b1fbbaf11ed1e2e7618f11612ac15f05c3b594dc823bb1e"))(193000, uint256S("0x00000105ac2f461955de4b5663c6871a7afe5ea18bc8239529c2b53b6f1503cb"))(210000, uint256S("0x0000006908c6a9184d071a9e8f9b37d02fd0f84044a7b182bdc220810a1d542f"))(216116, uint256S("0x000000e35c8a0e549ee954fad6efff7b8f65c0720795b30f9efcf4880032c649"))(225430, uint256S("0x0000004405aa2d7c79922975e3ac531b49edede893fc9a4c77e387b044cf8151"))(250000, uint256S("0x000000d4161504eb774c13268e8f0f371e39a5be36d42dd7c30fb13d9cc8996e"))(279000, uint256S("0x000000e804b257495bd3feb291d58bacc248b53630bb47f4fd3078620bf69a9e"))(295000, uint256S("0x00000037b1f1bfceb6df56e4e520b15b079b3ec03ac707ca6c0e6e45bdff0d66"))(300000, uint256S("0x0000000a4a23d51296fe5948b8b2744134b11463b1606e6e28ca0622366af7b9"))(350000, uint256S("0x00000045a016e610a174cc3ea23a04fd843ea0043f8fefb576bb7f38f6600c4f"))(400000, uint256S("0x00000000142b2cbbfeeff49c5f0260939ced57f04293c1be3f61e6798eb43751"))(430000, uint256S("0x0000000000170badae0204227d924abd1f624f5ab231b2f6ae178acb58f3dff0"))};

        chainTxData = ChainTxData{
            // Data as of block 0000000000438684d2f0cac4b01a3e4c3f9a7bc73ee773bad514922cc2790737 (height 430018).
            1622047650, // * UNIX timestamp of last known number of transactions
            430693,     // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            3.2         // * estimated number of transactions per second after that timestamp
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams
{
public:
    CTestNetParams()
    {
        strNetworkID = "test";
        // consensus.nSubsidyHalvingInterval = 210000;
        consensus.nSubsidyHalvingInterval = 160000;
        consensus.BIP34Height = 21111;
        consensus.BIP34Hash = uint256S("0x00000017a13e0a10612a1ea14578b57652f09890224bc6976e0ed67d9895da9d");
        consensus.BIP65Height = 21111; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP66Height = 21111; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 3 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 900; // 75% for testchains
        consensus.nMinerConfirmationWindow = 1000;      // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999;   // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1456790400; // March 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800;   // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1462060800; // May 1st 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1493596800;   // May 1st 2017

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000000167efea277"); //21112

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000000d8b3d2ca29ec76bf1da6059fb8e3af4baccf084e2aba8c76d1a53ad02"); //21112

        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;
        nDefaultPort = 10103;
        nPruneAfterHeight = 1000;

        // genesis = CreateGenesisBlock(1296688602, 414098458, 0x1d00ffff, 1, 50 * COIN);
        // consensus.hashGenesisBlock = genesis.GetHash();
        // assert(consensus.hashGenesisBlock == uint256S("0x000000000933ea01ad0ee984209779baaec3ced90fa3f408719526f8d77f4943"));
        // assert(genesis.hashMerkleRoot == uint256S("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));
        genesis = CreateGenesisBlock(1466861400, 184989, 0x1f00ffff, 1, 1048576000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000147ce63e90891448baaf2e5cd0cb56768498a7e02d8ceccd9fd033c4f504"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        // vSeeds.push_back(CDNSSeedData("testnetbitcoin.jonasschnelli.ch", "testnet-seed.bitcoin.jonasschnelli.ch", true));
        // vSeeds.push_back(CDNSSeedData("petertodd.org", "seed.tbtc.petertodd.org", true));
        // vSeeds.push_back(CDNSSeedData("bluematt.me", "testnet-seed.bluematt.me"));
        // vSeeds.push_back(CDNSSeedData("bitcoin.schildbach.de", "testnet-seed.bitcoin.schildbach.de"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = (CCheckpointData){
            boost::assign::map_list_of(555, uint256S("0000ef145b940fd89ba9417e398d8e4d32993865f4e24ec3abde27503482860e")),
        };

        chainTxData = ChainTxData{
            // Data as of block 00000000c2872f8f8a8935c8e3c5862be9038c97d4de2cf37ed496991166928a (height 1063660)
            1483546230,
            12834668,
            0.15};
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams
{
public:
    CRegTestParams()
    {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 3 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144;       // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 10105;
        nPruneAfterHeight = 1000;

        // genesis = CreateGenesisBlock(1296688602, 2, 0x207fffff, 1, 50 * COIN);
        // consensus.hashGenesisBlock = genesis.GetHash();
        // assert(consensus.hashGenesisBlock == uint256S("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));
        // assert(genesis.hashMerkleRoot == uint256S("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));
        genesis = CreateGenesisBlock(1466861400, 3, 0x207fffff, 1, 1048576000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x05c660a19b17c141e739eccab6c2aade26726c991162ec469c7dca67ad24af91"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of(0, uint256S("0x05c660a19b17c141e739eccab6c2aade26726c991162ec469c7dca67ad24af91"))};

        chainTxData = ChainTxData{
            0,
            0,
            0};

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
        return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
        return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
