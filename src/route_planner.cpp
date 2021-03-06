#include "route_planner.h"
#include <algorithm>
#include <cmath>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {

    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    //Discretize the starting and ending coordinates
    auto closest_2_start = model.FindClosestNode(start_x, start_y);
    auto closest_2_end = model.FindClosestNode(end_x, end_y);
    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.

    
    start_node=&closest_2_start;
    end_node=&closest_2_end;

}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    //using euclidian distance as an H value heuristic
    //wrapper for the Node distance function

    return std::sqrt(std::pow((node->x - end_node->x), 2) + std::pow((node->y - end_node->y), 2));

    //for some inexplicable reason, this errors but the exact same thing above does not.  Riddle me this.  
    //return node->distance(*end_node);
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors. (done)
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
        std::cout<<"asdf***************************"<<std::endl;
    current_node->FindNeighbors();
        std::cout<<"asdf***************************"<<std::endl;


    //puts it into a node data structure neighbors

    //for loop in this form: for (auto adjacentnode: current_node->neighbors ) segfaults for some reason

    for (int i=0;i<(current_node->neighbors).size();i++)
    {
        auto adjacentnode=(current_node->neighbors)[i];
        std::cout<<adjacentnode->x <<"     "<<adjacentnode->y<<std::endl;
    }

    std::cout<<(current_node->neighbors).size()<<std::endl;

    // for (RouteModel::Node * adjacentnode: current_node->neighbors )
    // {
    //     if (adjacentnode==nullptr)
    //         std::cout<<"nulpointer"<<std::endl;
    //     std::cout<<adjacentnode->x <<"     "<<adjacentnode->y<<std::endl;
    //     // adjacentnode->h_value=RoutePlanner::CalculateHValue(adjacentnode);
    //     // //g value is the number of steps from startingposition
    //     // adjacentnode->g_value=current_node->g_value+1;
    //     // adjacentnode->parent=current_node;
    // }


}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

static bool fValueSort(RouteModel::Node * e1, RouteModel::Node * e2)
{
    return (e1->h_value+e1->g_value > e2->h_value+e2->g_value);
}

RouteModel::Node *RoutePlanner::NextNode() {

    //sort in descending order and return the last/least costly element
    std::sort(open_list.begin(),open_list.end(), fValueSort);
    auto returnval=open_list.back();
    open_list.pop_back();
    return returnval;
    

    // float lowestcost=1.79e+308;
    // RouteModel::Node * bestnode;
    // for (auto node:open_list)
    // {
    //     float cost=node->h_value+node->g_value;
    //     if (cost<lowestcost)
    //     {
    //     lowestcost=cost;
    //     bestnode=node;}
    // }


}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    path_found.push_back(*current_node);
    while (current_node->parent)
    {
        path_found.push_back (*(current_node->parent));
        current_node= current_node->parent;
        distance=distance+current_node->distance(*current_node->parent);
    }

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    //RouteModel::Node *current_node = nullptr;
    RouteModel::Node *current_node = start_node;
    //auto next_node=start_node;

    // TODO: Implement your solution here.
    while (current_node!=end_node)
    {
        AddNeighbors(current_node);
        current_node=NextNode();
        
    }
        m_Model.path=ConstructFinalPath(current_node);
}