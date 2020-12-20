#include <fstream>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
using namespace std;

struct TreeNode
{
    std::string balance_name;
    long int self_weight;
    long int weight_needed_for_left_balance;
    long int weight_needed_for_right_balance;
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;
};

class BalancerProcessor
{
    std::shared_ptr<TreeNode> m_root = nullptr;
    std::vector<std::string> m_balancer_sequenced_input;
    std::unordered_map<std::string,std::pair<int,int>> m_balance_mapping;
    public:
        bool ParseInputFileToTree(const std::string& input_file)
        {
            ifstream input(input_file.c_str());
            if(!input.is_open())
            {   
                cout<<"Unable to open input/output files"<<endl;
                return false;
            }   
            std::string currValue;
            bool is_parsing_successful = true;
            while (input >> currValue && is_parsing_successful)
            {
                is_parsing_successful = ParseInputStringToNode(currValue);
            }
            return is_parsing_successful;
        }

        void PerformTreeBalancing()
        {
            CalculateBalancingDepthFirst(m_root);
        }

        int CalculateBalancingDepthFirst(std::shared_ptr<TreeNode> root)
        {
            if (!root)
            {
                return 0;
            }
            if ((!root->left && root->right) || (root->left && !root->right))
            {
                cout<<"Invalid/Unbalanced node found in tree" << std::endl;
                return 0;
            }
            if (root->balance_name == "") //Terminating condition
                return root->self_weight;
            int left_tree_weight = CalculateBalancingDepthFirst(root->left);
            int right_tree_weight = CalculateBalancingDepthFirst(root->right);
            if (left_tree_weight != right_tree_weight)
            {
                root->weight_needed_for_left_balance = (left_tree_weight < right_tree_weight) ? (right_tree_weight - left_tree_weight) : 0;
                root->weight_needed_for_right_balance = (left_tree_weight > right_tree_weight) ? (left_tree_weight - right_tree_weight) : 0;
                m_balance_mapping[root->balance_name] = std::make_pair(root->weight_needed_for_left_balance,root->weight_needed_for_right_balance);
            }
            return (left_tree_weight + right_tree_weight + root->weight_needed_for_left_balance + root->weight_needed_for_right_balance + root->self_weight); 
        }

        void InorderTraversal(std::shared_ptr<TreeNode> root)
        {
            if (root)
            {
                cout << "Balance Name=" << root->balance_name << " self_weight=" << root->self_weight << "\n";
                InorderTraversal(root->left);
                InorderTraversal(root->right);
            }
        }

        void PrintBalancerOutput()
        {
            for (const auto&elem : m_balancer_sequenced_input)
            {
                std::cout << elem << "," << m_balance_mapping[elem].first << "," << m_balance_mapping[elem].second  <<"\n";
            }
        }

        bool UpdateSpecificBalance(std::shared_ptr<TreeNode> root, const std::string& node_name, const std::string& left_name, const std::string& right_name)
        {
            if (root)
            {
                if (root->balance_name == node_name)
                {
                    std::shared_ptr<TreeNode> left_node = make_shared<TreeNode>();
                    std::shared_ptr<TreeNode> right_node = make_shared<TreeNode>();
                    if (isalpha(left_name[0]))//left child name found on inputs
                    {
                        left_node->self_weight = 1;//default weight of each balance
                        left_node->balance_name = left_name;
                        left_node->weight_needed_for_left_balance = 0; //Add default 0 to start
                        left_node->weight_needed_for_right_balance = 0; //Add default 0 to start
                    }
                    else
                    {
                        left_node->self_weight = stoi(left_name);// direct value found for weight.
                        left_node->balance_name = ""; //No Name for only weights
                        left_node->weight_needed_for_left_balance = 0; //Add default 0 to start
                        left_node->weight_needed_for_right_balance = 0; //Add default 0 to start
                    }
                    if (isalpha(right_name[0])) //right child name found on inputs
                    {
                        right_node->self_weight = 1;//default weight of each balance
                        right_node->balance_name = right_name;
                        right_node->weight_needed_for_left_balance = 0; //Add default 0 to start
                        right_node->weight_needed_for_right_balance = 0; //Add default 0 to start
                    }
                    else
                    {
                        right_node->self_weight = stoi(right_name);// direct value found for weight.
                        right_node->balance_name = ""; //No Name for only weights
                        right_node->weight_needed_for_left_balance = 0; //Add default 0 to start
                        right_node->weight_needed_for_right_balance = 0; //Add default 0 to start
                    }
                    root->left = left_node;
                    root->right = right_node;
                }
                else if (UpdateSpecificBalance(root->left,node_name,left_name,right_name))
                {
                    return true;
                }
                else if (UpdateSpecificBalance(root->right,node_name,left_name,right_name))
                {
                    return true;
                }
                else 
                {
                    return false; //node not found in tree
                }
            }
            else
            {
                return false; //root not found in tree
            }
        }

        bool ParseInputStringToNode(const std::string& input_string)
        {
            if (input_string.find("#") != std::string::npos)
            {
                //must be comment, ignore
                return true;
            }
            std::vector<std::string> tokens;
            std::string delimiter = ",";
            size_t last = 0; 
            size_t next = 0; 
            while ((next = input_string.find(delimiter, last)) != string::npos) 
            { 
                tokens.push_back(input_string.substr(last, next-last)); 
                last = next + 1; 
            } 
            tokens.push_back(input_string.substr(last));
            //std::cout << "token0=" << tokens[0] << " token1=" << tokens[1]  << " token2=" <<tokens[2] << "\n";

            if (tokens.size() == 3) //root, left_child, right_child
            {
                try
                {
                    m_balancer_sequenced_input.push_back(tokens[0]);
                    if (!m_root)
                    {
                        std::shared_ptr<TreeNode> root_node = make_shared<TreeNode>();
                        root_node->self_weight = 1;//default weight of each balance
                        root_node->balance_name = tokens[0];
                        root_node->weight_needed_for_left_balance = 0; //Add default 0 to start
                        root_node->weight_needed_for_right_balance = 0; //Add default 0 to start

                        std::shared_ptr<TreeNode> left_node = make_shared<TreeNode>();
                        std::shared_ptr<TreeNode> right_node = make_shared<TreeNode>();
                        if (isalpha(tokens[1][0]))//left child name found on inputs
                        {
                            left_node->self_weight = 1;//default weight of each balance
                            left_node->balance_name = tokens[1];
                            left_node->weight_needed_for_left_balance = 0; //Add default 0 to start
                            left_node->weight_needed_for_right_balance = 0; //Add default 0 to start
                        }
                        else
                        {
                            left_node->self_weight = stoi(tokens[1]);// direct value found for weight.
                            left_node->balance_name = ""; //No Name for only weights
                            left_node->weight_needed_for_left_balance = 0; //Add default 0 to start
                            left_node->weight_needed_for_right_balance = 0; //Add default 0 to start
                        }
                        if (isalpha(tokens[2][0])) //right child name found on inputs
                        {
                            right_node->self_weight = 1;//default weight of each balance
                            right_node->balance_name = tokens[2];
                            right_node->weight_needed_for_left_balance = 0; //Add default 0 to start
                            right_node->weight_needed_for_right_balance = 0; //Add default 0 to start
                        }
                        else
                        {
                            right_node->self_weight = stoi(tokens[2]);// direct value found for weight.
                            right_node->balance_name = ""; //No Name for only weights
                            right_node->weight_needed_for_left_balance = 0; //Add default 0 to start
                            right_node->weight_needed_for_right_balance = 0; //Add default 0 to start
                        }
                        root_node->left = left_node;
                        root_node->right = right_node;
                        m_root = root_node;
                    }
                    else
                    {
                        std::string node_name = tokens[0];
                        UpdateSpecificBalance(m_root,node_name, tokens[1], tokens[2]);
                    }
                }
                catch(std::exception& e)
                {
                    std::cout << "Parsing input failed due to error=" << e.what() << std::endl;
                    return false;
                }
            }
            else
            {
                std::cout << "Parsing input failed due to invalid number of parsed tokens=" << tokens.size() << std::endl;
                return false;
            }
            return true;
        }
};
