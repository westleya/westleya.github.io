#pragma once

#include "Point.hpp"
#include <vector>
#include <math.h>

template<int Dimension> //The dimension of the points.  This can be used like any other constant within.
class BucketKNN{

  using dimArr = std::array<int, Dimension>;
  using bucket = std::vector<Point<Dimension> >;
  
public:

  // Creates an array of buckets, each containing
  BucketKNN(const bucket& points, int divisions_)
  {
    
    //
    bounds = getBounds(points);
    for(int i = 0; i < Dimension; i++) {
      bucketSize[i] = (bounds.maxs[i] - bounds.mins[i]) / (float) divisions_;
    }
    
    num_divs = divisions_;
    buckets.resize(pow(divisions_, Dimension));

    //
    for(Point<Dimension> p : points) {
      dimArr indices = findBucket(p);
      int arr_loc = arrayIndex(indices);
      buckets[arr_loc].push_back(p);
    }

  }
  
  // Find all the points within a certain distance, radius, of the given point.
  bucket rangeQuery(const Point<Dimension>& p, float radius) const{

    std::array<float, Dimension> min_pt;
    std::array<float, Dimension> max_pt;
    
    //
    for(int i = 0; i < Dimension; i++) {
      min_pt[i] = p[i] - radius;///****try clamp if issues
      max_pt[i] = p[i] + radius;
    }
    
    dimArr min_coords = findBucket(Point<Dimension>{min_pt});
    dimArr max_coords = findBucket(Point<Dimension>{max_pt});
    
    bucket points;
    //
    for(dimArr coords = min_coords; coords <= max_coords;
        coords = nextBucket(coords, min_coords, max_coords)){
      
      int arr_loc = arrayIndex(coords);
      //
      for(Point<Dimension> pt: buckets[arr_loc]) {
        if ( distance(pt, p) <= radius) {
          points.push_back(pt);
        }
      }
    }

    return points;
  }

  //
  bucket KNN(const Point<Dimension>& p, int k) const{

    int numPoints = 0;
    for(bucket b: buckets) {
      numPoints += b.size();
    } //
	  assert(numPoints >= k);

    // Sets the initial radius of the query
    // and the increment for subsequent queries.
    float increment = bucketSize[0];
    for(float f: bucketSize){
      // Setting the increment to the smallest bucket
      // dimension promotes not wasting time
      // searching extra buckets.
      if(f < increment) {
        increment = f;
      }
    }
    
    // Perform a series of graduating range queries
    // until we find at least k points.
    bucket neighbors = {};
    while (neighbors.size() < k){
      neighbors = rangeQuery(p, increment);
      increment += increment;
    }
    
    // Cut the points down to k
    std::nth_element(neighbors.begin(), neighbors.begin() + k, neighbors.end(), DistanceComparator<Dimension>(p));
    neighbors.resize(k);
    return neighbors;
	
  }
  
private:

  //
  std::vector<bucket> buckets;
  AABB<Dimension> bounds;
  float bucketSize[Dimension - 1];
  int num_divs;
  
  //
  dimArr findBucket(Point<Dimension> p) const {
    dimArr index;
    for(int i = 0; i < Dimension; i++){
      int dim_slot = (p[i] - bounds.mins[i]) / bucketSize[i];
      // It's possible, because of the inexact nature in which buckets are sized,
      // that a point will be indexed outside of the provided range of divisions.
      // Clamp prevents this.
      index[i] = std::clamp(dim_slot, 0, num_divs - 1);
    }
    return index;
  }
  
  //
  int arrayIndex(dimArr indices) const {
    int index = 0;
    for(int i = 0; i < Dimension; i++) {
      index += indices[i] * pow(num_divs, Dimension - i - 1);
    }
    return index;
  }
  
  //
  dimArr nextBucket(dimArr current, dimArr& minCoords, dimArr& maxCoords) const {
    current[Dimension - 1]++; //increment the last dimension
    for(int i = Dimension - 1; i > 0; --i){
      //if we need to "carry"
      if(current[i] > maxCoords[i]){
        //reset this dimension
        current[i] = minCoords[i];
        //and add to the next "digit"
        current[i -1]++;
      } else {
        //no more carries... we're done here
        break;
      }
    }
    return current;
  }
  
};
