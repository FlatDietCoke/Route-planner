#include "route_planner.h"
#include <algorithm>
#include <vector>
#include <numeric>

using std::vector;
using std::cout;

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;
    start_node = &(m_Model.FindClosestNode(start_x, start_y));
    end_node = &(m_Model.FindClosestNode(end_x, end_y));
}

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
  return node->distance(*end_node)；
{

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();
    
    for (auto* neighbor_node : current_node->neighbors){
        neighbor_node->parent = current_node;
        neighbor_node->h_value = this->CalculateHValue(neighbor_node);
        neighbor_node->g_value = current_node->g_value + current_node->distance(*neighbor_node);
        neighbor_node->visited = true;
        this->open_list.emplace_back(neighbor_node);
    }
}
   
RouteModel::Node *RoutePlanner::NextNode() {
    sort(open_list.begin(), open_list.end(), [] (const RouteModel::Node* a, const RouteModel::Node* b){return a->g_value+a->h_value > b->g_value+b->h_value;});
}

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
  std::vector<RouteModel::Node> path_found;
  RouteModel::Node* parent_node = nullptr;
  distance = 0.0f;
  path_found.push_back(*current_node);
  while(current_node->parent){
      parent_node = current_node->parent;
      distance += current_node->distance(*parent_node);
      path_found.insert(path_found.begin(), *parent_node);
      current_node = parent_node;
  }
  distance *= m_Model.MetricScale(); 
  return path_found;
}

void RoutePlanner::AStarSearch() {
  this->start_node->visited = true;
  this->open_list.push_back(this->start_node);
  
  RouteModel::Node* current_node = nullptr;
  while (!open_list.empty()){
      current_node = this->NextNode();
      if (current_node == end_node) {          
        m_Model.path = this->ConstructFinalPath(current_node);
        return;
      } else {
        AddNeighbors(current_node);
      }
  }
}
