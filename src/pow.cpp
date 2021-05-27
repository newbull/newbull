// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader* pblock, const Consensus::Params& params)
{
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // Hard adjustment difficulty 2019-02-06
    // try 0.11.2, 0.12.1, dose this fix crazy crash?
    // Hard adjustment difficulty again 2020-03-28 How to fix the crazy crash???
    // try 0.13.2, dose this fix crazy crash?
    // yeah! 0.13.2 more better, may be 0.10.4 is very unstable.
    // let us move on to next 0.14.2
    if ((pindexLast->nHeight > 318095 && pindexLast->nHeight < 318099) ||
        (pindexLast->nHeight == 320660)) {
        // const CBlockIndex* pindexFirst = pindexLast;
        // int64_t nActualTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
        // LogPrintf("  nActualTimespan = %d  before bounds\n", nActualTimespan);
        // nActualTimespan = Params().TargetTimespan() / 4;
        // Retarget
        // uint256 bnNew;
        // // uint256 bnNew2;
        // uint256 bnOld;
        // bnNew.SetCompact(pindexLast->nBits);
        // bnOld = bnNew;
        // // bnNew2 = bnNew;
        // // bnNew *= nActualTimespan;
        // // bnNew /= Params().TargetTimespan();
        // bnNew /= 4; //nBits/4 means difficulty*4

        // if (bnNew > Params().ProofOfWorkLimit())
        //     bnNew = Params().ProofOfWorkLimit();

        const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
        arith_uint256 bnNew;
        // arith_uint256 bnOld;
        bnNew.SetCompact(pindexLast->nBits);
        // bnOld = bnNew;
        // bnNew *= nActualTimespan;
        bnNew /= 4;

        if (bnNew > bnPowLimit)
            bnNew = bnPowLimit;

        /// debug print
        // LogPrintf("GetNextWorkRequired RETARGET\n");
        // // LogPrintf("Params().TargetTimespan() = %d    nActualTimespan = %d\n", Params().TargetTimespan(), nActualTimespan);
        // LogPrintf("Before: %08x  %s\n", pindexLast->nBits, bnOld.ToString());
        // LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());
        // // LogPrintf("After2:  %08x  %s\n", bnNew2.GetCompact(), bnNew2.ToString());
        // // LogPrintf("Params().ProofOfWorkLimit():  %08x  %s\n", Params().ProofOfWorkLimit().GetCompact(), Params().ProofOfWorkLimit().ToString());

        return bnNew.GetCompact();
    }

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight + 1) % params.DifficultyAdjustmentInterval() != 0) {
        if (params.fPowAllowMinDifficultyBlocks) {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing * 2)
                return nProofOfWorkLimit;
            else {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval() - 1);
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan / 4)
        nActualTimespan = params.nPowTargetTimespan / 4;
    if (nActualTimespan > params.nPowTargetTimespan * 4)
        nActualTimespan = params.nPowTargetTimespan * 4;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
