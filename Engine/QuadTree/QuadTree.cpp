#include "PreCompiledHeader.h"
#include "QuadTree.h"
#include "Actor/BoundingBox.h"
#include "Engine/Engine.h"
#include "Level/Level.h"

QuadTree::QuadTree(const Bounds& bounds)
{
    // ��Ʈ ��� ����
    root = new Node(bounds);
}

QuadTree::~QuadTree()
{
    // ��Ʈ ��� ����
    SafeDelete(root);
}

void QuadTree::Insert(Node* node)
{
    root->Insert(node);
}

std::vector<Node*> QuadTree::Query(Node* queryNode)
{
    // ��ħ ���ɼ��� �ִ� ���� Ȯ��
    std::vector<Node*> possibleNodes;
    root->Query(queryNode->GetBounds(), possibleNodes);

    // ��ħ ���ɼ� �ִ� ��忡�� ������ ��ġ�� ���鸸 �˻�
    std::vector<Node*> intersects;
    for (Node* node : possibleNodes)
    {
        for (Node* point : node->Points())
        {
            if (point->GetBounds().Intersects(queryNode->GetBounds()))
            {
                intersects.emplace_back(point);
                continue;
            }
        }
    }

    // �˻��� ��� ���� ��ȯ
    return intersects;
}

void QuadTree::Clear(const Bounds& bounds)
{
    SafeDelete(root);
    root = new Node(bounds);
}
