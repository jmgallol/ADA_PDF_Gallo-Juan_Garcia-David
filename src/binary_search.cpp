#include "binary_search.hpp"

static int searchRecursive(const std::vector<Request> &requests, int left, int right, int k, int answer)
{
    if (left > right)
    {
        return answer;
    }

    int mid = left + (right - left) / 2;

    if (requests[mid].tenure >= k)
    {
        answer = mid;
        return searchRecursive(requests, left, mid - 1, k, answer);
    }

    return searchRecursive(requests, left, mid - 1, k, answer);
}

int recursiveBinarySearchFirstTenureGreaterEqual(const std::vector<Request> &requests, int k)
{
    if (requests.empty())
    {
        return -1;
    }

    return searchRecursive(requests, 0, requests.size() - 1, k, -1);
}