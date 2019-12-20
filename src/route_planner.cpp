#include "route_planner.h"
#include <algorithm>
#include <forward_list>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    start_node = &model.FindClosestNode(start_x, start_y);
    end_node = &model.FindClosestNode(end_x, end_y);
}


float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance(*end_node);
}


void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();
    for (auto* neighbor : current_node->neighbors) {
            neighbor->parent = current_node;
            neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);
            neighbor->h_value = CalculateHValue(neighbor);
            neighbor->visited = true;

            open_list.push_back(neighbor);        
    }
}


RouteModel::Node *RoutePlanner::NextNode() {
    std::sort(open_list.begin(), open_list.end(), [](auto* a, auto* b) {
        float a_value = a->h_value + a->g_value;
        float b_value = b->h_value + b->g_value;
        return a_value > b_value;
    });
    auto* node = open_list.back();
    open_list.pop_back();
    return node;
}



std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;
    std::forward_list<RouteModel::Node> path_list; 

    while (current_node != nullptr) {
        path_list.push_front(*current_node);
        if (current_node->parent != nullptr) {
            distance += current_node->distance(*current_node->parent);
        }

        current_node = current_node->parent;
    }

    for (auto node : path_list) {
        path_found.push_back(node);
    }

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;
    current_node = start_node;
    current_node->visited = true;

    while (current_node != end_node) {
        AddNeighbors(current_node);
        current_node = NextNode();
    }
    
    m_Model.path = ConstructFinalPath(end_node);    
}