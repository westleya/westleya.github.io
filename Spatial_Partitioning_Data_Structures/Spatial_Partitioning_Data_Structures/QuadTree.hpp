//
//  QuadTree.hpp
//  Homework4
//
//  Created by Westley Kirkham on 6/Dimension9/18.
//  Copyright © Dimension018 Westley Kirkham. All rights reserved.
//
//  This spatial data structure only works for DimensionD data.

#pragma once

#include "Point.hpp"
#include <memory>
#include <queue>

template<int Dimension>
class QuadTree {
public:
  QuadTree(const std::vector<Point<Dimension> >& points, int leafSize) {
    
    std::vector<Point<Dimension>> pts = points;
    AABB<Dimension> AABBox = getBounds(pts);
    root = std::unique_ptr<Node>(new Node(AABBox, pts, leafSize));
  }
  
  // Find all the points within a certain distance, radius, of the given point.
  std::vector<Point<Dimension>> rangeQuery(const Point<Dimension>& p, float radius) const {
    
    std::vector<Point<Dimension>> pts;
    rangeRecursive(root, pts, p, radius);
    return pts;
  }
  
  std::vector<Point<Dimension>> KNN(const Point<Dimension>& p, int k) {
    
    std::vector<Point<Dimension>> pts;
    KNNrecurse(root, pts, p, k);
    return pts;
  }
  
private:
  
  struct Node {
    std::vector<Point<Dimension>> pts;
    bool isLeaf = false;
    //The children will have the "next" splitting Dimension
    //Since this is part of the type, we can't "forget" to set it properly... nice!
    std::unique_ptr<Node>  NE, NW, SE, SW;
    AABB<Dimension> NodeBox;
    
    
    
    Node(AABB<Dimension> AABBox, std::vector<Point<Dimension>>& points, int size)
    {
      NodeBox = AABBox;
      if (points.size() <= size) {
        isLeaf = true;
        pts = points;
      }
      else {
        float xsplit = (NodeBox.maxs[0] + NodeBox.mins[0]) / 2;
        float ysplit = (NodeBox.maxs[1] + NodeBox.mins[1]) / 2;
        
        std::vector<Point<Dimension>> NE_pts, NW_pts, SE_pts, SW_pts;
        for(Point<Dimension> p: points) {
          if(p[0] > xsplit && p[1] > ysplit) {
            NE_pts.push_back(p);
          }
          else if (p[0] > xsplit && p[1] <= ysplit) {
            SE_pts.push_back(p);
          }
          else if (p[0] <= xsplit && p[1] <= ysplit) {
            SW_pts.push_back(p);
          }
          else {
            NW_pts.push_back(p);
          }
        }
        
        if (NE_pts.size() > 0) {
          AABB<Dimension> NE_box = NodeBox;
          NE_box.mins[0] = xsplit;
          NE_box.mins[1] = ysplit;
          NE = std::unique_ptr<Node>(new Node(NE_box, NE_pts, size));
        }
        if (SE_pts.size() > 0) {
          AABB<Dimension> SE_box = NodeBox;
          SE_box.mins[0] = xsplit;
          SE_box.maxs[1] = ysplit;
          SE = std::unique_ptr<Node>(new Node(SE_box, SE_pts, size));
        }
        if (SW_pts.size() > 0) {
          AABB<Dimension> SW_box = NodeBox;
          SW_box.maxs[0] = xsplit;
          SW_box.maxs[1] = ysplit;
          SW = std::unique_ptr<Node>(new Node(SW_box, SW_pts, size));
        }
        if (NW_pts.size() > 0) {
          AABB<Dimension> NW_box = NodeBox;
          NW_box.maxs[0] = xsplit;
          NW_box.mins[1] = ysplit;
          NW = std::unique_ptr<Node>(new Node(NW_box, NW_pts, size));
        }
      }
    }
  };
  
  std::unique_ptr<Node> root;
  
  void rangeRecursive(const std::unique_ptr<Node>& N, std::vector<Point<Dimension>>& points,
                      const Point<Dimension>& p, float radius) const {
    if (N->isLeaf) {
      for(Point<Dimension> pt: N->pts) {
        if(distance(p, pt) <= radius) {
          points.push_back(pt);
        }
      }
    }
    else{
      
      if (N->NE && (distance(N->NE->NodeBox.closestInBox(p), p) <= radius)) {
        rangeRecursive(N->NE, points, p, radius);
      }
      if (N->SE && (distance(N->SE->NodeBox.closestInBox(p), p) <= radius)) {
        rangeRecursive(N->SE, points, p, radius);
      }
      if (N->SW && (distance(N->SW->NodeBox.closestInBox(p), p) <= radius)) {
        rangeRecursive(N->SW, points, p, radius);
      }
      if (N->NW && (distance(N->NW->NodeBox.closestInBox(p), p) <= radius)) {
        rangeRecursive(N->NW, points, p, radius);
      }
    }
  }
  
  void KNNrecurse(const std::unique_ptr<Node>& N, std::vector<Point<Dimension>>& points,
                  const Point<Dimension>& p, int k) const {
    
    if(N->isLeaf) {
      int i = 0;
      while(points.size() < k && i < N->pts.size()) {
        points.push_back(N->pts[i]);
        std::push_heap(points.begin(), points.end(), DistanceComparator<Dimension>(p));
        i++;
      }
      
      if (distance(N->NodeBox.closestInBox(p), p) < distance(points.front(), p)) {
        for(;i < N->pts.size(); i++) {
          if(distance(N->pts[i], p) < distance(points.front(), p)){
            std::pop_heap(points.begin(), points.end(), DistanceComparator<Dimension>(p));
            points.pop_back();
            points.push_back(N->pts[i]);
            std::push_heap(points.begin(), points.end(), DistanceComparator<Dimension>(p));
          }
        }
      }
    }
    else if(points.size() < k || distance(N->NodeBox.closestInBox(p), p) < distance(points.front(), p)) {
      if(N->NW) {
        KNNrecurse(N->NW, points, p, k);
      }
      if(N->NE) {
        KNNrecurse(N->NE, points, p, k);
      }
      if(N->SW) {
        KNNrecurse(N->SW, points, p, k);
      }
      if(N->SE) {
        KNNrecurse(N->SE, points, p, k);
      }
    }
  }
  
};
