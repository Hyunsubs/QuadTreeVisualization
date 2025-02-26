#include "PreCompiledHeader.h"
#include "Node.h"
#include "QuadTree.h"

Node::Node(const Bounds& bounds, int depth)
	: bounds(bounds), depth(depth)
{

}

Node::~Node()
{
	Clear();
}

void Node::Insert(Node* node)
{
	// 겹치는 영역 확인
	NodeIndex result = TestRegion(node->GetBounds());

	// 겹치면 현재 노드에 추가
	if (result == NodeIndex::Straddling)
	{
		points.emplace_back(node);
	}

	// 겹치지 않은 경우
	else if (result != NodeIndex::OutOfArea)
	{
		if (Subdivide())
		{
			if (result == NodeIndex::TopLeft)
			{
				topLeft->Insert(node);
			}
			else if (result == NodeIndex::TopRight)
			{
				topRight->Insert(node);
			}
			else if (result == NodeIndex::BottomLeft)
			{
				bottomLeft->Insert(node);
			}
			else if (result == NodeIndex::BottomRight)
			{
				bottomRight->Insert(node);
			}
		}

		// 더 이상 나눌수 없는 경우
		else
		{
			points.emplace_back(node);
		}
	}
}

void Node::Query(const Bounds& queryBounds, std::vector<Node*>& possibleNodes)
{
	// 현재 노드 추가
	possibleNodes.emplace_back(this);

	// 영역이 분할됐으면, 자식 노드도 추가 검사
	if (IsDivided())
	{
		// 전달 받은 영역과 겹치는 4분면 목록 확인
		std::vector<NodeIndex> quads = GetQuads(queryBounds);

		// 겹치는 4분면을 기준으로 검사 진행(재귀)
		for (NodeIndex& index : quads)
		{
			if (index == NodeIndex::TopLeft)
			{
				topLeft->Query(queryBounds, possibleNodes);
			}
			else if (index == NodeIndex::TopRight)
			{
				topRight->Query(queryBounds, possibleNodes);
			}
			else if (index == NodeIndex::BottomLeft)
			{
				bottomLeft->Query(queryBounds, possibleNodes);
			}
			else if (index == NodeIndex::BottomRight)
			{
				bottomRight->Query(queryBounds, possibleNodes);
			}
		}
	}
}

void Node::Clear()
{
	// 바운딩 박스 제거
	bounds.GetBoundingBox()->Destroy();

	// 현재 노드 제거
	for (Node* node : points)
	{
		SafeDelete(node);
	}

	// 리스트 정리
	points.clear();

	// 분할된 경우 자식 정리
	if (IsDivided())
	{
		topLeft->Clear();
		topRight->Clear();
		bottomLeft->Clear();
		bottomRight->Clear();

		ClearChildren();
	}
}

void Node::Update(float x, float y)
{
	bounds.SetX(x);
	bounds.SetY(y);
	bounds.Update(x, y);
}

bool Node::Subdivide()
{
	// 최대 깊이에 도달했으면 더 이상 안나눔
	if (depth == QuadTree::maxDepth)
	{
		return false;
	}

	// 아직 분할 안됐으면, 분할 진행
	if (!IsDivided())
	{
		// 영역을 나누기 위한 값 계산
		float x = bounds.X();
		float y = bounds.Y();
		float halfWidth = bounds.Width() / 2;
		float halfHeight = bounds.Height() / 2;

		// 각 4분면의 자식 객체 생성
		topLeft = new Node(Bounds(x - halfWidth / 2, y - halfHeight / 2, halfWidth, halfHeight), depth + 1);
		topRight = new Node(Bounds(x + halfWidth / 2, y - halfHeight / 2, halfWidth, halfHeight), depth + 1);
		bottomLeft = new Node(Bounds(x - halfWidth / 2, y + halfHeight / 2, halfWidth, halfHeight), depth + 1);
		bottomRight = new Node(Bounds(x + halfWidth / 2, y + halfHeight / 2, halfWidth, halfHeight), depth + 1);
	}

	return true;
}

bool Node::IsDivided() 
{
	return topLeft != nullptr;
}

void Node::ClearChildren()
{
	// 분할 진행됐는지 확인
	if (IsDivided())
	{
		SafeDelete(topLeft);
		SafeDelete(topRight);
		SafeDelete(bottomLeft);
		SafeDelete(bottomRight);
	}
}

NodeIndex Node::TestRegion(const Bounds& bounds)
{
	// 전달된 bounds와 겹치는 4분면 목록 확인
	std::vector<NodeIndex> quads = GetQuads(bounds);

	// 겹치는 곳이 없으면 영역 밖으로 표시
	if (quads.size() == 0)
	{
		return NodeIndex::OutOfArea;
	}

	// 하나만 겹친 경우 해당 4분면 반환
	if (quads.size() == 1)
	{
		return quads[0];
	}

	// 여러 영역과 겹친 경우
	return NodeIndex::Straddling;
}

std::vector<NodeIndex> Node::GetQuads(const Bounds& bounds)
{
	std::vector<NodeIndex> quads;

	// 영역 계산에 필요한 변수
	float x = this->bounds.X();
	float y = this->bounds.Y();

	float halfWidth = this->bounds.Width() / 2.f;
	float halfHeight = this->bounds.Height() / 2.f;
	float boundsHalfWidth = bounds.Width() / 2.f;
	float boundsHalfHeight = bounds.Height() / 2.f;

	// 왼쪽 영역이랑 겹치는지 확인
	bool left = bounds.X() - boundsHalfWidth < x && bounds.MaxX() >= x - halfWidth;

	// 오른쪽 영역이랑 겹치는지 확인
	bool right = bounds.MaxX() >= x && bounds.X() - boundsHalfWidth < this->bounds.MaxX();

	// 위쪽 영역이랑 겹치는지 확인
	bool top = bounds.Y() - boundsHalfHeight < y && bounds.MaxY() >= y - halfHeight;

	// 아래쪽 영역이랑 겹치는지 확인
	bool bottom = bounds.MaxY() >= y && bounds.Y() - halfHeight < this->bounds.MaxY();

	if (top && left)
	{
		quads.emplace_back(NodeIndex::TopLeft);
	}
	if (top && right)
	{
		quads.emplace_back(NodeIndex::TopRight);
	}
	if (bottom && left)
	{
		quads.emplace_back(NodeIndex::BottomLeft);
	}
	if (bottom && right)
	{
		quads.emplace_back(NodeIndex::BottomRight);
	}

	return quads;

}