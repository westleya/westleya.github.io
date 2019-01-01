#pragma once

#include "Point.hpp"
#include <memory>
#include <queue>

template<int Dimension>
class KDTree{
public:

  KDTree(const std::vector<Point<Dimension> >& points){

    std::vector<Point<Dimension>> pts = points;
    root = std::unique_ptr<Node<0>>(new Node<0>(pts.begin(), pts.end()));
  }
  
  // Find all the points within a certain distance, radius, of the given point.
  std::vector<Point<Dimension>> rangeQuery(const Point<Dimension>& p, float radius) const{
    
    std::vector<Point<Dimension>> pts;
    rangeRecursive(root, pts, p, radius);
    return pts;
  }
  
  std::vector<Point<Dimension>> KNN(const Point<Dimension>& p, int k) {
    
    AABB<Dimension> AABBox;
    std::vector<Point<Dimension>> pts;
    KNNrecurse(root, AABBox, pts, p, k);
    return pts;
  }
private:
  
  template<int SplitDimension> //Don't store the split dimension explicitly
  struct Node{
	Point<Dimension> p;
	//The children will have the "next" splitting dimension
	//Since this is part of the type, we can't "forget" to set it properly... nice!
	std::unique_ptr<Node< (SplitDimension + 1)%Dimension> >  left, right;


	/*
	  We'll use iterators to describe the chunk of our points array that belong to this node/subtree
	 */
	template<typename Iter>
	Node(Iter begin, Iter end)
	{
	  //Our children (if we have any) will use the "next" splitting dimension
	  using ChildType = Node<(SplitDimension +1)%Dimension>;
	  
    Iter middle = begin + ((end - begin) / 2);
    
    std::nth_element(begin, middle, end, CompareBy<SplitDimension>());
    p = *middle;
	  
    if(begin != middle) {
      left = std::unique_ptr<ChildType>(new ChildType(begin, middle));
    }
    if (middle + 1 != end){
      right = std::unique_ptr<ChildType>(new ChildType(middle + 1, end));
    }
    
	}
	
  };
  
  std::unique_ptr<Node<0>> root;

  template<int SplitDimension>
  void rangeRecursive(const std::unique_ptr<Node<SplitDimension>>& N,
                      std::vector<Point<Dimension>>& pts, const Point<Dimension>& pt, float r) const {
    
    if(distance(N->p, pt) <= r){
      pts.push_back(N->p);
    }
    // pt @ split dimension <= pt(x) + r and/or >=pt(x) - r
    //left within range
    if(N->left && (N->p[SplitDimension] >= (pt[SplitDimension] - r))){
      rangeRecursive(N->left, pts, pt, r);
    }//right within range
    if (N->right && (N->p[SplitDimension] <= (pt[SplitDimension] + r))){
      rangeRecursive(N->right, pts, pt, r);
    }
  }
  
  template<int SplitDimension>
  void KNNrecurse(const std::unique_ptr<Node<SplitDimension>>& N,
                  AABB<Dimension>& AABBox, std::vector<Point<Dimension>>& pts,
                  const Point<Dimension>& pt, int k) const {
    
    if( pts.size() < k) {
      pts.push_back(N->p);
      std::push_heap(pts.begin(), pts.end(), DistanceComparator<Dimension>(pt));
    }
    else if (distance(N->p, pt) < distance(pts.front(), pt)) {
      std::pop_heap(pts.begin(), pts.end(), DistanceComparator<Dimension>(pt));
      pts.pop_back();
      pts.push_back(N->p);
      std::push_heap(pts.begin(), pts.end(), DistanceComparator<Dimension>(pt));
    }
    
    if(N->left) {
      AABB<Dimension> leftBox = AABBox;
      leftBox.maxs[SplitDimension] = N->p[SplitDimension];
      if(pts.size() < k || distance(leftBox.closestInBox(pt), pt) < distance(pts.front(), pt)) {
        KNNrecurse(N->left, leftBox, pts, pt, k);
      }
    }
    if(N->right) {
      AABB<Dimension> rightBox = AABBox;
      rightBox.mins[SplitDimension] = N->p[SplitDimension];
      if(pts.size() < k || distance(rightBox.closestInBox(pt), pt) < distance(pts.front(), pt)) {
        KNNrecurse(N->right, rightBox, pts, pt, k);
      }
    }
    
  }
};


//knn pointers from Mason
//pass in bounding box, vector you'll pass points to, call knn recurse
// pass in root to recurse, point, value, and bounding box
// krecurse
// if point vector isn't full yet, just fill it
// do heap stuff to a vector
// no room? check [0]th position
// pop heap
// pop vector
// push vector
// order vector
// then recurse down
// for left - copy AABB and reset max
// for right - copy AABB and reset min
// is it worth it to recurse? vector not full or distance, use AABB.closestinbox()
