#pragma once
#include <vector>

//expected that nums is sorted
int binary_search(const std::vector<int>& nums, int target)
{
    int start = 0, end = nums.size() - 1;
    while(start <= end)
    {
        int mid = start + (end - start)/2;
        if(nums.at(mid) == target) return mid;
        else if(nums.at(mid) < target) start = mid + 1;
        else end = mid - 1;
    }

    return -1;
}

std::vector<int>::iterator binary_search(std::vector<int>::iterator start, std::vector<int>::iterator end, int target)
{
	while(start <= end)
    {
		auto mid = start + (end - start)/2;
        if(*mid == target) return mid;
		if(target > *mid)  return binary_search(mid+1, end, target);
		else               return binary_search(start, mid - 1, target);
	}

	return end;
}

//leetcode.com/problems/search-in-rotated-sorted-array/description/
//So, left or right half of rotated sorted array is always sorted... basically if we know the point of rotation,
//we can do normal binary search in one of the halves
int searchRotatedArray(std::vector<int>& nums, int target) 
{
    size_t rotation_index = 0;
    for(size_t i=0; i<nums.size() - 1; ++i)
    {
       if(nums.at(i) > nums.at(i+1))
	   {
		   rotation_index=i;
		   break;
	   }
    }

   // std::cout<<"Rotation index at "<<rotation_index<<"\n";
	
	//Now, target is in which half?
    std::vector<int>::iterator it;
	if(target > nums[rotation_index])
	{//first half
       it = binary_search(nums.begin(), nums.begin() + rotation_index - 1, target);
	}
	else 
	{
		it = binary_search(nums.begin() + rotation_index, nums.end(), target);
	}

    if(it == nums.end()) return -1;

    return static_cast<std::size_t>(it - nums.begin());
}