#include <iostream>
#include <vector>
#include <algorithm>

#include "sort.hpp"

namespace NTape {
    namespace {
        void SortBatches(TUniquePtr<ITape>& tape, size_t batchSize) {
            size_t n = tape->Size();

            std::vector<uint32_t> buf;
            buf.reserve(batchSize);
            for (size_t l = 0; l < n; l += batchSize) {
                size_t r = std::min(n, l + batchSize);
                buf.clear();

                for (size_t i = l; i < r; i++) {
                    buf.push_back(tape->ReadUInt32());
                    tape->MoveRight();
                }

                std::sort(buf.begin(), buf.end());

                tape->SetCurrentPosition(l);

                for (size_t i = l; i < r; i++) {
                    tape->WriteUInt32(buf[i - l]);
                    tape->MoveRight();
                }
            }
        }
    } // anonymous namespace

    void Sort(TUniquePtr<ITape>& sorted, TUniquePtr<ITape>& temp1, TUniquePtr<ITape>& temp2, size_t maxMemory) {
        auto& a = sorted;
        auto& b = temp1;
        auto& c = temp2;

        auto n = a->Size();
        auto batchSize = maxMemory;

        SortBatches(a, batchSize);

        a->SetCurrentPosition(0);

        while (batchSize < n) {
            a->SetCurrentPosition(0);
            b->SetCurrentPosition(0);
            c->SetCurrentPosition(0);

            size_t batchesCount = (a->Size() + batchSize - 1) / batchSize;
            size_t mid = batchSize * (batchesCount / 2);

            for (size_t i = 0; i < mid; i++) {
                auto value = a->ReadUInt32();
                a->MoveRight();
                b->WriteUInt32(value, true);
                b->MoveRight();
            }

            for (size_t i = mid; i < n; i++) {
                auto value = a->ReadUInt32();
                a->MoveRight();
                c->WriteUInt32(value, true);
                c->MoveRight();
            }

            a->SetCurrentPosition(0);
            b->SetCurrentPosition(0);
            c->SetCurrentPosition(0);
            for (size_t batchNumber = 0; batchNumber < batchesCount / 2; batchNumber++) {
                size_t batchIndexStart = batchNumber * batchSize;
                size_t batchLeftIndexEnd = batchIndexStart + batchSize;
                size_t batchRightIndexEnd = std::min(batchIndexStart + batchSize, n - mid);

                size_t l = batchIndexStart;
                size_t r = batchIndexStart;

                while (l < batchLeftIndexEnd || r < batchRightIndexEnd) {
                    if (l == batchLeftIndexEnd) {
                        auto value = c->ReadUInt32();
                        c->MoveRight();
                        a->WriteUInt32(value);
                        a->MoveRight();
                        r++;
                    } else if (r == batchRightIndexEnd) {
                        auto value = b->ReadUInt32();
                        b->MoveRight();
                        a->WriteUInt32(value);
                        a->MoveRight();
                        l++;
                    } else {
                        auto bValue = b->ReadUInt32();
                        auto cValue = c->ReadUInt32();

                        if (bValue > cValue) {
                            a->WriteUInt32(cValue);
                            a->MoveRight();
                            c->MoveRight();
                            r++;
                        } else {
                            a->WriteUInt32(bValue);
                            a->MoveRight();
                            b->MoveRight();
                            l++;
                        }
                    }
                }
            }

            if (batchesCount & 1) {
                int64_t need = static_cast<int64_t>(n - (batchesCount - 1) * batchSize);
                while (need--) {
                    auto value = c->ReadUInt32();
                    c->MoveRight();
                    a->WriteUInt32(value);
                    a->MoveRight();
                }
            }

            batchSize <<= 1;
        }
    }
}
