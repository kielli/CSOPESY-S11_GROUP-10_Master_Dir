#include "PagingAllocator.h"

// PagingAllocator::PagingAllocator(int totalMemory, int minMemPerProc, int maxMemPerProc, int numFrames)
//     : totalMemory(totalMemory), minMemPerProc(minMemPerProc), maxMemPerProc(maxMemPerProc), numFrames (numFrames)
// {
//     for(int i = 0; i < numFrames; i++) {
//         this->freeFrameList.push({i, true});
//     }
// }