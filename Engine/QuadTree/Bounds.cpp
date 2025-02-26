#include "PreCompiledHeader.h"
#include "Bounds.h"

bool Bounds::Intersects(const Bounds& other) const
{
    // 다른 영역이 현재 영역의 오른쪽 바깥에 있는 경우.
    if (other.x - other.width / 2 > MaxX()) return false;

    // 다른 영역이 현재 영역의 왼쪽 바깥에 있는 경우.
    if (other.MaxX() < x - width / 2) return false;

    // 다른 영역이 현재 영역의 아래쪽 바깥에 있는 경우.
    if (other.y - other.height / 2 > MaxY()) return false;

    // 다른 영역이 현재 영역의 위쪽 바깥에 있는 경우.
    if (other.MaxY() < y - height / 2) return false;

    return true;
}