//
//  main.cpp
//  Homework4
//
//  Created by Westley Kirkham on 6/18/18.
//  Copyright © 2018 Westley Kirkham. All rights reserved.
//
#pragma once
#include <ctime>
#include <iostream>
#include <fstream>
#include "Point.hpp"
#include "KDTree.hpp"
#include "Generators.hpp"
#include "BucketKNN.hpp"
#include "QuadTree.hpp"
#include "Stopwatch.hpp"

const int dim = 2;
int num_points = 100;
int range_data = 100;
float query_range = 15;
int neighbors = 1;
// Testing was set to only run for dim = 2.
const bool testing = false;
const bool timing = true;
using ugen = UniformGenerator<dim>;
using ggen = GaussianGenerator<dim>;

void test_Constructors(std::vector<Point<dim>>& points, Point<dim>& p){
  // The constructors will throw errors if there are any problems
  BucketKNN bucket = BucketKNN<dim>(points, (std::rand() % 10) + 2);
  KDTree<dim> kd = KDTree<dim>(points);
  if(dim == 2) {
    QuadTree qt = QuadTree<dim>(points, (std::rand() % 64) + 1);
  }
  for(int i = 0; i < points.size(); i++) {
    std::vector<Point<dim>> kdknn = kd.KNN(points[i], 1);
    std::cout << "KD point: " << kdknn[0][0] << ", "<< kdknn[0][1] <<"\n";
    std::cout << "point: " << points[i][0] << ", "<< points[i][1] <<"\n";
    assert(kdknn[0][0] == points[i][0]);
    assert(kdknn[0][1] == points[i][1]);

  }
}

void test_RangeQueries( std::vector<Point<dim>>& points, Point<dim>& p) {
  assert(query_range > 0);
  BucketKNN bucket = BucketKNN<dim>(points, (std::rand() % 10) + 2);
  std::vector<Point<dim>> brq = bucket.rangeQuery(p, query_range);
  std::sort(brq.begin(), brq.end(),  DistanceComparator<dim>(p));

  KDTree<dim> kd = KDTree<dim>(points);
  std::vector<Point<dim>> kdrq = kd.rangeQuery(p, query_range);
  std::sort(kdrq.begin(), kdrq.end(),  DistanceComparator<dim>(p));
  assert(kdrq.size() == brq.size());
  for(int i = 0; i < dim; ++i) {
    for(int j =0; j < brq.size(); ++j) {
      assert(kdrq[j][i] == brq[j][i]);
      assert(distance(kdrq[j], p) <= query_range);
      assert(distance(brq[j], p) <= query_range);

    }
  }
  // Quadtree only works in two dimensions
  if(dim == 2) {
    QuadTree qt = QuadTree<dim>(points, (std::rand() % 64) + 1);
    std::vector<Point<dim>> qtrq = qt.rangeQuery(p, query_range);
    std::sort(qtrq.begin(), qtrq.end(),  DistanceComparator<dim>(p));
    for(int i = 0; i < dim; ++i) {
      for(int j =0; j < qtrq.size(); ++j) {
        assert(kdrq[j][i] == qtrq[j][i]);
        assert(distance(qtrq[j], p) < query_range);
      }
    }
  }
}
// Bucket and Quad currently agree
void test_KNN_Queries( std::vector<Point<dim>>& points, Point<dim>& p) {
  assert(neighbors <= num_points);
  
  std::vector<Point<dim>> test_arr;
  for(int i = 0; i < neighbors; i++) {
    test_arr.push_back(points[i]);
  }
  std::make_heap(test_arr.begin(), test_arr.end(), DistanceComparator<dim>(p));

  for(int i = neighbors; i < points.size(); i++){
    Point<dim> test_pt = points[i];
    if(distance(test_pt, p) < distance(test_arr.front(), p)) {
      std::pop_heap(test_arr.begin(), test_arr.end(), DistanceComparator<dim>(p));
      test_arr.pop_back();
      test_arr.push_back(test_pt);
      std::push_heap(test_arr.begin(), test_arr.end(), DistanceComparator<dim>(p));
    }
  }
  std::sort(test_arr.begin(), test_arr.end(), DistanceComparator<dim>(p));

  
  BucketKNN bucket = BucketKNN<dim>(points, (std::rand() % 10) + 2);
  std::vector<Point<dim>> bknn = bucket.KNN(p, neighbors);
  std::sort(bknn.begin(), bknn.end(), DistanceComparator<dim>(p));

  KDTree<dim> kd = KDTree<dim>(points);
  std::vector<Point<dim>> kdknn = kd.KNN(p, neighbors);
  std::sort(kdknn.begin(), kdknn.end(), DistanceComparator<dim>(p));

  //test - assert knn is the same size as the number requested
  assert(bknn.size() == neighbors);
  assert(kdknn.size() == neighbors);
  
  //test - assert the knn queries return the same closest neighbors
  for(int i = 0; i < dim; ++i) {
    for(int j =0; j < kdknn.size(); ++j) {
      std::cout << "query point: " << p[0] << ", " << p[1] << "\n";
      std::cout << "KD: " << distance(kdknn[j], p) << "\n";
      std::cout << "KD point: " << kdknn[j][0] << ", "<< kdknn[j][1] <<"\n";

      //std::cout << "Bucket: " << distance(bknn[j], p) << "\n";
      std::cout << "test_arr: " << distance(test_arr[j], p) << "\n";
      std::cout << "test_arr point: " << test_arr[j][0] << ", "<< test_arr[j][1] <<"\n";


      assert(bknn[j][i] == test_arr[j][i]);
      assert(kdknn[j][i] == test_arr[j][i]);
    }
  }
  
  // Quadtree only works in two dimensions
  if(dim == 2) {
    QuadTree qt = QuadTree<dim>(points, (std::rand() % 64) + 1);
    std::vector<Point<dim>> qtknn = qt.KNN(p, neighbors);
    std::sort(qtknn.begin(), qtknn.end(), DistanceComparator<dim>(p));
    //test - assert the knn queries return the same closest neighbors
    for(int i = 0; i < dim; ++i) {
      for(int j =0; j < kdknn.size(); ++j) {
        bknn;
        assert(kdknn[j][i] == qtknn[j][i]);
      }
    }
  }
}

void TestingSuite(int num_points, int range_data) {
  assert (dim > 0);
  std::vector<Point<dim>> points;
  Point<dim> p = ugen(-range_data, range_data).generatePoint();
  for(int i = 0;  i < num_points; ++i) {
    Point<dim> p= ugen(-range_data, range_data).generatePoint();
    points.push_back(p);
  }
  
  test_Constructors(points, p);
  test_RangeQueries(points, p);
  test_KNN_Queries(points, p);
}

void  Timeit(std::vector<std::string>& strings){
  
  // timing variable
  Stopwatch sw;
  
  // Use datasets of increasing size
  for(int N = 200; N <= 20000; N += 200){
    
    // The uniform dataset
    std::vector<Point<dim>> u_pts;
    // The uniform target
    Point<dim> u_p = ugen(-range_data, range_data).generatePoint();

    // The gaussian dataset
    std::vector<Point<dim>> g_pts;
    // The gaussian target
    Point<dim> g_p = ugen(-range_data, range_data).generatePoint();
    
    // populate pts
    for(int i = 0;  i < N; ++i) {
      Point<dim> up= ugen(-range_data, range_data).generatePoint();
      u_pts.push_back(up);
      Point<dim> gp= ugen(-range_data, range_data).generatePoint();
      g_pts.push_back(gp);
    }
    
    // Create data structures containing all the points from the pts datasets
    // QuadTrees must be commented out for timing to be done in dimensions != 2.
    QuadTree<dim> u_qt = QuadTree<dim>(u_pts, 32);
    KDTree<dim> u_kd = KDTree<dim>(u_pts);
    BucketKNN<dim> u_bt = BucketKNN<dim>(u_pts, (int) pow(N * .04, (float) 1 / dim));
    QuadTree<dim> g_qt = QuadTree<dim>(g_pts, 32);
    KDTree<dim> g_kd = KDTree<dim>(g_pts);
    BucketKNN<dim> g_bt = BucketKNN<dim>(g_pts, (int) pow(N * .04, (float) 1 / dim));
    
    // Do K-nearest neighbors queries of increasing difficulty
    // and time them ten times for each data structure
    // taking the average of the ten runs.
    for(int k = 1; k < N; k += N*.08){
      for(int j = 0; j < 3; j++) {
        // Total time to find K neighbors.
        double u_kdtime = 0, u_bktime = 0, u_qttime = 0;
        double g_kdtime = 0, g_bktime = 0, g_qttime = 0;
        
        // Time it!
        for(int i = 0; i < 10; i++) {
          
          // KD tree timing tests
          sw.start();
          u_kd.KNN(u_p, k);
          u_kdtime += sw.stop();
          sw.start();
          g_kd.KNN(g_p, k);
          g_kdtime += sw.stop();
          
          // Bucket timings tests
          sw.start();
          u_bt.KNN(u_p, k);
          u_bktime += sw.stop();
          sw.start();
          g_bt.KNN(g_p, k);
          g_bktime += sw.stop();
          
          // QuadTree only works for two dimensions
          if(dim == 2) {
            sw.start();
            u_qt.KNN(u_p, k);
            u_qttime += sw.stop();
            sw.start();
            g_qt.KNN(g_p, k);
            g_qttime += sw.stop();
          }
        }
        
        // Average the timing totals. Change to milliseconds
        u_kdtime *= 100; u_bktime *= 100; u_qttime *= 100;
        g_kdtime *= 100; g_bktime *= 100; g_qttime *= 100;
        
        // Create timing reports
        strings.push_back("K," + std::to_string(dim) + "," + std::to_string(k) +
                          "," + std::to_string(N) + "," + std::to_string(u_kdtime) + "," + "U\n");
        strings.push_back("B," + std::to_string(dim) + "," + std::to_string(k) +
                          "," + std::to_string(N) + "," + std::to_string(u_bktime) + "," + "U\n");
        strings.push_back("K," + std::to_string(dim) + "," + std::to_string(k) +
                          "," + std::to_string(N) + "," + std::to_string(g_kdtime) + "," + "G\n");
        strings.push_back("B," + std::to_string(dim) + "," + std::to_string(k) +
                          "," + std::to_string(N) + "," + std::to_string(g_bktime) + "," + "G\n");
        if(dim == 2) {
          strings.push_back("Q," + std::to_string(dim) + "," + std::to_string(k) +
                            "," + std::to_string(N) + "," + std::to_string(u_qttime) + "," + "U\n");
          strings.push_back("Q," + std::to_string(dim) + "," + std::to_string(k) +
                            "," + std::to_string(N) + "," + std::to_string(g_qttime) + "," + "G\n");
        }
      }
    }
  }
}

// DataStructure is the data structure (KD, Bucket, Quad) used.
// Dimensions is the number of dimensions. KNeighbors is the
// number of neighbors. NumPoints is the number of
// points stored in the structure. Time is how long
// the query took. Distribution is the distribution used:
// Gaussian or Uniform. (G/U)
void TimingSuite() {

  std::string output = "DataStructure,Dimensions,KNeighbors,NumPoints,Time(millis),Distribution\n";
  std::vector<std::string> strings;
  Timeit(strings);
  for(std::string s: strings){
    output += s;
  }
  std::ofstream result;
  result.open("data" + std::to_string(dim) + "k.csv");
  result << output;
  result.close();
}

int main(int argc, const char * argv[]) {

  // Testing is random
  std::srand((int) std::time(0));
  if(testing) {
    TestingSuite(num_points, range_data);
  }
  if(timing) {
    TimingSuite();
  }
  
  return 0;
}


